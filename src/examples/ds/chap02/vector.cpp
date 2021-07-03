// 2.1 - 向量 的代码效果展示


#include <iostream>
#include "vector.h"
using namespace std;

// 可通过调整编译选项来激活特定的代码效果展示模块
#define DISPLAY_RANGE // 展示range、fill函数生成向量、字符串化（和笔记正文无关）
#define DISPLAY_RANK  // 展示循秩访问

int main() {
#ifdef DISPLAY_RANGE    
    cout << range(1, 8) << endl;
    cout << range(1, 8, 2) << endl;
    cout << range(1, 8, -1) << endl;
    cout << range(8, 1, -3) << endl;
#endif

#ifdef DISPLAY_RANK
    auto V = range(1, 8, 2);
    cout << "V    = " << V << endl;
    cout << "V[2] = " << V[2] << endl;
    V[2] = 8;
    cout << "V[2] <- 8" << endl;
    cout << "V[2] = " << V[2] << endl;
    cout << "V    = " << V << endl;
#endif


    return 0;
}
