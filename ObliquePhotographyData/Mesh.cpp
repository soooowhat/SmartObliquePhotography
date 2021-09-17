#include "pch.h"
#include "Mesh.h"

using namespace hiveObliquePhotography;

CMesh::CMesh(const pcl::PolygonMesh& vPolMesh)
{
	__fillVertices(m_Vertices, vPolMesh);
	__fillFaces(m_Faces, vPolMesh.polygons);
}

CMesh::CMesh(const pcl::TextureMesh& vTexMesh)
{
	__fillVertices(m_Vertices, vTexMesh);
	__fillFaces(m_Faces, vTexMesh.tex_polygons[0]);
}

//*****************************************************************
//FUNCTION: 
pcl::PolygonMesh CMesh::toPolMesh() const
{
	pcl::PolygonMesh PolMesh;
	__fillCloud(m_Vertices, PolMesh.cloud);
	__fillPolygons(m_Faces, PolMesh.polygons);

	return PolMesh;
}

//*****************************************************************
//FUNCTION: 
pcl::TextureMesh CMesh::toTexMesh(const pcl::TexMaterial& vMaterial) const
{
	pcl::TextureMesh TexMesh;
	__fillCloud(m_Vertices, TexMesh.cloud);
	TexMesh.tex_polygons.resize(1);
	__fillPolygons(m_Faces, TexMesh.tex_polygons[0]);
	
	//tex coord
	std::vector<Eigen::Vector2f, Eigen::aligned_allocator<Eigen::Vector2f>> Coords;
	for (int i = 0; i < m_Vertices.size(); i++)
		Coords.push_back(Eigen::Vector2f{ m_Vertices[i].u, m_Vertices[i].v });
	TexMesh.tex_coordinates.push_back(Coords);
	TexMesh.tex_materials.push_back(vMaterial);

	return TexMesh;
}

std::vector<SFace> CMesh::findFacesByVertex(IndexType vVertex) const
{
	std::vector<SFace> Faces;
	_ASSERTE(vVertex < m_Vertices.size());
	for (auto& Face : m_Faces)
		for (int i = 0; i < 3; i++)	//三角形面片
			if (i == vVertex)
			{
				Faces.push_back(Face);
				break;
			}
	return Faces;
}

std::pair<Eigen::Vector3f, Eigen::Vector3f> CMesh::calcAABB() const
{
	Eigen::Vector3f Min{ FLT_MAX, FLT_MAX, FLT_MAX };
	Eigen::Vector3f Max{ -FLT_MAX, -FLT_MAX, -FLT_MAX };
	auto update = [&](const Eigen::Vector3f& vPos)
	{
		for (int i = 0; i < 3; i++)
		{
			if (vPos.data()[i] < Min.data()[i])
				Min.data()[i] = vPos.data()[i];
			if (vPos.data()[i] > Max.data()[i])
				Max.data()[i] = vPos.data()[i];
		}
	};

	for (auto& Vertex : m_Vertices)
		update(Vertex.xyz());
	return { Min, Max };
}

//*****************************************************************
//FUNCTION: 
void CMesh::__fillVertices(std::vector<SVertex>& vVertices, const pcl::PolygonMesh& vPolMesh) const
{
	auto OffsetTable = __getOffsetTable(vPolMesh.cloud.fields);
	__copyAttributes(vVertices, vPolMesh.cloud.data, OffsetTable, vPolMesh.cloud.point_step);
}

//*****************************************************************
//FUNCTION: 
void CMesh::__fillVertices(std::vector<SVertex>& vVertices, const pcl::TextureMesh& vTexMesh) const
{
	auto OffsetTable = __getOffsetTable(vTexMesh.cloud.fields);
	__copyAttributes(vVertices, vTexMesh.cloud.data, OffsetTable, vTexMesh.cloud.point_step);

	if (!vTexMesh.tex_coordinates[0].empty())
	{
		for (int i = 0; i < vVertices.size(); i++)
		{
			auto& TexCoord = vTexMesh.tex_coordinates[0][i];	//只有一套纹理坐标的
			vVertices[i].u = TexCoord.x();
			vVertices[i].v = TexCoord.y();
		}
	}
}

