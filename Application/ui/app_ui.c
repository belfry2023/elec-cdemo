#include "app_ui.h"
#include "OLED.h"
#include "message_center.h"
#include "module_key.h"
#include "task_def.h"
#include "module_ui.h"
#include "math.h"
#include "general_def.h"
#include "bsp_dwt.h"
static Subscriber_t *chassis_feed_sub;
static Chassis_Upload_UI_s chassis_fetch_data;        
static Subscriber_t *gimbal_feed_sub;     
static Gimbal_Upload_UI_s gimbal_fetch_data;
static Publisher_t *cmd_ui_pub;            
static Subscriber_t *cmd_feed_sub;  
static CMD_Ctrl_UI_s cmd_ui_send;   
static CMD_Upload_UI_s cmd_fetch_data;

static KeyInstance key;
static Page mainPage,setPage,chassisPage,gimbalPage,cmdPage,gMotorPage, waveformPage;
static Cube cube;
static MotorIcon yaw_motor, pitch_motor;
static uint16_t y;
static float Range = 30;
// 全局波形实例
Waveform wave;


// 更新电机状态
static void updateMotor(MotorIcon *motor, motor_data *measure) {
    static uint32_t lastUpdate = 0;
    uint32_t currentTime = HAL_GetTick();
    
    // 限制更新频率(约30FPS)
    if (currentTime - lastUpdate < 33) return;
    lastUpdate = currentTime;
    
    motor->angle = measure->dir;
    motor->speed = measure->spd;
    motor->torque = measure->tor;
    if (fabs(motor->speed) > 5.0f) {
        motor->state = MOTOR_RUNNING;
    }else
    {
        motor->state = MOTOR_STOPPING;
    }
    motor->power = (int)(motor->speed * motor->torque) * 100 / 3.0f;
}

static void CubeSet(Cube *cube, float pitch, float yaw, float roll)
{
    cube->pitch = pitch*M_PI/180;  // X轴旋转
    cube->yaw = yaw*M_PI/180;    // Y轴旋转
    cube->roll = roll*M_PI/180;  // Z轴旋转
}

static void gimbalPage_ui()
{
    CubeSet(&cube, gimbal_fetch_data.pitch, gimbal_fetch_data.yaw, gimbal_fetch_data.roll);
    drawCube(&cube); 
    // 绘制坐标轴
    drawAxes(&cube);
    // 绘制HUD信息
    OLED_ShowString(10,19,"yaw:",OLED_6X8);
    OLED_ShowFloatNum(60,19,gimbal_fetch_data.yaw,3,4,OLED_6X8);
    OLED_ShowString(10,29,"pitch:",OLED_6X8);
    OLED_ShowFloatNum(60,29,gimbal_fetch_data.pitch,3,4,OLED_6X8);
}

static void gMotorPage_ui()
{
    updateMotor(&yaw_motor,&gimbal_fetch_data.yaw_motor);
    updateMotor(&pitch_motor,&gimbal_fetch_data.pitch_motor);
    drawMotorIcon(&yaw_motor);
    drawMotorIcon(&pitch_motor);
    OLED_ShowFloatNum(50,10,gimbal_fetch_data.pitch_motor.dir,3,4,OLED_6X8);
    OLED_ShowFloatNum(50,20,gimbal_fetch_data.yaw_motor.dir,3,4,OLED_6X8);
}

static void cmdPage_ui()
{
    OLED_ShowString(10,30,"err yaw:",OLED_6X8);
    OLED_ShowString(10,40,"err_pitch",OLED_6X8);
    OLED_ShowFloatNum(40,30,cmd_fetch_data.k230_yaw,3,4,OLED_6X8);
    OLED_ShowFloatNum(40,40,cmd_fetch_data.k230_pitch,3,4,OLED_6X8);
}

static void Oscilloscope_ui()
{
    y = float_to_uint(gimbal_fetch_data.pitch_motor.err,-Range,Range,8);
    // 3. 更新波形数据
    Waveform_Update(&wave);
    // 4. 绘制波形
    Waveform_Draw(&wave);
}

