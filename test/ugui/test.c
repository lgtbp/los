#include "ugui.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
UG_GUI gc;
extern void functions(int, int, int, int);
void drae_point(UG_S16 x, UG_S16 y, UG_COLOR c)
{
    los_draw_point(x, y, c);  
}

#define MAX_OBJ_NUM 3
#define LCD_W 320
#define LCD_H 240
UG_GUI gui;
/* Window 1 */
UG_WINDOW window_1;
UG_OBJECT obj_buff_wnd_1[MAX_OBJ_NUM];
UG_BUTTON button1_1;
UG_BUTTON button1_2;

/* Window 2 */
UG_WINDOW window_2;
UG_OBJECT obj_buff_wnd_2[MAX_OBJ_NUM];
UG_BUTTON button2_1;
UG_TEXTBOX textbox2_1;
UG_TEXTBOX textbox2_2;

void window_1_callback(UG_MESSAGE *msg)
{

    if (msg->type == MSG_TYPE_OBJECT)
    {
        if (msg->id == OBJ_TYPE_BUTTON)
        {
            switch (msg->sub_id)
            {
            case BTN_ID_0:
            case BTN_ID_1: // Show UI info
            {
                if (msg->event == OBJ_EVENT_PRESSED)
                {
                    UG_WindowShow(&window_2);
                }
                break;
            }
            }
        }
    }
}
/* Callback function for the info window */
void window_2_callback(UG_MESSAGE *msg)
{
    if (msg->type == MSG_TYPE_OBJECT)
    {
        if (msg->id == OBJ_TYPE_BUTTON)
        {
            switch (msg->sub_id)
            {
            case BTN_ID_0:
            {
                if (msg->event == OBJ_EVENT_PRESSED)
                {
                    UG_WindowShow(&window_1);
                }

                break;
            }
            }
        }
    }
}

void los_irq(int num)
{
    int x, y;
    if (num)
        UG_TouchUpdate(-1, -1, TOUCH_STATE_RELEASED); //触摸坐标更新
    else
    {
        x = los_get_x();
        y = los_get_y();
        UG_TouchUpdate(x, y, TOUCH_STATE_PRESSED); //触摸坐标更新
    }
}

int main(void)
{
    int a = 4;
    // int x=30,y=30,j;
    UG_Init(&gc, drae_point, LCD_W, LCD_H);
    UG_FillScreen(C_GREEN);

    /* Callback function for the main menu */

    UG_Init(&gui, (void (*)(UG_S16, UG_S16, UG_COLOR))drae_point, LCD_W, LCD_H);

    UG_FillScreen(C_BLACK);
    /* Create Window 1 */
    UG_WindowCreate(&window_1, obj_buff_wnd_1, MAX_OBJ_NUM, window_1_callback);
    UG_WindowSetTitleText(&window_1, "uGUI @ los");
    UG_WindowSetTitleTextFont(&window_1, &FONT_12X20);

    /* Create some Buttons */
    UG_ButtonCreate(&window_1, &button1_1, BTN_ID_0, 10, 10, 120, 60);
    UG_ButtonCreate(&window_1, &button1_2, BTN_ID_1, 10, 80, 120, 130);

    /* Configure Button 1 */
    UG_ButtonSetFont(&window_1, BTN_ID_0, &FONT_12X20);
    UG_ButtonSetBackColor(&window_1, BTN_ID_0, C_LIME);
    UG_ButtonSetText(&window_1, BTN_ID_0, "About\nGUI");
    /* Configure Button 2 */
    UG_ButtonSetFont(&window_1, BTN_ID_1, &FONT_12X20);
    UG_ButtonSetBackColor(&window_1, BTN_ID_1, 0xFF);
    UG_ButtonSetText(&window_1, BTN_ID_1, "btn");

    /* -------------------------------------------------------------------------------- */
    /* Create Window 2 (UI Info)                                                      */
    /* -------------------------------------------------------------------------------- */
    UG_WindowCreate(&window_2, obj_buff_wnd_2, MAX_OBJ_NUM, window_2_callback);
    UG_WindowSetTitleText(&window_2, "About");
    UG_WindowSetTitleTextFont(&window_2, &FONT_12X20);
    UG_WindowResize(&window_2, 10, 10, 220, 220);
    /* Create Button 1 */
    UG_ButtonCreate(&window_2, &button2_1, BTN_ID_0, 10, 80, 120, 130);
    UG_ButtonSetFont(&window_2, BTN_ID_0, &FONT_12X20);
    UG_ButtonSetBackColor(&window_2, BTN_ID_0, C_LIME);
    UG_ButtonSetText(&window_2, BTN_ID_0, "OK");

    UG_WindowShow(&window_1);
    printf("test ugui\n");
    while (1)
    {
        UG_Update(); //Windows refesh
        los_sleep(1);
    }
    return 0;
}