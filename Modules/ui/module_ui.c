#include "module_ui.h"
#include "general_def.h"
#include "string.h"
#include "stdlib.h"
#include "bsp_dwt.h"
Title appTitle;

void renderPage(Page *page) {
    static uint8_t delay;
    delay++;
    delay %= 20;
    
    // 清空屏幕
    OLED_Clear();
    
    // 显示标题（固定位置）
    OLED_ShowString(10, 4, appTitle.text, OLED_6X8);
    OLED_ShowString(60, 4, page->title, OLED_6X8);
    if(page->API != NULL){
        page->API();
    }
    // 计算可视区域
    int startY = TITLE_HEIGHT; // 内容起始Y坐标
    int visibleHeight = SCREEN_HEIGHT - TITLE_HEIGHT; // 可视区域高度
    
    // 更新页面高度信息
    page->visibleHeight = visibleHeight;
    
    // 计算内容总高度
    int maxY = 0;
    for(int i = 0; i < page->buttonCount; i++) {
        Button *btn = &page->buttons[i];
        int btnBottom = btn->y + btn->height;
        if(btnBottom > maxY) maxY = btnBottom;
    }
    page->contentHeight = maxY;
    
    // 确保滚动位置在合理范围内
    if(page->scrollY > page->contentHeight - visibleHeight) 
        page->scrollY = page->contentHeight - visibleHeight;
    if(page->scrollY < 0) page->scrollY = 0;
    
    // 绘制所有按钮（考虑滚动偏移）
    for(int i = 0; i < page->buttonCount; i++) {
        Button *btn = &page->buttons[i];
        
        // 计算按钮在屏幕上的实际位置
        int screenY = btn->y - page->scrollY;
        
        // 只绘制在可视区域内的按钮
        if(screenY + btn->height > startY && screenY < startY + visibleHeight) {
            // 绘制按钮边框
            OLED_DrawRectangle(btn->x, screenY, btn->width, btn->height, OLED_UNFILLED);
            
            // 如果被选中则反色显示
            if(btn->isSelected && delay < 10) {
                OLED_ReverseArea(btn->x+1, screenY+1, btn->width-2, btn->height-2);
            }
            
            // 计算文本居中位置
            int textX = btn->x + (btn->width - strlen(btn->text)*6) / 2;
            int textY = screenY + (btn->height - 8) / 2;
            
            OLED_ShowString(textX, textY, btn->text, OLED_6X8);
        }
    }
    
    // 如果内容超过可视区域，绘制滚动条
    if(page->contentHeight > visibleHeight) {
        // 滚动条位置
        int scrollbarX = SCREEN_WIDTH - SCROLLBAR_WIDTH;
        
        // 滚动条背景
        OLED_DrawRectangle(scrollbarX, startY, SCROLLBAR_WIDTH, visibleHeight, OLED_UNFILLED);
        
        // 计算滑块位置和高度
        int sliderHeight = visibleHeight * visibleHeight / page->contentHeight;
        if(sliderHeight < 8) sliderHeight = 8; // 最小高度
        
        int sliderY = startY + (visibleHeight - sliderHeight) * page->scrollY / 
                     (page->contentHeight - visibleHeight);
        
        // 绘制滑块
        OLED_DrawRectangle(scrollbarX + 1, sliderY, SCROLLBAR_WIDTH - 2, sliderHeight, OLED_FILLED);
    }
}

void scrollPage(Page *page, int delta) {
    int newScrollY = page->scrollY + delta;
    
    // 确保滚动位置在合理范围内
    int maxScroll = page->contentHeight - page->visibleHeight;
    if(newScrollY < 0) newScrollY = 0;
    if(newScrollY > maxScroll) newScrollY = maxScroll;
    
    page->scrollY = newScrollY;
}


