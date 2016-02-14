#include "power.h"



/***************************************************************************************************
 * @fn      pwr_regist
 *
 * @brief   POWER资源注册
 * @param   NULL
 * @return  null
 ***************************************************************************************************/    
void pwr_regist(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //自动关机控制
	RCC_AHB1PeriphClockCmd(PWR_MCU_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = PWR_MCU_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(PWR_MCU_PORT, &GPIO_InitStructure);


    //内部RF模块PWR控制
	RCC_AHB1PeriphClockCmd(PWR_INT_RF_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = PWR_INT_RF_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(PWR_INT_RF_PORT, &GPIO_InitStructure);

    //外部RF模块PWR控制
	RCC_AHB1PeriphClockCmd(PWR_EXT_RF_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = PWR_EXT_RF_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(PWR_EXT_RF_PORT, &GPIO_InitStructure);
}


/***************************************************************************************************
 * @fn      pwr_init
 *
 * @brief   pwr设备初始化
 * @param   NULL
 * @return  null
 ***************************************************************************************************/  
void pwr_init(void)
{
    pwr_regist();
}


/***************************************************************************************************
 * @fn      pwr_up
 *
 * @brief   pwr设备上电
 * @param   NULL
 * @return  null
 ***************************************************************************************************/  
void pwr_on_off(PWR_MODULE_ENUM mod, PWR_ACTION_ENUM act)
{
    switch(mod)
    {
        case PWR_MODULE_MAIN:
            if(act == PWR_OFF)
            {
                GPIO_ResetBits(PWR_MCU_PORT, PWR_MCU_PIN);
            }
            else
            {
                GPIO_SetBits(PWR_MCU_PORT, PWR_MCU_PIN);
            }
            break;
        case PWR_MODULE_INT_RF:
            if(act == PWR_OFF)
            {
                GPIO_SetBits(PWR_INT_RF_PORT, PWR_INT_RF_PIN);
            }
            else
            {
                GPIO_ResetBits(PWR_INT_RF_PORT, PWR_INT_RF_PIN);
            }
            break;
        case PWR_MODULE_EXT_RF:
            if(act == PWR_OFF)
            {
                GPIO_ResetBits(PWR_EXT_RF_PORT, PWR_EXT_RF_PIN);
            }
            else
            {
                GPIO_SetBits(PWR_EXT_RF_PORT, PWR_EXT_RF_PIN);
            }
            break;
    }
}


//////////////end of file/////////////////////////////////////////////////////
