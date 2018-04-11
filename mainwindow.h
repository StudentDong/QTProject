#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QDockWidget>
#include <QLabel>
#include<qlist.h>
#include "ui_mainwindow.h"
#include <MyQtreeWidget.h>
#include <MyVtkWidget.h>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent=0);
	~MainWindow();
public:
	void removeAllDock();//�Ƴ�������dock
	//Displaying dock Window
	void showDock(const QList<int> &index);
public:
	int PointSize;
	bool isDrawCoordinate;
	bool isObbSwitch;
	bool isDisappear;
	//���Դ���
	//QDockWidget* Properties ;
	QLabel* Infromation;
private:
	Ui::MainWindowClass ui;
private:
	//������ر���
	QList<QDockWidget*> myDocks;
	QList<int> index;//dock����
	myVtkWidget *view;//������ʾ����
	myQTreeWidget* tree;//�ļ���dock�ڵ���״ͼ
	QToolBar* viewToolBar;//��ͼ������
	QLabel* label_PointSize;
	//������ر���
	std::vector<pcl::PointCloud<pcl::PointXYZRGB>::Ptr> cloud;
	struct PointMaxMin
	{
		pcl::PointXYZRGB MaxPoint;
		pcl::PointXYZRGB MinPoint;
	};
	
	std::vector<PointMaxMin> PointMaxMinArray;
	QStringList FilePathName;
	QStringList fileName;
	///////////////////////////
	QStringList head;//��ͷ
//˽�к�������
private:
	void creatdockwindow();
	void InitializeVar();
	void MainWindow::connectButtonSlots();
	void creatToolBar();
	void creatStatusBar();
	//�ۺ�������
	private slots:
	void onOpen();
	void onSetBackGroundColor();
	void onSetBackGroundBlack();
	void onSetBackGroundWhite();
	void onSelectPointColor();
	void onSetPointCloudRed();
	void onSetPointCloudGreen();
	void onSetPointCloudBlue();
	void onIncrease();
	void onDecrease();
	void onCoordinate();
	void onClear();
	void onRansacCycle();
	void onRansacPlane();
	void onFeatureValue();
	void onEuropeCluster();
	void onFeatureOutput();
	void onSave();
	////////////////////����ͼ
	void onTopView();
	void onLeftView();
	void onMainView();
	private slots:
	void onCreatureObbCube();
	///////////////////////////�����̲���
	void itemSelected(QTreeWidgetItem* item, int count);
	void treeItemChanged(QTreeWidgetItem* item, int);
	void popMenu(const QPoint&);
};

#endif // MAINWINDOW_H
