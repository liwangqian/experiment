# 使用std::array替代普通数组

## 背景

在C语言中，我们使用`int data[N]`来定义一个包含`N`个`int`型数据的静态数组，并且通过传递指针+数组长度的方式传递给子函数对数据进行处理。

在我们很多产品的代码中，只传递数组指针，而没有传递相应的数组长度的问题普遍存在，通常我们通过在子函数中**约定**数组长度的方式使用这个数组，这会导致许多不安全的问题，比如内存越界访问。该问题在Clean Code清理中耗费了相当大的人力进行接口整改。

```c
#include <stdint.h>

#define N 24
void fill_with(int *data, size_t len, int value)
{
    for (size_t i = 0; i < len; ++i) {
        data[len] = value;
    }
}

void deal_with(int *data)
{
    for (size_t i = 0; i < N; ++i) { // 假设data数组长度为N
        data[i] = i;
    }
}

int main(int argc, char *argv[])
{
    int data[N] = {0};

    // 调用子函数填充数据
    fill_with(data, N, 1);
    deal_with(data);
}
```

这样的写法通常没有什么问题，但是容易因为程序员的疏忽而引入BUG，比如数组的长度与实际的长度不符，或者接口使用者传递的数组长度与子函数约定的长度不一致，导致内存访问越界。

## 解决办法

在C++中，标准库提供了vector、array两种类，分别用来表述动态数组和静态数组，它们均提供`size()`成员函数，用于获取数组的长度信息，它们的使用方法为：

```c++
#include <vector>
#include <array>

#define N 24

void fill_with(std::vector<int> &vec, int value)
{
    for (auto &d : vec) {
        d = value;
    }
}

void fill_with(std::array<int, N> &data, int value)
{
    for (auto &d : data) {
        d = value;
    }
}

// 可以看到上述两个fill_with函数的实现时一样的，使用C++模板技术，可以提供一个通用的函数，避免代码重复
template <typename C>
void fill_with(C &c, typename C::value_type value)
{
    for (auto &d : data) {
        d = value;
    }
}

int main(int argc, char *argv[])
{
    std::vector<int> vec{N};
    fill_with(vec, 1);
    
    std::array<int, N> data{0};
    fill_with(data, 2);
}
```

