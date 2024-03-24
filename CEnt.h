#pragma once
#include "StdAfx.h"
#include "resource.h"


class CEnt //所有【实体基本单元】的基类
{
protected://这里声明为protected保护，只允许直属子类访问改函数
	static AcDbObjectId PostToModelSpace(AcDbEntity *pEnt);//将实体导入模型空间
	CString Layername;//图层名
	int Color:9;//索引颜色，-1~255，只需要9位就行了(0~256)
	AcDbObjectId ID;
	AcDbHandle Handle;//默认64位int整型（8字节）
	inline auto Point(AcGePoint2d pt) { return AcGePoint3d(pt.x, pt.y, 0.0); }//用两个重载的内联函数做2d和3d点的转化
	inline auto Point(AcGePoint3d pt) { return AcGePoint2d(pt.x, pt.y); }
public:
	//所有实体的共有特性（类的成员变量）
	CEnt() :Color(1), Layername(_T("0")){}//先默认所有实体的颜色随层、图层为0层
	/*CEnt() {
		Color = 1;
		Layername = _T("0");
	}*/
	//接口函数
	const CString GetLayername() { return Layername;}//获取图层名
	const int GetEntColor() { return Color; }//获取实体颜色
	AcDbObjectId GetObjectId() { return ID; }//返回对象ID
	AcDbHandle GetObjectHandle() { return Handle; }//返回句柄
public:
	virtual ~CEnt(){}//虚析构函数（只服务于基类），
};


class CLine : public CEnt	//创建直线示例。有新功能要加（普遍的新功能）就重载，否则一律派生。
{
public:
	CLine(double x1, double y1, double x2, double y2, const ACHAR* layername, const ACHAR* LineType, int scale)
	 {
		 Assign(x1, y1, x2, y2, layername);
		 ID = CreateLineWithHandle(p1, p2, layername, LineType, scale, Handle);
	 }
	 CLine(double x1, double y1, double x2, double y2, const ACHAR* layername) {//构造函数重载(但不能调用其余构造函数）
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
		auto GetPt = [](double a, double b) {return AcGePoint3d(a, b, 0); };//此处用Lambda表达式简化代码
		p1 = GetPt(x1, y1); p2 = GetPt(x2, y2);
		p1_2d = Point(p1); p2_2d = Point(p2);
	}
public://提供成员变量副本用于右值访问
	const double GetLXCooor() { return Lx; }//获取左端点X坐标
	const double GetLYCooor() { return Ly; }//获取左端点Y坐标
	const double GetRXCooor() { return Rx; }//获取右端点X坐标
	const double GetRYCooor() { return Rx; }//获取右端点Y坐标
	const double GetLength() { return length; }//获取长度
	const AcGePoint3d GetLPoint3d() { return p1; }//获取3d左端点
	const AcGePoint3d GetRPoint3d() { return p2; }//获取3d右端点
	const AcGePoint2d GetLPoint2d() { return p1_2d; }//获取2d左端点
	const AcGePoint2d GetRPoint2d() { return p2_2d; }//获取2d右端点
	~CLine() {}
};

class CPolyline : public CEnt	//多段线
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
	int numVertices;//顶点个数
	double Length;
	double CalLength(const AcGePoint2dArray &points, bool close);
	AcDbObjectId CreatePolyLineWithHandle(const AcGePoint2dArray &points, double width, double arc, int color, const ACHAR* layername, bool close, AcDbHandle &h);
public:
	const int GetNumofVertices() { return numVertices; }
	const double GetTotalLength() { return Length; }
	~CPolyline() {}
};

class CCircle : public CEnt		//创建圆
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

//文字和标注暂时先不引用句柄
class CText : public CEnt//文字
{
public:
	CText(bool MT, const AcGePoint3d &Insertpt, const ACHAR *text, AcDbObjectId style, double height, double rotation, int Attach, const ACHAR *layername) {
		if (MT == 0) {//插入单行文字
			ID = InsertText(Insertpt, text, style, height, rotation, layername);
			WidthOfMText = 0.0;
		}
		else ID = InsertMText(Insertpt, text, style, height, rotation, layername, Attach, WidthOfMText);//插入多行文字
		Color = 4;
		Layername = layername;
		Text = text;
	}
	static AcDbObjectId InsertText(const AcGePoint3d &Insertpt, const ACHAR *text, AcDbObjectId style
		, double height, double rotation, const ACHAR *layername); //插入单行文字
	static AcDbObjectId InsertMText(const AcGePoint3d &Insertpt, const ACHAR *text, AcDbObjectId style
		, double height, double rotation, const ACHAR *layername, int Attach, double &Width);//插入多行文字
private:
	CString Text;
	double WidthOfMText;
public:
	CString GetText() { return Text; }
	double GetWidthOfMText() { return WidthOfMText; }
	~CText(){}
};

class CDimension : public CEnt //尺寸标注：默认为转角标注
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


class CPointInsert final: public CEnt //插入点，用模板元方法可以节约一点代码量
	//加final防止类被继续继承
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

class CEntOperate //可考虑模板方法模式：C++一种设计模式
{
private:
	virtual void EmptyFunction() = 0;//抽象类无法实例化

public://采用构造-析构手段分拆完成，中间过程让用户操作指针
	
	template<typename T>
	static void WriteEntityInSpace(AcDbHandle h, T *pE) {
		AcApDocument *pDoc0 = curDoc();
		acDocManager->lockDocument(pDoc0);
		AcDbBlockTableRecordPointer pBlkRcd0(pDoc0->database()->currentSpaceId(), AcDb::kForWrite);
		Acad::ErrorStatus es = pBlkRcd0.openStatus();
		if (Acad::eOk != es)
		{
			acutPrintf(_T("\n错误码= %s"), acadErrorStatusText(es));
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
				if (b == h) {//句柄对应则开始操作
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
	//可用模板元函数方案，但无法做虚函数
	template<typename S>
	static void PointerOperation(S *pE) {//以下为示例（直线），可根据不同类型做一系列子函数（move系列）
		CAcDbLineOpe::LineOpe(pE);
	}

	
};
/*virtual void PointerOperation(AcDbLine *pE) = 0;
	virtual void PointerOperation(AcDbPolyline *pE) = 0;
	virtual void PointerOperation(AcDbCircle *pE) = 0;
	virtual void PointerOperation(AcDbText *pE) = 0;
	virtual void PointerOperation(AcDbMText *pE) = 0;
	virtual void PointerOperation(AcDbBlockReference *pE) = 0;*/
//模板方法模式可以用子类继承方式进行

class CAcDbLineOpe//可以随便写一个小类对其进行操作
{
public:
	static void LineOpe(AcDbLine *pE) {//以下为示例（直线），可根据不同类型做一系列子函数（move系列）
		pE->setStartPoint(AcGePoint3d(0.0, 2000.0, 0.0));
		pE->setColorIndex(2);
	}
};