#include "OLED_UI_MenuData.h"

/*���ļ����ڴ�Ų˵����ݡ�ʵ���ϲ˵����ݿ��Դ�����κεط�������ڴ˴���Ϊ�˹淶�����ģ�黯*/

// ColorMode ��һ����OLED_UI���ж����bool���ͱ��������ڿ���OLED��ʾ����ɫģʽ�� DARKMODE Ϊ��ɫģʽ�� LIGHTMOOD Ϊǳɫģʽ�����ｫ��������Ϊ�˴�����ѡ��˵��
extern bool ColorMode;
extern bool OLED_UI_ShowFps;
// OLED_UI_Brightness ��һ����OLED_UI���ж����int16_t���ͱ��������ڿ���OLED��ʾ�����ȡ����ｫ��������Ϊ�˴����������ȵĻ��������ڣ���Χ0-255��
extern int16_t OLED_UI_Brightness;
extern float yaw_v,pitch_v,roll_v;
float testfloatnum = 0.5;
int16_t testintnum = 1;
#define SPEED 4

//���ڴ��ڵĽṹ��
MenuWindow SetBrightnessWindow = {
	.General_Width = 80,								//���ڿ���
	.General_Height = 28, 							//���ڸ߶�
	.Text_String = "��Ļ������Ļ����",					//���ڱ���
	.Text_FontSize = OLED_UI_FONT_12,				//�ָ�
	.Text_FontSideDistance = 4,							//����������ľ���
	.Text_FontTopDistance = 3,							//������붥���ľ���
	.General_WindowType = WINDOW_ROUNDRECTANGLE, 	//��������
	.General_ContinueTime = 4.0,						//���ڳ���ʱ��

	.Prob_Data_Int = &OLED_UI_Brightness,				//��ʾ�ı�����ַ
	.Prob_DataStep = 5,								//����
	.Prob_MinData = 5,									//��Сֵ
	.Prob_MaxData = 255, 								//���ֵ
	.Prob_BottomDistance = 3,							//�ײ����
	.Prob_LineHeight = 8,								//�������߶�
	.Prob_SideDistance = 4,								//�߾�
};
/**
 * @brief ������ʾ���ȴ���
 */
void BrightnessWindow(void){
	OLED_UI_CreateWindow(&SetBrightnessWindow);
}

//Draw yaw���ڵĽṹ��
MenuWindow YawFloatDataWindow = {
	.General_Width = 80,								//���ڿ���
	.General_Height = 28, 							//���ڸ߶�
	.Text_String = "Yaw Speed",					//���ڱ���
	.Text_FontSize = OLED_UI_FONT_12,				//�ָ�
	.Text_FontSideDistance = 4,							//����������ľ���
	.Text_FontTopDistance = 3,							//������붥���ľ���
	.General_WindowType = WINDOW_ROUNDRECTANGLE, 	//��������
	.General_ContinueTime = 4.0,						//���ڳ���ʱ��

	.Prob_Data_Float = &yaw_v,				//��ʾ�ı�����ַ
	.Prob_DataStep = 0.01,								//����
	.Prob_MinData = 0,									//��Сֵ
	.Prob_MaxData = 360, 								//���ֵ
	.Prob_BottomDistance = 3,							//�ײ����
	.Prob_LineHeight = 8,								//�������߶�
	.Prob_SideDistance = 4,	
};
/**
 * @brief ��������yaw����
 */
void ShowYawFloatDataWindow(void){
	OLED_UI_CreateWindow(&YawFloatDataWindow);
}

//Draw pitch���ڵĽṹ��
MenuWindow PitchFloatDataWindow = {
	.General_Width = 80,								//���ڿ���
	.General_Height = 28, 							//���ڸ߶�
	.Text_String = "Pitch Speed",					//���ڱ���
	.Text_FontSize = OLED_UI_FONT_12,				//�ָ�
	.Text_FontSideDistance = 4,							//����������ľ���
	.Text_FontTopDistance = 3,							//������붥���ľ���
	.General_WindowType = WINDOW_ROUNDRECTANGLE, 	//��������
	.General_ContinueTime = 4.0,						//���ڳ���ʱ��

	.Prob_Data_Float = &pitch_v,				//��ʾ�ı�����ַ
	.Prob_DataStep = 0.01,								//����
	.Prob_MinData = 0,									//��Сֵ
	.Prob_MaxData = 360, 								//���ֵ
	.Prob_BottomDistance = 3,							//�ײ����
	.Prob_LineHeight = 8,								//�������߶�
	.Prob_SideDistance = 4,	
};
/**
 * @brief ��������yaw����
 */
void ShowPitchFloatDataWindow(void){
	OLED_UI_CreateWindow(&PitchFloatDataWindow);
}

//Draw pitch���ڵĽṹ��
MenuWindow RollFloatDataWindow = {
	.General_Width = 80,								//���ڿ���
	.General_Height = 28, 							//���ڸ߶�
	.Text_String = "Roll Speed",					//���ڱ���
	.Text_FontSize = OLED_UI_FONT_12,				//�ָ�
	.Text_FontSideDistance = 4,							//����������ľ���
	.Text_FontTopDistance = 3,							//������붥���ľ���
	.General_WindowType = WINDOW_ROUNDRECTANGLE, 	//��������
	.General_ContinueTime = 4.0,						//���ڳ���ʱ��

	.Prob_Data_Float = &roll_v,				//��ʾ�ı�����ַ
	.Prob_DataStep = 0.01,								//����
	.Prob_MinData = 0,									//��Сֵ
	.Prob_MaxData = 360, 								//���ֵ
	.Prob_BottomDistance = 3,							//�ײ����
	.Prob_LineHeight = 8,								//�������߶�
	.Prob_SideDistance = 4,	
};
/**
 * @brief ��������yaw����
 */
