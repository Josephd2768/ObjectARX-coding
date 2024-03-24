#include "stdafx.h"
#include "CStyle.h"


AcDbObjectId CLineType::LineType(const ACHAR* linename, const ACHAR* comments, int NumDashes, double DashLengthAt, double PatternLength) {//����ֱ����ʽ������㻮�ߣ�
	AcDbLinetypeTable* pLinetypeTable = NULL;
	Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()->getLinetypeTable(pLinetypeTable, AcDb::kForWrite);
	AcDbLinetypeTableRecord *pLinetypeTableRec = new AcDbLinetypeTableRecord;
	AcDbObjectId pLineStyleId;
	//������������ע��,�����淶
	pLinetypeTableRec->setName(linename);//����CENTER���Ƶĵ㻮��

	// �ж�������ʽ�Ƿ����
	bool HasLineSt = pLinetypeTable->has(linename);
	if (HasLineSt == 1) {
		pLinetypeTable->getAt(linename, pLineStyleId);
		pLinetypeTable->close();
		pLinetypeTableRec->close();
		return pLineStyleId;
	}
	//����˵�����ָ߰汾������setComments
	pLinetypeTableRec->setComments(comments);//e.g. _T("�㻮��")
	//����Dash�̻�������
	pLinetypeTableRec->setNumDashes(NumDashes);//e.g. 5
	//���ö̻��ߵĳ��ȣ���ֵΪʵ�ߣ���ֵΪ�ո�0�ǵ�
	pLinetypeTableRec->setDashLengthAt(0, DashLengthAt);//e.g. 5.0
	pLinetypeTableRec->setIsScaledToFit(0);
	//����ͼ������һ�����dashLengthʵ�ʾ���֮��
	pLinetypeTableRec->setPatternLength(PatternLength);//e.g. 20.0
	pLinetypeTable->add(pLinetypeTableRec);
	pLineStyleId = pLinetypeTableRec->objectId();
	pLinetypeTableRec->close();
	pLinetypeTable->close();
	return pLineStyleId;
}

//����������ʽ
AcDbObjectId CTextStyle::TextStyle(const ACHAR* textname, const ACHAR* filename, const ACHAR* bigfontFile)
{
	//ֻд��ʽ�������¼����
	AcDbTextStyleTable *pTextStyleTable;
	acdbHostApplicationServices()->workingDatabase()->getTextStyleTable(pTextStyleTable, AcDb::kForWrite);
	//�½�һ��������ʽ��¼
	AcDbTextStyleTableRecord *pTextRcd = new AcDbTextStyleTableRecord;
	//������ʽ����
	pTextRcd->setName(textname);
	AcDbObjectId pTextStyleId;
	// �ж�������ʽ�Ƿ����
	bool HasTextSt = pTextStyleTable->has(textname);
	if (HasTextSt == 1) {
		pTextStyleTable->getAt(textname, pTextStyleId);
		pTextStyleTable->close();
		pTextRcd->close();
		return pTextStyleId;
	}
	//���������ļ���Ѱ�ҵ�autocadĬ�ϵ������ļ�·������Ҳ������setfont����ֱ��Ѱ��windows����
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

//������ע��ʽ���ߴ����+����������ʽ
void CDimStyle::SetDimVar(AcDbDimStyleTableRecord *pDimRcd, double height, AcDbObjectId &TextSTId)
{
	pDimRcd->setDimasz(50);//��ͷ��СΪ50
	pDimRcd->setDimexe(30);//�ߴ���߳����ߴ��߾���Ϊ50
	pDimRcd->setDimexo(50);//���óߴ���ߵ����ƫ����
	pDimRcd->setDimtxt(height);//�������ָ߶�
	pDimRcd->setDimtad(1);//��������λ��-��ֱΪ�Ϸ���ˮƽĬ��Ϊ���У���������
	pDimRcd->setDimgap(20);//��������λ��-�ӳߴ��ߵ�ƫ����
	pDimRcd->setDimtih(0);
	pDimRcd->setDimtix(0);//���ñ�ע����ʼ�ջ����ڳߴ����֮�䣨��
	pDimRcd->setDimtofl(1);//��ʹ��ͷ�����ڲ�����֮�⣬�ߴ���Ҳ�������ڲ�����֮��
	pDimRcd->setDimgap(30);//�������ע��֮��ļ��
	pDimRcd->setDimdec(0);//���ñ���С����λ��
	pDimRcd->setDimrnd(0);//���ñ���С����λ��
	pDimRcd->setDimldrblk(_T("�������"));//���ü�ͷ����
	pDimRcd->setDimblk1(_T("�������"));
	pDimRcd->setDimblk2(_T("�������"));
	pDimRcd->setDimblk(_T("�������"));
	AcCmColor colort;//����������ɫ
	colort.setColorIndex(4);
	pDimRcd->setDimclrt(colort);
	AcCmColor colord;//�����ߵ���ɫ
	colord.setColorIndex(1);
	pDimRcd->setDimclrd(colord);
	pDimRcd->setDimclre(colord);
	//TextSTId = CTextStyle::TextStyle(_T("ECADI_DIM"), _T("wltools.shx"), _T("hztxt.shx"));
	pDimRcd->setDimtxsty(TextSTId);
}
//������ע��ʽ
AcDbObjectId CDimStyle::SetDim(const ACHAR *DimStyle, double height, AcDbObjectId &TextSTId)
{
	AcDbDimStyleTable *pDimStTable;
	acdbHostApplicationServices()->workingDatabase()->getDimStyleTable(pDimStTable, AcDb::kForWrite);
	AcDbDimStyleTableRecord *pDimStTableRcd = new AcDbDimStyleTableRecord;
	pDimStTableRcd->setName(DimStyle);
	AcDbObjectId pDimStyleId;
	// �жϱ�ע��ʽ�Ƿ����
	bool HasDimStyle = pDimStTable->has(DimStyle);
	if (HasDimStyle == 1) {
		pDimStTable->getAt(DimStyle, pDimStyleId);
		pDimStTable->close();
		pDimStTableRcd->close();
		return pDimStyleId;
	}
	SetDimVar(pDimStTableRcd, height, TextSTId);//���ߴ�����������ָ��
	pDimStTable->add(pDimStTableRcd);
	pDimStyleId = pDimStTableRcd->objectId();
	pDimStTableRcd->close();
	pDimStTable->close();
	return pDimStyleId;
}

//����ָ�����Ƶı�ע��ʽ
/*AcDbObjectId CStyle::GetDim(const ACHAR *DimStyle)
{
	AcDbDimStyleTable *pDimStTable;
	acdbHostApplicationServices()->workingDatabase()->getDimStyleTable(pDimStTable, AcDb::kForRead);
	AcDbDimStyleTableRecord *pDimStTableRcd = new AcDbDimStyleTableRecord;
	pDimStTableRcd->getName(DimStyle);
	AcDbObjectId pDimStyleId;
	// �жϱ�ע��ʽ�Ƿ����
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