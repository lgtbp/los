# los使用教程

## 宏定义
* MEM_CUSTOM:用户自行实现lpram_malloc和lpram_free，默认使用malloc和free内存管理,
* LOS_DEBUG:检查栈是否溢出,打印运行信息,默认不开启
* SOC_32_BIT:是32位的host，默认是64位

# 使用los
1. 定义los_function_arr函数列表
2. 添加实现自己需要的函数如:function1,function2,function3
3. 如果使用自动生成los.ld脚本，必须添加 [//los_api_start] 和 [//los_api_end]
4. 使用arch/gapi.py生成los.ld脚本文件,提供给clang编译
```
void function1(void){  /* doing something */  }
void function2(void){  /* doing something */  }
void function3(void){  /* doing something */  }
const fun_os los_function_arr[] = {
    //los_api_start
    function1,
    function2,
    function3,
    //los_api_end
};
int los_app_frist(uint8_t *los)
{
    los_set_function(los_function_arr);//注册函数
    return los_app_first(los, 1);//加载并运行,运行结束才返回
}
```

# 编译代码
1. 编译使用与clang,gcc用法一样，可参考test文件夹的makefile文件
2. 注意把los.ld放同目录下，或指定链接脚本文件