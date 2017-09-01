#pragma once

#include <DND.h>
using namespace DND;

struct cs_Info
{
	WCHAR wcsInfo[256];
};

//空应答
struct sc_Ok
{
};

//心跳包请求
struct cs_Beat
{
};

//心跳包回复
struct sc_Beat
{
};