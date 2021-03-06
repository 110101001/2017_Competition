#include "main.h"
#define USART_REC_LEN 200
extern unsigned int x_pos,y_pos;
extern unsigned int x_pre,y_pre;
unsigned int area[9][2];
extern int time_count_begin;
extern int time;
extern int x_speed,y_speed;

extern PID_Type* Speed_X;
extern PID_Type* Speed_Y;

extern PID_Type* Motor_X;
extern PID_Type* Motor_Y;

u8 USART_RX_BUF[USART_REC_LEN];
u16 USART_RX_STA=0; 
/*----------USART1---PA9---PA10----*/
/*----------USART2---PD5---PD6-----*/
/************************************/
/*******串口1变量*********************/
u8 Rx_1_Buf[256];	
u8 Tx1Buffer[256];
u8 Tx1DMABuffer[256];
u8 Tx1Counter=0;
u8 count1=0; 
/***********************************/

void Usart1_Init(u32 br_num)
{  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); 
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = br_num;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure); 
	
  USART_Cmd(USART1, ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);	
	
	DMA_DeInit(DMA2_Stream7);
	
	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE){}//??DMA??? 
	
  /* ?? DMA Stream */
  DMA_InitStructure.DMA_Channel = DMA_Channel_4;  //????
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;//DMA????
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)Tx1DMABuffer;//DMA ???0??
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//????????
  DMA_InitStructure.DMA_BufferSize = 0;//????? 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//???????
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//???????
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//??????:8?
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//???????:8?
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ?????? 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//?????
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//?????????
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//????????
  DMA_Init(DMA2_Stream7, &DMA_InitStructure);//???DMA Stream

}




void USART1_IRQHandler(void)
{
	u8 len;
	u8 count=0;
	unsigned int x_temp=0,y_temp=0;
	static u8 fail_count=0;
	u8 res;	
	if( USART_GetITStatus(USART1,USART_IT_RXNE) )					//???????
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		
	
	if(1)//?????
	{	 
		res=USART1->DR; 
		if((USART_RX_STA&0x8000)==0)//?????
		{
			if(USART_RX_STA&0x4000)//????0x0d
			{
				if(res!=0x0a)USART_RX_STA=0;//????,????
				else USART_RX_STA|=0x8000;	//????? 
			}else //????0X0D
			{	
				if(res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=res;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//??????,??????	  
				}		 
			}
		}  		 									     
	} 
		//USART1->DR='A';
		//USART1->DR='\n';
		//delay_ms(500);
		if( USART_RX_STA&0x8000)              //????
		{
			len=USART_RX_STA&0x3fff;          //????
			//for(count=0;count<len;count++)    //????????
			//{
				//value[count]=USART_RX_BUF[count]-48;
				
			//}
			//if(USART_RX_BUF[count++]=='\n'){
			if(USART_RX_BUF[count++]=='B'){
				while(USART_RX_BUF[count]=='B') count++;
				for(;USART_RX_BUF[count]!=' '&&count!=len;count++){
						x_temp*=10;
						x_temp+=USART_RX_BUF[count]-'0';
				}	
				count++;
				for(;USART_RX_BUF[count]!='A'&&count<len;count++){
						y_temp*=10;
						y_temp+=USART_RX_BUF[count]-'0';
				}
				if((USART_RX_BUF[++count]-'0')==(x_temp+y_temp)%10){
					time=(TIM5->CNT - time_count_begin);
					x_pos=x_temp;
					y_pos=y_temp;
					x_speed=(x_pos-x_pre)/(float)time;
					y_speed=(y_pos-y_pre)/(float)time;
					x_pre=x_pos;
					y_pre=y_pos;
					time_count_begin=TIM5->CNT;
				}
				count=0;
				if(USART_RX_BUF[count++]=='A'){
				while(USART_RX_BUF[count]=='A') count++;
				for(;USART_RX_BUF[count]!=' '&&count!=len;count++){
						x_temp*=10;
						x_temp+=USART_RX_BUF[count]-'0';
				}	
				count++;
				for(;USART_RX_BUF[count]!='B'&&count<len;count++){
						y_temp*=10;
						y_temp+=USART_RX_BUF[count]-'0';
				}
				if((USART_RX_BUF[++count]-'0')==(x_temp+y_temp)%10){
					area[(USART_RX_BUF[++count]-'0')][0]=x_temp;
					area[(USART_RX_BUF[++count]-'0')][1]=y_temp;
				}
			}
			}
			//}
			//printf("\r\n");
			USART_RX_STA=0;                       //?????????,??????
			for (count=0;count<len;count++){
				USART_RX_BUF[count]=0;
			}
		}
	}		
}