void ShowRollFloatDataWindow(void){
	OLED_UI_CreateWindow(&RollFloatDataWindow);
}



//���ڴ��ڵĽṹ��
MenuWindow NullWindow = {
	.General_Width = 80,								//���ڿ���
	.General_Height = 28, 							//���ڸ߶�
	.General_WindowType = WINDOW_ROUNDRECTANGLE, 	//��������
	.General_ContinueTime = 4.0,						//���ڳ���ʱ��
};
/**
 * @brief ������ʾ���ȴ���
 */
void EmptyWindow(void){
	OLED_UI_CreateWindow(&NullWindow);
}
//���ڴ��ڵĽṹ��
MenuWindow TextWindow = {
	.General_Width = 75,								//���ڿ���
	.General_Height = 18, 							//���ڸ߶�
	.General_WindowType = WINDOW_ROUNDRECTANGLE, 	//��������
	.General_ContinueTime = 4.0,						//���ڳ���ʱ��

	.Text_String = "����text",					//���ڱ���
	.Text_FontSize = OLED_UI_FONT_12,				//�ָ�
	.Text_FontSideDistance = 8,							//����������ľ���
	.Text_FontTopDistance = 2,							//������붥���ľ���
	

};
/**
 * @brief ������ʾ���ȴ���
 */
void ShowTextWindow(void){
	OLED_UI_CreateWindow(&TextWindow);
}
//���ڴ��ڵĽṹ��
MenuWindow FloatDataWindow = {
	.General_Width = 80,								//���ڿ���
	.General_Height = 28, 							//���ڸ߶�
	.Text_String = "�������ݲ���",					//���ڱ���
	.Text_FontSize = OLED_UI_FONT_12,				//�ָ�
	.Text_FontSideDistance = 4,							//����������ľ���
	.Text_FontTopDistance = 3,							//������붥���ľ���
	.General_WindowType = WINDOW_ROUNDRECTANGLE, 	//��������
	.General_ContinueTime = 4.0,						//���ڳ���ʱ��

	.Prob_Data_Float = &testfloatnum,				//��ʾ�ı�����ַ
	.Prob_DataStep = 0.1,								//����
	.Prob_MinData = -100,									//��Сֵ
	.Prob_MaxData = 100, 								//���ֵ
	.Prob_BottomDistance = 3,							//�ײ����
	.Prob_LineHeight = 8,								//�������߶�
	.Prob_SideDistance = 4,	
};
/**
 * @brief ������ʾ���ȴ���
 */
void ShowFloatDataWindow(void){
	OLED_UI_CreateWindow(&FloatDataWindow);
}
//���ڴ��ڵĽṹ��
MenuWindow IntDataWindow = {
	.General_Width = 80,								//���ڿ���
	.General_Height = 28, 							//���ڸ߶�
	.Text_String = "�������ݲ���",					//���ڱ���
	.Text_FontSize = OLED_UI_FONT_12,				//�ָ�
	.Text_FontSideDistance = 4,							//����������ľ���
	.Text_FontTopDistance = 3,							//������붥���ľ���
	.General_WindowType = WINDOW_ROUNDRECTANGLE, 	//��������
	.General_ContinueTime = 4.0,						//���ڳ���ʱ��

	.Prob_Data_Int = &testintnum,				//��ʾ�ı�����ַ
	.Prob_DataStep = 1,								//����
	.Prob_MinData = -100,									//��Сֵ
	.Prob_MaxData = 100, 								//���ֵ
	.Prob_BottomDistance = 3,							//�ײ����
	.Prob_LineHeight = 8,								//�������߶�
	.Prob_SideDistance = 4,	
};
/**
 * @brief ������ʾ���ȴ���
 */
void ShowIntDataWindow(void){
	OLED_UI_CreateWindow(&IntDataWindow);
}
//��LOGO�ƶ��Ľṹ��
OLED_ChangePoint LogoMove;
//��LOGO�����ƶ��Ľṹ��
OLED_ChangePoint LogoTextMove;
//welcome�����ƶ��Ľṹ��
OLED_ChangePoint WelcomeTextMove;

extern OLED_ChangePoint OLED_UI_PageStartPoint ;



//���ò˵���ĸ�����ʾ����
void SettingAuxFunc(void){
	//�ڹ涨λ����ʾLOGO
	if(fabs(OLED_UI_PageStartPoint.CurrentPoint.X - OLED_UI_PageStartPoint.TargetPoint.X) < 4){
		LogoMove.TargetPoint.X = 0;
		LogoMove.TargetPoint.Y = 0;
	}
	//��LOGOTEXT�ƶ�����Ļ�Ҳ࿴�����ĵط�
	LogoTextMove.TargetPoint.X = 129;
	LogoTextMove.TargetPoint.Y = 0;
	//��Welcome�����ƶ�����Ļ�ײ��������ĵط�
	WelcomeTextMove.TargetPoint.X = 128;
	WelcomeTextMove.TargetPoint.Y = 0;
	ChangePoint(&LogoMove);
	OLED_ShowImageArea(LogoMove.CurrentPoint.X,LogoMove.CurrentPoint.Y,32,64,0,0,128,128,OLED_UI_SettingsLogo);
	ChangePoint(&LogoTextMove);
	OLED_ShowImageArea(LogoTextMove.CurrentPoint.X,LogoTextMove.CurrentPoint.Y,26,64,0,0,128,128,OLED_UI_LOGOTEXT64);
	ChangePoint(&WelcomeTextMove);
	OLED_ShowImageArea(WelcomeTextMove.CurrentPoint.X,WelcomeTextMove.CurrentPoint.Y,16,64,0,0,128,128,OLED_UI_LOGOGithub);
}

