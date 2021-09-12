#pragma once
#include "PointClusterSet.h"

namespace hiveObliquePhotography
{
	namespace PointCloudRetouch
	{
		class INeighborhoodBuilder;

		class CPointCloudScene 
		{
		public:
			CPointCloudScene();
			~CPointCloudScene();

			void init(const std::vector<PointCloud_t::Ptr>& vPointCloudScene);
			void reset() { m_pPointCloudScene.clear(); }

			Eigen::Vector4f getPositionAt(pcl::index_t vIndex) const;
			Eigen::Vector4f getNormalAt(pcl::index_t vIndex) const;
			Eigen::Vector3i getColorAt(pcl::index_t vIndex) const;
			
			std::size_t getNumPoint() const { int Num = 0; for (auto pCloud : m_pPointCloudScene) Num += pCloud.second->size(); return Num; }

			std::pair<Eigen::Vector3f, Eigen::Vector3f> getBoundingBox(const std::vector<pcl::index_t>& vIndices) const;
			std::vector<pcl::index_t> getPointsInBox(const std::pair<Eigen::Vector3f, Eigen::Vector3f>& vBox, const Eigen::Matrix3f& vRotationMatrix) const;

			template<typename Point_t>
			void dumpPoint(pcl::index_t vIndex, Point_t& voPoint) const
			{
				_ASSERTE(vIndex < m_pPointCloudScene->size());
				pcl::copyPoint(m_pPointCloudScene->at(vIndex), voPoint);
			}
			
			template<typename Point_t>
			void dumpPointCloud(pcl::PointCloud<Point_t>& voPointCloud) const
			{
				pcl::copyPointCloud(*m_pPointCloudScene, voPointCloud);
			}
			
			template<typename Point_t>
			void dumpPointCloud(const std::vector<pcl::index_t>& vIndices, pcl::PointCloud<Point_t>& voPointCloud) const
			{
				pcl::copyPointCloud(*m_pPointCloudScene, vIndices, voPointCloud);
			}

		private:
			Eigen::Vector3i __extractRgba(float vRgba) const;

			std::vector<std::pair<int, PointCloud_t::Ptr>> m_pPointCloudScene;
		};
	}
}


