#include "stdafx.h"

#include <LineConfigDataManager.h>
#include <GlobalDataConfig.h>
#include <LMAUtils.h>

#include <iostream>
#include <fstream>

#include <assert.h>
#include <acutads.h>

using namespace com::guch::assistant::data;

#pragma warning(disable : 4267)
#pragma warning(disable : 4244)

namespace com
{

namespace guch
{

namespace assistant
{

namespace config
{

LineConfigDataManager* LineConfigDataManager::instance = NULL;
LPCWSTR LineConfigDataManager::LMA_CONFIG_FILE = L"C:\\管线改移辅助系统配置文件.txt";

wstring LineConfigDataManager::CONFIG_LINE_NAME = L"管线种类";
wstring LineConfigDataManager::CONFIG_SHAPE_NAME = L"管线形状";
wstring LineConfigDataManager::CONFIG_BLOCK_NAME = L"阻隔体种类";

LineConfigDataManager* LineConfigDataManager::Instance()
{
	if( instance == NULL )
	{
		instance = new LineConfigDataManager();
	}

	return instance;
}

LineConfigDataManager::LineConfigDataManager(void)
{
	mLineConfigData = new LineCommonConfigVector();

#ifdef _DEMO_DATA
	const int MAX_ITEM = 10;

	for( int i = 0; i < MAX_ITEM; i++)
	{
		CString ID;
		ID.Format(L"%d",i);

		CommonConfig* item = new CommonConfig(wstring(ID.GetBuffer()), 
						L"测试管道",
						GlobalData::KIND_LINE,
						GlobalData::LINE_CATEGORY_SHANGSHUI,
						GlobalData::LINE_SHAPE_CIRCLE,
						L"15",
						L"5",
						GlobalData::LINE_UNIT_CM,
						L"测试数据");

		mLineConfigData->push_back(item);
	}
#else
	CFile archiveFile;

	try
	{
		//read data from file LMA_CONFIG_FILE
		BOOL result = archiveFile.Open(LMA_CONFIG_FILE,CFile::modeRead);
		if( !result )
		{
			acutPrintf(L"\n打开管线类型配置文件失败.");
			return;
		}

		//得到文件内容长度
		int length = (ULONGLONG)archiveFile.GetLength()+1;

		//得到文件的窄字符内容
		char* content = new char[length];
		memset(content,0,length);
		archiveFile.Read(content,length);

		//将其转换为宽字符
		string strCnt(content,length);
		wstring wContent = StringToWString( strCnt );

		//查找回车以决定行
		size_t lineFrom = 0;
		size_t linePos = wContent.find_first_of(L"\n",lineFrom);

		wstring category;

		while( linePos != wstring::npos )
		{
			//得到一行数据
			wstring& wLine = wContent.substr(lineFrom, linePos-lineFrom);

			//将此行拆分
			size_t columnFrom = 0;
			size_t columnPos = wLine.find_first_of(L"\t",columnFrom);

			if( columnPos == wstring::npos )
			{
				//得到种类
				category = wLine;
			}
			else
			{
				CommonConfig* newItem = new CommonConfig();
				newItem->mCategory = category;

				int indexCol = 0;
				while( columnPos != wstring::npos )
				{
					//得到一个分段
					wstring& rColumn = wLine.substr(columnFrom,columnPos-columnFrom);

					//决定其属性
					if( indexCol == 0 )
						newItem->mName = rColumn;

					indexCol++;

					//继续下一个column
					columnFrom = columnPos + 1;
					columnPos =  wLine.find_first_of(L'\t',columnFrom);
				}

				if( indexCol == 1 )
				{
					wstring& subName = wLine.substr(columnFrom);
					newItem->mSubName = subName;

					acutPrintf(L"\n配置数据：");
					acutPrintf(L"\n种类【%s】名称【%s】子项【%s】", newItem->mCategory.c_str(), newItem->mName.c_str(), newItem->mSubName.c_str());

					mLineConfigData->push_back(newItem);
				}
			}

			//从下一个字符开始查找另外一行
			lineFrom = linePos + 1;
			linePos = wContent.find_first_of(L"\n",lineFrom + 1);
		}

		//关闭文件
		archiveFile.Close();
	}
	catch(exception& e)
	{
		acutPrintf(L"\n打开管线类型配置文件异常【%s】",e.what());
	}

#endif
}

LineConfigDataManager::~LineConfigDataManager(void)
{
}

LineCommonConfigVector* LineConfigDataManager::FindConfig( const wstring& category ) const
{
	LineCommonConfigVector* configLig = new LineCommonConfigVector();

	for( ConfigIterator iter = mLineConfigData->begin();
		iter != mLineConfigData->end();
		iter++)
	{
		//不是按管线配置的种类查找，而是按用户新建管线的类型查找
		if( (*iter)->mCategory.find(category) != wstring::npos )
		{
			configLig->push_back( (*iter) );
		}
	}

	return configLig;
}

} // end of data

} // end of assistant

} // end of guch

} // end of com
