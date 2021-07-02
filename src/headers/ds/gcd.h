// 问题1.2 - 求最大公因数
// 输入：正整数a、b
// 输出：a和b的最大公因数

class Gcd {
protected:
    virtual int gcd(int, int) { return 0; }
public:
    int operator()(int a, int b) {
        return gcd(a, b);
    }
};

// 算法1.2A
// 递归版Euclid辗转相除法
class GcdEuclidRecursion : public Gcd {
public:
    int gcd(int a, int b) {
        if (b == 0) {
            return a;
        } else {
            return gcd(b, a % b);
        }
    }
};