//*****************************************************************
//FUNCTION: 
std::map<std::uint32_t, std::uint32_t> CMesh::__getOffsetTable(const std::vector<pcl::PCLPointField>& vVertexAttributes) const
{
	std::map<std::uint32_t, std::uint32_t> OffsetTable;
	for (auto& VertexAttribute : vVertexAttributes)
	{
		if (VertexAttribute.name == "x")
			OffsetTable[offsetof(SVertex, x)] = VertexAttribute.offset;
		if (VertexAttribute.name == "y")
			OffsetTable[offsetof(SVertex, y)] = VertexAttribute.offset;
		if (VertexAttribute.name == "z")
			OffsetTable[offsetof(SVertex, z)] = VertexAttribute.offset;

		if (VertexAttribute.name == "normal_x")
			OffsetTable[offsetof(SVertex, nx)] = VertexAttribute.offset;
		if (VertexAttribute.name == "normal_y")
			OffsetTable[offsetof(SVertex, ny)] = VertexAttribute.offset;
		if (VertexAttribute.name == "normal_z")
			OffsetTable[offsetof(SVertex, nz)] = VertexAttribute.offset;
	}
	return OffsetTable;
}

//*****************************************************************
//FUNCTION: 
void CMesh::__copyAttributes(std::vector<SVertex>& vVertices, const std::vector<uint8_t>& vData, const std::map<uint32_t, uint32_t>& vOffsetTable, int vPointStep) const
{
	vVertices.resize(vData.size() / vPointStep);
	for (int i = 0; i < vVertices.size(); i++)
	{
		for (auto Iter = vOffsetTable.begin(); Iter != vOffsetTable.end(); Iter++)
		{
			void* VertexOffset = (char*)(&vVertices[i]) + Iter->first;
			void* DataOffset = (char*)(&vData[i * vPointStep] + Iter->second);
			*(static_cast<DataType*>(VertexOffset)) = *(static_cast<DataType*>(DataOffset));
		}
	}
}

//*****************************************************************
//FUNCTION: 
void CMesh::__fillFaces(std::vector<SFace>& vFaces, const std::vector<pcl::Vertices>& vFaceData) const
{
	_ASSERTE(vFaceData[0].vertices.size() == 3);
	for (auto& Face : vFaceData)
		vFaces.push_back({ Face.vertices[0], Face.vertices[1], Face.vertices[2] });
}

//*****************************************************************
//FUNCTION: 
void CMesh::__fillCloud(const std::vector<SVertex>& vVertices, pcl::PCLPointCloud2& vCloud) const
{
	vCloud.width = vVertices.size();
	vCloud.height = 1;
	vCloud.is_bigendian = 0;

	const std::vector<std::string> AttributeNames = 
	{
		"x", "y", "z", "normal_x", "normal_y", "normal_z", "u", "v"
	};
	for (int i = 0; i < AttributeNames.size(); i++)
	{
		pcl::PCLPointField Attribute;
		Attribute.name = AttributeNames[i];
		Attribute.offset = i * sizeof(DataType);
		Attribute.datatype = pcl::PCLPointField::FLOAT32;
		Attribute.count = 1;
		vCloud.fields.push_back(Attribute);
	}
	vCloud.point_step = AttributeNames.size() * sizeof(DataType);

	vCloud.row_step = vCloud.point_step * vCloud.width;
	vCloud.data.resize(vCloud.row_step);

	for (int i = 0; i < vCloud.width; i++)
		memcpy(&vCloud.data[i * vCloud.point_step], &vVertices[i], vCloud.point_step);
}

//*****************************************************************
//FUNCTION: 
void CMesh::__fillPolygons(const std::vector<SFace>& vFaces, std::vector<pcl::Vertices>& vPolygons) const
{
	for (int i = 0; i < vFaces.size(); i++)
	{
		pcl::Vertices Face;
		Face.vertices = { vFaces[i].a, vFaces[i].b, vFaces[i].c };
		vPolygons.push_back(Face);
	}
}

