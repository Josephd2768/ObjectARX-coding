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
	//����������ʽ���߶�Ĭ��Ϊ0�����������ֻ����趨��עʱ��ȷ��(size�趨��
	static AcDbObjectId TextStyle(const ACHAR* textname, const ACHAR* filename, const ACHAR* bigfontFile);
};

class CLineType :public CStyle
{
public:
	CLineType(const ACHAR* linename, const ACHAR* comments, int NumDashes, double DashLengthAt, double PatternLength) {
		StyleId = LineType(linename, comments, NumDashes, DashLengthAt, PatternLength);
		name = linename;
	}
	//����ֱ����ʽ������㻮�ߣ�LineType(_T("Center"), _T("�㻮��"), 5, 5.0, 20.0)��
	static AcDbObjectId LineType(const ACHAR* linename, const ACHAR* comments, int NumDashes, double DashLengthAt, double PatternLength);
	static void ReloadLineType(const ACHAR* linename);//���¼�������
};

class CDimStyle : public CStyle
{
public:
	CDimStyle() {}
	//���ɱ�ע��ʽ
	CDimStyle(const ACHAR *DimStyle, double height, AcDbObjectId &TextSTId) {
		StyleId = SetDim(DimStyle, height, TextSTId);
		name = DimStyle;
	}
	static AcDbObjectId SetDim(const ACHAR *DimStyle, double height, AcDbObjectId &TextSTId);
	//static AcDbObjectId GetDim(const ACHAR *DimStyle);���ر�ע��ʽ
private:
	static void SetDimVar(AcDbDimStyleTableRecord *pDimRcd, double height, AcDbObjectId &TextSTId);//����չʾ��һ�ֱ�ע��ʽ����
};

class CStyleSoilLayer //�����ʾ����ֱ�ӷ�װʹ�á�����ָ����ʽ���ã�����ʡ�ռ䡣
{
public:
	CTextStyle *pTextStyle;
	CDimStyle *pDimStyle;
	
	CStyleSoilLayer() {
		//������ע������ʽ
		pTextStyle = new CTextStyle(_T("ECADI2"), _T("wltools.shx"), _T("hztxt.shx"));
		pDimStyle = new CDimStyle(_T("ECADI_DIM2"), 200.0, pTextStyle->StyleId);
	}
	~CStyleSoilLayer() {};
};

class CStyleWall //�����ʾ����ֱ�ӷ�װʹ�á�
{
public:
	CTextStyle TextStyle;
	CDimStyle DimStyle;

	CStyleWall() {
		//������ע������ʽ
		TextStyle = CTextStyle(_T("ECADI2"), _T("wltools.shx"), _T("hztxt.shx"));
		DimStyle = CDimStyle(_T("ECADI_DIM2"), 200.0, TextStyle.StyleId);
	}
	~CStyleWall() {};
};
