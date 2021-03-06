#include "usarts.h"


/////////////////////全局变量定义///////////////////////////////////////
uint8_t 	Sport_TxBuffer[S_PORT_TX_BUFF_LEN];
uint8_t 	Sport_RxBuffer[S_PORT_RX_BUFF_LEN];

uint8_t		dbg_TxBuffer[DBG_TX_BUF_LEN];
uint8_t		dbg_RxBuffer[DBG_TX_BUF_LEN];


uint8_t		sPortWriteBusyFlag;					//s-port写繁忙标志 
uint8_t		dbgWriteBusyFlag;					//调试口写繁忙标志


/***************************************************************************************************
 * @fn      uarts_regist
 *
 * @brief   串口资源注册
 * @param   NULL
 * @return  null
 ***************************************************************************************************/  
void uarts_regist(void)
{
	GPIO_InitTypeDef 	GPIO_InitStructure;
	NVIC_InitTypeDef    NVIC_InitStructure;
  	USART_InitTypeDef   USART_InitStructure;
  	DMA_InitTypeDef     DMA_InitStructure;
	uint32_t            temp;

	//USART_S_PORT_TxPin
	RCC_AHB1PeriphClockCmd(USART_S_PORT_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = USART_S_PORT_TxPin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(USART_S_PORT_GPIO, &GPIO_InitStructure);
	GPIO_PinAFConfig(USART_S_PORT_GPIO, USART_S_PORT_Tx_Pinsource, USART_S_PORT_GPIO_AF);

	//USART_S_PORT_RxPin
	GPIO_InitStructure.GPIO_Pin = USART_S_PORT_RxPin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(USART_S_PORT_GPIO, &GPIO_InitStructure);
	GPIO_PinAFConfig(USART_S_PORT_GPIO, USART_S_PORT_Rx_Pinsource, USART_S_PORT_GPIO_AF);

	//USARTdbg_TxPin
	RCC_AHB1PeriphClockCmd(USARTdbg_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = USARTdbg_TxPin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(USARTdbg_GPIO, &GPIO_InitStructure);
	GPIO_PinAFConfig(USARTdbg_GPIO, USARTdbg_Tx_Pinsource, USARTdbg_GPIO_AF);

	//USARTdbg_RxPin
	GPIO_InitStructure.GPIO_Pin = USARTdbg_RxPin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(USARTdbg_GPIO, &GPIO_InitStructure);
	GPIO_PinAFConfig(USARTdbg_GPIO, USARTdbg_Rx_Pinsource, USARTdbg_GPIO_AF);


	//---------------------USART_S_PORT串口功能配置---------------------
	//打开串口对应的外设时钟  
	RCC_APB1PeriphClockCmd(USART_S_PORT_CLK, ENABLE); 
#if 0
	//串口发DMA配置  
	//启动DMA时钟
	RCC_AHB1PeriphClockCmd(USART_S_PORT_DMA_CLK, ENABLE);

	//DMA1通道配置
	//关闭通道
	DMA_Cmd(USART_S_PORT_Tx_DMA_STREAM,DISABLE);
	DMA_DeInit(USART_S_PORT_Tx_DMA_STREAM);
	//通道地址
	DMA_InitStructure.DMA_Channel = USART_S_PORT_Tx_DMA_Channel;
	//外设地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART_S_PORT->DR);
	//内存地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Sport_TxBuffer;
	//dma传输方向单向
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	//设置DMA在传输时缓冲区的长度
	DMA_InitStructure.DMA_BufferSize = S_PORT_TX_BUFF_LEN;
	//设置DMA的外设递增模式，一个外设
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//设置DMA的内存递增模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//外设数据字长
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//内存数据字长
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	//设置DMA的传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	//设置DMA的优先级别
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(USART_S_PORT_Tx_DMA_STREAM,&DMA_InitStructure);
	//清中断
	DMA_ClearITPendingBit(USART_S_PORT_Tx_DMA_STREAM, USART_S_PORT_Tx_DMA_FLAG);
	//使能中断
	DMA_ITConfig(USART_S_PORT_Tx_DMA_STREAM,DMA_IT_TC,ENABLE);

	//串口收DMA配置  
	//关闭通道
	DMA_Cmd(USART_S_PORT_Rx_DMA_STREAM,DISABLE);
	DMA_DeInit(USART_S_PORT_Rx_DMA_STREAM);
	//通道地址
	DMA_InitStructure.DMA_Channel = USART_S_PORT_Rx_DMA_Channel;
	//外设地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART_S_PORT->DR);
	//内存地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Sport_RxBuffer;
	//dma传输方向单向
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	//设置DMA在传输时缓冲区的长度
	DMA_InitStructure.DMA_BufferSize = S_PORT_RX_BUFF_LEN;
	//设置DMA的外设递增模式，一个外设
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//设置DMA的内存递增模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//外设数据字长
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//内存数据字长
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	//设置DMA的传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	//设置DMA的优先级别
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(USART_S_PORT_Rx_DMA_STREAM,&DMA_InitStructure);
	DMA_Cmd(USART_S_PORT_Rx_DMA_STREAM,ENABLE);  
#endif

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//初始化串口 
	USART_Init(USART_S_PORT,&USART_InitStructure);  
	  
	//中断配置
	USART_ITConfig(USART_S_PORT,USART_IT_TC,DISABLE);  
	USART_ITConfig(USART_S_PORT,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART_S_PORT,USART_IT_IDLE,ENABLE);

	//配置UART中断  
	NVIC_InitStructure.NVIC_IRQChannel = USART_S_PORT_IRQn;                           //通道设置为串口1中断  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART_S_PORT_Rx_NVIC_PRIORITY;   //中断占先等级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;              //中断响应优先级0  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //打开中断  
	NVIC_Init(&NVIC_InitStructure);   
#if 0
	//DMA发送中断  
	NVIC_InitStructure.NVIC_IRQChannel = USART_S_PORT_Tx_DMA_IRQn;                        			//通道设置为串口1中断  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART_S_PORT_Tx_DMA_NVIC_PRIORITY;       //中断占先等级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;              //中断响应优先级0  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //打开中断  
	NVIC_Init(&NVIC_InitStructure);  
#endif
	//清中断
	temp = USART_S_PORT->SR;
	temp = USART_S_PORT->DR; //清USART_IT_IDLE标志
	  
	//采用DMA方式发送
//	USART_DMACmd(USART_S_PORT,USART_DMAReq_Tx,ENABLE);
	//  //采用DMA方式接收
//	USART_DMACmd(USART_S_PORT,USART_DMAReq_Rx,ENABLE);
	
	//启动串口  
	USART_Cmd(USART_S_PORT, ENABLE); 

	//---------------------USARTdbg串口功能配置---------------------
	//打开串口对应的外设时钟  
	RCC_APB1PeriphClockCmd(USARTdbg_CLK, ENABLE); 	
#if 0    
    //启动DMA时钟
	RCC_AHB1PeriphClockCmd(USARTdbg_DMA_CLK, ENABLE);
	//串口发DMA配置  
	//关闭通道
	DMA_Cmd(USARTdbg_Tx_DMA_STREAM,DISABLE);
	DMA_DeInit(USARTdbg_Tx_DMA_STREAM);
	//通道地址
	DMA_InitStructure.DMA_Channel = USARTdbg_Tx_DMA_Channel;
	//外设地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USARTdbg->DR);
	//内存地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)dbg_TxBuffer;
	//dma传输方向单向
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	//设置DMA在传输时缓冲区的长度
	DMA_InitStructure.DMA_BufferSize = DBG_TX_BUF_LEN;
	//设置DMA的外设递增模式，一个外设
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//设置DMA的内存递增模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//外设数据字长
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//内存数据字长
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	//设置DMA的传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	//设置DMA的优先级别
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(USARTdbg_Tx_DMA_STREAM,&DMA_InitStructure);
	//清中断
	DMA_ClearITPendingBit(USARTdbg_Tx_DMA_STREAM, USARTdbg_Tx_DMA_FLAG);
	//使能中断
	DMA_ITConfig(USARTdbg_Tx_DMA_STREAM,DMA_IT_TC,ENABLE);

	//串口收DMA配置  
	//关闭通道
	DMA_Cmd(USARTdbg_Rx_DMA_STREAM,DISABLE);
	DMA_DeInit(USARTdbg_Rx_DMA_STREAM);
	//通道地址
	DMA_InitStructure.DMA_Channel = USARTdbg_Rx_DMA_Channel;
	//外设地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USARTdbg->DR);
	//内存地址
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)dbg_RxBuffer;
	//dma传输方向单向
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	//设置DMA在传输时缓冲区的长度
	DMA_InitStructure.DMA_BufferSize = DBG_RX_BUF_LEN;
	//设置DMA的外设递增模式，一个外设
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	//设置DMA的内存递增模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//外设数据字长
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	//内存数据字长
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	//设置DMA的传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	//设置DMA的优先级别
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(USARTdbg_Rx_DMA_STREAM,&DMA_InitStructure);
	DMA_Cmd(USARTdbg_Rx_DMA_STREAM,ENABLE); 	
#endif
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//初始化串口 
	USART_Init(USARTdbg,&USART_InitStructure); 

	//中断配置
	USART_ITConfig(USARTdbg,USART_IT_TC,DISABLE);  
	USART_ITConfig(USARTdbg,USART_IT_RXNE,ENABLE);
	//USART_ITConfig(USARTdbg,USART_IT_IDLE,ENABLE);

	USART_ClearITPendingBit(USARTdbg, USART_IT_RXNE);
	//清中断
//	temp = USARTdbg->SR;
//	temp = USARTdbg->DR; //清USART_IT_IDLE标志

	//配置UART中断  
	NVIC_InitStructure.NVIC_IRQChannel = USARTdbg_IRQn;                         		//通道设置为串口1中断  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USARTdbg_Rx_NVIC_PRIORITY;   //中断占先等级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;              //中断响应优先级0  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //打开中断  
	NVIC_Init(&NVIC_InitStructure);  
#if 0
	//DMA发送中断  
	NVIC_InitStructure.NVIC_IRQChannel = USARTdbg_Tx_DMA_IRQn;                        			//通道设置为串口1中断  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USARTdbg_Tx_DMA_NVIC_PRIORITY;       //中断占先等级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;              //中断响应优先级0  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                 //打开中断  
	NVIC_Init(&NVIC_InitStructure);  	
#endif	 
	//采用DMA方式发送
//	USART_DMACmd(USARTdbg,USART_DMAReq_Tx,ENABLE);
	//采用DMA方式接收
//	USART_DMACmd(USARTdbg,USART_DMAReq_Rx,ENABLE);
	//启动串口  
	USART_Cmd(USARTdbg, ENABLE); 
}


/***************************************************************************************************
 * @fn      uarts_init
 *
 * @brief   串口资源初始化
 * @param   NULL
 * @return  null
 ***************************************************************************************************/  
void uarts_init(void)
{
	uarts_regist();	
}


/***************************************************************************************************
 * @fn      uarts_sport_send
 *
 * @brief   s-port串口发送
 * @param   str -- 发送数据
 *			len -- 数据长度
 * @return  0 -- success
 *          -1 - failure
 *          1 -- busy
 ***************************************************************************************************/  
int uarts_sport_send(uint8_t *Str, uint8_t len)
{
#if 0
    IF_PTR_IS_NULL_RET_NULLPTR_ERR(Str);
	if(len > S_PORT_TX_BUFF_LEN)
		return -1;

	if(sPortWriteBusyFlag)
		return 1;

	sPortWriteBusyFlag = 1;	

	memcpy(Sport_TxBuffer, Str, len);
	DMA_SetCurrDataCounter(USART_S_PORT_Tx_DMA_STREAM,len);
	//采用DMA方式发送
	USART_DMACmd(USART_S_PORT,USART_DMAReq_Tx,ENABLE);
	DMA_Cmd(USART_S_PORT_Tx_DMA_STREAM,ENABLE);

	return 0;
#endif
    uint16_t i;
    for(i=0; i<len; i++)
    {
        USART_SendData(USART_S_PORT, Str[i]);

        while (USART_GetFlagStatus(USART_S_PORT, USART_FLAG_TC) == RESET)
        {}
    }
}


/***************************************************************************************************
 * @fn      uarts_sport_tx_dma_irq_handler_callback
 *
 * @brief   s-port串口DMA发送中断回调
 * @param   null
 * @return  null
 ***************************************************************************************************/  
void uarts_sport_tx_dma_irq_handler_callback(void)
{
	if(DMA_GetITStatus(USART_S_PORT_Tx_DMA_STREAM, USART_S_PORT_Tx_DMA_FLAG) != RESET)
	{
		DMA_ClearITPendingBit(USART_S_PORT_Tx_DMA_STREAM, USART_S_PORT_Tx_DMA_FLAG);
		USART_DMACmd(USART_S_PORT,USART_DMAReq_Tx,DISABLE);
		DMA_Cmd(USART_S_PORT_Tx_DMA_STREAM,DISABLE);
		sPortWriteBusyFlag = 0;		
	}
}

/***************************************************************************************************
 * @fn      uarts_sport_irq_handler_callback
 *
 * @brief   s-port串口中断回调
 * @param   null
 * @return  null
 ***************************************************************************************************/ 
uint8_t     *pSport_RxBuffer = &Sport_RxBuffer[0];
uint16_t    g_sport_rx_len;
extern void uarts_sport_irq_handler_cb_hook(uint8_t *msg, uint16_t len);
void uarts_sport_irq_handler_callback(void)
{
	uint32_t temp = 0;
#if 0    
	uint16_t len = 0;
    uint8_t  RxBuffer[S_PORT_RX_BUFF_LEN];

	if(USART_GetITStatus(USART_S_PORT, USART_IT_IDLE) != RESET)
	{
		DMA_Cmd(USART_S_PORT_Rx_DMA_STREAM,DISABLE);

		temp = USART_S_PORT->SR;
		temp = USART_S_PORT->DR; //清USART_IT_IDLE标志  
		temp = temp;

		//接收字节数
		len = S_PORT_RX_BUFF_LEN - DMA_GetCurrDataCounter(USART_S_PORT_Rx_DMA_STREAM);

        memcpy(RxBuffer, Sport_RxBuffer, len);

		//设置传输数据长度
		DMA_SetCurrDataCounter(USART_S_PORT_Rx_DMA_STREAM,S_PORT_RX_BUFF_LEN);
		//打开DMA
		DMA_Cmd(USART_S_PORT_Rx_DMA_STREAM,ENABLE);

        uarts_sport_irq_handler_cb_hook(RxBuffer, len);
	} 
#endif

    if(USART_GetITStatus(USART_S_PORT, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART_S_PORT, USART_IT_RXNE);
        //*pSport_RxBuffer++ = USART_ReceiveData(USART_S_PORT);
        //g_sport_rx_len++;
        uarts_sport_irq_handler_cb_hook(USART_ReceiveData(USART_S_PORT), 1);
	}

#if 0    
    if(USART_GetITStatus(USART_S_PORT, USART_IT_IDLE) != RESET)
    {
        temp = USART_S_PORT->SR;
		temp = USART_S_PORT->DR; //清USART_IT_IDLE标志  
		temp = temp;

        uarts_sport_irq_handler_cb_hook(Sport_RxBuffer, g_sport_rx_len);

        pSport_RxBuffer = &Sport_RxBuffer[0];
        g_sport_rx_len = 0;
    }
#endif	
}


/***************************************************************************************************
 * @fn      uarts_dbg_send
 *
 * @brief   调试串口发送
 * @param   str -- 发送数据
 *			len -- 数据长度
 * @return  0 -- success
 *          -1 -- failure
 *          1 -- busy
 ***************************************************************************************************/  
int uarts_dbg_send(uint8_t *Str, uint16_t len)
{
#if 0
    IF_PTR_IS_NULL_RET_NULLPTR_ERR(Str);
    
	if(len > DBG_TX_BUF_LEN)
		return -1;
	
	if(dbgWriteBusyFlag)
		return 1;

	dbgWriteBusyFlag = 1;

	memcpy(dbg_TxBuffer, Str, len);
	DMA_SetCurrDataCounter(USARTdbg_Tx_DMA_STREAM,len);
	//采用DMA方式发送
	USART_DMACmd(USARTdbg,USART_DMAReq_Tx,ENABLE);
	DMA_Cmd(USARTdbg_Tx_DMA_STREAM,ENABLE);

	return 0;
#endif
    uint16_t i;
    for(i=0; i<len; i++)
    {
        USART_SendData(USARTdbg, Str[i]);

        while (USART_GetFlagStatus(USARTdbg, USART_FLAG_TC) == RESET)
        {}
    }

}


/***************************************************************************************************
 * @fn      uarts_dbg_tx_dma_irq_handler_callback
 *
 * @brief   调试串口DMA发送中断回调
 * @param   null
 * @return  null
 ***************************************************************************************************/  
void uarts_dbg_tx_dma_irq_handler_callback(void)
{
	if(DMA_GetITStatus(USARTdbg_Tx_DMA_STREAM, USARTdbg_Tx_DMA_FLAG) != RESET)
	{
		DMA_ClearITPendingBit(USARTdbg_Tx_DMA_STREAM, USARTdbg_Tx_DMA_FLAG);
		//在闭DMA方式发送
		USART_DMACmd(USARTdbg,USART_DMAReq_Tx,DISABLE);
		DMA_Cmd(USARTdbg_Tx_DMA_STREAM,DISABLE);
		dbgWriteBusyFlag = 0;		
	}
}

/***************************************************************************************************
 * @fn      uarts_dbg_irq_handler_callback
 *
 * @brief   s-port串口中断回调
 * @param   null
 * @return  null
 ***************************************************************************************************/ 
uint8_t  DGB_RxBuffer[DBG_RX_BUF_LEN];
uint8_t  *p_DBG_Rx = &DGB_RxBuffer[0];
uint16_t  g_dbg_rx_len;
extern void uarts_dbg_irq_handler_cb_hook(uint8_t *msg, uint16_t len);
void uarts_dbg_irq_handler_callback(void)
{
	uint32_t temp = 0;
//	uint16_t len = 0;
//    uint8_t  RxBuffer[DBG_RX_BUF_LEN];

	if(USART_GetITStatus(USARTdbg, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USARTdbg, USART_IT_RXNE);
        *p_DBG_Rx++ = USART_ReceiveData(USARTdbg);
        g_dbg_rx_len++;
	}
	
	if(USART_GetITStatus(USARTdbg, USART_IT_IDLE) != RESET)
	{
		//DMA_Cmd(USARTdbg_Rx_DMA_STREAM,DISABLE);

		temp = USARTdbg->SR;
		temp = USARTdbg->DR; //清USART_IT_IDLE标志  
		temp = temp;

		printf("enter usart3 idle interrupt!, rx len:%d.\r\n", g_dbg_rx_len);

        uarts_dbg_irq_handler_cb_hook(DGB_RxBuffer, g_dbg_rx_len);
        p_DBG_Rx = &DGB_RxBuffer[0];
        g_dbg_rx_len = 0;

		//接收字节数
		//len = DBG_RX_BUF_LEN - DMA_GetCurrDataCounter(USARTdbg_Rx_DMA_STREAM);

		//memcpy(RxBuffer, dbg_RxBuffer, len);

		//printf("receive msg len is %d \r\n", len);

		//设置传输数据长度
		//DMA_SetCurrDataCounter(USARTdbg_Rx_DMA_STREAM,DBG_RX_BUF_LEN);
		//打开DMA
		//DMA_Cmd(USARTdbg_Rx_DMA_STREAM,ENABLE);

       // uarts_dbg_irq_handler_cb_hook(RxBuffer, len);
	} 
}


//////////////end of file/////////////////////////////////////////////////////
