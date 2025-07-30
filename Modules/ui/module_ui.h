#ifndef __MODULE_UI_H
#define __MODULE_UI_H

#include "OLED.h"
#define SCREEN_HEIGHT 64       // OLED屏幕高度
#define TITLE_HEIGHT 20        // 标题栏高度
#define SCROLLBAR_WIDTH 4      // 滚动条宽度
#define SCREEN_WIDTH 128

typedef struct Button {
    char *text;          // 按钮显示的文本
    int x, y;               // 按钮位置坐标
    int width, height;      // 按钮尺寸
    uint8_t isSelected;     // 是否被选中
    uint8_t isPressed;
    void (*onPress)(void);  // 点击回调函数（可选）
    struct Page *targetPage; // 指向的目标页面
} Button;

// 定义页面结构体
typedef struct Page {
    char *title;         // 页面标题
    Button buttons[5];     // 按钮数组指针
    uint8_t buttonCount; // 按钮数量
    void (*API)(void);  // API
    struct Page *parent; // 父页面指针
    int scrollY;         // 当前滚动位置
    int contentHeight;   // 页面内容总高度
    int visibleHeight;   // 可视区域高度
} Page;
// 定义标题结构体
typedef struct {
    char *text;          // 标题文本
    Page *currentPage;      // 当前显示的页面
} Title;

typedef struct  {
    char *title;         // 页面标题
    Button buttons[5];        // 按钮数组指针
    uint8_t buttonCount;    // 按钮数量
    void (*API)(void);  // API
    struct Page *parent;    // 父页面指针（用于返回）
} Page_Init_Config;

// 全局标题实例
extern Title appTitle;

void PageRegister(Page* page, Page_Init_Config *config);
void renderPage(Page *page);
void handleButtonPress(Page *currentPage);
void navigateTo(Page *target);
void goBack();
void handleInput(uint8_t select, uint8_t press);
#endif