void ensureButtonVisible(Page *page, Button *btn) {
    int screenTop = TITLE_HEIGHT;
    int screenBottom = screenTop + page->visibleHeight;
    
    int btnTop = btn->y;
    int btnBottom = btn->y + btn->height;
    
    // 如果按钮在可视区域上方
    if(btnTop < page->scrollY + screenTop) {
        page->scrollY = btnTop - screenTop;
    }
    // 如果按钮在可视区域下方
    else if(btnBottom > page->scrollY + screenBottom) {
        page->scrollY = btnBottom - screenBottom;
    }
    
    // 确保滚动位置在合理范围内
    if(page->scrollY < 0) page->scrollY = 0;
    int maxScroll = page->contentHeight - page->visibleHeight;
    if(page->scrollY > maxScroll) page->scrollY = maxScroll;
}


void handleInput(uint8_t select, uint8_t press) {
    switch(select) {
        case 1:
            // 查找当前选中的按钮
            for(int i = 0; i < appTitle.currentPage->buttonCount; i++) {
                if(appTitle.currentPage->buttons[i].isSelected) {
                    // 如果当前是第一个按钮，尝试滚动
                    if(i == 0) {
                        appTitle.currentPage->buttons[i].isSelected = 0;
                        appTitle.currentPage->buttons[appTitle.currentPage->buttonCount-1].isSelected = 1;
                        ensureButtonVisible(appTitle.currentPage, &appTitle.currentPage->buttons[appTitle.currentPage->buttonCount-1]);
                    } else {
                        // 否则移动到上一个按钮
                        appTitle.currentPage->buttons[i].isSelected = 0;
                        appTitle.currentPage->buttons[i-1].isSelected = 1;
                        // 确保新按钮在可视区域内
                        ensureButtonVisible(appTitle.currentPage, &appTitle.currentPage->buttons[i-1]);
                    }
                    return;
                }
            }
            // 如果没有选中的按钮，选中第一个
            if(appTitle.currentPage->buttonCount > 0) {
                appTitle.currentPage->buttons[0].isSelected = 1;
            }
            break;
            
        case 0:
            for(int i = 0; i < appTitle.currentPage->buttonCount; i++){
                if(appTitle.currentPage->buttons[i].isSelected){
                    appTitle.currentPage->buttons[i].isPressed = press;
                    handleButtonPress(appTitle.currentPage);
                    return;
                }
            }
            if(appTitle.currentPage->buttonCount > 0) {
                appTitle.currentPage->buttons[0].isSelected = 1;
                appTitle.currentPage->buttons[0].isPressed = press;
                ensureButtonVisible(appTitle.currentPage, &appTitle.currentPage->buttons[0]);
                handleButtonPress(appTitle.currentPage);
            }
            break;
    }
}


void handleButtonPress(Page *currentPage) {
    for(int i = 0; i < currentPage->buttonCount; i++) {
        if(currentPage->buttons[i].isSelected) {
            // 如果有目标页面，切换到该页面
            if(currentPage->buttons[i].isPressed == 1)
            {
                if(currentPage->buttons[i].targetPage != NULL) {
                    currentPage->buttons[i].isPressed = 0;
                    appTitle.currentPage = currentPage->buttons[i].targetPage;
                    appTitle.currentPage->parent = currentPage; // 设置父页面
                }
                // 如果有回调函数，执行回调
                if(currentPage->buttons[i].onPress != NULL) {
                    currentPage->buttons[i].isPressed = 0;
                    currentPage->buttons[i].onPress();
                }
            }
            break;
        }
    }
}

// 导航函数（示例）
void navigateTo(Page *target) {
    appTitle.currentPage = target;
}

// 返回上一级页面
void goBack() {
    if(appTitle.currentPage->parent != NULL) {
        appTitle.currentPage = appTitle.currentPage->parent;
    }
}

void PageRegister(Page* page, Page_Init_Config *config)
{
    page->title = config->title;
    memcpy(page->buttons, config->buttons, sizeof(page->buttons));
    page->buttonCount = config->buttonCount;
    page->parent = config->parent;
    page->scrollY = 0; // 初始滚动位置为0
    page->contentHeight = 0;
    page->visibleHeight = SCREEN_HEIGHT - TITLE_HEIGHT;
    page->API = config->API;
}