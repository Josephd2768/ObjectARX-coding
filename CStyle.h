#pragma once
#include "StdAfx.h"
#include "resource.h"
class CStyle
{
public:
	CStyle() {}
	virtual ~CStyle() {};
protected:
	CString name;
public:
	AcDbObjectId StyleId;
	CString GetStyleName() { return name; }
};

class CTextStyle :public CStyle 
{
public:
	CTextStyle() {};
	CTextStyle(const ACHAR* textname, const ACHAR* filename, const ACHAR* bigfontFile) {
		StyleId = TextStyle(textname, filename, bigfontFile);
		name = textname;
	}
	//创建文字样式，高度默认为0，待插入文字或者设定标注时可确定(size设定）
	static AcDbObjectId TextStyle(const ACHAR* textname, const ACHAR* filename, const ACHAR* bigfontFile);
};

class CLineType :public CStyle
{
public:
	CLineType(const ACHAR* linename, const ACHAR* comments, int NumDashes, double DashLengthAt, double PatternLength) {
		StyleId = LineType(linename, comments, NumDashes, DashLengthAt, PatternLength);
		name = linename;
	}
	//创建直线样式（例如点划线：LineType(_T("Center"), _T("点划线"), 5, 5.0, 20.0)）
	static AcDbObjectId LineType(const ACHAR* linename, const ACHAR* comments, int NumDashes, double DashLengthAt, double PatternLength);
	static void ReloadLineType(const ACHAR* linename);//重新加载线型
};

class CDimStyle : public CStyle
{
public:
	CDimStyle() {}
	//生成标注样式
	CDimStyle(const ACHAR *DimStyle, double height, AcDbObjectId &TextSTId) {
		StyleId = SetDim(DimStyle, height, TextSTId);
		name = DimStyle;
	}
	static AcDbObjectId SetDim(const ACHAR *DimStyle, double height, AcDbObjectId &TextSTId);
	//static AcDbObjectId GetDim(const ACHAR *DimStyle);返回标注样式
private:
	static void SetDimVar(AcDbDimStyleTableRecord *pDimRcd, double height, AcDbObjectId &TextSTId);//这里展示了一种标注样式例子
};

class CStyleSoilLayer //组合类示例：直接封装使用。这种指针形式更好，更节省空间。
{
public:
	CTextStyle *pTextStyle;
	CDimStyle *pDimStyle;
	
	CStyleSoilLayer() {
		//创建标注文字样式
		pTextStyle = new CTextStyle(_T("ECADI2"), _T("wltools.shx"), _T("hztxt.shx"));
		pDimStyle = new CDimStyle(_T("ECADI_DIM2"), 200.0, pTextStyle->StyleId);
	}
	~CStyleSoilLayer() {};
};

class CStyleWall //组合类示例：直接封装使用。
{
public:
	CTextStyle TextStyle;
	CDimStyle DimStyle;

	CStyleWall() {
		//创建标注文字样式
		TextStyle = CTextStyle(_T("ECADI2"), _T("wltools.shx"), _T("hztxt.shx"));
		DimStyle = CDimStyle(_T("ECADI_DIM2"), 200.0, TextStyle.StyleId);
	}
	~CStyleWall() {};
};