void Usart1_Send(unsigned char *DataToSend ,u8 data_num)
{
  u8 i;
	static uint16_t num=0;
	static u8 len=0;
	
	DMA_Cmd(DMA2_Stream7, DISABLE);
	DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);//??DMA2_Steam7??????
	num = DMA_GetCurrDataCounter(DMA2_Stream7);
	
	for(i=0;i<data_num;i++)
	{
		Tx1Buffer[count1++] = *(DataToSend+i);
	}
	for (i=0;i<num;i++)
	{
		Tx1DMABuffer[i]=Tx1Buffer[((u8)(len-num+i))];
	}
	for (;i<num+data_num;i++)
	{
		Tx1DMABuffer[i]=*(DataToSend+i-num);
	}
	len=count1;
	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE){}	//??DMA?????  
	DMA2_Stream7->NDTR = (uint16_t)(num+data_num);          //?????  
	DMA_Cmd(DMA2_Stream7, ENABLE);      

}

/*******串口2变量*********************/
u8 Rx_2_Buf[256];	
u8 Tx2Buffer[256];
u8 Tx2Counter=0;
u8 count2=0; 
u8 Tx2DMABuffer[256]={0};
/***********************************/

void Usart2_Init(u32 br_num)
{
	USART_InitTypeDef USART_InitStructure;
	USART_ClockInitTypeDef USART_ClockInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);	
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 ; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 

	USART_InitStructure.USART_BaudRate = br_num;     
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 
	USART_InitStructure.USART_Parity = USART_Parity_No; 
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; 
	USART_ClockInitStruct.USART_Clock = USART_Clock_Disable; 
	USART_ClockInitStruct.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStruct.USART_CPHA = USART_CPHA_2Edge; 
	USART_ClockInitStruct.USART_LastBit = USART_LastBit_Disable;
	
	USART_Init(USART2, &USART_InitStructure);
	USART_ClockInit(USART2, &USART_ClockInitStruct);

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART2, ENABLE); 
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);

	DMA_DeInit(DMA1_Stream6);
	
	while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE){}//等待DMA可配置 
	
  /* 配置 DMA Stream */
  DMA_InitStructure.DMA_Channel = DMA_Channel_4;  //通道选择
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;//DMA外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)Tx2DMABuffer;//DMA 存储器0地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//存储器到外设模式
  DMA_InitStructure.DMA_BufferSize = 0;//数据传输量 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
  DMA_Init(DMA1_Stream6, &DMA_InitStructure);//初始化DMA Stream
	
}


void USART2_IRQHandler(void)
{
	u8 com_data;
		if(USART2->SR & USART_SR_ORE)
	{
		com_data = USART2->DR;
	}
	if( USART_GetITStatus(USART2,USART_IT_RXNE) )				//接收中断
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		com_data = USART2->DR;
		Usart2_DataPrepare(com_data);											//数据解析
	}

}


void Usart2_Send(unsigned char *DataToSend ,u8 data_num)
{
	u8 i;
	static uint16_t num=0;
	static u8 len=0;
	
	DMA_Cmd(DMA1_Stream6, DISABLE);
	DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);//清除DMA2_Steam7传输完成标志
	num = DMA_GetCurrDataCounter(DMA1_Stream6);
	
	for(i=0;i<data_num;i++)
	{
		Tx2Buffer[count2++] = *(DataToSend+i);
	}
	for (i=0;i<num;i++)
	{
		Tx2DMABuffer[i]=Tx2Buffer[len-num+i];
	}
	for (;i<num+data_num;i++)
	{
		Tx2DMABuffer[i]=*(DataToSend+i-num);
	}
	len=count2;
	while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE){}	//确保DMA可以被设置  
	DMA1_Stream6->NDTR = (uint16_t)(num+data_num);          //数据传输量  
	DMA_Cmd(DMA1_Stream6, ENABLE);       
}

void Usart2_Send_DMA(unsigned char *DataToSend ,u8 data_num)
{
	u8 i;
	static uint16_t num=0;
	static u8 len=0;
	
	DMA_Cmd(DMA1_Stream6, DISABLE);
	DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);//清除DMA2_Steam7传输完成标志
	num = DMA_GetCurrDataCounter(DMA1_Stream6);
	
	for(i=0;i<data_num;i++)
	{
		Tx2Buffer[count2++] = *(DataToSend+i);
	}
	for (i=0;i<num;i++)
	{
		Tx2DMABuffer[i]=Tx2Buffer[len-num+i];
	}
	for (;i<num+data_num;i++)
	{
		Tx2DMABuffer[i]=*(DataToSend+i-num);
	}
	len=count2;
	while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE){}	//确保DMA可以被设置  
	DMA1_Stream6->NDTR = (uint16_t)(num+data_num);          //数据传输量  
	DMA_Cmd(DMA1_Stream6, ENABLE);       
}

void Usart2_SendByte(unsigned char DataToSend)
{

		Tx2Buffer[count2++] = DataToSend;


	if(!(USART2->CR1 & USART_CR1_TXEIE))
	{
		USART_ITConfig(USART2, USART_IT_TXE, ENABLE); 
	}

}


int fputc(int ch, FILE *f)
{
		Usart2_SendByte((uint8_t)ch);
    return ch;
}

