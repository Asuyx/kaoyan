// 3.1 - 栈
// 尾递归消除的情况1
// 无返回值的函数

#include <iostream>
#include <functional>
#include <vector>
#include "vector.h"
#include "stack.h"
using namespace std;

// 模板
template <typename ParameterType>
class TailRecursion {
public:
    typedef function<ParameterType(ParameterType)> GenerateFunction;
    function<void(ParameterType)> basicProcedure;  // 函数除了尾递归之外的程序体
    function<bool(ParameterType)> recursiveBorder; // 判断是否是递归边界的函数
    function<void(ParameterType)> borderProcedure; // 到达递归边界之后的处理函数
    Vector<GenerateFunction> dependentParameters;  // 递归是如何产生新参数的函数列表
    void recursiveFunction(ParameterType para) {   // 递归版本的函数模板
        if (recursiveBorder(para)) {               // 判断是否到达递归边界
            borderProcedure(para);
        } else {
            basicProcedure(para);                  // 程序体
            for (int i = 0; i < dependentParameters.size(); ++i) {
                recursiveFunction(dependentParameters[i](para));
            }                                      // 尾递归
        }
    }
    void iterativeFunction(ParameterType para) {    // 迭代版本的函数模板
        Stack<ParameterType> S;  // 用来存储所有待执行的函数的参数
        S.push(para);            // 从外部传进来的参数开始，准备递归
        while (!S.empty()) {     // 当栈非空的时候说明还没有执行完所有的递归实例
            para = S.pop();      // 取出栈顶元素作为参数，运行递归函数
            if (recursiveBorder(para)) {            // 判断是否到达递归边界
                borderProcedure(para);
            } else {
                basicProcedure(para);               // 程序体
                for (int i = dependentParameters.size() - 1; i >= 0; --i) {
                    S.push(dependentParameters[i](para));
                }                                   // 尾递归改为入栈
            }
        }
    }
};

// 下面是举例：
// 输入：无
// 输出：所有满足条件的4位数
//      它的4个数位均为1、2、3或4
//      输出顺序为从小到大的次序

// 为了方便对比，首先输出到向量里而不是直接输出到cout
Vector<Vector<int>> answerVectors;
Vector<int>* answerVectorPointer;

void f1(int x = 0) {
    if (x > 1000) {           // 4位数，输出
        answerVectorPointer->push_back(x);
    } else {
        for (int i = 1; i < 5; ++i) {
            f1(x*10 + i);     // 枚举可能的下一位
        }
    }
}

void f2() {
    int x = 0;
    Stack<int> S; S.push(x);
    while (!S.empty()) {
        x = S.pop();
        if (x > 1000) {          // 4位数，输出
            answerVectorPointer->push_back(x);
        } else {
            for (int i = 4; i > 0; --i) {
                S.push(x*10 + i); // 枚举可能的下一位
            }
        }
    }
}

int main() {
    TailRecursion<int> TR;
    TR.basicProcedure = ([](int) -> void {});
    TR.borderProcedure = ([&](int x) -> void { answerVectorPointer->push_back(x); });
    TR.recursiveBorder = ([](int x) -> bool { return x > 1000; });
    TR.dependentParameters = Vector<TailRecursion<int>::GenerateFunction>(
        4, 4, [](int y) -> TailRecursion<int>::GenerateFunction {
            return [=](int x) -> int { return x*10 + y + 1; };
        }
    );
    vector<function<void()>> functionVersions = {
        []() -> void { f1(); },   // 直接的递归版本
        []() -> void { f2(); },   // 直接的迭代版本
        [&]() -> void { TR.recursiveFunction(0); }, // 使用模板的递归版本
        [&]() -> void { TR.iterativeFunction(0); }  // 使用模板的迭代版本
    };
    for (int i = 0; i < 4; ++i) { // 分别运行4种版本的函数，输出到向量里
        answerVectorPointer = &answerVectors[i];
        functionVersions[i]();
    }
    // 将各个版本的函数的运行结果，按照列输出
    bool allEqual = true;
    for (int i = 0; i < answerVectorPointer->size(); ++i) {
        for (int j = 0; j < 4; ++j) {
            cout << answerVectors[j][i] << "\t";
            if (answerVectors[j][i] != answerVectors[0][i]) {
                allEqual = false;
            }
        }
        cout << endl;
    }
    cout << "All Equal = " << allEqual << endl;
    return 0;
}