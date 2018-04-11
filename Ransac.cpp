#include "Ransac.h"
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/passthrough.h>
#include <pcl/features/normal_3d.h>
#include <pcl/sample_consensus/method_types.h>
//#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/sample_consensus/ransac.h>
#include <pcl/sample_consensus/sac_model_plane.h>
#include <boost/thread/thread.hpp>
#include <pcl/kdtree/kdtree.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/extract_clusters.h>
pcl::PointCloud<pcl::PointXYZRGB>::Ptr  CylinderSegmentation(pcl::PointCloud<pcl::PointXYZRGB>::Ptr &Cloud, float &InParam1, float &InParam2,
	float &InParam3, float &InParam4)
{
	// All the objects needed
	pcl::PassThrough<pcl::PointXYZRGB> pass;
	pcl::NormalEstimation<pcl::PointXYZRGB, pcl::Normal> ne;
	pcl::SACSegmentationFromNormals<pcl::PointXYZRGB, pcl::Normal> seg;
	pcl::ExtractIndices<pcl::PointXYZRGB> extract;
	pcl::ExtractIndices<pcl::Normal> extract_normals;
	pcl::search::KdTree<pcl::PointXYZRGB>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZRGB>());
	// Datasets
	//pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
	//pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZRGB>);
	pcl::PointCloud<pcl::Normal>::Ptr cloud_normals(new pcl::PointCloud<pcl::Normal>);
	//pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_filtered2(new pcl::PointCloud<pcl::PointXYZRGB>);
	//pcl::PointCloud<pcl::Normal>::Ptr cloud_normals2(new pcl::PointCloud<pcl::Normal>);
	pcl::ModelCoefficients::Ptr coefficients_plane(new pcl::ModelCoefficients), coefficients_cylinder(new pcl::ModelCoefficients);
	pcl::PointIndices::Ptr inliers_plane(new pcl::PointIndices), inliers_cylinder(new pcl::PointIndices);
	//���ƽ��з���ͳ��
	ne.setInputCloud(Cloud);

	ne.setSearchMethod(tree);
	//ne.setRadiusSearch(0.02);
	ne.setKSearch(30);
	ne.compute(*cloud_normals);
	// Create the segmentation object for cylinder segmentation and set all the parameters
	seg.setOptimizeCoefficients(true);   //���öԹ���ģ���Ż�
	seg.setModelType(pcl::SACMODEL_CYLINDER);  //���÷ָ�ģ��ΪԲ����
	seg.setMethodType(pcl::SAC_RANSAC);       //�������Ʒ���
	seg.setNormalDistanceWeight(InParam1);       //���ñ��淨��Ȩ��ϵ��
	seg.setMaxIterations(InParam2);              //���õ�����������10000
	seg.setDistanceThreshold(InParam3);         //�����ڵ㵽ģ�͵ľ����������ֵ
	seg.setRadiusLimits(0, InParam4);             //���ù��Ƴ���Բ��ģ�͵İ뾶�ķ�Χ
	seg.setInputCloud(Cloud);
	seg.setInputNormals(cloud_normals);
	// Obtain the cylinder inliers and coefficients
	seg.segment(*inliers_cylinder, *coefficients_cylinder);
	extract.setInputCloud(Cloud);
	extract.setIndices(inliers_cylinder);

	extract.setNegative(false);//��ȡΪ���ture
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_cylinder(new pcl::PointCloud<pcl::PointXYZRGB>());
	extract.filter(*cloud_cylinder);
	return cloud_cylinder;
}
//////////////////////////////
pcl::PointCloud<pcl::PointXYZRGB>::Ptr  RandomSampleConsensus(pcl::PointCloud<pcl::PointXYZRGB>::Ptr &Cloud, float &InParam1, float &a, float &b, float &c, float &d)
{
	//������ڵ�����
	std::vector<int > inliers;
	//����һ����
	pcl::SampleConsensusModelPlane<pcl::PointXYZRGB>::Ptr model_p(new pcl::SampleConsensusModelPlane<pcl::PointXYZRGB>(Cloud));
	pcl::RandomSampleConsensus<pcl::PointXYZRGB> ransac(model_p);
	ransac.setDistanceThreshold(InParam1);
	ransac.computeModel();
	ransac.getInliers(inliers);
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr final(new pcl::PointCloud<pcl::PointXYZRGB>);
	final->resize(inliers.size());

	pcl::copyPointCloud<pcl::PointXYZRGB>(*Cloud, inliers, *final);
	Eigen::VectorXf coef = Eigen::VectorXf::Zero(4, 1);
	ransac.getModelCoefficients(coef);
	//a = coef[0] / coef[3]; b = coef[1] / coef[3]; c = coef[2] / coef[3]; d = 1
	a = 0.0f, b = 0.0f, c = 0.0f, d = 0.0f;
	a = coef[0]; c = coef[2];
	b = coef[1]; d = coef[3];
	return final;

}

