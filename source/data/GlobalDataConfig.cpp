#include "stdafx.h"
#include "GlobalDataConfig.h"

using namespace com::guch::assistant::data;

/**
* 管线类型
**/
const wstring GlobalData::KIND_LINE = L"管线";
const wstring GlobalData::KIND_BLOCK = L"阻隔体";

/**
* 管道种类
**/
const wstring GlobalData::LINE_CATEGORY_SHANGSHUI = L"上水";
const wstring GlobalData::LINE_CATEGORY_XIASHUI = L"下水";
const wstring GlobalData::LINE_CATEGORY_NUANQI = L"暖气";
const wstring GlobalData::LINE_CATEGORY_DIANLAN = L"电缆";
const wstring GlobalData::LINE_CATEGORY_YUSUI = L"雨水";
const wstring GlobalData::LINE_CATEGORY_TONGXIN = L"通信";

/**
* 阻隔体种类
**/
const wstring GlobalData::BLOCK_CATEGORY_WUSHUI = L"污水管道";
const wstring GlobalData::BLOCK_CATEGORY_HANGDAO = L"巷道";
const wstring GlobalData::BLOCK_CATEGORY_NUANQIGUAN = L"暖气管";
const wstring GlobalData::BLOCK_CATEGORY_GUODAO = L"过道";
const wstring GlobalData::BLOCK_CATEGORY_CHEZHAN = L"车站";

/**
* 穿越方向
**/
const wstring GlobalData::BLOCK_CATEGORY_ABOVE = L"上方";
const wstring GlobalData::BLOCK_CATEGORY_BELLOW = L"下方";

/**
* 管道形状
**/
const wstring GlobalData::LINE_SHAPE_CIRCLE = L"圆形";
const wstring GlobalData::LINE_SHAPE_SQUARE = L"矩形";
const wstring GlobalData::LINE_SHAPE_GZQPD = L"拱直墙平底";
const wstring GlobalData::LINE_SHAPE_GZQYG = L"拱直墙仰拱";
const wstring GlobalData::LINE_SHAPE_QQMTX = L"曲墙(马蹄形)";

/**
* 管道单位
**/
const wstring GlobalData::LINE_UNIT_MM = L"毫米";
const wstring GlobalData::LINE_UNIT_CM = L"厘米";
const wstring GlobalData::LINE_UNIT_M = L"米";

/**
* 字符长度
**/
const UINT GlobalData::ITEM_TEXT_MAX_LENGTH = 250;

/**
* 出错标题
**/
const wstring GlobalData::ERROR_DIALOG_CAPTION = L"管线辅助系统告警";

/**
* 配置种类
**/
const wstring GlobalData::CONFIG_LINE_KIND = L"管线种类";
const wstring GlobalData::CONFIG_SHAPE_KIND = L"管线形状";
const wstring GlobalData::CONFIG_BLOCK_KIND = L"阻隔体种类";

/**
* 颜色配置
**/
const int GlobalData::LINE_COLOR = 0;
const int GlobalData::SAFELINE_COLOR = 1; //红色
const int GlobalData::INTERSET_COLOR = 1; //红色
const int GlobalData::INTERSET_WALLLINE_COLOR = 2; //黄色
const int GlobalData::WALLLINE_COLOR = 3; //绿色

/**
* 透明度
**/
const double GlobalData::TRANS_SAFELINE_COLOR = 128;
