#include "Ui.h"


void (*page)(void);
void (*page_late)(void);

char list[10][20]={0};

Page_Flag page_flag;


//菜单相关初始化
void Page_Init(void)
{
		page = Page_Main_Menu;
		page_late = page;
		page_flag.Mouse_Now = 0;
		page_flag.Mouse_Last = 0;
		OLED_ShowString(0,0,(u8 *)"hello!",12);
}

//菜单显示
void Pages_Show(void)
{	
		page_flag.Key_Get = Key_Scan();
	
		if(page_late != page)
		{
			OLED_Clear();
		}
		
		page_late=page;
		
//		OLED_ShowString(0,page_flag.Mouse_Last,(uint8_t *)" ",12);	
//		OLED_ShowString(0,abs(page_flag.Mouse_Now%OLED_ROW),(uint8_t *)page_flag.str,12);
//		
//		page_flag.Mouse_Last = abs(page_flag.Mouse_Now%OLED_ROW);
		
		page(); 	
		
		page_flag.Key_Get = 0;
}

//主菜单
void Page_Main_Menu(void)
{
		sprintf(page_flag.str,">");
		sprintf(list[0],"**main_menu**");
		sprintf(list[1],"Image_Show");
		sprintf(list[2],"Speed_PID_Show");
		sprintf(list[3],"Dir_Inner_PID_Show");
		sprintf(list[4],"Dir_Outer_PID_Show");
		sprintf(list[5],"Adc_Value_Show");
	    sprintf(list[6],"Out_Bound_Value");
		sprintf(list[7],"Value_Show");
		switch(page_flag.Key_Get)
		{
				case KEY_UP:
					page_flag.Mouse_Now--;break;
				case KEY_DOWN:
					page_flag.Mouse_Now++;break;
		}
		Print_To_Oled(8);
}


//速度环菜单
void Speed_PID_Show(void)
{
	sprintf(page_flag.str,">");
//    sprintf(list[0],"EXS:%4d",Car_Except.Speed);
//    sprintf(list[1],"SP:%f",Speed_PID.P);
//    sprintf(list[2],"SI:%f",Speed_PID.I);
//    sprintf(list[3],"SD:%f",Speed_PID.D);
//    sprintf(list[4],"PWM:%f",Speed_PID.Out);
//    sprintf(list[5],"LSP:%4d",Car_Speed.Left);
//    sprintf(list[6],"RSP:%4d",Car_Speed.Right);
//    sprintf(list[7],"CRS:%4d",Car_Speed.Real);

    switch(page_flag.Key_Get)
    {
        case KEY_UP:
            page_flag.Mouse_Now--;break;
        case KEY_DOWN:
            page_flag.Mouse_Now++;break;
    }
    Print_To_Oled(8);
}

//方向环菜单
void Dir_PID_Show(void)
{
//    sprintf(page_flag.str,">");
//    sprintf(list[0],"*Dir_Inn_Show*");
//    sprintf(list[1],"DIP:%f",Dir_Inner_PID.P);
//    sprintf(list[2],"DII:%f",Dir_Inner_PID.I);
//    sprintf(list[3],"DID:%f",Dir_Inner_PID.D);
//    sprintf(list[4],"PWM:%f",Dir_Inner_PID.Out);
//    sprintf(list[5],"UOTBOUN:%d",Judge.OutBound_Value);
//    sprintf(list[6],"EXDI:%d",Car_Except.Icm_gz);
    switch(page_flag.Key_Get)
    {
        case KEY_UP:
            page_flag.Mouse_Now--;break;
        case KEY_DOWN:
            page_flag.Mouse_Now++;break;
      
    }
    Print_To_Oled(7);
}


//打印到屏幕
void Print_To_Oled(uint8_t j)
{
    uint8_t i=0;
    for(;i<j;i++)
    {
       OLED_ShowString(8,i,(uint8_t *)list[i],12);
    }
}
