// 3.1 - 栈
// 这个文件实现了三种表达式
// 前缀表达式、中缀表达式、后缀表达式
// 在这个文件中，只讨论整数之间的运算
// 并且只讨论加、减、乘、除、乘方和阶乘
// 其中，负号在前缀、后缀表达式中用@代替

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <iostream>
#include <cctype>
#include "power.h"
#include "stack.h"
#include "factorial.h"
using namespace std;

// 用来表示表达式中的一个元素
class ExpressionElement {
    bool _isOperator; // 是否是运算符
    union ExpressionElementField {
        int number;
        char op;
    } field;

public:
    bool isOperator() const { return _isOperator; }
    bool isNumber() const { return !_isOperator; }

    char getOperator() const { return field.op; }
    int getNumber () const { return field.number; }

    ExpressionElement(char op) {
        _isOperator = true;
        field.op = op;
    }

    ExpressionElement(int n) {
        _isOperator = false;
        field.number = n;
    }

    // 运算符关联的操作数数量；如果本身为操作数，返回0
    int operationNumberCount() const {
        if (isNumber()) { return 0; }
        if (getOperator() == '!' || getOperator() == '@') { return 1; } // 实现的运算符里只有负号和阶乘是一元的
        else { return 2; }
    }

    // 进行运算
    int apply(Vector<int> args) const {
        int opCount = operationNumberCount();
        if (opCount == 0) { return getNumber(); }
        if (opCount == 1) {
            int arg1 = args[0];
            switch (getOperator()) {
                case '@' : return - arg1;
                case '!' : return factorial(arg1);
            }
        } else {
            int arg2 = args[0];
            int arg1 = args[1];
            switch (getOperator()) {
                case '+' : return arg1 + arg2;
                case '-' : return arg1 - arg2;
                case '*' : return arg1 * arg2;
                case '/' : return arg1 / arg2;
                case '^' : return PowerRecursion()(arg1, arg2);
            }
        }
        return 0; // 未定义的计算
    }
};

class PrefixExpression;
class InfixExpression;
class SuffixExpression;

// 表达式的基类
// 表达式可以表示成元素组成的向量
class Expression : public virtual Vector<ExpressionElement> {
public:
    virtual int getResult() const;
    friend ostream& operator<< (ostream& out, const Expression& exp);
};

ostream& operator<< (ostream& out, const Expression& exp) {
    exp.traverse([&](Rank r, const ExpressionElement& e) -> void {
        if (e.isOperator()) {
            out << e.getOperator();
        } else {
            out << e.getNumber();
        }
    });
}

// 中缀表达式
class InfixExpression : public Expression {
public:
    int getResult() const;
    InfixExpression(string exp);
};

// 字符串转中缀表达式
InfixExpression::InfixExpression(string exp) {
    int numberBuffer = 0;                               // 用来存放正在读取的数字
    bool isReadingNumber = false;                       // 是否正在读取数字
    for (char c : exp) {
        if (isdigit(c)) {                               // 更新正在读取的数字
            numberBuffer *= 10;
            numberBuffer += c - '0';
            isReadingNumber = true;
        } else {
            push_back(ExpressionElement(numberBuffer)); // 将读取完的数字输出
            numberBuffer = 0; isReadingNumber = false;  // 重置读取的数字
            push_back(ExpressionElement(c));            // 然后将当前的运算符输出
        }
    }
    if (isReadingNumber) {                              // 如果表达式以数字结尾
        push_back(numberBuffer);                        // 需要将最后的数字加入进去
    }
}

// 后缀表达式
class SuffixExpression : public Expression {
public:
    int getResult() const;
};

// 算法3.2 - 后缀表达式的计算
int SuffixExpression::getResult() const {
    Stack<int> S;                               // 计算使用的辅助栈
    traverse([&](Rank, const ExpressionElement& e) -> void {
        int opCount = e.operationNumberCount(); // 获取运算符所需的操作数数量
        S.push(e.apply(Vector<int>(opCount, opCount, [&](int) -> int {
            return S.pop();                     // 进行opCount次pop
        })));                                   // 运算后push回去
    });
    return S.pop();                             // 最终栈里剩下了一个计算结果
}


#endif