#include "display.h"
#include "global.h"
#include "font.h"


void Task_disp(void const * argument)
{
	dispCharacter(0,0,"H");
	
	dispCharStr(0,16,"Hello world!");

	while(1)
	{
	}
}


//--首页入口显示函数  显示舵量、电量、OSD



//--首页按键行为函数



//--ID2入口显示函数		显示设置项



//--ID2按键行为函数


//--ID3入口显示函数    进入设置项



//--ID3按键行为函数


//--ID4入口显示函数   设置值修改



//--ID4按键行为函数




//////////////end of file/////////////////////////////////////////////////////
