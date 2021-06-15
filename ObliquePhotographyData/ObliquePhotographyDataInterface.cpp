#include "pch.h"
#include "ObliquePhotographyDataInterface.h"
#include "PointCloudScene.h"

using namespace hiveObliquePhotography;

//*****************************************************************
//FUNCTION: 
pcl::PointCloud<pcl::PointSurfel>::Ptr hiveObliquePhotography::hiveInitPointCloudScene(const std::vector<std::string>& vFileNameSet)
{
	_ASSERTE(!vFileNameSet.empty());

	return CPointCloudScene::getInstance()->loadScene(vFileNameSet);
}

bool hiveObliquePhotography::hiveSavePointCloudScene(PointCloud_t& vPointCloud, std::string vFileName)
{
	_ASSERTE(!vFileName.empty());

	return CPointCloudScene::getInstance()->saveScene(vPointCloud, vFileName);
}