#include "Hal_Inf_Receiver.h"

/**
*红外接收使用PD2,只能使用IO中断, 与定时器配合,工程使用的TIM3定时器,定时56us一次中断
*捕获 NEC协议的红外程序,分别为 同步码 地址码 地址反码 控制码 控制反码 连续发生码
*管脚默认为高电平
*检测9ms低电平和4.5ms高电平为开始信号
*检测560us 低电平和560us高电平 为逻辑0
*检测560us 低电平和1680us高电平为逻辑1
*检测9ms低电平2.5ms高电平(连续码引导吗),0.56ms低电平,周期110ms  为连续码
*超过20ms没检测到电平变化,表示一个数据发送完成(不算连续发生码)
*连续码是把数据接收完成后(等于34个下降沿中断后),检测连续码
*数据bit为从低有效位开始发
*/


extern TIM_HandleTypeDef htim3;

typedef struct
{
	char syn_flag; //是否接受到同步码
	char time_start_flag; //定时器启动标记
	
	uint32_t all_time; //定时时间单位定时器溢出时间
	uint32_t last_data_time;//上一次数据的时间, 这一次减去上一次的时间，就表示上一个中断到这个中断的时间
	uint16_t eint_count ; //中断次数
	uint16_t differ_time[40];//记录本次中断与上一次中断相差的时间
}TInf_Recive_State;

#define TIME_UNIT 56  //定时器单位us
#define ERROR_TIME (TIME_UNIT*2) //误差范围
#define DATA_SYN 13500//同步码 中断间隔时间(9000+4500)
#define DATA_L 1120  //数据0 的中断间隔时间,(560+560)
#define DATA_H 2240  //数据1 的中断间隔时间,560+1680
#define MAX_WAIT_TIME 100000  //最长等待100ms,当接收到到同步码后,如果超过这个时间没有中断了,表示数据全部结束(包括连续码)

TInf_Recive_State gInf_State;
TIM_HandleTypeDef *ginf_htim = &htim3;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

	if(htim == ginf_htim)
	{
		gInf_State.all_time+=TIME_UNIT; 
	}

}
/**
*处理红外中断管脚的电平
*/
void Pro_Inf_Receiver_Pin_Eint(void)
{
	//1.第一次中断后,开启定时器
	uint32_t differ_time = 0; 
	if (gInf_State.time_start_flag == 0)
	{
		memset(&gInf_State,0,sizeof(gInf_State));
		HAL_TIM_Base_Start_IT(ginf_htim); //第一次进入中断的时候。开启定时器的时间比较晚,导致第一次相差时间不准确，可以提到外部中断那里处理
		gInf_State.time_start_flag = 1;
	}
	else
	{
		//2.求出差异时间
		differ_time = gInf_State.all_time-gInf_State.last_data_time;
		
		//3.寻找同步码
		if(gInf_State.syn_flag == 0 && gInf_State.eint_count == 0) //如果没有收到同步码,需要等待同步码
		{
			if((DATA_SYN-5*ERROR_TIME)<differ_time && differ_time < (DATA_SYN+ERROR_TIME))////第一次进入中断的时候打开时间有点晚。导致第一个计数误差比较大
			{
				gInf_State.syn_flag = 1;
			}
		}

		gInf_State.differ_time[gInf_State.eint_count++]=differ_time;//计算的中断个数,没办法无限期延迟记录连续码
		gInf_State.last_data_time = gInf_State.all_time;
 }

}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == GPIO_PIN_2)
	{
		Pro_Inf_Receiver_Pin_Eint();
	}

}

/**
*获取红外接收头数据
*有数据,返回1,数据有效
*/
uint8_t Check_Inf_Reciver_Data(uint8_t *addr,uint8_t * cmd)
{
	uint8_t ret = 0;
	char j=0,k=0;
	uint8_t inf_data[4]={0};
	if((gInf_State.all_time-gInf_State.last_data_time)>MAX_WAIT_TIME) //
	{
		HAL_TIM_Base_Stop_IT(ginf_htim);
		if (gInf_State.syn_flag == 1)
		{
			//解析红外数据, 第一个differ为同步码,y严谨的处理需要做数据校验
			for(j=0;j<4;j++)
			{
				for(k=0;k<8;k++)
				{
					if(((DATA_L-ERROR_TIME)<=gInf_State.differ_time[1+j*8+k])
						&&(gInf_State.differ_time[1+j*8+k] <= (DATA_L+ERROR_TIME))) //0 允许误差
					{
						inf_data[j] &= ~(1<<k); //0

					}
					else if(((DATA_H-ERROR_TIME)<=gInf_State.differ_time[1+j*8+k]) //1允许误差
						&&(gInf_State.differ_time[1+j*8+k] <= (DATA_H+ERROR_TIME)))
					{
						inf_data[j] |= (1<<k); //1

					}
					else
					{
						LOG("ERROR,%d,%d\r\n",DATA_H-TIME_UNIT,DATA_H+TIME_UNIT);
					}
				}
			}
			//LOG("addr=0x%02X,addr_f=0x%02X,con=0x%02X,con=0x%02X\r\n",inf_data[0],inf_data[1],inf_data[2],inf_data[3]);
			*addr = inf_data[0];
			*cmd = inf_data[2];
			ret = 1;

		}
		else
		{
			LOG("no syn flag ,no data\r\n");
		}
		memset(&gInf_State,0,sizeof(gInf_State));
	}

	return ret;
}

void Test_Inf_Receiver(void)
{
	char i=0,j=0,k=0;
	uint8_t inf_data[4]={0};

	if((gInf_State.all_time-gInf_State.last_data_time)>MAX_WAIT_TIME) //
	{
		HAL_TIM_Base_Stop_IT(ginf_htim);
		LOG("inf data finsh\r\n");
		for(i=0;i<32;i++)
		{
			//LOG("a=%d\r\n",gInf_State.differ_time[i]);
		}
		
		if (gInf_State.syn_flag == 1)
		{
			LOG("Geted syn flag ,have data\r\n");
			//解析红外数据, 第一个differ为同步码
			for(j=0;j<4;j++)
			{
				for(k=0;k<8;k++)
				{
					//LOG("differ[%d]=%d\r\n",1+j*8+k,gInf_State.differ_time[1+j*8+k]);
					if(((DATA_L-ERROR_TIME)<=gInf_State.differ_time[1+j*8+k])
						&&(gInf_State.differ_time[1+j*8+k] <= (DATA_L+ERROR_TIME))) //0 允许误差
					{
						inf_data[j] &= ~(1<<k); //0
						//LOG("0\r\n");
					}
					else if(((DATA_H-ERROR_TIME)<=gInf_State.differ_time[1+j*8+k]) //1允许误差
						&&(gInf_State.differ_time[1+j*8+k] <= (DATA_H+ERROR_TIME)))
					{
						inf_data[j] |= (1<<k); //1
						//LOG("1\r\n");
					}
					else
					{
						LOG("ERROR,%d,%d\r\n",DATA_H-TIME_UNIT,DATA_H+TIME_UNIT);
					}
				}
			}
			
			LOG("addr=0x%02X,addr_f=0x%02X,con=0x%02X,con=0x%02X\r\n",inf_data[0],inf_data[1],inf_data[2],inf_data[3]);
		}
		else
		{
			LOG("no syn flag ,no data\r\n");
		}
		memset(&gInf_State,0,sizeof(gInf_State));
	}

}



