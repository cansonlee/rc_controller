#include "switches.h"




/***************************************************************************************************
 * @fn		switches_regist
 *
 * @brief	钮子开关资源注册
 * @param	NULL
 * @return	null
 ***************************************************************************************************/  
void switches_regist(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//拨动开关
	//SA
	RCC_AHB1PeriphClockCmd(SW_SA_H_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SW_SA_H_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(SW_SA_H_PORT, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(SW_SA_L_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SW_SA_L_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(SW_SA_L_PORT, &GPIO_InitStructure);

	//SB
	RCC_AHB1PeriphClockCmd(SW_SB_H_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SW_SB_H_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(SW_SB_H_PORT, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(SW_SB_L_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SW_SB_L_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(SW_SB_L_PORT, &GPIO_InitStructure);

	//SC
	RCC_AHB1PeriphClockCmd(SW_SC_H_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SW_SC_H_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(SW_SC_H_PORT, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(SW_SC_L_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SW_SC_L_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(SW_SC_L_PORT, &GPIO_InitStructure);

	//SD
	RCC_AHB1PeriphClockCmd(SW_SD_H_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SW_SD_H_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(SW_SD_H_PORT, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(SW_SD_L_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SW_SD_L_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(SW_SD_L_PORT, &GPIO_InitStructure);

	//SE
	RCC_AHB1PeriphClockCmd(SW_SE_H_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SW_SE_H_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(SW_SE_H_PORT, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(SW_SE_L_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = SW_SE_L_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(SW_SE_L_PORT, &GPIO_InitStructure);
}


 /***************************************************************************************************
 * @fn	  switches_init
 *
 * @brief	钮子开关初始化函数
 * @param	NULL
 * @return	NULL
 ***************************************************************************************************/
void swithes_init(void)
{
	switches_regist();
}


 /***************************************************************************************************
 * @fn	  switches_misc_sw_read
 *
 * @brief	拨档开关组合读取
 * @param	NULL
 * @return	key value
 ***************************************************************************************************/	
MISC_SW_VALUE switches_misc_sw_read(void)
{
	uint16_t misc_slide_switch;

	//SA
	if(GPIO_ReadInputDataBit(SW_SA_H_PORT, SW_SA_H_PIN) == RESET)
		misc_slide_switch |= (uint16_t)SLIDE_UP << SW_SA_L_SHIFT;
	else if(GPIO_ReadInputDataBit(SW_SA_L_PORT, SW_SA_L_PIN) == RESET)
		misc_slide_switch |= (uint16_t)SLIDE_DOWN << SW_SA_L_SHIFT;
	else
		misc_slide_switch |= (uint16_t)SLIDE_MID << SW_SA_L_SHIFT;

	//SB
	if(GPIO_ReadInputDataBit(SW_SB_H_PORT, SW_SB_H_PIN) == RESET)
		misc_slide_switch |= (uint16_t)SLIDE_UP << SW_SB_L_SHIFT;
	else if(GPIO_ReadInputDataBit(SW_SB_L_PORT, SW_SB_L_PIN) == RESET)
		misc_slide_switch |= (uint16_t)SLIDE_DOWN << SW_SB_L_SHIFT;
	else
		misc_slide_switch |= (uint16_t)SLIDE_MID << SW_SB_L_SHIFT;

	//SC
	if(GPIO_ReadInputDataBit(SW_SC_H_PORT, SW_SC_H_PIN) == RESET)
		misc_slide_switch |= (uint16_t)SLIDE_UP << SW_SC_L_SHIFT;
	else if(GPIO_ReadInputDataBit(SW_SC_L_PORT, SW_SC_L_PIN) == RESET)
		misc_slide_switch |= (uint16_t)SLIDE_DOWN << SW_SC_L_SHIFT;
	else
		misc_slide_switch |= (uint16_t)SLIDE_MID << SW_SC_L_SHIFT;

	//SD
	if(GPIO_ReadInputDataBit(SW_SD_H_PORT, SW_SD_H_PIN) == RESET)
		misc_slide_switch |= (uint16_t)SLIDE_UP << SW_SD_L_SHIFT;
	else if(GPIO_ReadInputDataBit(SW_SD_L_PORT, SW_SD_L_PIN) == RESET)
		misc_slide_switch |= (uint16_t)SLIDE_DOWN << SW_SD_L_SHIFT;
	else
		misc_slide_switch |= (uint16_t)SLIDE_MID << SW_SD_L_SHIFT;

	//SE
	if(GPIO_ReadInputDataBit(SW_SE_H_PORT, SW_SE_H_PIN) == RESET)
		misc_slide_switch |= (uint16_t)SLIDE_UP << SW_SE_L_SHIFT;
	else if(GPIO_ReadInputDataBit(SW_SE_L_PORT, SW_SE_L_PIN) == RESET)
		misc_slide_switch |= (uint16_t)SLIDE_DOWN << SW_SE_L_SHIFT;
	else
		misc_slide_switch |= (uint16_t)SLIDE_MID << SW_SE_L_SHIFT;

	return (MISC_SW_VALUE)misc_slide_switch;
}



