// 问题1.8 - 幂
// 输入：正整数a、b
// 输出：a的b次幂

#ifndef POWER_H
#define POWER_H

class Power {
protected:
    virtual int power(int, int) { return 0; }
public:
    int operator()(int a, int b) {
        return power(a, b);
    }
};

// 算法1.8B
// 递归版快速幂
class PowerRecursion : public Power {
public:
    int power(int a, int b) {
        if (b == 1) {
            return a;
        }
        int temp = power(a, b/2);
        if (b % 2 == 1) { 
            return temp*temp*a;
        } else {
            return temp*temp;
        }
    }
};

#endif