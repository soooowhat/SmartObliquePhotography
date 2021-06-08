#pragma once
#include "AutoRetouchCommon.h"
#include "OpResultQueue.h"
#include "PointLabel4Classfier.h"

namespace hiveObliquePhotography
{
	class CNeighborhood;

	namespace AutoRetouch
	{
		class CPointCloudAutoRetouchScene : public hiveDesignPattern::CSingleton<CPointCloudAutoRetouchScene>
		{
		public:
			~CPointCloudAutoRetouchScene();

			CNeighborhood* buildNeighborhood(std::uint64_t vSeed, const std::string& vBuilderSig);
			CNeighborhood* buildNeighborhood(std::uint64_t vSeed, const std::vector<std::uint64_t>& vRestrictedSet, const std::string& vBuilderSig);

			bool undoLastOp();

			void recordCurrentOp(IOpResult* vResult);
			void init(pcl::PointCloud<pcl::PointSurfel>::Ptr vPointCloudScene);

			std::size_t getNumPoint() const { _ASSERTE(m_pPointCloudScene); return m_pPointCloudScene->size(); }

			const pcl::PointCloud<pcl::PointSurfel>::Ptr getPointCloudScene() const { return m_pPointCloudScene; }
			pcl::search::KdTree<pcl::PointSurfel>* getGlobalKdTree() const { return m_pGlobalKdTree; }

			CGlobalPointLabelSet* fetchPointLabelSet() { return &m_PointLabelSet; }

		private:
			CPointCloudAutoRetouchScene();

			COpResultQueue m_OpResultQueue;
			CGlobalPointLabelSet m_PointLabelSet;

			pcl::PointCloud<pcl::PointSurfel>::Ptr m_pPointCloudScene = nullptr;
			pcl::search::KdTree<pcl::PointSurfel>* m_pGlobalKdTree = nullptr;

		friend class hiveDesignPattern::CSingleton<CPointCloudAutoRetouchScene>;
		};
	}
}


