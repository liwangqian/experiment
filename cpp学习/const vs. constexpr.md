# const vs. constexpr

`const`关键字最先在C++语言中支持，后来引入到C语言中。`constexpr`则是c++11引入的新的关键字，只在C++11及后续版本支持。在进一步讨论之前，我们 有必要先了解一下计算机程序中的**常量**和**变量**的概念。

### 常量

**常量**，的概念就是“不变的量”。在计算机程序中，通常表示值不变的量，常量又可以分为**常值变量**和**字面值常量**，**字面值常量**通常存放在程序的“常量区”，该内存区域为只读内存区，试图修改该区内的内存值，将会触发程序段错误。如下代码片段中，字符串`"Hello world"`为字面值常量，它的内存分配在只读数据区，而`msg`则为局部常值变量，它的内存分配在程序栈中。

```c++
int main(int argc, char *argv[])
{
    const char *msg = "Hello World";
    *(char*)(&msg[0]) = 'D'; // 尝试绕过编译器检查修改字面值常量的值
    return 0;
}

// 使用gdb进行调试，程序输出如下异常信息
//Program received signal SIGSEGV, Segmentation fault.
//0x00000000004011bf in main () at /xxx/main.cpp:6
//6	    msg[0] = 'D';

// 使用命令查看字面字符串常量的信息：objdump -s cpp_explore
//Contents of section .rodata: （只读数据区）
// 402000 01000200 48656c6c 6f20576f 726c6400  ....Hello World.
```

常值变量，表示的是一个变量的值在初始化后，不再改变。局部常值变量在程序的栈区分配内存，全局常值变量在程序的数据区分配内存。

```c++
const char *version = "1.0.0";

int main(int argc, char *argv[])
{
    const char *msg = "Hello World";
    return 0;
}

// (gdb) p &version
// $2 = (const char **) 0x404028 <version> // 全局常值变量内存分配在程序的数据区
// (gdb) p &msg
// $3 = (const char **) 0x7ffffffeeb50  // 局部常值变量内存分配在栈区
// (gdb) p/x *0x404028
// $6 = 0x402004
// (gdb) p/x *0x7ffffffeeb50
// $7 = 0x40200a

// 使用pbjdump -s命令查看程序数据段信息：
// Contents of section .rodata:
//  402000 01000200 312e302e 30004865 6c6c6f20  ....1.0.0.Hello
//  402010 576f726c 6400                        World.
// Contents of section .data:
//  404018 00000000 00000000 00000000 00000000  ................
//  404028 04204000 00000000                    . @.....
// 其中：04204000 --> 00402004

```

### 变量

**变量**，顾名思义就是变化的量。在计算机程序中，局部变量通常在程序栈区分配内存，全局变量则在程序的数据段或者bss段分配内存。

```c++
int g_InitVar = 1;
int g_UninitVar;

int GetValue()
{
    return 0;
}

int main(int argc, char *argv[])
{
    int localVar = GetValue();
    return 0;
}
```

查看可执行程序的代码段信息，可以看到g_InitVar位于.data段内，而g_UninitVar位于.bss段内，localVar的内存则位于程序栈内存空间中。

```shell
(gdb) p/x &g_InitVar
$1 = 0x404028
(gdb) p/x &g_UninitVar
$2 = 0x404030
(gdb) p/x &localVar
$3 = 0x7ffffffeeb4c
```

```shell
// readelf -S cpp_explore
  [22] .data             PROGBITS         0000000000404018  00003018
       0000000000000014  0000000000000000  WA       0     0     8
  [23] .bss              NOBITS           000000000040402c  0000302c
       000000000000000c  0000000000000000  WA       0     0     4
```



- `const`为`constant`的缩写，从单词字面意思来理解，表示"**常量**"的意思。然而在C/C++语言中，`const`修饰的量并不是**常量**，通常使用`const`来修饰**变量**，主要是用于告诉编译器，这个**变量的值**在初始化后，在变量的整个生命周期过程中，都不会被修改，以帮助编译器进行代码检查以及指令优化。
- 
- 两者的区别在于：`const`修饰的量为运行期**不可修改的变量**，而`constexpr`为编译期**常量**，`constexpr`表达式的值。

