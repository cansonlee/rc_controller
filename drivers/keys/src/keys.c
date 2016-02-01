#include "keys.h"
#include "delay_timer.h"

#include <stdlib.h>
#include <string.h>


/////////////////////全局变量定义///////////////////////////////////////



/***************************************************************************************************
 * @fn      keys_regist
 *
 * @brief   KEY资源注册
 * @param   NULL
 * @return  null
 ***************************************************************************************************/  
void keys_regist(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//MENU KEY
	RCC_AHB1PeriphResetCmd(KEY_MENU_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = KEY_MENU_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(KEY_MENU_PORT, &GPIO_InitStructure);

	//PAGE KEY
	RCC_AHB1PeriphResetCmd(KEY_PAGE_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = KEY_PAGE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(KEY_PAGE_PORT, &GPIO_InitStructure);

	//EXIT KEY
	RCC_AHB1PeriphResetCmd(KEY_EXIT_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = KEY_EXIT_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(KEY_EXIT_PORT, &GPIO_InitStructure);

	//PLUS KEY
	RCC_AHB1PeriphResetCmd(KEY_PLUS_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = KEY_PLUS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(KEY_PLUS_PORT, &GPIO_InitStructure);

	//MINUS KEY
	RCC_AHB1PeriphResetCmd(KEY_MINUS_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = KEY_MINUS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(KEY_MINUS_PORT, &GPIO_InitStructure);

	//ENTER KEY
	RCC_AHB1PeriphResetCmd(KEY_ENTER_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = KEY_ENTER_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(KEY_ENTER_PORT, &GPIO_InitStructure);	
}

/***************************************************************************************************
* @fn 	 keys_init
*
* @brief   按键初始化函数
* @param	 NULL
* @return  NULL
***************************************************************************************************/
void keys_init(void)
{
	keys_regist();
}

/***************************************************************************************************
* @fn 	 keys_scan
*
* @brief   KEY读取key值
* @param   NULL
* @return  key_value
***************************************************************************************************/	
KEY_VAL keys_scan(void)
{
	if(GPIO_ReadInputDataBit(KEY_MENU_PORT, KEY_MENU_PIN) == RESET)
		return KEY_MENU;
	if(GPIO_ReadInputDataBit(KEY_PAGE_PORT, KEY_PAGE_PIN) == RESET)
		return KEY_PAGE;
	if(GPIO_ReadInputDataBit(KEY_EXIT_PORT, KEY_EXIT_PIN) == RESET)
		return KEY_EXIT;
	if(GPIO_ReadInputDataBit(KEY_PLUS_PORT, KEY_PLUS_PIN) == RESET)
		return KEY_PLUS;
	if(GPIO_ReadInputDataBit(KEY_MINUS_PORT, KEY_MINUS_PIN) == RESET)
		return KEY_MINUS;
	if(GPIO_ReadInputDataBit(KEY_ENTER_PORT, KEY_ENTER_PIN) == RESET)
		return KEY_ENTER;

	return KEY_READY;
}


 /***************************************************************************************************
 * @fn	  keys_read
 *
 * @brief	  按键滤抖，>10ms运行一次
 * @param	  state -- 返回按键当前状态
 * @return	key_value
 ***************************************************************************************************/	 
KEY_VAL keys_read(KEY_STATUS *state)
{
	KEY_VAL			currentKeyValue;
	static KEY_VAL 	lastKeyVaule = KEY_READY;
	static uint8_t	keyTimer = 0;
	

	currentKeyValue = keys_scan();
	if(currentKeyValue == KEY_READY)
	{
		keyTimer = 0;
		*state = KEY_RELEASED;
		return KEY_READY;
	}
	else
	{
		if((lastKeyVaule == KEY_READY) || (lastKeyVaule != currentKeyValue))
		{
			lastKeyVaule = currentKeyValue;
			keyTimer = 0;
			*state = KEY_PRESSED;
			return KEY_READY;
		}
		else
		{
			keyTimer++;
			if(keyTimer > 1)
			{
				*state = KEY_PRESSED;
				return currentKeyValue;
			}
			else if(keyTimer > 10)		// >100ms 长按
			{
				*state = KEY_HOLD;
				return currentKeyValue;
			}
		}
			
	}
	
}


 //////////////end of file/////////////////////////////////////////////////////
