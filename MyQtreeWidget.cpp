#include "MyQtreeWidget.h"
#include <QMessageBox>

//myQTreeWidget::myQTreeWidget()
//{
//}
myQTreeWidget::myQTreeWidget(QWidget *parent /*= 0*/) :QTreeWidget(parent)
{
	this->m_tree = new QTreeWidget;

}
myQTreeWidget::~myQTreeWidget()
{
}

void myQTreeWidget::creattree(QStringList Title)
{
	
	//ģ���ȡ���ļ���
	//��� QTreeWidgetItem(�����Ƕ�ȡ�ļ������ļ���������Ϊitem������)
	for (unsigned i = 0; i < Title.size(); i++)
	{
		
		QTreeWidgetItem *leaf = new QTreeWidgetItem(this->m_tree, QStringList() << Title[i]);//�������
		leaf->setFlags(leaf->flags() | Qt::ItemIsEditable);// ���ø���ı�־����������Ƿ�ѡ���Ƿ񱻵�����Ƿ�ʹ�ܵ�
		leaf->setCheckState(0, Qt::Checked);
		//root->addChild(leaf);
	}
}


