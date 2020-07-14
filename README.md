# los
#### c Language vm,c语言解释器

### 使用教程
* 先把exe文件内win64.7z解压并把路径添加到环境系统变量
* 进入sdl2/los文件中，执行make 生成losr.exe
* 注意:(需要把SDL2.dll添加到环境系统变量，或同一目录下)

#### lvgl教程
* 进入tset/lvgl文件中，执行make生成test.los。
* 运行[losr.exe test.los]执行
* 运行其他demo，请在test\lvgl\lv_ex_conf.h改宏定义，以及test.c中改一下调用demo

#### ugui教程
* 进入tset/ugui文件中，执行make生成test.los。
* 运行[losr.exe test.los]执行

### 自定义添加函数
* int los_irq(int irq_num);中断函数,有返回值
* los.ld 文件内从167760000开始递增 如:PROVIDE ( functions = 167760000); functions可以在代码中直接调用
* 使用los_set_function注册回调函数
* 例子如下
```
los_set_function(los_function_arr); //设置函数

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
void los_sprintf(void)
{
    uint8_t buf[256];
    los_sprintf_low(buf, los_get_argp(0), 1);
    printf("%s", buf);
}
```

### 说明
* clang,losld,llvm-objcopy,使用方式请看:http://llvm.org/
* obj文件可以使用ar进行打包生成.a库文件