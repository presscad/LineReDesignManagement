// ------------------------------------------------
//                  LineManagementAssistant
// Copyright 2012-2013, Chengyong Yang & Changhai Gu. 
//               All rights reserved.
// ------------------------------------------------
//	ArxWrapper.h
//	written by Changhai Gu
// ------------------------------------------------
// $File:\\LineManageAssitant\main\source\intersect\LineIntersectManage.cpp $
// $Author: Changhai Gu $
// $DateTime: 2013/4/13 06:13:00
// $Revision: #1 $
// ------------------------------------------------
//

#include "stdafx.h"

#include "LineIntersectManage.h"

#include "acedads.h"
#include "accmd.h"
#include <adscodes.h>

#include <adsdlg.h>

#include <dbapserv.h>

#include <dbregion.h>

#include <gepnt3d.h>

//symbol table
#include <dbsymtb.h>

#include <acdocman.h>

//3D Object
#include <dbsol3d.h>

#include <ArxWrapper.h>

#include <ArxCustomObject.h>

#include <GlobalDataConfig.h>

#include <LMAUtils.h>

using namespace com::guch::assistant::arx;

namespace com
{

namespace guch
{

namespace assistant
{

namespace Intersect
{
LineIntersectManage* LineIntersectManage::mLineIntersectInstance = NULL;

LineIntersectManage* LineIntersectManage::Instance()
{
	if( mLineIntersectInstance == NULL )
		mLineIntersectInstance = new LineIntersectManage();

	return mLineIntersectInstance;
}

LineIntersectManage::LineIntersectManage()
	:mIntersectEntities()
	,m_pCheckLine(NULL)
{
}

LineIntersectManage::~LineIntersectManage()
{
	Clear();
}

void LineIntersectManage::Clear()
{
	for( int i = 0; i < mIntersectEntities.length(); i++ )
	{
		delete mIntersectEntities[i];
	}

	mIntersectEntities.removeAll();
}

void LineIntersectManage::CheckInteract()
{
	//�õ�����ʵ���ļ�������
	m_pCheckLine = LineEntityFileManager::GetCurrentLineEntryFile();

	acutPrintf(L"\n�Թ����ļ���%s�����������ж�.",m_pCheckLine->m_FileName.c_str());

	//ɾ����һ�ε��жϽ��
	Reset();

	//�Ը������߽����ж�
	CheckLineInteract(  );
}

void LineIntersectManage::CheckLineInteract()
{
	//����ѱȽϼ���
	m_CheckedEntities.clear();

	LineList* lineList = m_pCheckLine->GetList();

	if( lineList == NULL 
		|| lineList->size() <= 1 )
	{
		acutPrintf(L"\n��ǰ������ĿС��2�������Բ��������ּ��");
	}

	for( LineIterator line = lineList->begin();
			line != lineList->end();
			line++ )
	{
		PointList* pointList = (*line)->m_PointList;
		if( pointList == NULL 
			|| pointList->size() == 0 )
		{
			acutPrintf(L"\n��ǰ����û�����߶Σ����Բ����м��");
			continue;
		}

		for( PointIter point = pointList->begin();
				point != pointList->end();
				point++ )
		{
			if( (*point)->m_DbEntityCollection.mSequenceNO == 0 )
			{
				acutPrintf(L"\n��һ����û�й��ߣ���ֻ�Ǹ����.");
				continue;
			}

			//�뵱ǰ�ļ������е�����ֱ���������ж�
			CheckLineInteract( *point );

			//�����ѱȽ϶��У������ظ��Ƚ�
			m_CheckedEntities.insert( LinePointID((*point)->m_DbEntityCollection.mLineID, (*point)->m_DbEntityCollection.mSequenceNO) );
		}
	}
}

//�ж�һ�����߶����������ߵ��������
void LineIntersectManage::CheckLineInteract( PointEntity* checkPoint )
{
	wstring& lineName = checkPoint->m_DbEntityCollection.mLayerName;
	Adesk::Int32& checkLineID = checkPoint->m_DbEntityCollection.mLineID;
	Adesk::Int32& checkSeqNO = checkPoint->m_DbEntityCollection.mSequenceNO;

#ifdef DEBUG
	acutPrintf(L"\n�ԡ�%s���ĵڡ�%d�������������ж�.",lineName.c_str(), checkSeqNO);
#endif

	AcDbObjectId beCheckedSafeLine = checkPoint->m_DbEntityCollection.GetSafeLineEntity();
	AcDbEntity *pCheckSafeLine;
	Acad::ErrorStatus es = acdbOpenAcDbEntity(pCheckSafeLine, beCheckedSafeLine, AcDb::kForRead);

	if( es != Acad::eOk )
	{
		acutPrintf(L"\n�õ���������޵Ĺ��ߵİ�ȫ��Χʵ��ʱ����");
		rxErrorMsg(es);
	}

	if( pCheckSafeLine == NULL )
	{
		acutPrintf(L"\n��������û�����ݿ�ʵ��.");
		return;
	}

	LineList* lineList = m_pCheckLine->GetList();
	for( LineIterator line = lineList->begin();
			line != lineList->end();
			line++ )
	{
		PointList* pointList = (*line)->m_PointList;
		if( pointList == NULL 
			|| pointList->size() == 0 )
		{
			acutPrintf(L"\n��ǰ����û�����߶Σ�����Ҫ������߶ν��������ж�");
			continue;
		}

		for( PointIter point = pointList->begin();
				point != pointList->end();
				point++ )
		{
			Adesk::Int32& lineID = (*point)->m_DbEntityCollection.mLineID;
			Adesk::Int32& seqNO = (*point)->m_DbEntityCollection.mSequenceNO;
			if( seqNO == 0 )
			{
				acutPrintf(L"\n������ʼ�㣬����Ҫ�ж�");
				continue;
			}

			acutPrintf(L"\n�롾%s���ĵڡ�%d�������߶ν����ж�",(*point)->m_DbEntityCollection.mLayerName.c_str(), seqNO );

			if( lineID == checkLineID && abs( seqNO - checkSeqNO ) <= 1 )
			{
				acutPrintf(L"\n�����߶�,�����������ж�");
				continue;
			}

			if( m_CheckedEntities.find(LinePointID(lineID,seqNO)) != m_CheckedEntities.end() )
			{
				acutPrintf(L"\n�����߶��ѱ��ȽϹ�,����");
				continue;
			}

			//�õ������߶ε����ݿⰲȫ��Χ����
			AcDbObjectId safeLine = (*point)->m_DbEntityCollection.GetSafeLineEntity();
			AcDbEntity *pSafeLine;
			acdbOpenAcDbEntity(pSafeLine, safeLine, AcDb::kForRead);

			if( es != Acad::eOk )
			{
				acutPrintf(L"\n����������޵Ĺ��ߵİ�ȫ��Χʵ��ʱ����");
				rxErrorMsg(es);
			}

			//�õ�2�ߵ����ֵĲ�λ
			AcDb3dSolid* intersetObj = ArxWrapper::GetInterset( pSafeLine, pCheckSafeLine );
			
			if( intersetObj != NULL )
			{
				acutPrintf(L"\n�롾%s���ĵڡ�%d�������߶����ޣ�����Ϊ����ɫ��",(*point)->m_DbEntityCollection.mLayerName.c_str(), seqNO );

				//��������
				IntersectStruct* pIntersect = new IntersectStruct();
				pIntersect->intersetcA = checkPoint;
				pIntersect->intersetcB = (*point);

				//���صİ�ȫ��Χ���߿ɼ�������Ϊ��ɫ
				pSafeLine->upgradeOpen();
				pSafeLine->setColorIndex(1);
				pSafeLine->setVisibility(AcDb::kVisible);

				pCheckSafeLine->upgradeOpen();
				pCheckSafeLine->setColorIndex(1);
				pCheckSafeLine->setVisibility(AcDb::kVisible);

				intersetObj->setColorIndex(3);
				pIntersect->intersctcId = ArxWrapper::PostToModelSpace( intersetObj, lineName );

				//���ڻָ�����
				mIntersectEntities.append(pIntersect);
			}

			pSafeLine->close();
		}
	}

	pCheckSafeLine->close();
}

void LineIntersectManage::Reset()
{
	//�Ա���Ľ���н��д���
	for( int i = 0; i < mIntersectEntities.length(); i++ )
	{
		IntersectStruct* intersect = mIntersectEntities[i];

		//����ȫ��Χʵ������
		if( intersect->intersetcA )
		{
			AcDbObjectId intersectAId = intersect->intersetcA->m_DbEntityCollection.GetSafeLineEntity();
			ArxWrapper::ShowDbObject(intersectAId, AcDb::kInvisible );
		}

		if( intersect->intersetcB )
		{
			AcDbObjectId intersectBId = intersect->intersetcB->m_DbEntityCollection.GetSafeLineEntity();
			ArxWrapper::ShowDbObject(intersectBId, AcDb::kInvisible );
		}

		//ɾ���ཻ��ʵ��
		ArxWrapper::RemoveDbObject(intersect->intersctcId);
	}

	//��ս��
	Clear();
}

} // end of Intersect

} // end of assistant

} // end of guch

} // end of com