void oled_ui_init(void)
{
    OLED_Init();
    initCube(&cube);
    initMotor(&yaw_motor,0);
    initMotor(&pitch_motor,1);
    Waveform_Register(&wave,&y);
    Waveform_SetParameters(&wave,1.0f, 32.0f, 60.0f);
    key_init_config_t key_config = {
        .GPIO = GPIOA,
        .GPIO_PIN = GPIO_PIN_0,
        .Press_Level = GPIO_PIN_RESET
    };
    Key_Init(&key, &key_config);

    Page_Init_Config page1_config = {
        .title = "mainPage",
        .buttons[0] = {"Settings", 10, 30, 80, 20, 0, 0, NULL, &setPage},
        .buttons[1] = {"Chassis", 10, 60, 80, 20, 0, 0, NULL, &chassisPage},
        .buttons[2] = {"Gimbal", 10, 90, 80, 20, 0, 0, NULL, &gimbalPage},
        .buttons[3] = {"Cmd", 10, 120, 80, 20, 0, 0, NULL, &cmdPage},
        .buttonCount = 4,
        .parent = NULL,
        .API = NULL
    };
    PageRegister(&mainPage,&page1_config);

    Page_Init_Config page2_config = {
        .title = "setPage",
        .buttons[0] = {"Back", 10, 30, 80, 20, 0, 0,goBack, NULL},
        .buttonCount = 1,
        .parent = &mainPage,
        .API = NULL
    };
    PageRegister(&setPage,&page2_config);

    Page_Init_Config page3_config = {
        .title = "chassisPage",
        .buttons[0] = {"Back", 10, 30, 80, 20, 0, 0,goBack, NULL},
        .buttonCount = 1,
        .parent = &mainPage,
        .API = NULL
    };
    PageRegister(&chassisPage,&page3_config);

    Page_Init_Config page4_config = {
        .title = "gimbalPage",
        .buttons[0] = {"X", 10, 50, 10, 10, 0, 0,goBack, NULL},
        .buttons[1] = {"Motor", 80, 50, 40, 10, 0, 0,NULL, &gMotorPage},
        .buttonCount = 2,
        .parent = &mainPage,
        .API = gimbalPage_ui
    };
    PageRegister(&gimbalPage,&page4_config);

    Page_Init_Config page5_config = {
        .title = "cmdPage",
        .buttons[0] = {"x", 10, 50, 10, 10, 0, 0,goBack, NULL},
        .buttonCount = 1,
        .parent = &mainPage,
        .API = cmdPage_ui
    };
    PageRegister(&cmdPage,&page5_config);

    Page_Init_Config page6_config = {
        .title = "gMotorPage",
        .buttons[0] = {"X", 80, 50, 10, 10, 0, 0,goBack, NULL},
        .buttons[1] = {"ERR", 8, 60, 20, 10, 0, 0, NULL, &waveformPage},
        .buttonCount = 2,
        .parent = &mainPage,
        .API = gMotorPage_ui
    };
    PageRegister(&gMotorPage,&page6_config);

    Page_Init_Config page7_config = {
        .title = "waveformPage",
        .buttons[0] = {"X", 80, 50, 10, 10, 0, 0,goBack, NULL},
        .buttonCount = 1,
        .parent = &mainPage,
        .API = Oscilloscope_ui
    };
    PageRegister(&waveformPage,&page7_config);

    appTitle.text = "MyAPP";
    appTitle.currentPage = &mainPage;
    
    chassis_feed_sub = SubRegister("chassis_feed_ui", sizeof(chassis_state_t));
    gimbal_feed_sub = SubRegister("gimbal_feed_ui", sizeof(Gimbal_Upload_UI_s));
    cmd_feed_sub = SubRegister("cmd_feed_ui",sizeof(CMD_Upload_UI_s));
    cmd_ui_pub = PubRegister("ui_ctrl_cmd",sizeof(CMD_Ctrl_UI_s));

    OLED_ClearArea(10, 4, 96, 30);
    OLED_ShowString(10, 4, "UI OK!", OLED_6X8);
    OLED_ShowString(10, 14, "CMD Init...", OLED_6X8);
    OLED_DrawRectangle(0, 40, 24, 15, OLED_FILLED);
    OLED_DrawRectangle(0, 40, 96, 15, OLED_UNFILLED);
    OLED_Update();
}

void oled_ui_task(void)
{
    SubGetMessage(chassis_feed_sub,&chassis_fetch_data);
    SubGetMessage(gimbal_feed_sub,&gimbal_fetch_data);
    SubGetMessage(cmd_feed_sub,&cmd_fetch_data);
    Key_Scan(&key);
    // if(!OLED_GetPoint(0,0))
    //     OLED_Init();
    handleInput(key.Key_Short_Press[0],key.Key_Long_Press[0]);
    renderPage(appTitle.currentPage);
    // OLED_DrawPoint(0,0);
    if(OLED_FLAG != 0)
    {
        OLED_Update();
    }
    PubPushMessage(cmd_ui_pub,&cmd_ui_send);
}