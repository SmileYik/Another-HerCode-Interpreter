# Another HerCode interpreter - 另一个HerCode解释器

使用 C++ 实现的 HerCode 解释器.
原项目: https://github.com/nkxingxh/HerCode

## 编译 HerCode VM / HerCode Console

```
cmake -S . -B build
cd build
make
```

## 使用

### 交互模式

在终端中直接运行即可进入交互模式.

```
sh-5.2$ ./hercode-console 
function test_func_a:
>> say "I am function A!"
>> end
function test_func_b:
>> say "Before A"
>> test_func_a
>> say "After A"
>> end
start:
>> say "Hello HerCode!!!"
Hello HerCode!!!
>> test_func_b
Before A
I am function A!
After A
>> end
^C
```

### 运行文件

使用如下命令运行HerCode代码文件.

```
./hercode-console 代码路径
```

例子:

```
sh-5.2$ ./hercode-console ../resources/test.hc 
Hello HerCode!!
Before calling function A!
I am function A!
Before called function B!
sh-5.2$ 
```

# 许可

你拿到了那就是你的.