//���ڲ˵��ĸ�����ʾ����
void AboutThisDeviceAuxFunc(void){
	//��LOGO�ƶ�����Ļ�Ϸ��������ĵط�
	LogoMove.TargetPoint.X = 0;
	LogoMove.TargetPoint.Y = -80;
	ChangePoint(&LogoMove);
    OLED_ShowImageArea(LogoMove.CurrentPoint.X,LogoMove.CurrentPoint.Y,32,64,0,0,128,128,OLED_UI_SettingsLogo);
	//����Ļ�Ҳ���ʾLOGO����
	if(fabs(OLED_UI_PageStartPoint.CurrentPoint.X - OLED_UI_PageStartPoint.TargetPoint.X) < 4){
		LogoTextMove.TargetPoint.X = 102;
		LogoTextMove.TargetPoint.Y = 0;
	}
	ChangePoint(&LogoTextMove);
	OLED_ShowImageArea(LogoTextMove.CurrentPoint.X,LogoTextMove.CurrentPoint.Y,26,64,0,0,128,128,OLED_UI_LOGOTEXT64);
}
//����OLED UI�ĸ�����ʾ����
void AboutOLED_UIAuxFunc(void){
	//��LOGO�ƶ�����Ļ�Ϸ��������ĵط�
	LogoMove.TargetPoint.X = 0;
	LogoMove.TargetPoint.Y = -80;
	ChangePoint(&LogoMove);
	OLED_ShowImageArea(LogoMove.CurrentPoint.X,LogoMove.CurrentPoint.Y,32,64,0,0,128,128,OLED_UI_SettingsLogo);
	//����Ļ�Ҳ���ʾWelcome����
	if(fabs(OLED_UI_PageStartPoint.CurrentPoint.X - OLED_UI_PageStartPoint.TargetPoint.X) < 4){
		WelcomeTextMove.TargetPoint.X = 110;
		WelcomeTextMove.TargetPoint.Y = 0;
	}
	ChangePoint(&WelcomeTextMove);
	OLED_ShowImageArea(WelcomeTextMove.CurrentPoint.X,WelcomeTextMove.CurrentPoint.Y,16,64,0,0,128,128,OLED_UI_LOGOGithub);

}
//����Draw�ĸ�����ʾ����
void DrawAuxFunc(void){
	DrawCube3D(96, 32, 24, 0); // ƽ��ͶӰ
}
//���˵��ĸ�����ʾ����
void MainAuxFunc(void){
	//����ʾ
	LogoMove.TargetPoint.X = -200;
	LogoMove.TargetPoint.Y = 0;
	LogoMove.CurrentPoint.X = -200;
	LogoMove.CurrentPoint.Y = 0;

	LogoTextMove.TargetPoint.X = 129;
	LogoTextMove.TargetPoint.Y = 0;
	LogoTextMove.CurrentPoint.X = 129;
	LogoTextMove.CurrentPoint.Y = 0;
	
	WelcomeTextMove.TargetPoint.X = 128;
	WelcomeTextMove.TargetPoint.Y = 0;
	WelcomeTextMove.CurrentPoint.X = 128;
	WelcomeTextMove.CurrentPoint.Y = 0;
}



//���˵��Ĳ˵���
MenuItem MainMenuItems[] = {

	{.General_item_text = "Settings",.General_callback = NULL,.General_SubMenuPage = &SettingsMenuPage,.Tiles_Icon = Image_setings},
	{.General_item_text = "Cube",.General_callback = NULL,.General_SubMenuPage = &DrawMenuPage,.Tiles_Icon = Image_cube,.Tiles_GifIcon = Gif_cube},
	{.General_item_text = "Alipay",.General_callback = NULL,.General_SubMenuPage = NULL,.Tiles_Icon = Image_alipay},
	{.General_item_text = "������ Calc ���ı����� LongText",.General_callback = NULL,.General_SubMenuPage = NULL,.Tiles_Icon = Image_calc},
	{.General_item_text = "Night",.General_callback = NULL,.General_SubMenuPage = NULL,.Tiles_Icon = Image_night},
	{.General_item_text = "More",.General_callback = NULL,.General_SubMenuPage = &MoreMenuPage,.Tiles_Icon = Image_more},
	
	{.General_item_text = NULL},/*���һ���General_item_text��ΪNULL����ʾ����Ϊ�ָ���*/

};

