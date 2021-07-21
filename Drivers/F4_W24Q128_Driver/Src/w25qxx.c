#include "w25qxx.h"
#include "base.h"
#include "spi.h"
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//W25QXX 驱动代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

u16 W25QXX_TYPE = W25Q64; //默认是W25Q128

//4Kbytes为一个Sector
//16个扇区为1个Block
//W25Q128
//容量为16M字节,共有128个Block,4096个Sector

//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{
	while (__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_TXE) == RESET);

	SPI1->DR = TxData;

	while (__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_RXNE) == RESET); //等待接收完一个byte

	return SPI1->DR; //返回通过SPIx最近接收的数据
}
//以下是SPI模块的初始化代码，配置成主机模式
//SPI口初始化
//这里针是对SPI1的初始化
void SPI1_Init(void)
{
	// GPIO_InitTypeDef GPIO_InitStructure;
	// SPI_InitTypeDef SPI_InitStructure;

	// RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能GPIOA时钟
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);  //使能SPI1时钟

	// //GPIOFB3,4,5初始化设置
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5; //PB3~5复用功能输出
	// GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;						//复用功能
	// GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;						//推挽输出
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;					//100MHz
	// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;						//上拉
	// GPIO_Init(GPIOB, &GPIO_InitStructure);								//初始化

	// GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1); //PB3复用为 SPI1
	// GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1); //PB4复用为 SPI1
	// GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1); //PB5复用为 SPI1

	// //这里只针对SPI口初始化
	// RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, ENABLE);  //复位SPI1
	// RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE); //停止复位SPI1

	// SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	 //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	// SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						 //设置SPI工作模式:设置为主SPI
	// SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					 //设置SPI的数据大小:SPI发送接收8位帧结构
	// SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;							 //串行同步时钟的空闲状态为高电平
	// SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;						 //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	// SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	// SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; //定义波特率预分频的值:波特率预分频值为256
	// SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					 //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	// SPI_InitStructure.SPI_CRCPolynomial = 7;							 //CRC值计算的多项式
	// SPI_Init(SPI1, &SPI_InitStructure);									 //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

	// SPI_Cmd(SPI1, ENABLE); //使能SPI外设

  //以上内容都已通过CubeMX配置
  __HAL_SPI_ENABLE(&hspi1); //使能SPI1
	SPI1_ReadWriteByte(0xff); //启动传输
}
//SPI1速度设置函数
//SPI速度=fAPB2/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256
//fAPB2时钟一般为84Mhz：
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  __HAL_SPI_DISABLE(&hspi1);                    //??SPI1
	SPI1->CR1 &= 0XFFC7;											//位3-5清零，用来设置波特率
	SPI1->CR1 |= SPI_BaudRatePrescaler;								//设置SPI1速度
  __HAL_SPI_ENABLE(&hspi1);                    //??SPI1
}

//初始化SPI FLASH的IO口
void W25QXX_Init(void)
{
//	GPIO_InitTypeDef GPIO_InitStructure;
//
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //使能GPIOB时钟
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE); //使能GPIOG时钟
//
//	//GPIOB14
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;		   //PB14
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	   //输出
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	   //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100MHz
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   //上拉
//	GPIO_Init(GPIOB, &GPIO_InitStructure);			   //初始化
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //PG7
//	GPIO_Init(GPIOG, &GPIO_InitStructure);	  //初始化

	PGout(7)=1;		//PG7输出1,防止NRF干扰SPI FLASH的通信
	W25QXX_CS = 1;							//SPI FLASH不选中
	SPI1_Init();							//初始化SPI
	SPI1_SetSpeed(SPI_BAUDRATEPRESCALER_4); //设置为42M时钟,高速模式
	W25QXX_TYPE = W25QXX_ReadID();			//读取FLASH ID.
}

