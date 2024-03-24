#pragma once
#include "StdAfx.h"
#include "resource.h"


class CEnt //���С�ʵ�������Ԫ���Ļ���
{
protected://��������Ϊprotected������ֻ����ֱ��������ʸĺ���
	static AcDbObjectId PostToModelSpace(AcDbEntity *pEnt);//��ʵ�嵼��ģ�Ϳռ�
	CString Layername;//ͼ����
	int Color:9;//������ɫ��-1~255��ֻ��Ҫ9λ������(0~256)
	AcDbObjectId ID;
	AcDbHandle Handle;//Ĭ��64λint���ͣ�8�ֽڣ�
	inline auto Point(AcGePoint2d pt) { return AcGePoint3d(pt.x, pt.y, 0.0); }//���������ص�����������2d��3d���ת��
	inline auto Point(AcGePoint3d pt) { return AcGePoint2d(pt.x, pt.y); }
public:
	//����ʵ��Ĺ������ԣ���ĳ�Ա������
	CEnt() :Color(1), Layername(_T("0")){}//��Ĭ������ʵ�����ɫ��㡢ͼ��Ϊ0��
	/*CEnt() {
		Color = 1;
		Layername = _T("0");
	}*/
	//�ӿں���
	const CString GetLayername() { return Layername;}//��ȡͼ����
	const int GetEntColor() { return Color; }//��ȡʵ����ɫ
	AcDbObjectId GetObjectId() { return ID; }//���ض���ID
	AcDbHandle GetObjectHandle() { return Handle; }//���ؾ��
public:
	virtual ~CEnt(){}//������������ֻ�����ڻ��ࣩ��
};


class CLine : public CEnt	//����ֱ��ʾ�������¹���Ҫ�ӣ��ձ���¹��ܣ������أ�����һ��������
{
public:
	CLine(double x1, double y1, double x2, double y2, const ACHAR* layername, const ACHAR* LineType, int scale)
	 {
		 Assign(x1, y1, x2, y2, layername);
		 ID = CreateLineWithHandle(p1, p2, layername, LineType, scale, Handle);
	 }
	 CLine(double x1, double y1, double x2, double y2, const ACHAR* layername) {//���캯������(�����ܵ������๹�캯����
		Assign(x1, y1, x2, y2, layername);		
		ID = CreateLineWithHandle(p1, p2, layername, NULL, 1, Handle);
	 }
	 CLine(AcGePoint3d PtStart, AcGePoint3d PtEnd, const ACHAR* layername, const ACHAR* LineType, int scale) {
		 Assign(PtStart.x, PtStart.y, PtEnd.x, PtEnd.y, layername);		
		 ID = CreateLineWithHandle(p1, p2, layername, LineType, scale, Handle);
	 }
	 CLine(AcGePoint3d PtStart, AcGePoint3d PtEnd, const ACHAR* layername) {
		 Assign(PtStart.x, PtStart.y, PtEnd.x, PtEnd.y, layername);		 
		 ID = CreateLineWithHandle(p1, p2, layername, NULL, 1, Handle);
	 }
	static AcDbObjectId CreateLine(AcGePoint3d PtStart, AcGePoint3d PtEnd, const ACHAR* layername, const ACHAR* LineType, int scale);
	static AcDbObjectId CreateLine(AcGePoint3d PtStart, AcGePoint3d PtEnd, const ACHAR* layername);
private:
	double Lx, Ly, Rx, Ry;
	double length;
	AcGePoint3d p1, p2;
	AcGePoint2d p1_2d, p2_2d;
	AcDbObjectId CreateLineWithHandle(AcGePoint3d PtStart, AcGePoint3d PtEnd, const ACHAR* layername, const ACHAR* LineType, int scale, AcDbHandle &h);
	void Assign(double x1, double y1, double x2, double y2, CString layername) {
		Lx = x1; Ly = y1; Rx = x2; Ry = y2;
		length = sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
		Layername = layername;
		auto GetPt = [](double a, double b) {return AcGePoint3d(a, b, 0); };//�˴���Lambda���ʽ�򻯴���
		p1 = GetPt(x1, y1); p2 = GetPt(x2, y2);
		p1_2d = Point(p1); p2_2d = Point(p2);
	}
public://�ṩ��Ա��������������ֵ����
	const double GetLXCooor() { return Lx; }//��ȡ��˵�X����
	const double GetLYCooor() { return Ly; }//��ȡ��˵�Y����
	const double GetRXCooor() { return Rx; }//��ȡ�Ҷ˵�X����
	const double GetRYCooor() { return Rx; }//��ȡ�Ҷ˵�Y����
	const double GetLength() { return length; }//��ȡ����
	const AcGePoint3d GetLPoint3d() { return p1; }//��ȡ3d��˵�
	const AcGePoint3d GetRPoint3d() { return p2; }//��ȡ3d�Ҷ˵�
	const AcGePoint2d GetLPoint2d() { return p1_2d; }//��ȡ2d��˵�
	const AcGePoint2d GetRPoint2d() { return p2_2d; }//��ȡ2d�Ҷ˵�
	~CLine() {}
};

