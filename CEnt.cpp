#include "stdafx.h"
#include "CEnt.h"


AcDbObjectId CEnt::PostToModelSpace(AcDbEntity *pEnt)
{
	//只读方式打开活动状态的模型数据库表（定义指针）
	AcDbBlockTable *pTableEnt;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pTableEnt, AcDb::kForRead);
	//只写方式打开模型空间块表记录
	AcDbBlockTableRecord *pRecordEnt;
	pTableEnt->getAt(ACDB_MODEL_SPACE, pRecordEnt, AcDb::kForWrite);
	//将对象添加到块表记录中
	AcDbObjectId EntID;
	pRecordEnt->appendAcDbEntity(EntID, pEnt);
	EntID = pRecordEnt->objectId();
	//释放资源
	pTableEnt->close();
	pRecordEnt->close();
	pEnt->close();
	return EntID;
}


//创建直线对象
AcDbObjectId CLine::CreateLine(AcGePoint3d PtStart, AcGePoint3d PtEnd, const ACHAR* layername, const ACHAR* LineType, int scale)		//函数实现
{
	AcDbLine *pLine = new AcDbLine(PtStart, PtEnd);
	pLine->setLineWeight(AcDb::kLnWtByLayer, TRUE);
	pLine->setColorIndex(-1);//-1指随层
	pLine->setLayer(layername);
	pLine->setLinetype(LineType);
	pLine->setLinetypeScale(scale);//线型比例调整到5，方便CENTER线型
	AcDbObjectId LineId;
	//此时，直线对象仅被在内存上创建，并没有添加到图形数据库中，因此不可能会显示在图形窗口中。
	LineId = CEnt::PostToModelSpace(pLine);
	pLine->close();
	return LineId;
}
//创建直线对象（重载）
AcDbObjectId CLine::CreateLine(AcGePoint3d PtStart, AcGePoint3d PtEnd, const ACHAR* layername)		//函数实现
{
	return CreateLine(PtStart, PtEnd, layername, NULL, 1);
}

//创建直线对象
AcDbObjectId CLine::CreateLineWithHandle(AcGePoint3d PtStart, AcGePoint3d PtEnd, const ACHAR* layername, const ACHAR* LineType, int scale, AcDbHandle &h)		//函数实现
{
	AcDbLine *pLine = new AcDbLine(PtStart, PtEnd);
	pLine->setLineWeight(AcDb::kLnWtByLayer, TRUE);
	pLine->setColorIndex(-1);//-1指随层
	pLine->setLayer(layername);
	pLine->setLinetype(LineType);
	pLine->setLinetypeScale(scale);//线型比例调整到5，方便CENTER线型
	AcDbObjectId LineId;
	//此时，直线对象仅被在内存上创建，并没有添加到图形数据库中，因此不可能会显示在图形窗口中。
	LineId = CEnt::PostToModelSpace(pLine);
	pLine->getAcDbHandle(h);
	pLine->close();
	return LineId;
}

//画圆
AcDbObjectId CCircle::CreateCircle(AcGePoint3d ptCenter, double radius, int color, const ACHAR* layername)
{
	AcGeVector3d vec(0.0, 0.0, 1.0);
	AcDbCircle *pCircle = new AcDbCircle(ptCenter, vec, radius);

	pCircle->setColorIndex(color);
	pCircle->setLayer(layername);
	// 将实体添加到图形数据库
	AcDbObjectId circleId;
	circleId = CEnt::PostToModelSpace(pCircle);
	pCircle->close();
	return circleId;
}

//画圆
AcDbObjectId CCircle::CreateCircleWithHandle(AcGePoint3d ptCenter, double radius, int color, const ACHAR* layername, AcDbHandle &h)
{
	AcGeVector3d vec(0.0, 0.0, 1.0);
	AcDbCircle *pCircle = new AcDbCircle(ptCenter, vec, radius);

	pCircle->setColorIndex(color);
	pCircle->setLayer(layername);
	// 将实体添加到图形数据库
	AcDbObjectId circleId;
	circleId = CEnt::PostToModelSpace(pCircle);
	pCircle->getAcDbHandle(h);
	pCircle->close();
	return circleId;
}