/////////////////�㵽���̵ľ���
void PointToPlant(pcl::PointCloud<pcl::PointXYZRGB>::Ptr &Cloud,float &ResidualSum,float &ResidualSquares)
{
	float a, b, c, d;
	float Inparame = 0.1f;
	ResidualSum = 0;
	//std::vector<float> &Dis_V;
	RandomSampleConsensus(Cloud, Inparame, a, b, c, d);
	float dis;
	for (int i = 0; i < Cloud->points.size(); i++)
	{
		dis = abs(a*Cloud->points[i].x + b*Cloud->points[i].y + c*Cloud->points[i].z + d) / sqrt(a*a + b*b + c*c);
		//Dis_V.push_back(dis);
		////�ܲβ�
		ResidualSum += (dis*dis);
		
	}
	ResidualSquares = sqrt(ResidualSum / Cloud->points.size());
}
///////////////ŷʽ����
void EuropeCluster(pcl::PointCloud<pcl::PointXYZRGB>::Ptr &Cloud, float &a, float &b, float &c, 
	std::vector<pcl::PointCloud<pcl::PointXYZRGB>::Ptr> &OutCloud)
{

	pcl::PointCloud<pcl::PointXYZRGB>::Ptr Incloud(new pcl::PointCloud<pcl::PointXYZRGB>);
	pcl::search::KdTree<pcl::PointXYZRGB>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZRGB>);
	tree->setInputCloud(Cloud);//����������������
	std::vector<pcl::PointIndices> cluster_indices;//�����飬���ڴ�������ĵ���
	pcl::EuclideanClusterExtraction<pcl::PointXYZRGB> ec;
	ec.setClusterTolerance(a);                     // ���ý��������������뾶Ϊ2cm
	ec.setMinClusterSize(b);                 //����һ��������Ҫ�����ٵĵ���ĿΪ100
	ec.setMaxClusterSize(c);               //����һ��������Ҫ��������ĿΪ25000
	ec.setSearchMethod(tree);
	ec.setInputCloud(Cloud);
	ec.extract(cluster_indices);
	//�������ʵ���������ֱ���ֳ�������
	int j = 0;
	for (std::vector<pcl::PointIndices>::const_iterator it = cluster_indices.begin(); it != cluster_indices.end(); ++it)
	{
		pcl::PointCloud<pcl::PointXYZRGB>::Ptr Cloud_Cluster(new pcl::PointCloud<pcl::PointXYZRGB>);
		for (std::vector<int>::const_iterator pit = it->indices.begin(); pit != it->indices.end(); pit++)
		{
			Cloud_Cluster->points.push_back(Cloud->points[*pit]);
		}
		Cloud_Cluster->width = Cloud_Cluster->points.size();
		Cloud_Cluster->height = 1;
		Cloud_Cluster->is_dense = true;
		OutCloud.push_back(Cloud_Cluster);
		j++;
	}
	
}

