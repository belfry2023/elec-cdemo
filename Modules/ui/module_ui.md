# OLED UI 模块文档

## 概述

OLED UI 模块是一个基于 STM32 的轻量级用户界面框架，专为小型 OLED 显示屏设计。它提供了页面管理、按钮交互、滚动显示等核心功能，使开发者能够快速构建层次化的菜单系统。该模块具有以下特点：

- 支持多级页面导航
- 按钮选择与交互功能
- 自动滚动条管理
- 自定义页面渲染
- 简洁的 API 设计

## 文件结构

```txt
├── module_ui.h        // UI 模块头文件
├── module_ui.c        // UI 模块实现
├── OLED.h             // OLED 驱动头文件
├── OLED.c             // OLED 驱动实现
├── OLED_Data.h        // 字模数据头文件
├── OLED_Data.c        // 字模数据实现
├── app_ui.h           // 应用层头文件
├── app_ui.c           // 应用层实现
```

## 核心数据结构

### 按钮 (Button)

```c
typedef struct Button {
    char *text;          // 按钮显示的文本
    int x, y;            // 按钮位置坐标
    int width, height;   // 按钮尺寸
    uint8_t isSelected;  // 是否被选中
    uint8_t isPressed;   // 是否被按下
    void (*onPress)(void); // 点击回调函数
    struct Page *targetPage; // 指向的目标页面
} Button;
```

### 页面 (Page)

```c
typedef struct Page {
    char *title;         // 页面标题
    Button buttons[10];  // 按钮数组
    uint8_t buttonCount; // 按钮数量
    void (*API)(void);   // 页面自定义绘制函数
    struct Page *parent; // 父页面指针
    int scrollY;         // 当前滚动位置
    int contentHeight;   // 页面内容总高度
    int visibleHeight;   // 可视区域高度
} Page;
```

### 页面初始化配置 (Page_Init_Config)

```c
typedef struct {
    char *title;         // 页面标题
    Button buttons[10];  // 按钮数组
    uint8_t buttonCount; // 按钮数量
    void (*API)(void);   // 自定义绘制函数
    struct Page *parent; // 父页面指针
} Page_Init_Config;
```

### 标题 (Title)

```c
typedef struct {
    char *text;          // 标题文本
    Page *currentPage;   // 当前显示的页面
} Title;
```

## API 函数

### 页面注册

```c
void PageRegister(Page* page, Page_Init_Config *config);
```

初始化页面结构，配置按钮和回调函数。

### 渲染页面

```c
void renderPage(Page *page);
```

渲染指定页面，包括标题、按钮和滚动条。

### 处理按钮按下

```c
void handleButtonPress(Page *currentPage);
```

处理当前页面的按钮按下事件。

### 导航函数

```c
void navigateTo(Page *target);  // 导航到目标页面
void goBack();                  // 返回上一级页面
```

### 输入处理

```c
void handleInput(uint8_t select, uint8_t press);
```

处理用户输入：

- `select`: 0 = 确认/向下, 1 = 上一个按钮
- `press`: 0 = 释放, 1 = 按下

## 使用指南

### 1. 初始化 OLED 和 UI 系统

```c
void oled_ui_init(void) {
    OLED_Init();
    
    // 创建页面
    static Page mainPage, settingsPage;
    
    // 配置主页面
    Page_Init_Config main_config = {
        .title = "Main",
        .buttons = {
            {"Settings", 10, 30, 80, 20, 0, 0, NULL, &settingsPage},
        },
        .buttonCount = 1,
        .parent = NULL,
        .API = NULL
    };
    PageRegister(&mainPage, &main_config);
    
    // 配置设置页面
    Page_Init_Config settings_config = {
        .title = "Settings",
        .buttons = {
            {"Back", 10, 30, 80, 20, 0, 0, goBack, NULL},
        },
        .buttonCount = 1,
        .parent = &mainPage,
        .API = NULL
    };
    PageRegister(&settingsPage, &settings_config);
    
    // 设置全局标题
    appTitle.text = "MyApp";
    appTitle.currentPage = &mainPage;
}
```

### 2. 自定义页面渲染

```c
void customPageUI() {
    // 绘制自定义内容
    OLED_ShowString(10, 40, "Custom Content", OLED_8X16);
    OLED_DrawLine(0, 30, 127, 30);
}

// 在页面配置中指定自定义渲染函数
Page_Init_Config custom_config = {
    .title = "Custom",
    .API = customPageUI,
    // ...其他配置
};
```

### 3. 按钮回调函数

```c
void saveSettings() {
    // 保存设置逻辑
    OLED_ShowString(10, 50, "Settings Saved!", OLED_6X8);
}

// 在按钮配置中指定回调
Button saveBtn = {
    .text = "Save",
    .onPress = saveSettings,
    // ...其他配置
};
```

### 4. 主任务循环

```c
void oled_ui_task(void) {
    // 获取输入（按键扫描）
    uint8_t select = getSelectState(); // 0 = 确认/向下, 1 = 上一个
    uint8_t press = getPressState();   // 0 = 释放, 1 = 按下
    
    // 处理输入
    handleInput(select, press);
    
    // 渲染当前页面
    renderPage(appTitle.currentPage);
    
    // 更新显示
    OLED_Update();
}
```

