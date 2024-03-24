#include "stdafx.h"
#include "CLayer.h"

AcDbObjectId CLayer::CreateLayer(const ACHAR *layername, int colorindex) //创建新图层函数实现
{//打开图层表
	AcDbLayerTable *pLayerTable;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pLayerTable, AcDb::kForWrite);
	//初始化层表记录对象
	AcDbLayerTableRecord *pLayerTableRecord =
		new AcDbLayerTableRecord;
	pLayerTableRecord->setName(layername);
	AcDbObjectId pLayerId;
	// 判断图层是否存在
	bool HasLayer = pLayerTable->has(layername);
	if (HasLayer == 1) {

		//AfxMessageBox(_T("该图层已存在"));
		pLayerTable->getAt(layername, pLayerId);
		pLayerTable->close();
		pLayerTableRecord->close();
		return pLayerId;
	}
	//图层的其它属性（例如颜色、线形等）都用缺省值,如要修改
	AcCmColor color;//设置图层颜色
	color.setColorIndex(colorindex);
	pLayerTableRecord->setColor(color);
	//pLayerTableRecord->setLinetypeObjectId()
	//将新建的图层表记录添加到图层表中，并将图层表记录的ID
	//保存到pLayerId中，作为函数的返回值
	pLayerId = pLayerTableRecord->objectId();
	pLayerTable->add(pLayerId, pLayerTableRecord);
	pLayerTable->close();
	pLayerTableRecord->close();
	return pLayerId;
}