#include "eeprom.h"

void I2C_EE_PageWrite(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t NumByteToWrite);
void I2C_EE_WaitEepromStandbyState(void);


/***************************************************************************************************
 * @fn      eepromInit
 *
 * @brief   eeprom 资源初始化
 * @param   null
 * @return  NULL
 ***************************************************************************************************/ 
void eepromInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef  I2C_InitStructure;

	
	RCC_AHB1PeriphResetCmd(IIC_EEPROM_WP_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = IIC_EEPROM_WP_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(IIC_EEPROM_WP_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(IIC_EEPROM_WP_PORT, IIC_EEPROM_WP_PIN);

  	RCC_AHB1PeriphResetCmd(IIC_EEPROM_SCL_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = IIC_EEPROM_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(IIC_EEPROM_SCL_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(IIC_EEPROM_SCL_PORT, IIC_EEPROM_SCL_PIN_SOURCE, IIC_EEPROM_SCL_GPIO_AF);

	RCC_AHB1PeriphResetCmd(IIC_EEPROM_SDA_GPIO_CLK, ENABLE);
	GPIO_InitStructure.GPIO_Pin = IIC_EEPROM_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(IIC_EEPROM_SDA_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(IIC_EEPROM_SDA_PORT, IIC_EEPROM_SDA_PIN_SOURCE, IIC_EEPROM_SDA_GPIO_AF);

	RCC_APB1PeriphResetCmd(IIC_EEPROM_I2C_CLK, ENABLE);
	I2C_InitStructure.I2C_ClockSpeed = IIC_EEPROM_SPEED;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(IIC_EEPROM_I2C, &I2C_InitStructure);
	I2C_Cmd(IIC_EEPROM_I2C, ENABLE);
}

/***************************************************************************************************
 * @fn      I2C_CheckEventTimeout
 *
 * @brief   I2C响应超时检查
 * @param   event -- I2C_EVENT
 * @return  NULL
 ***************************************************************************************************/ 

uint8_t I2C_CheckEventTimeout(uint32_t event)
{
  uint32_t timeout = I2C_TIMEOUT_MAX;
  while (!I2C_CheckEvent(IIC_EEPROM_I2C, event)) {
    if ((timeout--) == 0) return 0;
  }
  return 1;
}

/**
  * @brief  Reads a block of data from the EEPROM.
  * @param  pBuffer : pointer to the buffer that receives the data read
  *   from the EEPROM.
  * @param  ReadAddr : EEPROM's internal address to read from.
  * @param  NumByteToRead : number of bytes to read from the EEPROM.
  * @retval None
  */
void eepromReadBlock(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead)
{
  while (I2C_CheckEvent(IIC_EEPROM_I2C, I2C_FLAG_BUSY));

  I2C_GenerateSTART(IIC_EEPROM_I2C, ENABLE);
  if (!I2C_CheckEventTimeout(I2C_EVENT_MASTER_MODE_SELECT))
    return;

  I2C_Send7bitAddress(IIC_EEPROM_I2C, I2C_ADDRESS_EEPROM, I2C_Direction_Transmitter);
  if (!I2C_CheckEventTimeout(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    return;

  I2C_SendData(IIC_EEPROM_I2C, (uint8_t)((ReadAddr & 0xFF00) >> 8));
  if (!I2C_CheckEventTimeout(I2C_EVENT_MASTER_BYTE_TRANSMITTING))
    return;
  I2C_SendData(IIC_EEPROM_I2C, (uint8_t)(ReadAddr & 0x00FF));
  if (!I2C_CheckEventTimeout(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    return;

  I2C_GenerateSTART(IIC_EEPROM_I2C, ENABLE);
  if (!I2C_CheckEventTimeout(I2C_EVENT_MASTER_MODE_SELECT))
    return;

  I2C_Send7bitAddress(IIC_EEPROM_I2C, I2C_ADDRESS_EEPROM, I2C_Direction_Receiver);
  if (!I2C_CheckEventTimeout(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    return;

  if (NumByteToRead > 1) {
    I2C_AcknowledgeConfig(IIC_EEPROM_I2C, ENABLE);
  }

  while (NumByteToRead) {
    if (NumByteToRead == 1) {
      I2C_AcknowledgeConfig(IIC_EEPROM_I2C, DISABLE);
    }
    if (!I2C_CheckEventTimeout(I2C_EVENT_MASTER_BYTE_RECEIVED))
      return;
    *pBuffer++ = I2C_ReceiveData(IIC_EEPROM_I2C);
    NumByteToRead--;
  }

  I2C_GenerateSTOP(IIC_EEPROM_I2C, ENABLE);
}

/**
  * @brief  Writes buffer of data to the I2C EEPROM.
  * @param  pBuffer : pointer to the buffer  containing the data to be
  *   written to the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @param  NumByteToWrite : number of bytes to write to the EEPROM.
  * @retval None
  */
void eepromWriteBlock(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t offset = WriteAddr % I2C_FLASH_PAGESIZE;
  uint8_t count = I2C_FLASH_PAGESIZE - offset;
  if (NumByteToWrite < I2C_FLASH_PAGESIZE)
    count = NumByteToWrite;
  while (count > 0) {
    I2C_EE_PageWrite(pBuffer, WriteAddr, count);
    I2C_EE_WaitEepromStandbyState();
    WriteAddr += count;
    pBuffer += count;
    NumByteToWrite -= count;
    count = I2C_FLASH_PAGESIZE;
    if (NumByteToWrite < I2C_FLASH_PAGESIZE)
      count = NumByteToWrite;
  }
}

/**
  * @brief  Writes more than one byte to the EEPROM with a single WRITE cycle.
  * @note   The number of byte can't exceed the EEPROM page size.
  * @param  pBuffer : pointer to the buffer containing the data to be
  *   written to the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @param  NumByteToWrite : number of bytes to write to the EEPROM.
  * @retval None
  */
void I2C_EE_PageWrite(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t NumByteToWrite)
{
  while (I2C_CheckEvent(IIC_EEPROM_I2C, I2C_FLAG_BUSY));

  I2C_GenerateSTART(IIC_EEPROM_I2C, ENABLE);
  if (!I2C_CheckEventTimeout(I2C_EVENT_MASTER_MODE_SELECT))
    return;

  I2C_Send7bitAddress(IIC_EEPROM_I2C, I2C_ADDRESS_EEPROM, I2C_Direction_Transmitter);
  if (!I2C_CheckEventTimeout(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    return;

  I2C_SendData(IIC_EEPROM_I2C, (uint8_t)((WriteAddr & 0xFF00) >> 8));
  if (!I2C_CheckEventTimeout(I2C_EVENT_MASTER_BYTE_TRANSMITTING))
    return;
  I2C_SendData(IIC_EEPROM_I2C, (uint8_t)(WriteAddr & 0x00FF));
  if (!I2C_CheckEventTimeout(I2C_EVENT_MASTER_BYTE_TRANSMITTING))
    return;

  /* While there is data to be written */
  while (NumByteToWrite--) {
    I2C_SendData(IIC_EEPROM_I2C, *pBuffer);
    if (!I2C_CheckEventTimeout(I2C_EVENT_MASTER_BYTE_TRANSMITTING))
      return;
    pBuffer++;
  }

  if (!I2C_CheckEventTimeout(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    return;

  I2C_GenerateSTOP(IIC_EEPROM_I2C, ENABLE);
}

/**
  * @brief  Wait for EEPROM Standby state
  * @param  None
  * @retval None
  */
void I2C_EE_WaitEepromStandbyState(void)
{
  do {
    I2C_GenerateSTART(IIC_EEPROM_I2C, ENABLE);
    if (!I2C_CheckEventTimeout(I2C_EVENT_MASTER_MODE_SELECT))
      return;

    I2C_Send7bitAddress(IIC_EEPROM_I2C, I2C_ADDRESS_EEPROM, I2C_Direction_Transmitter);
  } while (!I2C_CheckEventTimeout(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  I2C_GenerateSTOP(IIC_EEPROM_I2C, ENABLE);
}

//////////////end of file/////////////////////////////////////////////////////
