#include "pch.h"
#include "PlanarityFeature.h"
#include <pcl/sample_consensus/impl/sac_model_plane.hpp>
#include <pcl/sample_consensus/impl/ransac.hpp>

using namespace hiveObliquePhotography::PointCloudRetouch;

_REGISTER_EXCLUSIVE_PRODUCT(CPlanarityFeature, KEYWORD::PLANARITY_FEATURE)

//*****************************************************************
//FUNCTION: 
double CPlanarityFeature::generateFeatureV(const std::vector<pcl::index_t>& vDeterminantPointSet, const std::vector<pcl::index_t>& vValidationSet, pcl::index_t vClusterCenter)
{
	auto pDeterminantCloud = __createPositionCloud(vDeterminantPointSet);
	auto Plane = __fitPlane(pDeterminantCloud);
	if (Plane.norm() == 0)
		return 0.0;
	else
		m_Plane = Plane;
	m_Peak = __computePeakDistance(pDeterminantCloud, m_Plane);
	
	float SumMatch = 0.0f;
	int VaildNum = 0;
	for (auto& i : vValidationSet)
	{
		auto Match = evaluateFeatureMatchFactorV(i);
		if (Match > 0)
		{
			++VaildNum;
			SumMatch += Match;
		}
	}

	return SumMatch / VaildNum;
}

//*****************************************************************
//FUNCTION: 
double CPlanarityFeature::evaluateFeatureMatchFactorV(pcl::index_t vInputPoint)
{
	const auto& Position = CPointCloudRetouchManager::getInstance()->getRetouchScene().getPositionAt(vInputPoint);
	float Distance = m_Plane.dot(Position);

	if (Distance > m_Peak.second || Distance < m_Peak.first)
		return 0;

	if (Distance < 0)
		Distance /= m_Peak.first;
	else
		Distance /= m_Peak.second;
	return pow(Distance, 4) - 2.0f * pow(Distance, 2) + 1.0f;
}

//*****************************************************************
//FUNCTION: 
PointCloud_t::Ptr CPlanarityFeature::__createPositionCloud(const std::vector<pcl::index_t>& vIndexSet)
{
	PointCloud_t::Ptr pCloud(new PointCloud_t);
	for (auto& i : vIndexSet)
	{
		const auto& Position = CPointCloudRetouchManager::getInstance()->getRetouchScene().getPositionAt(i);
		PointCloud_t::PointType Point;
		Point.x = Position.x();
		Point.y = Position.y();
		Point.z = Position.z();

		pCloud->push_back(Point);
	}
	return pCloud;
}

//*****************************************************************
//FUNCTION: 
Eigen::Vector4f CPlanarityFeature::__fitPlane(PointCloud_t::Ptr vCloud) const
{
	Eigen::VectorXf Coeff;
	pcl::SampleConsensusModelPlane<PointCloud_t::PointType>::Ptr ModelPlane
		(new pcl::SampleConsensusModelPlane<PointCloud_t::PointType>(vCloud));
	pcl::RandomSampleConsensus<PointCloud_t::PointType> Ransac(ModelPlane);
	//TODO: move to config
	Ransac.setDistanceThreshold(0.3);
	Ransac.computeModel();
	Ransac.getModelCoefficients(Coeff);
	if (!Coeff.size())
		return { 0, 0, 0, 0 };
	const Eigen::Vector3f Normal(Coeff.x(), Coeff.y(), Coeff.z());
	//TODO: move to config
	const Eigen::Vector3f Up(0.0f, 0.0f, 1.0f);
	if (Normal.dot(Up) < 0.0f)
		Coeff *= -1.0f;

	return Coeff / Normal.norm();
}

//*****************************************************************
//FUNCTION: 
std::pair<float, float> CPlanarityFeature::__computePeakDistance(PointCloud_t::Ptr vCloud, Eigen::Vector4f vPlane)
{
	float MinDistance = FLT_MAX;
	float MaxDistance = -FLT_MAX;
	for (auto& i : *vCloud)
	{
		MinDistance = std::min(MinDistance, vPlane.dot(i.getVector4fMap()));
		MaxDistance = std::max(MaxDistance, vPlane.dot(i.getVector4fMap()));
	}

	return { MinDistance, MaxDistance };
}