//读取W25QXX的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
u8 W25QXX_ReadSR(void)
{
	u8 byte = 0;
	W25QXX_CS = 0;							//使能器件
	SPI1_ReadWriteByte(W25X_ReadStatusReg); //发送读取状态寄存器命令
	byte = SPI1_ReadWriteByte(0Xff);		//读取一个字节
	W25QXX_CS = 1;							//取消片选
	return byte;
}
//写W25QXX状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
void W25QXX_Write_SR(u8 sr)
{
	W25QXX_CS = 0;							 //使能器件
	SPI1_ReadWriteByte(W25X_WriteStatusReg); //发送写取状态寄存器命令
	SPI1_ReadWriteByte(sr);					 //写入一个字节
	W25QXX_CS = 1;							 //取消片选
}
//W25QXX写使能
//将WEL置位
void W25QXX_Write_Enable(void)
{
	W25QXX_CS = 0;						  //使能器件
	SPI1_ReadWriteByte(W25X_WriteEnable); //发送写使能
	W25QXX_CS = 1;						  //取消片选
}
//W25QXX写禁止
//将WEL清零
void W25QXX_Write_Disable(void)
{
	W25QXX_CS = 0;						   //使能器件
	SPI1_ReadWriteByte(W25X_WriteDisable); //发送写禁止指令
	W25QXX_CS = 1;						   //取消片选
}
//读取芯片ID
//返回值如下:
//0XEF13,表示芯片型号为W25Q80
//0XEF14,表示芯片型号为W25Q16
//0XEF15,表示芯片型号为W25Q32
//0XEF16,表示芯片型号为W25Q64
//0XEF17,表示芯片型号为W25Q128
u16 W25QXX_ReadID(void)
{
	u16 Temp = 0;
	W25QXX_CS = 0;
	SPI1_ReadWriteByte(0x90); //发送读取ID命令
	SPI1_ReadWriteByte(0x00);
	SPI1_ReadWriteByte(0x00);
	SPI1_ReadWriteByte(0x00);
	Temp |= SPI1_ReadWriteByte(0xFF) << 8;
	Temp |= SPI1_ReadWriteByte(0xFF);
	W25QXX_CS = 1;
	return Temp;
}
//读取SPI FLASH
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void W25QXX_Read(u8 *pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
	u16 i;
	W25QXX_CS = 0;								//使能器件
	SPI1_ReadWriteByte(W25X_ReadData);			//发送读取命令
	SPI1_ReadWriteByte((u8)((ReadAddr) >> 16)); //发送24bit地址
	SPI1_ReadWriteByte((u8)((ReadAddr) >> 8));
	SPI1_ReadWriteByte((u8)ReadAddr);
	for (i = 0; i < NumByteToRead; i++)
	{
		pBuffer[i] = SPI1_ReadWriteByte(0XFF); //循环读数
	}
	W25QXX_CS = 1;
}
//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!
void W25QXX_Write_Page(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u16 i;
	W25QXX_Write_Enable();						 //SET WEL
	W25QXX_CS = 0;								 //使能器件
	SPI1_ReadWriteByte(W25X_PageProgram);		 //发送写页命令
	SPI1_ReadWriteByte((u8)((WriteAddr) >> 16)); //发送24bit地址
	SPI1_ReadWriteByte((u8)((WriteAddr) >> 8));
	SPI1_ReadWriteByte((u8)WriteAddr);
	for (i = 0; i < NumByteToWrite; i++)
		SPI1_ReadWriteByte(pBuffer[i]); //循环写数
	W25QXX_CS = 1;						//取消片选
	W25QXX_Wait_Busy();					//等待写入结束
}
//无检验写SPI FLASH
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void W25QXX_Write_NoCheck(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u16 pageremain;
	pageremain = 256 - WriteAddr % 256; //单页剩余的字节数
	if (NumByteToWrite <= pageremain)
		pageremain = NumByteToWrite; //不大于256个字节
	while (1)
	{
		W25QXX_Write_Page(pBuffer, WriteAddr, pageremain);
		if (NumByteToWrite == pageremain)
			break; //写入结束了
		else	   //NumByteToWrite>pageremain
		{
			pBuffer += pageremain;
			WriteAddr += pageremain;

			NumByteToWrite -= pageremain; //减去已经写入了的字节数
			if (NumByteToWrite > 256)
				pageremain = 256; //一次可以写入256个字节
			else
				pageremain = NumByteToWrite; //不够256个字节了
		}
	};
}
//写SPI FLASH
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
u8 W25QXX_BUFFER[4096];
void W25QXX_Write(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u32 secpos;
	u16 secoff;
	u16 secremain;
	u16 i;
	u8 *W25QXX_BUF;
	W25QXX_BUF = W25QXX_BUFFER;
	secpos = WriteAddr / 4096; //扇区地址
	secoff = WriteAddr % 4096; //在扇区内的偏移
	secremain = 4096 - secoff; //扇区剩余空间大小
	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
	if (NumByteToWrite <= secremain)
		secremain = NumByteToWrite; //不大于4096个字节
	while (1)
	{
		W25QXX_Read(W25QXX_BUF, secpos * 4096, 4096); //读出整个扇区的内容
		for (i = 0; i < secremain; i++)				  //校验数据
		{
			if (W25QXX_BUF[secoff + i] != 0XFF)
				break; //需要擦除
		}
		if (i < secremain) //需要擦除
		{
			W25QXX_Erase_Sector(secpos);	//擦除这个扇区
			for (i = 0; i < secremain; i++) //复制
			{
				W25QXX_BUF[i + secoff] = pBuffer[i];
			}
			W25QXX_Write_NoCheck(W25QXX_BUF, secpos * 4096, 4096); //写入整个扇区
		}
		else
			W25QXX_Write_NoCheck(pBuffer, WriteAddr, secremain); //写已经擦除了的,直接写入扇区剩余区间.
		if (NumByteToWrite == secremain)
			break; //写入结束了
		else	   //写入未结束
		{
			secpos++;	//扇区地址增1
			secoff = 0; //偏移位置为0

			pBuffer += secremain;		 //指针偏移
			WriteAddr += secremain;		 //写地址偏移
			NumByteToWrite -= secremain; //字节数递减
			if (NumByteToWrite > 4096)
				secremain = 4096; //下一个扇区还是写不完
			else
				secremain = NumByteToWrite; //下一个扇区可以写完了
		}
	};
}
//擦除整个芯片
//等待时间超长...
void W25QXX_Erase_Chip(void)
{
	W25QXX_Write_Enable(); //SET WEL
	W25QXX_Wait_Busy();
	W25QXX_CS = 0;						//使能器件
	SPI1_ReadWriteByte(W25X_ChipErase); //发送片擦除命令
	W25QXX_CS = 1;						//取消片选
	W25QXX_Wait_Busy();					//等待芯片擦除结束
}
//擦除一个扇区
//Dst_Addr:扇区地址 根据实际容量设置
//擦除一个山区的最少时间:150ms
void W25QXX_Erase_Sector(u32 Dst_Addr)
{
	//监视falsh擦除情况,测试用
	// printf("fe:%x\r\n", Dst_Addr);
	Dst_Addr *= 4096;
	W25QXX_Write_Enable(); //SET WEL
	W25QXX_Wait_Busy();
	W25QXX_CS = 0;								//使能器件
	SPI1_ReadWriteByte(W25X_SectorErase);		//发送扇区擦除指令
	SPI1_ReadWriteByte((u8)((Dst_Addr) >> 16)); //发送24bit地址
	SPI1_ReadWriteByte((u8)((Dst_Addr) >> 8));
	SPI1_ReadWriteByte((u8)Dst_Addr);
	W25QXX_CS = 1;		//取消片选
	W25QXX_Wait_Busy(); //等待擦除完成
}
//等待空闲
void W25QXX_Wait_Busy(void)
{
	while ((W25QXX_ReadSR() & 0x01) == 0x01)
		; // 等待BUSY位清空
}
//进入掉电模式
void W25QXX_PowerDown(void)
{
	W25QXX_CS = 0;						//使能器件
	SPI1_ReadWriteByte(W25X_PowerDown); //发送掉电命令
	W25QXX_CS = 1;						//取消片选
	HAL_Delay(3);						//等待TPD
}
//唤醒
void W25QXX_WAKEUP(void)
{
	W25QXX_CS = 0;							   //使能器件
	SPI1_ReadWriteByte(W25X_ReleasePowerDown); //  send W25X_PowerDown command 0xAB
	W25QXX_CS = 1;							   //取消片选
	HAL_Delay(3);							   //等待TRES1
}
