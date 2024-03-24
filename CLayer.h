#pragma once
class CLayer
{
public:
	CLayer(const ACHAR* layername, int color) {
		LayerId = CreateLayer(layername, color);
		name = layername;
	}
	static AcDbObjectId CreateLayer(const ACHAR* layername, int color);
public:
	AcDbObjectId LayerId;
	CString name;
};

class CLayerSec1//示例代码：初始化某一个图的层表
{
public:
	CLayerSec1() {
		CLayer *p1 = new CLayer(_T("围护结构"), 2);
		Structure = p1->name;
		StructureId = p1->LayerId;
		delete p1;
		CLayer *p2 = new CLayer(_T("支护标注"), 1);
		Dim = p2->name;
		DimId = p2->LayerId;
		delete p2;
		CLayer *p3 = new CLayer(_T("支护结构标识"), 4);
		Notation = p3->name;
		NotationId = p3->LayerId;
		delete p3;
		CLayer *p4 = new CLayer(_T("符号"), 1);
		Symbols = p4->name;
		SymbolsId = p4->LayerId;
		delete p4;
		CLayer *p5 = new CLayer(_T("文字"), 4);
		Text = p5->name;
		TextId = p5->LayerId;
		delete p5;
	}
public:
	CString Structure; AcDbObjectId StructureId;
	CString Dim; AcDbObjectId DimId;
	CString Notation; AcDbObjectId NotationId;
	CString Symbols; AcDbObjectId SymbolsId;
	CString Text; AcDbObjectId TextId;
};