#ifndef RANSAC_H
#define RANSAC_H



#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <vector>

pcl::PointCloud<pcl::PointXYZRGB>::Ptr  CylinderSegmentation(pcl::PointCloud<pcl::PointXYZRGB>::Ptr &Cloud,float &InParam1,float &InParam2,
	float &InParam3, float &InParam4);
///ƽ�����
pcl::PointCloud<pcl::PointXYZRGB>::Ptr  RandomSampleConsensus(pcl::PointCloud<pcl::PointXYZRGB>::Ptr &Cloud, float &InParam1, float &a, float &b, float &c, float &d);
///�㵽��ľ���
void PointToPlant(pcl::PointCloud<pcl::PointXYZRGB>::Ptr &Cloud,float &ResidualSum, float &ResidualSquares);
///ŷʽ����
void EuropeCluster(pcl::PointCloud<pcl::PointXYZRGB>::Ptr &Cloud, float &a, float &b, float &c, std::vector<pcl::PointCloud<pcl::PointXYZRGB>::Ptr> &OutCloud);

#endif // RANSAC_H