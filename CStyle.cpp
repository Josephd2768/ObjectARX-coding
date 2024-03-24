#include "stdafx.h"
#include "CStyle.h"


AcDbObjectId CLineType::LineType(const ACHAR* linename, const ACHAR* comments, int NumDashes, double DashLengthAt, double PatternLength) {//创建直线样式（例如点划线）
	AcDbLinetypeTable* pLinetypeTable = NULL;
	Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->getLinetypeTable(pLinetypeTable, AcDb::kForWrite);
	AcDbLinetypeTableRecord *pLinetypeTableRec = new AcDbLinetypeTableRecord;
	AcDbObjectId pLineStyleId;
	//设置线型名称注意,命名规范
	pLinetypeTableRec->setName(linename);//创建CENTER名称的点划线

	// 判断线性样式是否存在
	bool HasLineSt = pLinetypeTable->has(linename);
	if (HasLineSt == 1) {
		pLinetypeTable->getAt(linename, pLineStyleId);
		pLinetypeTable->close();
		pLinetypeTableRec->close();
		return pLineStyleId;
	}
	//线型说明文字高版本建议用setComments
	pLinetypeTableRec->setComments(comments);//e.g. _T("点划线")
	//设置Dash短划线数量
	pLinetypeTableRec->setNumDashes(NumDashes);//e.g. 5
	//设置短划线的长度，正值为实线，负值为空格，0是点
	pLinetypeTableRec->setDashLengthAt(0, DashLengthAt);//e.g. 5.0
	pLinetypeTableRec->setIsScaledToFit(0);
	//设置图案长度一般等于dashLength实际距离之和
	pLinetypeTableRec->setPatternLength(PatternLength);//e.g. 20.0
	pLinetypeTable->add(pLinetypeTableRec);
	pLineStyleId = pLinetypeTableRec->objectId();
	pLinetypeTableRec->close();
	pLinetypeTable->close();
	return pLineStyleId;
}

//创建文字样式
AcDbObjectId CTextStyle::TextStyle(const ACHAR* textname, const ACHAR* filename, const ACHAR* bigfontFile)
{
	//只写方式打开字体记录表中
	AcDbTextStyleTable *pTextStyleTable;
	acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTextStyleTable, AcDb::kForWrite);
	//新建一条字体样式记录
	AcDbTextStyleTableRecord *pTextRcd = new AcDbTextStyleTableRecord;
	//字体样式名称
	pTextRcd->setName(textname);
	AcDbObjectId pTextStyleId;
	// 判断字体样式是否存在
	bool HasTextSt = pTextStyleTable->has(textname);
	if (HasTextSt == 1) {
		pTextStyleTable->getAt(textname, pTextStyleId);
		pTextStyleTable->close();
		pTextRcd->close();
		return pTextStyleId;
	}
	//定义字体文件（寻找到autocad默认的字体文件路径）。也可以用setfont函数直接寻找windows字体
	pTextRcd->setFileName(filename);
	pTextRcd->setXScale(0.7);
	//pTextRcd->setTextSize(size);
	pTextRcd->setBigFontFileName(bigfontFile);
	pTextStyleTable->add(pTextRcd);
	pTextStyleId = pTextRcd->objectId();
	pTextRcd->close();
	pTextStyleTable->close();
	return pTextStyleId;
}

//创建标注样式：尺寸变量+引入文字样式
void CDimStyle::SetDimVar(AcDbDimStyleTableRecord *pDimRcd, double height, AcDbObjectId &TextSTId)
{
	pDimRcd->setDimasz(50);//箭头大小为50
	pDimRcd->setDimexe(30);//尺寸界线超过尺寸线距离为50
	pDimRcd->setDimexo(50);//设置尺寸界线的起点偏移量
	pDimRcd->setDimtxt(height);//设置文字高度
	pDimRcd->setDimtad(1);//设置文字位置-垂直为上方，水平默认为居中，不用设置
	pDimRcd->setDimgap(20);//设置文字位置-从尺寸线的偏移量
	pDimRcd->setDimtih(0);
	pDimRcd->setDimtix(0);//设置标注文字始终绘制在尺寸界线之间（否）
	pDimRcd->setDimtofl(1);//即使箭头放置于测量点之外，尺寸线也将绘制在测量点之间
	pDimRcd->setDimgap(30);//文字与标注线之间的间距
	pDimRcd->setDimdec(0);//设置保留小数点位数
	pDimRcd->setDimrnd(0);//设置保留小数点位数
	pDimRcd->setDimldrblk(_T("建筑标记"));//设置箭头类型
	pDimRcd->setDimblk1(_T("建筑标记"));
	pDimRcd->setDimblk2(_T("建筑标记"));
	pDimRcd->setDimblk(_T("建筑标记"));
	AcCmColor colort;//设置文字颜色
	colort.setColorIndex(4);
	pDimRcd->setDimclrt(colort);
	AcCmColor colord;//设置线的颜色
	colord.setColorIndex(1);
	pDimRcd->setDimclrd(colord);
	pDimRcd->setDimclre(colord);
	//TextSTId = CTextStyle::TextStyle(_T("ECADI_DIM"), _T("wltools.shx"), _T("hztxt.shx"));
	pDimRcd->setDimtxsty(TextSTId);
}
//创建标注样式
AcDbObjectId CDimStyle::SetDim(const ACHAR *DimStyle, double height, AcDbObjectId &TextSTId)
{
	AcDbDimStyleTable *pDimStTable;
	acdbHostApplicationServices()->workingDatabase()->getDimStyleTable(pDimStTable, AcDb::kForWrite);
	AcDbDimStyleTableRecord *pDimStTableRcd = new AcDbDimStyleTableRecord;
	pDimStTableRcd->setName(DimStyle);
	AcDbObjectId pDimStyleId;
	// 判断标注样式是否存在
	bool HasDimStyle = pDimStTable->has(DimStyle);
	if (HasDimStyle == 1) {
		pDimStTable->getAt(DimStyle, pDimStyleId);
		pDimStTable->close();
		pDimStTableRcd->close();
		return pDimStyleId;
	}
	SetDimVar(pDimStTableRcd, height, TextSTId);//将尺寸变量赋给这个指针
	pDimStTable->add(pDimStTableRcd);
	pDimStyleId = pDimStTableRcd->objectId();
	pDimStTableRcd->close();
	pDimStTable->close();
	return pDimStyleId;
}

