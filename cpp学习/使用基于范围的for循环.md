# 基于范围的for循环

## 问题背景

1、C/C++程序中绝大多数的BUG都与内存非法访问有关，内存越限访问通常会导致读取的数据错误、数据踩踏、栈溢出等等安全问题。而循环又是内存越界访问的多发地，在for循环中常常因为循环的结束条件错误而导致内存访问异常。

```c++
/// 举例1：差1错误，导致内存越界访问
const size_t DATA_ITEM_NUM = 123
int data[DATA_ITEM_NUM] = {0};
int i;
for (i = 0; i <= DATA_ITEM_NUM; ++i) {
    data[i] = i; // 当i=123时，内存越界写，可能会改写i的值，导致循环异常
}

/// 举例2：忘记累加计数器，导致死循环
while (i < 123) {
    if (i % 2) {
        foo(i);
        i++;
    } else {
        bar(i);
    }
}
```

2、在for循环中使用迭代器，代码过于繁琐且容易出错。

```c++
/// 举例1：在C++11之前，基于迭代器的循环代码显得繁琐
vector<int> data = {1,2,3,4};
vector<int>::const_iterator iter = data.begin();
for (; iter != data.end(); ++iter) {
    cout << *iter << endl;
}
```

## 解决方案

为了提供更安全更简洁的循环，C++11引入了基于范围的for循环特性，避免了手写循环结束条件导致的内存越界访问以及死循环等问题，配合C++11的auto类型推导系统，使得循环代码更加简洁。

```c++
/// 例子1：普通数组遍历
const size_t DATA_ITEM_NUM = 123
int data[DATA_ITEM_NUM] = {0};
int i = 0;
for (auto &d : data) {
    d = i++;
}

// 例子2：标准容器遍历
vector<int> data = {1,2,3,4};
for (auto d : data) {
    cout << d << endl;
}
```