//���ò˵�����������
MenuItem SettingsMenuItems[] = {
	{.General_item_text = "����",.General_callback = BrightnessWindow,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL,.List_IntBox = &OLED_UI_Brightness},
	{.General_item_text = "�ڰ�ģʽ",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = &ColorMode},
	{.General_item_text = "��ʾ֡��",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = &OLED_UI_ShowFps},
	{.General_item_text = "���豸",.General_callback = NULL,.General_SubMenuPage = &AboutThisDeviceMenuPage,.List_BoolRadioBox = NULL},
	{.General_item_text = "����OLED UI",.General_callback = NULL,.General_SubMenuPage = &AboutOLED_UIMenuPage,.List_BoolRadioBox = NULL},
	{.General_item_text = "��л�ۿ�,һ������! Thanks for watching, three clicks!",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "[����]",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},

	{.General_item_text = NULL},/*���һ���General_item_text��ΪNULL����ʾ����Ϊ�ָ���*/
};

MenuItem AboutThisDeviceMenuItems[] = {
	{.General_item_text = "-[MCU:]",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " STM32F407",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " RAM:192+4KB",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " ROM:1024KB",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "-[Screen:]",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " SSD1306 128x64 OLED",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "-[CP:]",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " HardWare SPI",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "[����]",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},

	{.General_item_text = NULL},/*���һ���General_item_text��ΪNULL����ʾ����Ϊ�ָ���*/
};

MenuItem AboutOLED_UIMenuItems[] = {
	{.General_item_text = "-[Author:]",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " bilibili @��nm������ @5AKURA1.",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "-[Version:]",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " Ver.0.2.1",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "-[Adress:]",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = " https://github.com/bdth-7777777/OLED_UI",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},

	{.General_item_text = "[����]",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},

	{.General_item_text = NULL},/*���һ���General_item_text��ΪNULL����ʾ����Ϊ�ָ���*/
};

MenuItem DrawMenuItems[] = {
	{.General_item_text = "Yaw",.General_callback = ShowYawFloatDataWindow,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL,.List_FloatBox = &yaw_v},
	{.General_item_text = "Pitch",.General_callback = ShowPitchFloatDataWindow,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL,.List_FloatBox = &pitch_v},
	{.General_item_text = "Roll",.General_callback = ShowRollFloatDataWindow,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL,.List_FloatBox = &roll_v},

	{.General_item_text = "[����]",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},

	{.General_item_text = NULL},/*���һ���General_item_text��ΪNULL����ʾ����Ϊ�ָ���*/
};

