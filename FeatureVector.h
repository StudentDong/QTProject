#ifndef  FEATURE_VECTOR_H
#define  FEATURE_VECTOR_H
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
//��������
void FeaVector(pcl::PointCloud<pcl::PointXYZRGB>::Ptr &cloud,float &m1, float &m2,float &m3);
//������ײ�ͶӰ��ֵ

//ͶӰ�ܶ�
void ProjectedDensity(pcl::PointCloud<pcl::PointXYZRGB>::Ptr &cloud, float &m, float &Density_XOY, float &Density_YOZ, float &Density_XOZ);

#endif



