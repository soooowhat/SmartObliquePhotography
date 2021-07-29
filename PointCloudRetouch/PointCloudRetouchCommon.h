#pragma once

namespace hiveObliquePhotography
{
	namespace PointCloudRetouch
	{
		using PointCloud_t = pcl::PointCloud<pcl::PointSurfel>;

		namespace KEYWORD
		{
			const std::string PLANARITY_FEATURE = "PLANARITY_FEATURE";
			const std::string VFH_FEATURE = "VFH_FEATURE";
			const std::string COLOR_FEATURE = "COLOR_FEATURE";
			const std::string NORMAL_COMPLEXITY = "NORMAL_COMPLEXITY";

			const std::string EUCLIDEAN_NEIGHBOR_BUILDER = "EUCLIDEAN_NEIGHBOR_BUILDER";

			const std::string CLUSTER_EXPANDER = "CLUSTER_EXPANDER";
			const std::string CLUSTER_EXPANDER_MULTITHREAD = "CLUSTER_EXPANDER_MULTITHREAD";
			const std::string OUTLIER_DETECTOR = "OUTLIER_DETECTOR";
			const std::string BOUNDARY_DETECTOR = "BOUNDARY_DETECTOR";
			const std::string HOLE_REPAIRER = "HOLE_REPAIRER";

			const std::string PRECOMPUTE_FOLDER = "Temp/";

		}
		enum class EPointLabel : unsigned char
		{
			DISCARDED,
			KEPT,
			UNWANTED,
			UNDETERMINED,
			FILLED,
		};

		template <typename T>
		T NormalDistribution(T vX, T vDelta  = 1)
		{
			return exp(-(vX * vX) / (2 * vDelta * vDelta)) / (2.50662827464 * vDelta);
		}
	}
}