## 高级功能

### 滚动支持

当页面内容超出屏幕高度时，会自动显示滚动条。系统会自动管理：

- 页面滚动位置 (`scrollY`)
- 内容总高度 (`contentHeight`)
- 可视区域高度 (`visibleHeight`)

### 3D 模型渲染

```c
// 初始化立方体
Cube cube;
initCube(&cube);

// 设置旋转角度
cube.pitch = angleX * M_PI/180;
cube.yaw = angleY * M_PI/180;
cube.roll = angleZ * M_PI/180;

// 渲染立方体
drawCube(&cube);
```

### 波形显示

```c
// 初始化波形
Waveform wave;
uint16_t dataSource = 0;
Waveform_Register(&wave, &dataSource);

// 配置波形参数
Waveform_SetParameters(&wave, 1.0f, 32.0f, 60.0f);

// 更新波形数据
dataSource = getSensorData();
Waveform_Update(&wave);

// 渲染波形
Waveform_Draw(&wave);
```

### 电机状态显示

```c
// 初始化电机图标
MotorIcon motor;
initMotor(&motor, 0); // 0 = 电机ID

// 更新电机状态
motor.angle = getMotorAngle();
motor.speed = getMotorSpeed();
motor.state = isMotorRunning() ? MOTOR_RUNNING : MOTOR_STOPPING;

// 渲染电机图标
drawMotorIcon(&motor);
```

## 最佳实践

1. **页面结构设计**：
   - 保持页面层级不超过3级
   - 每页按钮不超过5个
   - 使用明确的页面标题

2. **性能优化**：
   - 只在数据变化时更新显示
   - 使用局部刷新 (`OLED_UpdateArea()`) 代替全屏刷新
   - 避免在渲染函数中进行复杂计算

3. **内存管理**：
   - 静态分配页面和按钮结构
   - 复用常用组件
   - 避免在回调函数中分配大内存

4. **错误处理**：
   - 检查按钮数量不超过最大值
   - 验证坐标在屏幕范围内
   - 处理OLED初始化失败情况

## 示例应用

以下是一个完整的温度监控系统示例：

```c
// app_ui.h
void temp_monitor_init(void);
void temp_monitor_task(void);

// app_ui.c
#include "app_ui.h"
#include "module_ui.h"
#include "OLED.h"

Page mainPage, tempPage;
float currentTemp = 25.0f;

void tempPageUI() {
    char tempStr[20];
    sprintf(tempStr, "Temp: %.1fC", currentTemp);
    OLED_ShowString(10, 30, tempStr, OLED_8X16);
    
    // 绘制温度计图标
    OLED_DrawRectangle(100, 20, 5, 30, OLED_UNFILLED);
    int tempHeight = (int)((currentTemp - 20) * 3); // 20-30℃范围映射
    OLED_DrawRectangle(100, 50 - tempHeight, 5, tempHeight, OLED_FILLED);
}

void updateTemperature() {
    // 模拟温度变化
    currentTemp += 0.1f;
    if(currentTemp > 30.0f) currentTemp = 20.0f;
}

void temp_monitor_init(void) {
    OLED_Init();
    
    // 主页面配置
    Page_Init_Config main_config = {
        .title = "Main Menu",
        .buttons = {
            {"Temperature", 10, 30, 100, 20, 0, 0, NULL, &tempPage},
        },
        .buttonCount = 1,
        .parent = NULL,
        .API = NULL
    };
    PageRegister(&mainPage, &main_config);
    
    // 温度页面配置
    Page_Init_Config temp_config = {
        .title = "Temperature",
        .buttons = {
            {"Back", 10, 50, 50, 20, 0, 0, goBack, NULL},
        },
        .buttonCount = 1,
        .parent = &mainPage,
        .API = tempPageUI
    };
    PageRegister(&tempPage, &temp_config);
    
    appTitle.text = "Temp Monitor";
    appTitle.currentPage = &mainPage;
}

void temp_monitor_task(void) {
    static uint32_t lastUpdate = 0;
    uint32_t now = HAL_GetTick();
    
    // 每500ms更新温度
    if(now - lastUpdate > 500) {
        updateTemperature();
        lastUpdate = now;
    }
    
    // 处理输入（简化示例）
    handleInput(0, 0); // 无实际输入
    
    // 渲染当前页面
    renderPage(appTitle.currentPage);
    
    // 更新显示
    OLED_Update();
}
```

## 常见问题解决

1. **屏幕不显示内容**：
   - 检查 OLED 初始化是否成功
   - 验证 I2C 连接是否正确
   - 确保调用 `OLED_Update()` 函数

2. **按钮无响应**：
   - 确认正确调用了 `handleInput()`
   - 检查按钮的 `isSelected` 和 `isPressed` 状态
   - 验证按钮坐标是否在屏幕可见区域

3. **显示内容错位**：
   - 检查坐标计算是否正确
   - 确保内容高度不超过屏幕限制
   - 验证滚动位置管理逻辑

4. **性能问题**：
   - 减少全屏刷新频率
   - 优化自定义渲染函数
   - 使用局部刷新代替全局刷新

本模块提供了构建丰富OLED界面的基础框架，开发者可以根据具体应用需求扩展更多功能组件和交互逻辑。