class CPolyline : public CEnt	//�����
{
public:
	CPolyline(const AcGePoint2dArray &points, double width, double arc, int color, const ACHAR* layername, bool close) {
		ID = CreatePolyLineWithHandle(points, width, arc, color, layername, close, Handle);
		numVertices = points.length();
		Color = color;
		Layername = layername;
		Length = CalLength(points, close);
	}
	static AcDbObjectId CreatePolyLine(const AcGePoint2dArray &points, double width, double arc, int color, const ACHAR* layername, bool close);
private:	
	int numVertices;//�������
	double Length;
	double CalLength(const AcGePoint2dArray &points, bool close);
	AcDbObjectId CreatePolyLineWithHandle(const AcGePoint2dArray &points, double width, double arc, int color, const ACHAR* layername, bool close, AcDbHandle &h);
public:
	const int GetNumofVertices() { return numVertices; }
	const double GetTotalLength() { return Length; }
	~CPolyline() {}
};

class CCircle : public CEnt		//����Բ
{
public:
	CCircle(AcGePoint3d ptCenter, double radius, int color, const ACHAR* layername) {
		ID = CreateCircleWithHandle(ptCenter, radius, color, layername, Handle);
		Color = color;
		Layername = layername;
		Center = ptCenter;
		R = radius;
	}
	static AcDbObjectId CreateCircle(AcGePoint3d ptCenter, double radius, int color, const ACHAR* layername);
private:
	AcGePoint3d Center;
	double R;
	AcDbObjectId CreateCircleWithHandle(AcGePoint3d ptCenter, double radius, int color, const ACHAR* layername, AcDbHandle &h);
public:
	const AcGePoint3d GetCenterPt() { return Center; }
	const double GetRadius() { return R; }
	~CCircle() {}
};

//���ֺͱ�ע��ʱ�Ȳ����þ��
class CText : public CEnt//����
{
public:
	CText(bool MT, const AcGePoint3d &Insertpt, const ACHAR *text, AcDbObjectId style, double height, double rotation, int Attach, const ACHAR *layername) {
		if (MT == 0) {//���뵥������
			ID = InsertText(Insertpt, text, style, height, rotation, layername);
			WidthOfMText = 0.0;
		}
		else ID = InsertMText(Insertpt, text, style, height, rotation, layername, Attach, WidthOfMText);//�����������
		Color = 4;
		Layername = layername;
		Text = text;
	}
	static AcDbObjectId InsertText(const AcGePoint3d &Insertpt, const ACHAR *text, AcDbObjectId style
		, double height, double rotation, const ACHAR *layername); //���뵥������
	static AcDbObjectId InsertMText(const AcGePoint3d &Insertpt, const ACHAR *text, AcDbObjectId style
		, double height, double rotation, const ACHAR *layername, int Attach, double &Width);//�����������
private:
	CString Text;
	double WidthOfMText;
public:
	CString GetText() { return Text; }
	double GetWidthOfMText() { return WidthOfMText; }
	~CText(){}
};

