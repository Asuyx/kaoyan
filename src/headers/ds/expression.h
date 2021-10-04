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
#include "binary_tree.h"
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

// 用来比较两个运算符的优先级
// 返回负数表示e1的优先级更高，返回正数表示e2的优先级更高
int orderBetween(const ExpressionElement& e1, const ExpressionElement& e2) {
    // 左边右括号：不可能存在，因为右括号不会进栈
    // 右边左括号：-1（最高优先级）
    // 阶乘：0
    // 乘方：1
    // 乘、除：2
    // 加、减、负号：3
    // 右边右括号：4（最低优先级，但需要比左边左括号高）
    // 左边左括号：5（最低优先级）
    int p1 = -1, p2 = -1;
    switch (e1.getOperator()) {
        case '+' : case '-' : case '@' : p1 = 3; break;
        case '*' : case '/' : p1 = 2; break;
        case '^' : p1 = 1; break;
        case '!' : p1 = 0; break;
        case '(' : p1 = 5; break; default:; 
    }
    switch (e2.getOperator()) {
        case '+' : case '-' : case '@' : p2 = 3; break;
        case '*' : case '/' : p2 = 2; break;
        case '^' : p2 = 1; break;
        case '!' : p2 = 0; break;
        case ')' : p2 = 4; break; default:; 
    }
    return p1 - p2;
}

class PrefixExpression;
class InfixExpression;
class SuffixExpression;
class ExpressionTree;

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
    int getResult() const { return 0; }
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
        } else {                                        // 如果新扫描到的是运算符
            if (isReadingNumber) {                      // 此前正在读取数字，则需要输出数字
                push_back(ExpressionElement(numberBuffer)); // 将读取完的数字输出
                numberBuffer = 0; isReadingNumber = false;  // 重置读取的数字
            } else {
                if ((size() == 0 || back().getOperator() == '(') && c == '-') {
                    push_back(ExpressionElement('@')); continue;
                }                                       // 特别判断：负号用“@”代替
            }
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

class ExpressionTree : public BinaryTree<ExpressionElement> {
public:
    int getResult() const;
    ExpressionTree(const InfixExpression& exp);
    ExpressionTree(const SuffixExpression& exp);
};

// 算法4.5 - 后缀表达式转换为表达式树
ExpressionTree::ExpressionTree(const SuffixExpression& exp) {
    Stack<BTNode<ExpressionElement>*> S;        // 构造使用的辅助栈
    exp.traverse([&](Rank, const ExpressionElement& e) -> void {
        auto v = new BTNode<ExpressionElement>(e); // 创建对应的节点
        if (e.getOperator() == '@') {           // 负号： null @ x
            v->rc = S.pop();
        } else if (e.getOperator() == '!') {    // 阶乘： x @ null
            v->lc = S.pop();
        } else if (e.isOperator()) {            // 其他运算符都是双目的
            v->rc = S.pop();
            v->lc = S.pop();
        }
        S.push(v);                              // 组合成子树，push回去
    });
    _root = S.pop();                            // 最后剩下的就是整个表达式树的根
    _size = exp.size();                         // 树的规模和后缀表达式长度相等
}

int ExpressionTree::getResult() const {
    Stack<int> S;                               // 用于计算结果的辅助栈
    postorderTraverse([&](const BTNode<ExpressionElement>* v) -> void {

    });
}

// 中缀表达式转换为表达式树
ExpressionTree::ExpressionTree(const InfixExpression& exp) {
    Stack<BTNode<ExpressionElement>*> So; // 运算符栈，用于保存优先级较低、还未进行计算的运算符
    Stack<BTNode<ExpressionElement>*> St; // 存储子树的栈
    auto buildTree = [&]() -> void {      // 用于构建一棵子树
        auto op = So.pop();               // 此次运算的运算符（树根） 
        (op->rc = St.pop())->parent = op;
        (op->lc = St.pop())->parent = op; // 注意在栈里面，左和右是反的
        St.push(op);                      // 构建好的子树，压回St
    };
    exp.traverse([&](Rank, const ExpressionElement& e) -> void {
        if (e.isOperator()) {             // 如果e是运算符
            while (orderBetween(So.top()->value, e) <= 0) { // e的优先级较低
                buildTree();                                // 需要先把优先级高的部分构建成树
            }
            if (e.getOperator() == ')') {                   // 右括号，构建到对应的左括号为止
                delete So.pop();                            // 将左括号弹出，右括号无需进栈
            } else {
                So.push(new BTNode<ExpressionElement>(e));  // 否则将运算符进栈
            }
        } else {                                            // 如果e不是运算符
            St.push(new BTNode<ExpressionElement>(e));      // 则直接将e进栈
        } 
    });
    while (!So.empty()) {                  // 清理最后还未接入的运算符
        buildTree();
    }
    _root = St.pop();                      // 剩下的唯一子树，就是运算符树
    // updateSize();                       // 如有必要，返回前可计算运算符树的规模
}


#endif