MenuItem MoreMenuItems[] = {
	{.General_item_text = "[����]",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "����߶�8demo",.General_callback = NULL,.General_SubMenuPage = &Font8MenuPage,.List_BoolRadioBox = NULL},
	{.General_item_text = "����߶�12demo",.General_callback = NULL,.General_SubMenuPage = &Font12MenuPage,.List_BoolRadioBox = NULL},
	{.General_item_text = "����߶�16demo",.General_callback = NULL,.General_SubMenuPage = &Font16MenuPage,.List_BoolRadioBox = NULL},
	{.General_item_text = "����߶�20demo",.General_callback = NULL,.General_SubMenuPage = &Font20MenuPage,.List_BoolRadioBox = NULL},
	{.General_item_text = "�����ı�demo",.General_callback = NULL,.General_SubMenuPage = &LongMenuPage,.List_BoolRadioBox = NULL},
	{.General_item_text = "�ص�����demo",.General_callback = NULL,.General_SubMenuPage = &SpringMenuPage,.List_BoolRadioBox = NULL},
	{.General_item_text = "�����˵��б�demo",.General_callback = NULL,.General_SubMenuPage = &LongListMenuPage,.List_BoolRadioBox = NULL},
	{.General_item_text = "��С����˵�demo",.General_callback = NULL,.General_SubMenuPage = &SmallAreaMenuPage,.List_BoolRadioBox = NULL},
	{.General_item_text = "�մ���demo",.General_callback = EmptyWindow,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "���ִ���demo",.General_callback = ShowTextWindow,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "����������������demo",.General_callback = ShowFloatDataWindow,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "��������������demo",.General_callback = ShowIntDataWindow,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	
	{.General_item_text = NULL},/*���һ���General_item_text��ΪNULL����ʾ����Ϊ�ָ���*/
};

MenuItem Font8MenuItems[] = {
	{.General_item_text = "[����]",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "�����ı�",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "English Text",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "1234567890",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "abcdefghijklmnopqrstuvwxyz",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = ",.[]!@#$+-/^&*()",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	
	{.General_item_text = NULL},/*���һ���General_item_text��ΪNULL����ʾ����Ϊ�ָ���*/
};

MenuItem Font12MenuItems[] = {
	{.General_item_text = "[����]",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "�����ı�",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "English Text",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "1234567890",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "abcdefghijklmnopqrstuvwxyz",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = ",.[]!@#$+-/^&*()",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	
	{.General_item_text = NULL},/*���һ���General_item_text��ΪNULL����ʾ����Ϊ�ָ���*/
};

MenuItem Font16MenuItems[] = {
	{.General_item_text = "[����]",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "�����ı�",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "English Text",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "1234567890",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "abcdefghijklmnopqrstuvwxyz",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = ",.[]!@#$+-/^&*()",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	
	{.General_item_text = NULL},/*���һ���General_item_text��ΪNULL����ʾ����Ϊ�ָ���*/
};

MenuItem Font20MenuItems[] = {
	{.General_item_text = "[����]",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "�����ı�",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "English Text",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "1234567890",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "abcdefghijklmnopqrstuvwxyz",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = ",.[]!@#$+-/^&*()",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "�����ı�",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "English Text",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "1234567890",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "abcdefghijklmnopqrstuvwxyz",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = ",.[]!@#$+-/^&*()",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	
	{.General_item_text = NULL},/*���һ���General_item_text��ΪNULL����ʾ����Ϊ�ָ���*/
};

MenuItem LongMenuItems[] = {
	{.General_item_text = "[����]",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "�ǳ��ǳ��ǳ��������ı�",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "Very Very Very Long English Text",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	
	{.General_item_text = NULL},/*���һ���General_item_text��ΪNULL����ʾ����Ϊ�ָ���*/
};

MenuItem SpringMenuItems[] = {
	{.General_item_text = "[����]",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "�����ı�",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "English Text",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "1234567890",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "abcdefghijklmnopqrstuvwxyz",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = ",.[]!@#$+-/^&*()",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "�����ı�",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "English Text",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "1234567890",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "abcdefghijklmnopqrstuvwxyz",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = ",.[]!@#$+-/^&*()",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	
	{.General_item_text = NULL},/*���һ���General_item_text��ΪNULL����ʾ����Ϊ�ָ���*/
};

MenuItem LongListMenuItems[] = {
	{.General_item_text = "[����]",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "Item1",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "Item2",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "Item3",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "Item4",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "Item5",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "Item6",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},

	{.General_item_text = NULL},/*���һ���General_item_text��ΪNULL����ʾ����Ϊ�ָ���*/
};

MenuItem SmallAreaMenuItems[] = {
	{.General_item_text = "[����]",.General_callback = OLED_UI_Back,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "�����ı�",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "English Text",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "1234567890",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "abcdefghijklmnopqrstuvwxyz",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = ",.[]!@#$+-/^&*()",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "�����ı�",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "English Text",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "1234567890",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = "abcdefghijklmnopqrstuvwxyz",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = ",.[]!@#$+-/^&*()",.General_callback = NULL,.General_SubMenuPage = NULL,.List_BoolRadioBox = NULL},
	{.General_item_text = NULL},/*���һ���General_item_text��ΪNULL����ʾ����Ϊ�ָ���*/
};



MenuPage MainMenuPage = {
	//ͨ�����ԣ�����
	.General_MenuType = MENU_TYPE_TILES,  		 //�˵�����Ϊ��������
	.General_CursorStyle = NOT_SHOW,			 //�������
	.General_FontSize = OLED_UI_FONT_16,			//�ָ�
	.General_ParentMenuPage = NULL,				//�������Ǹ��˵������Ը��˵�ΪNULL
	.General_LineSpace = 5,						//������� ��λ�����أ����ڴ������Ͳ˵�����ֵ��ʾÿ������֮��ļ�࣬�����б����Ͳ˵�����ֵ��ʾ�м�ࣩ
	.General_MoveStyle = PID_CURVE,				//�ƶ���ʽ
	.General_MovingSpeed = SPEED,					//�����ƶ��ٶ�(��ֵ����ʵ��Ч������)
	.General_ShowAuxiliaryFunction = MainAuxFunc,		 //��ʾ��������
	.General_MenuItems = MainMenuItems,			//�˵�����������

	//�������ԣ�����.General_MenuType������ѡ��
	.Tiles_ScreenHeight = 64,					//��Ļ�߶�
	.Tiles_ScreenWidth = 128,						//��Ļ����
	.Tiles_TileWidth = 32,						 //��������
	.Tiles_TileHeight = 32,						 //�����߶�
};

MenuPage SettingsMenuPage = {
	//ͨ�����ԣ�����
	.General_MenuType = MENU_TYPE_LIST,  		 //�˵�����Ϊ�б�����
	.General_CursorStyle = REVERSE_ROUNDRECTANGLE,	 //�������Ϊ����
	.General_FontSize = OLED_UI_FONT_12,			//�ָ�
	.General_ParentMenuPage = &MainMenuPage,		 //���˵�Ϊ���˵�
	.General_LineSpace = 4,						//�м�� ��λ������
	.General_MoveStyle = UNLINEAR,				//�ƶ���ʽΪ���������߶���
	.General_MovingSpeed = SPEED,					//�����ƶ��ٶ�(��ֵ����ʵ��Ч������)
	.General_ShowAuxiliaryFunction = SettingAuxFunc,		 //��ʾ��������
	.General_MenuItems = SettingsMenuItems,		 //�˵�����������

	//�������ԣ�����.General_MenuType������ѡ��
	.List_MenuArea = {32, 0, 95, 64},			 //�б���ʾ����
	.List_IfDrawFrame = false,					 //�Ƿ���ʾ�߿�
	.List_IfDrawLinePerfix = true,				 //�Ƿ���ʾ��ǰ׺
	.List_StartPointX = 4,                        //�б���ʼ��X����
	.List_StartPointY = 2,                        //�б���ʼ��Y����
};

MenuPage AboutThisDeviceMenuPage = {
	//ͨ�����ԣ�����
	.General_MenuType = MENU_TYPE_LIST,  		 //�˵�����Ϊ�б�����
	.General_CursorStyle = REVERSE_BLOCK,	 //�������ΪԲ�Ǿ���
	.General_FontSize = OLED_UI_FONT_12,			//�ָ�
	.General_ParentMenuPage = &SettingsMenuPage,		 //���˵�Ϊ���˵�
	.General_LineSpace = 4,						//�м�� ��λ������
	.General_MoveStyle = UNLINEAR,				//�ƶ���ʽΪ���������߶���
	.General_MovingSpeed = SPEED,					//�����ƶ��ٶ�(��ֵ����ʵ��Ч������)
	.General_ShowAuxiliaryFunction = AboutThisDeviceAuxFunc,		 //��ʾ��������
	.General_MenuItems = AboutThisDeviceMenuItems,		 //�˵�����������

	//�������ԣ�����.General_MenuType������ѡ��
	.List_MenuArea = {0, 0, 100, 64},			 //�б���ʾ����
	.List_IfDrawFrame = false,					 //�Ƿ���ʾ�߿�
	.List_IfDrawLinePerfix = false,				 //�Ƿ���ʾ��ǰ׺
	.List_StartPointX = 4,                        //�б���ʼ��X����
	.List_StartPointY = 2,                        //�б���ʼ��Y����

};

MenuPage AboutOLED_UIMenuPage = {
	//ͨ�����ԣ�����
	.General_MenuType = MENU_TYPE_LIST,  		 //�˵�����Ϊ�б�����
	.General_CursorStyle = REVERSE_BLOCK,	 //�������ΪԲ�Ǿ���
	.General_FontSize = OLED_UI_FONT_12,			//�ָ�
	.General_ParentMenuPage = &SettingsMenuPage,		 //���˵�Ϊ���˵�
	.General_LineSpace = 4,						//�м�� ��λ������
	.General_MoveStyle = UNLINEAR,				//�ƶ���ʽΪ���������߶���
	.General_MovingSpeed = SPEED,					//�����ƶ��ٶ�(��ֵ����ʵ��Ч������)
	.General_ShowAuxiliaryFunction = AboutOLED_UIAuxFunc,		 //��ʾ��������
	.General_MenuItems = AboutOLED_UIMenuItems,		 //�˵�����������

	//�������ԣ�����.General_MenuType������ѡ��
	.List_MenuArea = {0, 0, 105, 64},			 //�б���ʾ����
	.List_IfDrawFrame = false,					 //�Ƿ���ʾ�߿�
	.List_IfDrawLinePerfix = false,				 //�Ƿ���ʾ��ǰ׺
	.List_StartPointX = 4,                        //�б���ʼ��X����
	.List_StartPointY = 2,                        //�б���ʼ��Y����

};

MenuPage DrawMenuPage = {
	//ͨ�����ԣ�����
	.General_MenuType = MENU_TYPE_LIST,  		 //�˵�����Ϊ�б�����
	.General_CursorStyle = REVERSE_ROUNDRECTANGLE,	 //�������Ϊ����
	.General_FontSize = OLED_UI_FONT_12,			//�ָ�
	.General_ParentMenuPage = &MainMenuPage,		 //���˵�Ϊ���˵�
	.General_LineSpace = 4,						//�м�� ��λ������
	.General_MoveStyle = UNLINEAR,				//�ƶ���ʽΪ���������߶���
	.General_MovingSpeed = SPEED,					//�����ƶ��ٶ�(��ֵ����ʵ��Ч������)
	.General_ShowAuxiliaryFunction = DrawAuxFunc,		 //��ʾ��������
	.General_MenuItems = DrawMenuItems,		 //�˵�����������

	//�������ԣ�����.General_MenuType������ѡ��
	.List_MenuArea = {0, 0, 64, 64},			 //�б���ʾ����
	.List_IfDrawFrame = false,					 //�Ƿ���ʾ�߿�
	.List_IfDrawLinePerfix = true,				 //�Ƿ���ʾ��ǰ׺
	.List_StartPointX = 4,                        //�б���ʼ��X����
	.List_StartPointY = 2,                        //�б���ʼ��Y����
};

MenuPage MoreMenuPage = {
	//ͨ�����ԣ�����
	.General_MenuType = MENU_TYPE_LIST,  		 //�˵�����Ϊ�б�����
	.General_CursorStyle = REVERSE_ROUNDRECTANGLE,	 //�������ΪԲ�Ǿ���
	.General_FontSize = OLED_UI_FONT_12,			//�ָ�
	.General_ParentMenuPage = &MainMenuPage,		 //���˵�Ϊ���˵�
	.General_LineSpace = 4,						//�м�� ��λ������
	.General_MoveStyle = UNLINEAR,				//�ƶ���ʽΪ���������߶���
	.General_MovingSpeed = SPEED,					//�����ƶ��ٶ�(��ֵ����ʵ��Ч������)
	.General_ShowAuxiliaryFunction = NULL,		 //��ʾ��������
	.General_MenuItems = MoreMenuItems,		 //�˵�����������

	//�������ԣ�����.General_MenuType������ѡ��
	.List_MenuArea = {1, 1, 128, 64},			 //�б���ʾ����
	.List_IfDrawFrame = false,					 //�Ƿ���ʾ�߿�
	.List_IfDrawLinePerfix = true,				 //�Ƿ���ʾ��ǰ׺
	.List_StartPointX = 4,                        //�б���ʼ��X����
	.List_StartPointY = 2,                        //�б���ʼ��Y����

};

MenuPage Font8MenuPage = {
	//ͨ�����ԣ�����
	.General_MenuType = MENU_TYPE_LIST,  		 //�˵�����Ϊ�б�����
	.General_CursorStyle = REVERSE_ROUNDRECTANGLE,	 //�������ΪԲ�Ǿ���
	.General_FontSize = OLED_UI_FONT_8,			//�ָ�
	.General_ParentMenuPage = &MoreMenuPage,		 //���˵�Ϊ���˵�
	.General_LineSpace = 5,						//�м�� ��λ������
	.General_MoveStyle = UNLINEAR,				//�ƶ���ʽΪ���������߶���
	.General_MovingSpeed = SPEED,					//�����ƶ��ٶ�(��ֵ����ʵ��Ч������)
	.General_ShowAuxiliaryFunction = NULL,		 //��ʾ��������
	.General_MenuItems = Font8MenuItems,		 //�˵�����������

	//�������ԣ�����.General_MenuType������ѡ��
	.List_MenuArea = {0, 0, 128, 64},			 //�б���ʾ����
	.List_IfDrawFrame = false,					 //�Ƿ���ʾ�߿�
	.List_IfDrawLinePerfix = true,				 //�Ƿ���ʾ��ǰ׺
	.List_StartPointX = 4,                        //�б���ʼ��X����
	.List_StartPointY = 2,                        //�б���ʼ��Y����

};

MenuPage Font12MenuPage = {
	//ͨ�����ԣ�����
	.General_MenuType = MENU_TYPE_LIST,  		 //�˵�����Ϊ�б�����
	.General_CursorStyle = REVERSE_ROUNDRECTANGLE,	 //�������ΪԲ�Ǿ���
	.General_FontSize = OLED_UI_FONT_12,			//�ָ�
	.General_ParentMenuPage = &MoreMenuPage,		 //���˵�Ϊ���˵�
	.General_LineSpace = 4,						//�м�� ��λ������
	.General_MoveStyle = UNLINEAR,				//�ƶ���ʽΪ���������߶���
	.General_MovingSpeed = SPEED,					//�����ƶ��ٶ�(��ֵ����ʵ��Ч������)
	.General_ShowAuxiliaryFunction = NULL,		 //��ʾ��������
	.General_MenuItems = Font12MenuItems,		 //�˵�����������

	//�������ԣ�����.General_MenuType������ѡ��
	.List_MenuArea = {0, 0, 128, 64},			 //�б���ʾ����
	.List_IfDrawFrame = false,					 //�Ƿ���ʾ�߿�
	.List_IfDrawLinePerfix = true,				 //�Ƿ���ʾ��ǰ׺
	.List_StartPointX = 4,                        //�б���ʼ��X����
	.List_StartPointY = 2,                        //�б���ʼ��Y����

};

MenuPage Font16MenuPage = {
	//ͨ�����ԣ�����
	.General_MenuType = MENU_TYPE_LIST,  		 //�˵�����Ϊ�б�����
	.General_CursorStyle = REVERSE_ROUNDRECTANGLE,	 //�������ΪԲ�Ǿ���
	.General_FontSize = OLED_UI_FONT_16,			//�ָ�
	.General_ParentMenuPage = &MoreMenuPage,		 //���˵�Ϊ���˵�
	.General_LineSpace = 5,						//�м�� ��λ������
	.General_MoveStyle = UNLINEAR,				//�ƶ���ʽΪ���������߶���
	.General_MovingSpeed = SPEED,					//�����ƶ��ٶ�(��ֵ����ʵ��Ч������)
	.General_ShowAuxiliaryFunction = NULL,		 //��ʾ��������
	.General_MenuItems = Font16MenuItems,		 //�˵�����������

	//�������ԣ�����.General_MenuType������ѡ��
	.List_MenuArea = {1, 1, 126, 62},			 //�б���ʾ����
	.List_IfDrawFrame = false,					 //�Ƿ���ʾ�߿�
	.List_IfDrawLinePerfix = true,				 //�Ƿ���ʾ��ǰ׺
	.List_StartPointX = 4,                        //�б���ʼ��X����
	.List_StartPointY = 2,                        //�б���ʼ��Y����

};

MenuPage Font20MenuPage = {
	//ͨ�����ԣ�����
	.General_MenuType = MENU_TYPE_LIST,  		 //�˵�����Ϊ�б�����
	.General_CursorStyle = REVERSE_ROUNDRECTANGLE,	 //�������ΪԲ�Ǿ���
	.General_FontSize = OLED_UI_FONT_20,			//�ָ�
	.General_ParentMenuPage = &MoreMenuPage,		 //���˵�Ϊ���˵�
	.General_LineSpace = 10,						//�м�� ��λ������
	.General_MoveStyle = UNLINEAR,				//�ƶ���ʽΪ���������߶���
	.General_MovingSpeed = SPEED,					//�����ƶ��ٶ�(��ֵ����ʵ��Ч������)
	.General_ShowAuxiliaryFunction = NULL,		 //��ʾ��������
	.General_MenuItems = Font20MenuItems,		 //�˵�����������

	//�������ԣ�����.General_MenuType������ѡ��
	.List_MenuArea = {0, 0, 128, 64},			 //�б���ʾ����
	.List_IfDrawFrame = false,					 //�Ƿ���ʾ�߿�
	.List_IfDrawLinePerfix = true,				 //�Ƿ���ʾ��ǰ׺
	.List_StartPointX = 4,                        //�б���ʼ��X����
	.List_StartPointY = 2,                        //�б���ʼ��Y����

};

MenuPage LongMenuPage = {
	//ͨ�����ԣ�����
	.General_MenuType = MENU_TYPE_LIST,  		 //�˵�����Ϊ�б�����
	.General_CursorStyle = REVERSE_ROUNDRECTANGLE,	 //�������ΪԲ�Ǿ���
	.General_FontSize = OLED_UI_FONT_12,			//�ָ�
	.General_ParentMenuPage = &MoreMenuPage,		 //���˵�Ϊ���˵�
	.General_LineSpace = 4,						//�м�� ��λ������
	.General_MoveStyle = UNLINEAR,				//�ƶ���ʽΪ���������߶���
	.General_MovingSpeed = SPEED,					//�����ƶ��ٶ�(��ֵ����ʵ��Ч������)
	.General_ShowAuxiliaryFunction = NULL,		 //��ʾ��������
	.General_MenuItems = LongMenuItems,		 //�˵�����������

	//�������ԣ�����.General_MenuType������ѡ��
	.List_MenuArea = {0, 0, 128, 64},			 //�б���ʾ����
	.List_IfDrawFrame = false,					 //�Ƿ���ʾ�߿�
	.List_IfDrawLinePerfix = true,				 //�Ƿ���ʾ��ǰ׺
	.List_StartPointX = 4,                        //�б���ʼ��X����
	.List_StartPointY = 2,                        //�б���ʼ��Y����

};

MenuPage SpringMenuPage = {
	//ͨ�����ԣ�����
	.General_MenuType = MENU_TYPE_LIST,  		 //�˵�����Ϊ�б�����
	.General_CursorStyle = REVERSE_ROUNDRECTANGLE,	 //�������ΪԲ�Ǿ���
	.General_FontSize = OLED_UI_FONT_12,			//�ָ�
	.General_ParentMenuPage = &MoreMenuPage,		 //���˵�Ϊ���˵�
	.General_LineSpace = 4,						//�м�� ��λ������
	.General_MoveStyle = PID_CURVE,				//�ƶ���ʽΪ���������߶���
	.General_MovingSpeed = SPEED,					//�����ƶ��ٶ�(��ֵ����ʵ��Ч������)
	.General_ShowAuxiliaryFunction = NULL,		 //��ʾ��������
	.General_MenuItems = SpringMenuItems,		 //�˵�����������

	//�������ԣ�����.General_MenuType������ѡ��
	.List_MenuArea = {0, 0, 128, 64},			 //�б���ʾ����
	.List_IfDrawFrame = false,					 //�Ƿ���ʾ�߿�
	.List_IfDrawLinePerfix = true,				 //�Ƿ���ʾ��ǰ׺
	.List_StartPointX = 4,                        //�б���ʼ��X����
	.List_StartPointY = 2,                        //�б���ʼ��Y����

};

MenuPage LongListMenuPage = {
	//ͨ�����ԣ�����
	.General_MenuType = MENU_TYPE_LIST,  		 //�˵�����Ϊ�б�����
	.General_CursorStyle = REVERSE_ROUNDRECTANGLE,	 //�������ΪԲ�Ǿ���
	.General_FontSize = OLED_UI_FONT_12,			//�ָ�
	.General_ParentMenuPage = &MoreMenuPage,		 //���˵�Ϊ���˵�
	.General_LineSpace = 4,						//�м�� ��λ������
	.General_MoveStyle = UNLINEAR,				//�ƶ���ʽΪ���������߶���
	.General_MovingSpeed = SPEED,					//�����ƶ��ٶ�(��ֵ����ʵ��Ч������)
	.General_ShowAuxiliaryFunction = NULL,		 //��ʾ��������
	.General_MenuItems = LongListMenuItems,		 //�˵�����������

	//�������ԣ�����.General_MenuType������ѡ��
	.List_MenuArea = {0, 0, 128, 64},			 //�б���ʾ����
	.List_IfDrawFrame = false,					 //�Ƿ���ʾ�߿�
	.List_IfDrawLinePerfix = true,				 //�Ƿ���ʾ��ǰ׺
	.List_StartPointX = 4,                        //�б���ʼ��X����
	.List_StartPointY = 2,                        //�б���ʼ��Y����

};

MenuPage SmallAreaMenuPage = {
	//ͨ�����ԣ�����
	.General_MenuType = MENU_TYPE_LIST,  		 //�˵�����Ϊ�б�����
	.General_CursorStyle = REVERSE_ROUNDRECTANGLE,	 //�������ΪԲ�Ǿ���
	.General_FontSize = OLED_UI_FONT_12,			//�ָ�
	.General_ParentMenuPage = &MoreMenuPage,		 //���˵�Ϊ���˵�
	.General_LineSpace = 6,						//�м�� ��λ������
	.General_MoveStyle = UNLINEAR,				//�ƶ���ʽΪ���������߶���
	.General_MovingSpeed = SPEED,					//�����ƶ��ٶ�(��ֵ����ʵ��Ч������)
	.General_ShowAuxiliaryFunction = NULL,		 //��ʾ��������
	.General_MenuItems = SmallAreaMenuItems,		 //�˵�����������

	//�������ԣ�����.General_MenuType������ѡ��
	.List_MenuArea = {10, 10, 60, 36},			 //�б���ʾ����
	.List_IfDrawFrame = true,					 //�Ƿ���ʾ�߿�
	.List_IfDrawLinePerfix = true,				 //�Ƿ���ʾ��ǰ׺
	.List_StartPointX = 4,                        //�б���ʼ��X����
	.List_StartPointY = 2,                        //�б���ʼ��Y����

};