class CDimension : public CEnt //�ߴ��ע��Ĭ��Ϊת�Ǳ�ע
{
public:
	CDimension(const AcGePoint3d &pt1, const AcGePoint3d &pt2, const AcGePoint3d&ptLine,
		double rotation, const AcDbObjectId &dimStyle, const ACHAR *dimText) {
		ID = CreateDimRotated(pt1, pt2, ptLine, rotation, dimText, dimStyle);
	}
	static AcDbObjectId CreateDimRotated(const AcGePoint3d &pt1, const AcGePoint3d &pt2, const AcGePoint3d
		&ptLine, double rotation, const ACHAR *dimText, const AcDbObjectId &dimStyle);
	~CDimension(){}
};


class CPointInsert final: public CEnt //����㣬��ģ��Ԫ�������Խ�Լһ�������
	//��final��ֹ�౻�����̳�
{
public:
	template<typename R>
	static AcGePoint3d InsertRelativePoint3d(R a, double dx, double dy) {
		AcGePoint3d pt; 
		if (typeid(AcGePoint3d) == typeid(a) || typeid(AcGePoint2d) == typeid(a)) { pt.x = a.x + dx; pt.y = a.y + dy; }
		return pt;
	}
	template<typename S>
	static AcGePoint2d InsertRelativePoint2d(S a, double dx, double dy) {
		AcGePoint2d pt2;
		if (typeid(AcGePoint3d) == typeid(a) || typeid(AcGePoint2d) == typeid(a)) { pt2.x = a.x + dx; pt2.y = a.y + dy; }
		return pt2;
	}
};

class CEntOperate //�ɿ���ģ�巽��ģʽ��C++һ�����ģʽ
{
private:
	virtual void EmptyFunction() = 0;//�������޷�ʵ����

public://���ù���-�����ֶηֲ���ɣ��м�������û�����ָ��
	
	template<typename T>
	static void WriteEntityInSpace(AcDbHandle h, T *pE) {
		AcApDocument *pDoc0 = curDoc();
		acDocManager->lockDocument(pDoc0);
		AcDbBlockTableRecordPointer pBlkRcd0(pDoc0->database()->currentSpaceId(), AcDb::kForWrite);
		Acad::ErrorStatus es = pBlkRcd0.openStatus();
		if (Acad::eOk != es)
		{
			acutPrintf(_T("\n������= %s"), acadErrorStatusText(es));
			return;
		}

		AcDbBlockTableRecordIterator *it = NULL;
		pBlkRcd0->newIterator(it);
		for (it->start(); !it->done(); it->step())
		{
			AcDbEntity *pEnt0 = NULL;
			Acad::ErrorStatus es = it->getEntity(pEnt0, AcDb::kForWrite);

			if (es == Acad::eOk)
			{
				AcDbHandle b;
				pEnt0->getAcDbHandle(b); 
				if (b == h) {//�����Ӧ��ʼ����
					if (pEnt0->isKindOf(T::desc())) {
						pE = T::cast(pEnt0);
						PointerOperation(pE);
						pE->close();
					}
				}
				pEnt0->close();
			}
			else pEnt0->close();
		}
		delete it;
		pBlkRcd0->close();
		acedUpdateDisplay();
	}
	//����ģ��Ԫ�������������޷����麯��
	template<typename S>
	static void PointerOperation(S *pE) {//����Ϊʾ����ֱ�ߣ����ɸ��ݲ�ͬ������һϵ���Ӻ�����moveϵ�У�
		CAcDbLineOpe::LineOpe(pE);
	}

	
};
/*virtual void PointerOperation(AcDbLine *pE) = 0;
	virtual void PointerOperation(AcDbPolyline *pE) = 0;
	virtual void PointerOperation(AcDbCircle *pE) = 0;
	virtual void PointerOperation(AcDbText *pE) = 0;
	virtual void PointerOperation(AcDbMText *pE) = 0;
	virtual void PointerOperation(AcDbBlockReference *pE) = 0;*/
//ģ�巽��ģʽ����������̳з�ʽ����

class CAcDbLineOpe//�������дһ��С�������в���
{
public:
	static void LineOpe(AcDbLine *pE) {//����Ϊʾ����ֱ�ߣ����ɸ��ݲ�ͬ������һϵ���Ӻ�����moveϵ�У�
		pE->setStartPoint(AcGePoint3d(0.0, 2000.0, 0.0));
		pE->setColorIndex(2);
	}
};