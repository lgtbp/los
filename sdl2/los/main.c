#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "los.h"
#include <windows.h>
#include <unistd.h>
#include <fcntl.h>
SDL_Renderer *renderer;
//SDL_Thread *thread;
HANDLE thread;
SDL_mutex *lock;
#define RGB565toRGB(rgb565) ((uint32_t)(((uint8_t)((((rgb565)&0xF800) >> 11) << 3) | ((uint32_t)((uint8_t)((((rgb565)&0x07E0) >> 5) << 2)) << 8)) | (((uint32_t)(uint8_t)(((rgb565)&0x001F) << 3)) << 16)))
int px = 0, py = 0, pm = 0;
//buf_path当前路径
//len代码空间
uint8_t *read_file_bin(char *buf_path, uint32_t *len);
extern int los_sprintf_low(char *buf, const char *fmt, uint8_t va_num);
extern void lcd_draw_point(uint16_t x, uint16_t y, uint32_t c);
void los_sprintf(void)
{
    uint8_t buf[256];
    los_sprintf_low(buf, los_get_argp(0), 1);
    printf("%s", buf);
}
void los_sellp(void)
{
    Sleep(*(uint32_t *)los_get_arg(0));
}
void los_get_x(void)
{
    los_set_return(px - 10);
}
void los_get_y(void)
{
    los_set_return(py - 10);
}

void los_draw_point(void) //
{

    uint32_t rgb;
    uint8_t r, g, b;
    uint16_t rgb565 = *(uint16_t *)los_get_arg(2);
    //uint32_t rgb565 = *(uint32_t *)los_get_arg(2);
    //printf("rgb=%x\n", rgb565);
    b = rgb565 & 0x1f;
    g = (rgb565 >> 5) & 0x3f;
    r = (rgb565 >> 11) & 0x1f;
    SDL_LockMutex(lock);
    //SDL_SetRenderDrawColor(renderer, (rgb565 >> 16) & 0xff, (rgb565 >> 8) & 0xff, rgb565 & 0xff, (rgb565 >> 24) & 0xff);
    SDL_SetRenderDrawColor(renderer, r << 3, g << 2, b << 3, 255);
    SDL_RenderDrawPoint(renderer, 10 + *(uint16_t *)los_get_arg(0), 10 + *(uint16_t *)los_get_arg(1));
    SDL_UnlockMutex(lock);
}
typedef union
{
    struct
    {
        uint16_t blue : 5;
        uint16_t green : 6;
        uint16_t red : 5;
    } ch;
    uint16_t full;
} los_color16_t;
void los_disp_flush(void)
{
    uint32_t rgb;
    uint8_t r, g, b;
    int32_t x, x1, x2;
    int32_t y, y1, y2;
    los_color16_t *color_p = (los_color16_t *)los_get_argp(4);
    uint16_t rgb565;

    x1 = *(int16_t *)los_get_arg(0);
    y1 = *(int16_t *)los_get_arg(1);
    x2 = *(int16_t *)los_get_arg(2);
    y2 = *(int16_t *)los_get_arg(3);
    SDL_LockMutex(lock);
    for (y = y1; y <= y2; y++)
    {
        for (x = x1; x <= x2; x++)
        {
            rgb565 = color_p->full;
            b = rgb565 & 0x1f;
            g = (rgb565 >> 5) & 0x3f;
            r = (rgb565 >> 11) & 0x1f;
            SDL_SetRenderDrawColor(renderer, r << 3, g << 2, b << 3, 255);
            SDL_RenderDrawPoint(renderer, 10 + x, 10 + y);
            color_p++;
        }
    }
    //SDL_SetRenderDrawColor(renderer, (rgb565 >> 16) & 0xff, (rgb565 >> 8) & 0xff, rgb565 & 0xff, (rgb565 >> 24) & 0xff);
    SDL_UnlockMutex(lock);
}
void los_mouse_is_pressed()
{
    los_set_return(pm);
}
const fun_os los_function_arr[] = {
    los_sprintf,
    los_sellp,
    los_get_x,
    los_get_y,
    los_draw_point,
    los_disp_flush,
    0,
    los_mouse_is_pressed,
};

int los_app_frist(char *path)
{
    int res;
    uint32_t len, clen = 0;
    uint8_t *los;
    los = read_file_bin(path, &len);
    if (los == 0)
        return 2;
    printf("path=%s,%d\r\n", path, len);
    los_set_function(los_function_arr);
    res = los_app_first((uint32_t *)los, 1);
    //printf("res:%d\n", res);
    return 0;
}
int main(int argc, char *argv[])
{
    char *losname = 0;
    SDL_Window *window = SDL_CreateWindow("los vm ui", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 820, 500, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    bool quit = false;
    SDL_Event event;
    lock = SDL_CreateMutex();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    if (argc > 1)
        losname = argv[1];
    thread = SDL_CreateThread(los_app_frist, "los", losname);
    // thread = CreateThread(NULL, 0, los_app_frist, losname, 0, NULL);
    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEMOTION:
                if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1))
                {
                    px = event.motion.x;
                    py = event.motion.y;
                    pm = 1;
                    SDL_RenderDrawPoint(renderer, event.motion.x, event.motion.y);
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                px = event.motion.x;
                py = event.motion.y;
                pm = 1;
                // SuspendThread(thread);
                // los_irq(0);
                // ResumeThread(thread);
                break;
            case SDL_MOUSEBUTTONUP:
                pm = 0;
                // SuspendThread(thread);
                // los_irq(1);
                // ResumeThread(thread);
                break;
            }
        }
        SDL_LockMutex(lock);
        SDL_RenderPresent(renderer);
        SDL_UnlockMutex(lock);
        SDL_Delay(1000 / 60);
    }
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    return 0;
}
//buf_path当前路径
//len代码空间
uint8_t *read_file_bin(char *buf_path, uint32_t *len)
{
    FILE *pF;
    uint8_t *buf;
    pF = fopen(buf_path, "rb");
    if (NULL == pF)
    {
        fprintf(stderr, "open bin ERR= %s\r\n", buf_path);
        len[0] = 0;
        return NULL;
    }
    fseek(pF, 0, SEEK_END);
    len[0] = ftell(pF);
    buf = (uint8_t *)malloc(len[0]);
    rewind(pF);
    fread(buf, 1, len[0], pF);
    fclose(pF);
    return buf;
}