#pragma once

namespace hiveObliquePhotography
{
	class CPointCloudTile;

	class IPointCloudLoader : public hiveDesignPattern::IProduct
	{
	public:
		IPointCloudLoader();
		~IPointCloudLoader();

		pcl::PointCloud<pcl::PointSurfel>* loadDataFromFile(const std::string& vFileName);

	private:
		virtual bool __loadDataFromFileV(const std::string& vFileName, pcl::PointCloud<pcl::PointSurfel>& voPointCloud);
	};
}