//多段线：生成纵向钢筋，外沿，箍筋组，箍筋尾巴，箍筋示意图等实体
AcDbObjectId CPolyline::CreatePolyLine(const AcGePoint2dArray &points, double width, double arc, int color, const ACHAR* layername, bool close)
{
	//Array中点的数量
	int numVertices = points.length();
	//定义多段线
	AcDbPolyline *pPoly = new AcDbPolyline(numVertices);
	//将每个顶点添加到多段线中
	for (int i = 0; i < numVertices; i++)
	{
		//index,point,0:直线,1:半圆，>1:优弧,0~1:劣弧
		pPoly->addVertexAt(i, points.at(i), arc, width, width);
	}
	pPoly->setColorIndex(color);
	pPoly->setLayer(layername);
	pPoly->setClosed(close);
	AcDbObjectId PolyId;
	PolyId = CEnt::PostToModelSpace(pPoly);
	pPoly->close();
	return PolyId;
}
AcDbObjectId CPolyline::CreatePolyLineWithHandle(const AcGePoint2dArray &points, double width, double arc, int color, const ACHAR* layername, bool close, AcDbHandle &h)
{
	//Array中点的数量
	int numVertices = points.length();
	//定义多段线
	AcDbPolyline *pPoly = new AcDbPolyline(numVertices);
	//将每个顶点添加到多段线中
	for (int i = 0; i < numVertices; i++)
	{
		//index,point,0:直线,1:半圆，>1:优弧,0~1:劣弧
		pPoly->addVertexAt(i, points.at(i), arc, width, width);
	}
	pPoly->setColorIndex(color);
	pPoly->setLayer(layername);
	pPoly->setClosed(close);
	AcDbObjectId PolyId;
	PolyId = CEnt::PostToModelSpace(pPoly);
	pPoly->getAcDbHandle(h);
	pPoly->close();
	return PolyId;
}

double CPolyline::CalLength(const AcGePoint2dArray &points, bool close) {
	double Length = 0.0;
	int n = points.length();
	for (int i = 0; i < n-1; i++) {
		double L;
		L = sqrt((points[i].x - points[i + 1].x)*(points[i].x - points[i + 1].x) +
			(points[i].y - points[i + 1].y)*(points[i].y - points[i + 1].y));
		Length += L;
	}
	if (close == 1) {
		double L;
		L = sqrt((points[0].x - points[n - 1].x)*(points[0].x - points[n - 1].x) +
			(points[0].y - points[n - 1].y)*(points[0].y - points[n - 1].y));
		Length += L;
	}
	return Length;
}


//插入单行文字
AcDbObjectId CText::InsertText(const AcGePoint3d &Insertpt, const ACHAR *text, AcDbObjectId style
	/* = AcDbObjectId::kNull 即为默认字体样式*/, double height, double rotation, const ACHAR *layername)
{
	AcDbText *pText = new AcDbText(Insertpt, text, style, height, rotation);
	pText->setLayer(layername);
	pText->setWidthFactor(0.7);
	pText->kTextAlignmentMiddleCenter;//直接指定对齐方式
	return CEnt::PostToModelSpace(pText);
}

//插入多行文字
AcDbObjectId CText::InsertMText(const AcGePoint3d &Insertpt, const ACHAR *text, AcDbObjectId style
	, double height, double rotation, const ACHAR *layername, int Attach, double &Width)
{
	AcDbMText *pMText = new AcDbMText();
	pMText->setLayer(layername);
	pMText->setHeight(height);
	pMText->setTextHeight(height);
	pMText->setContents(text);
	pMText->setTextStyle(style);
	pMText->setLocation(Insertpt);
	pMText->setRotation(rotation);
	pMText->setAttachment(AcDbMText::AttachmentPoint(Attach));//基点
	/*kTopLeft    = 1,
	kTopCenter  = 2,
	kTopRight   = 3,
	kMiddleLeft = 4,
	kMiddleCenter   = 5,
	kMiddleRight    = 6,
	kBottomLeft = 7,
	kBottomCenter   = 8,
	kBottomRight    = 9,
*/
	Width = pMText->actualWidth();

	return CEnt::PostToModelSpace(pMText);
}


//插入转角标注
AcDbObjectId CDimension::CreateDimRotated(const AcGePoint3d &pt1, const AcGePoint3d &pt2, const AcGePoint3d
	&ptLine, double rotation, const ACHAR *dimText, const AcDbObjectId &dimStyle)
{
	AcDbRotatedDimension *pDim = new AcDbRotatedDimension(rotation, pt1, pt2, ptLine, dimText, dimStyle);
	return CEnt::PostToModelSpace(pDim);
}