//返回指定名称的标注样式
/*AcDbObjectId CStyle::GetDim(const ACHAR *DimStyle)
{
	AcDbDimStyleTable *pDimStTable;
	acdbHostApplicationServices()->workingDatabase()->getDimStyleTable(pDimStTable, AcDb::kForRead);
	AcDbDimStyleTableRecord *pDimStTableRcd = new AcDbDimStyleTableRecord;
	pDimStTableRcd->getName(DimStyle);
	AcDbObjectId pDimStyleId;
	// 判断标注样式是否存在
	bool HasDimStyle = pDimStTable->has(DimStyle);
	if (HasDimStyle == 1) {
		pDimStTable->getAt(DimStyle, pDimStyleId);
		pDimStTable->close();
		pDimStTableRcd->close();
		return pDimStyleId;
	}

	pDimStTableRcd->close();
	pDimStTable->close();

	return pDimStyleId;
}*/


void CLineType::ReloadLineType(const ACHAR* linename) {
	Acad::ErrorStatus es;
	AcDbDatabase *pDb =
		acdbHostApplicationServices()->workingDatabase();
	TCHAR szLtFile[MAX_PATH];
	if (RTNORM !=
		acedFindFile(_T("acadiso.lin"), szLtFile))
	{
		acutPrintf(ACRX_T("\\nLinetype file not  found !"));
		return;
	}

	AcDbLinetypeTable *pLtTable = NULL;
	es = pDb->getLinetypeTable(pLtTable, AcDb::kForRead);
	CString szLtype = linename;
	bool isLinetypeLoaded = pLtTable->has(szLtype);
	es = pLtTable->close();

	if (isLinetypeLoaded)
	{
		// Already loaded, try  reloading the linetype
		AcDbDatabase *pTempDatabase
			= new  AcDbDatabase(true, false);
		es = pTempDatabase->loadLineTypeFile
		(szLtype, szLtFile);
		if (Acad::eOk == es)
		{
			AcDbLinetypeTable *pTempLtTable;
			AcDbLinetypeTableRecord *pTempLtRec = NULL;

			es = pTempDatabase->getLinetypeTable(
				pTempLtTable, AcDb::kForRead);
			AcDbObjectId ltRecId = AcDbObjectId::kNull;
			es = pTempLtTable->getAt(szLtype, ltRecId);
			pTempLtTable->close();

			AcDbObjectIdArray objIdArray;
			objIdArray.append(ltRecId);

			AcDbIdMapping idMap;
			es = pDb->wblockCloneObjects(
				objIdArray,
				pDb->linetypeTableId(),
				idMap,
				AcDb::kDrcReplace);
			if (Acad::eOk == es)
			{
				acutPrintf(
					ACRX_T("\\nLinetype reloaded !"));
			}
			else
			{
				acutPrintf(
					ACRX_T("\\nSorry, could not  reload Linetype !"));
			}
		}
		else
		{
			acutPrintf(
				ACRX_T("\\nError loading linetype from  file !"));
		}
		delete pTempDatabase;
	}
	else
	{// Not  loaded, try  loading the linetype
		if (Acad::eOk ==
			pDb->loadLineTypeFile(szLtype, szLtFile))
		{
			acutPrintf(
				ACRX_T("\\nLinetype loaded from  file !"));
		}
		else
		{
			acutPrintf(
				ACRX_T("\\nError loading linetype from  file !"));
		}
	}
}