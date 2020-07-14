#include "stdint.h"
#include "lvgl.h"
#include "lv_demo_printer.h"
#include "lv_demo_keypad_encoder.h"
#include "lv_demo_stress.h"
#include "lv_demo_benchmark.h"

static bool mouse_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
  /*Get the current x and y coordinates*/
  data->point.x = los_get_x();
  data->point.y = los_get_y();
  /*Get whether the mouse button is pressed or released*/
  if (los_mouse_is_pressed())
  {
    data->state = LV_INDEV_STATE_PR;
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }

  /*Return `false` because we are not buffering and no more data to read*/
  return false;
}
static bool touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
  static lv_coord_t last_x = 0;
  static lv_coord_t last_y = 0;
  /*Save the pressed coordinates and the state*/
  if (los_mouse_is_pressed())
  {
    last_x = los_get_x();
    last_y = los_get_y();
    data->state = LV_INDEV_STATE_PR;
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }

  /*Set the last pressed coordinates*/
  data->point.x = last_x;
  data->point.y = last_y;

  /*Return `false` because we are not buffering and no more data to read*/
  return false;
}

static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
  // int32_t x;
  // int32_t y;
  // for (y = area->y1; y <= area->y2; y++)
  // {
  //   for (x = area->x1; x <= area->x2; x++)
  //   {
  //     /* Put a pixel to the display. For example: */
  //     los_draw_point(x, y, color_p->full);
  //     color_p++;
  //   }
  // }
  los_disp_flush(area->x1, area->y1, area->x2, area->y2, &color_p->full);
  /* IMPORTANT!!!
     * Inform the graphics library that you are ready with the flushing*/
  lv_disp_flush_ready(disp_drv);
}

static lv_disp_buf_t disp_buf_2;
static lv_color_t buf2_1[LV_HOR_RES_MAX * 400]; /*A buffer for 10 rows*/
//static lv_color_t buf2_2[LV_HOR_RES_MAX * LV_VER_RES_MAX]; /*An other buffer for 10 rows*/
lv_indev_t *indev_touchpad;
lv_indev_drv_t indev_drv;
lv_indev_t *indev_mouse;
void touch_pad()
{
  /*Register a touchpad input device*/
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = touchpad_read;
  indev_touchpad = lv_indev_drv_register(&indev_drv);
}
void mouse_init()
{
  /*Register a mouse input device*/
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = mouse_read;
  indev_mouse = lv_indev_drv_register(&indev_drv);

  /*Set cursor. For simplicity set a HOME symbol now.*/
  lv_obj_t *mouse_cursor = lv_img_create(lv_disp_get_scr_act(NULL), NULL);
  lv_img_set_src(mouse_cursor, LV_SYMBOL_HOME);
  lv_indev_set_cursor(indev_mouse, mouse_cursor);
}

int main(void)
{
  lv_init();
  lv_disp_buf_init(&disp_buf_2, buf2_1, NULL, LV_HOR_RES_MAX * 400); /*Initialize the display buffer*/
  lv_disp_drv_t disp_drv;                                            /*Descriptor of a display driver*/
  lv_disp_drv_init(&disp_drv);                                       /*Basic initialization*/
  /*Set up the functions to access to your display*/

  /*Set the resolution of the display*/
  disp_drv.hor_res = 800;
  disp_drv.ver_res = 480;

  /*Used to copy the buffer's content to the display*/
  disp_drv.flush_cb = disp_flush;

  /*Set a display buffer*/
  disp_drv.buffer = &disp_buf_2;
  lv_disp_drv_register(&disp_drv);

  mouse_init();

  lv_demo_printer();
  // lv_demo_stress();
  // lv_demo_keypad_encoder();
  // lv_demo_benchmark();
  while (1)
  {
    lv_task_handler();
    los_sleep(1);
    lv_tick_inc(20);
  }
  return 0;
}
