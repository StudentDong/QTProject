//#pragma once
#include <QTreeWidget>
class myQTreeWidget:public QTreeWidget
{
	Q_OBJECT
public:
	//���캯��
	myQTreeWidget(QWidget *parent = 0);
	//myQTreeWidget();
	//myQTreeWidget();
	//��������
	~myQTreeWidget();
public:
	QTreeWidget* m_tree;//�ڵ㸸����
	QStringList m_title;
	bool Flag;
public:
	void creattree(QStringList fileName);
private:
	private slots:
	

};


