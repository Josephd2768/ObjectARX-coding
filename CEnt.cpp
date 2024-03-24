#include "stdafx.h"
#include "CEnt.h"


AcDbObjectId CEnt::PostToModelSpace(AcDbEntity *pEnt)
{
	//ֻ����ʽ�򿪻״̬��ģ�����ݿ������ָ�룩
	AcDbBlockTable *pTableEnt;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pTableEnt, AcDb::kForRead);
	//ֻд��ʽ��ģ�Ϳռ����¼
	AcDbBlockTableRecord *pRecordEnt;
	pTableEnt->getAt(ACDB_MODEL_SPACE, pRecordEnt, AcDb::kForWrite);
	//��������ӵ�����¼��
	AcDbObjectId EntID;
	pRecordEnt->appendAcDbEntity(EntID, pEnt);
	EntID = pRecordEnt->objectId();
	//�ͷ���Դ
	pTableEnt->close();
	pRecordEnt->close();
	pEnt->close();
	return EntID;
}


//����ֱ�߶���
AcDbObjectId CLine::CreateLine(AcGePoint3d PtStart, AcGePoint3d PtEnd, const ACHAR* layername, const ACHAR* LineType, int scale)		//����ʵ��
{
	AcDbLine *pLine = new AcDbLine(PtStart, PtEnd);
	pLine->setLineWeight(AcDb::kLnWtByLayer, TRUE);
	pLine->setColorIndex(-1);//-1ָ���
	pLine->setLayer(layername);
	pLine->setLinetype(LineType);
	pLine->setLinetypeScale(scale);//���ͱ���������5������CENTER����
	AcDbObjectId LineId;
	//��ʱ��ֱ�߶���������ڴ��ϴ�������û����ӵ�ͼ�����ݿ��У���˲����ܻ���ʾ��ͼ�δ����С�
	LineId = CEnt::PostToModelSpace(pLine);
	pLine->close();
	return LineId;
}
//����ֱ�߶������أ�
AcDbObjectId CLine::CreateLine(AcGePoint3d PtStart, AcGePoint3d PtEnd, const ACHAR* layername)		//����ʵ��
{
	return CreateLine(PtStart, PtEnd, layername, NULL, 1);
}

//����ֱ�߶���
AcDbObjectId CLine::CreateLineWithHandle(AcGePoint3d PtStart, AcGePoint3d PtEnd, const ACHAR* layername, const ACHAR* LineType, int scale, AcDbHandle &h)		//����ʵ��
{
	AcDbLine *pLine = new AcDbLine(PtStart, PtEnd);
	pLine->setLineWeight(AcDb::kLnWtByLayer, TRUE);
	pLine->setColorIndex(-1);//-1ָ���
	pLine->setLayer(layername);
	pLine->setLinetype(LineType);
	pLine->setLinetypeScale(scale);//���ͱ���������5������CENTER����
	AcDbObjectId LineId;
	//��ʱ��ֱ�߶���������ڴ��ϴ�������û����ӵ�ͼ�����ݿ��У���˲����ܻ���ʾ��ͼ�δ����С�
	LineId = CEnt::PostToModelSpace(pLine);
	pLine->getAcDbHandle(h);
	pLine->close();
	return LineId;
}

//��Բ
AcDbObjectId CCircle::CreateCircle(AcGePoint3d ptCenter, double radius, int color, const ACHAR* layername)
{
	AcGeVector3d vec(0.0, 0.0, 1.0);
	AcDbCircle *pCircle = new AcDbCircle(ptCenter, vec, radius);

	pCircle->setColorIndex(color);
	pCircle->setLayer(layername);
	// ��ʵ����ӵ�ͼ�����ݿ�
	AcDbObjectId circleId;
	circleId = CEnt::PostToModelSpace(pCircle);
	pCircle->close();
	return circleId;
}

//��Բ
AcDbObjectId CCircle::CreateCircleWithHandle(AcGePoint3d ptCenter, double radius, int color, const ACHAR* layername, AcDbHandle &h)
{
	AcGeVector3d vec(0.0, 0.0, 1.0);
	AcDbCircle *pCircle = new AcDbCircle(ptCenter, vec, radius);

	pCircle->setColorIndex(color);
	pCircle->setLayer(layername);
	// ��ʵ����ӵ�ͼ�����ݿ�
	AcDbObjectId circleId;
	circleId = CEnt::PostToModelSpace(pCircle);
	pCircle->getAcDbHandle(h);
	pCircle->close();
	return circleId;
}

//����ߣ���������ֽ���أ������飬����β�ͣ�����ʾ��ͼ��ʵ��
AcDbObjectId CPolyline::CreatePolyLine(const AcGePoint2dArray &points, double width, double arc, int color, const ACHAR* layername, bool close)
{
	//Array�е������
	int numVertices = points.length();
	//��������
	AcDbPolyline *pPoly = new AcDbPolyline(numVertices);
	//��ÿ��������ӵ��������
	for (int i = 0; i < numVertices; i++)
	{
		//index,point,0:ֱ��,1:��Բ��>1:�Ż�,0~1:�ӻ�
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
	//Array�е������
	int numVertices = points.length();
	//��������
	AcDbPolyline *pPoly = new AcDbPolyline(numVertices);
	//��ÿ��������ӵ��������
	for (int i = 0; i < numVertices; i++)
	{
		//index,point,0:ֱ��,1:��Բ��>1:�Ż�,0~1:�ӻ�
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


//���뵥������
AcDbObjectId CText::InsertText(const AcGePoint3d &Insertpt, const ACHAR *text, AcDbObjectId style
	/* = AcDbObjectId::kNull ��ΪĬ��������ʽ*/, double height, double rotation, const ACHAR *layername)
{
	AcDbText *pText = new AcDbText(Insertpt, text, style, height, rotation);
	pText->setLayer(layername);
	pText->setWidthFactor(0.7);
	pText->kTextAlignmentMiddleCenter;//ֱ��ָ�����뷽ʽ
	return CEnt::PostToModelSpace(pText);
}

//�����������
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
	pMText->setAttachment(AcDbMText::AttachmentPoint(Attach));//����
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


//����ת�Ǳ�ע
AcDbObjectId CDimension::CreateDimRotated(const AcGePoint3d &pt1, const AcGePoint3d &pt2, const AcGePoint3d
	&ptLine, double rotation, const ACHAR *dimText, const AcDbObjectId &dimStyle)
{
	AcDbRotatedDimension *pDim = new AcDbRotatedDimension(rotation, pt1, pt2, ptLine, dimText, dimStyle);
	return CEnt::PostToModelSpace(pDim);
}



