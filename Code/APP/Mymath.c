#include "Mymath.h"

/*****************************************************************************
//###@�������ƣ�����ƽ���˲�(����ƽ���˲�)
//###@�������ܣ������ݽ���ƽ������
//###@�������壺value_bufΪ������    sizeΪ�����С    NewValueΪ���Ƶ�����ֵ																				
//
//###@���ߣ�071
*******************************************************************************/
int16_t Smooth_filter_int(int16_t *value_buf,uint8_t size,int16_t NewValue)
{
			uint8_t count=0;
			int16_t sum=0;	
			for ( count=0;count<size;count++)
			{					
					sum += value_buf[count]/size;
					value_buf[count]=value_buf[count+1];
			}
					value_buf[count-1] = NewValue;
			return sum;
}

/*****************************************************************************
//###@�������ƣ�����ƽ���˲�(����ƽ���˲�)
//###@�������ܣ������ݽ���ƽ������
//###@�������壺value_bufΪ������    sizeΪ�����С    NewValueΪ���Ƶ�����ֵ																				
//
//###@���ߣ�071
*******************************************************************************/
float Smooth_filter(float *value_buf,uint8_t size,float NewValue)
{
			uint8_t count=0;
			float sum=0;	
			for ( count=0;count<size;count++)
			{					
					sum += value_buf[count]/size;
					value_buf[count]=value_buf[count+1];
			}
					value_buf[count-1] = NewValue;
			return sum;
}

 /*****************************************************************************
//###@�������ƣ���Ȩ����ƽ���˲�
//###@�������ܣ������ݽ��м�Ȩƽ������
//###@�������壺value_bufΪ������    sizeΪ�����С    NewValueΪ���Ƶ�����ֵ																				
//
//###@���ߣ�071
*******************************************************************************/
int16_t Weighted_average_filter(int16_t *value_buf,uint8_t size/*<=12*/,int16_t NewValue) 
{ 
        uint8_t count=0,sum_core=0; 
        int16_t sum=0;
        uint8_t core[12]={1,2,3,4,5,6,7,8,9,10,11,12};
        for(count=0;count<size;count++) 
        {
            sum += value_buf[count]*core[count];
            sum_core += count+1;
            value_buf[count]=value_buf[count+1];
        }
            value_buf[count-1] = NewValue;
		 return (sum/sum_core); 
} 
/********************************************************************************
//###@�������ƣ��޷�����
//###@�������ܣ��Խ�Ҫ��������ݽ����޷������𱣻�����
//###@�������壺dutyΪ��Ҫ���Ƶı���     minΪ���Ƶ���Сֵ     maxΪ���Ƶ����ֵ
//###@���ߣ�071
*********************************************************************************/
int16_t Limiting(int16_t duty, int16_t min, int16_t max)
{
			if(duty > max)
			{
				return max;
			}else if(duty < min)
			{
				return min;
			}else
			{
				return duty;
			}
}


uint16_t Limiting_uint16(uint16_t duty, uint16_t min, uint16_t max)
{
			if(duty > max)
			{
				return max;
			}else if(duty < min)
			{
				return min;
			}else
			{
				return duty;
			}
}

/********************************************************************************
//###@�������ƣ��޷�����
//###@�������ܣ��Խ�Ҫ��������ݽ����޷������𱣻�����
//###@�������壺dutyΪ��Ҫ���Ƶı���     minΪ���Ƶ���Сֵ     maxΪ���Ƶ����ֵ
//###@���ߣ�071
*********************************************************************************/
float Limiting_float(float duty, float min, float max)
{
			if(duty >= max)
			{
				return max;
			}else if(duty <= min)
			{
				return min;
			}else
			{
				return duty;
			}
}

/********************************************************************************
//###@�������ƣ�����ֵ����
//###@�������ܣ�������Ĳ������о���ֵ�������ֵ����Ϊ��
//###@�������壺Value Ϊ����Ĳ���
//###@���ߣ�071
*********************************************************************************/
int16_t My_abs(int16_t Value)
{
			if(Value < 0)
			{
					Value = - Value;
					return Value;
			}
			else
			{
					return Value;
			}
}

// һ�׵�ͨ�˲�
float firstOrderLowPassFilter(float parameter,float *input,float *Last_output)
{
		float output = 0;
		output = parameter * *input + (1.0f - parameter) * *Last_output;
		*Last_output = output;
		return output;
}

//��ĳ�����ĸ������ĵ���
float InvSqrt(float x)
{
		float xhalf = 0.5f*x;
		int i = *(int*)&x; // get bits for floating VALUE 
		i = 0x5f375a86- (i>>1); // gives initial guess y0
		x = *(float*)&i; // convert bits BACK to float
		x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
		return x;
}







