# los
#### c Language vm,c语言解释器

### 使用教程
* 先把exe文件内win64.7z解压并把路径添加到环境系统变量
* 进入tset文件中，执行make 生成los.exe
* 运行build.bat生成test.los
* 运行los.exe执行。
#### ugui教程
* 进入tset/ugui文件中，执行make -f m生成los.exe。
* 执行make 生成test.los
* 运行los.exe执行

### 自定义添加函数
* int los_irq(int irq_num);中断函数,有返回值
* los.ld 文件内从167760000开始递增 如:PROVIDE ( functions = 167760000); functions可以在代码中直接调用
* 使用los_set_function注册回调函数
* 例子如下
```
los_set_function(los_cb);

void los_cb(int func_num)
{
    switch(func_num)
    {
        case 0: //这是167760000的函数即 函数functions

        break;
        case ....

        break;
    }
}
```

### 说明
* clang,losld,llvm-objcopy,使用方式请看:http://llvm.org/
* obj文件可以使用ar进行打包生成.a库文件