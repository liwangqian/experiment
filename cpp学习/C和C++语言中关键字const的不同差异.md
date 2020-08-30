# C和C++语言中关键字`const`的差异

在C和C++语言中，关键字`const`都用来表示其修饰的变量是只读的，它的值必须初始化，且初始化后的值在程序运行过程中不会被修改，`const`修饰的变量不能作为左值，否则编译期会报错，以防止变量被误修改。

```c++
const uint32_t maxObjNum = 10;
maxObjNum = 12; // compile error: cannot assign to variable 'maxObjNum' with const-qualified type 'const uint32_t' (aka 'const unsigned int')
```

`const`关键字是左结合的，修饰的是其左侧的变量，当左侧为空时，转为右结合。

```c++
uint32_t const maxObjNum = 10; 		      		// (1) OK
uint32_t const *pMaxObjNum = &maxObjNum;  		// (2) OK
uint32_t * const pMaxObjNum = &maxObjNum; 		// (3) Error
uint32_t const * const pMaxObjNum = &maxObjNum; // (4) OK

typedef char * cchar_t;
const cchar_t ver = "Hello World"; // (5) warning: char const * ==> char * const
```

- (1) `const`修饰的是`uint32_t`，表示`maxObjNum`的值是只读的，等效于`const uint32_t maxObjNum = 10;`。

- (2) `const`修饰的是`uint32_t`，表示指针指向的地址中保存的整型值是只读的，不能通过指针间接修改整型值，等效于`const uint32_t *pMaxObjNum = &maxObjNum;`。

- (3) `const`修饰的是指针`uint32_t*`，表示指向`uint32_t`的`const`指针，指针变量的值(地址)为只读的，可以通过该指针修改指针地址内存中保存的整型值，而`&maxObjNum`的类型为指向`uint32_t const`的指针，指针变量的值(地址)可读写，但是不能通过该指针间接修改指向内存的整型值，因此赋值表达式两端的类型不一致，编译器报错。
- (4) `pMaxObjNum`的值以及它指向的内存中的整型值均为只读的。
- (5) 当`const`用于修饰自定义类型时，只需要按照`const`的左结合特点进行类型展开即可得到`const`真实修饰的类型。

`const`关键字在C和C++语言中除了表示变量的只读属性外，还有一些比较大的差异。

| 功能           | 作用                     | 例子                                      | C语言 | C++语言 |
| -------------- | ------------------------ | ----------------------------------------- | ----- | ------- |
| 修饰变量       | 变量的值初始化后不变     | const uint32_t var = 8;                   | ✔     | ✔       |
| 修饰指针变量①  | 指针的值在初始化后不变   | FILE * const fp= open(file);              | ✔     | ✔       |
| 修饰参数       | 函数内部不修改该参数的值 | void Foo(const char *bar);                | ✔     | ✔       |
| 修饰返回值类型 | 返回的值不能被修改       | const char *GetName();                    | ✔     | ✔       |
| 修饰数组       | 数组的所有元素均为只读   | const uint32_t serdesId[] = { 1, 2 };     | ✔     | ✔       |
| 修饰类成员函数 | 该成员函数不修改类的状态 | class Foo { uint32_t GetCount() const; }; | --    | ✔       |
| 修饰类成员变量 | 同“修饰变量”             | class Foo { const uint32_t count{1}; };   | --    | ✔       |
|                |                          |                                           |       |         |
|                |                          |                                           |       |         |

接下来我们举例对上述功能点进行探讨。

```c
#include <stdio.h>

int main(int argc, char *argv[])
{
    const int x = 10;
    int *y = (int*)&x;
    (*y)++;

    printf("%d:", *y);
    printf("%d", x);
    return 0;
}
```

![image-20200816185726955](C%E5%92%8CC++%E8%AF%AD%E8%A8%80%E4%B8%AD%E5%85%B3%E9%94%AE%E5%AD%97const%E7%9A%84%E4%B8%8D%E5%90%8C%E5%B7%AE%E5%BC%82.assets/image-20200816185726955.png)

![image-20200816185801977](C%E5%92%8CC++%E8%AF%AD%E8%A8%80%E4%B8%AD%E5%85%B3%E9%94%AE%E5%AD%97const%E7%9A%84%E4%B8%8D%E5%90%8C%E5%B7%AE%E5%BC%82.assets/image-20200816185801977.png)