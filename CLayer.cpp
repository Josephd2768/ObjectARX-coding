#include "stdafx.h"
#include "CLayer.h"

AcDbObjectId CLayer::CreateLayer(const ACHAR *layername, int colorindex) //������ͼ�㺯��ʵ��
{//��ͼ���
	AcDbLayerTable *pLayerTable;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pLayerTable, AcDb::kForWrite);
	//��ʼ������¼����
	AcDbLayerTableRecord *pLayerTableRecord =
		new AcDbLayerTableRecord;
	pLayerTableRecord->setName(layername);
	AcDbObjectId pLayerId;
	// �ж�ͼ���Ƿ����
	bool HasLayer = pLayerTable->has(layername);
	if (HasLayer == 1) {

		//AfxMessageBox(_T("��ͼ���Ѵ���"));
		pLayerTable->getAt(layername, pLayerId);
		pLayerTable->close();
		pLayerTableRecord->close();
		return pLayerId;
	}
	//ͼ����������ԣ�������ɫ�����εȣ�����ȱʡֵ,��Ҫ�޸�
	AcCmColor color;//����ͼ����ɫ
	color.setColorIndex(colorindex);
	pLayerTableRecord->setColor(color);
	//pLayerTableRecord->setLinetypeObjectId()
	//���½���ͼ����¼��ӵ�ͼ����У�����ͼ����¼��ID
	//���浽pLayerId�У���Ϊ�����ķ���ֵ
	pLayerId = pLayerTableRecord->objectId();
	pLayerTable->add(pLayerId, pLayerTableRecord);
	pLayerTable->close();
	pLayerTableRecord->close();
	return pLayerId;
}