// 3.1 - 栈
// 算法3.1 - 合法括号序列
// 输入：一个只包含"("和")"的字符串
// 输出：这个字符串是否为合法的括号序列

#include <iostream>
using namespace std;

// 算法3.1
bool isLegal(string seq) {
    int stackSize = 0;        // 这里不需要真的实现栈
    for (char c : seq) {      // 因为用到的只有栈的规模
        if (c == '(') {
            ++stackSize;      // 左括号对应push
        } else {              // 右括号对应pop
            if (--stackSize < 0) { 
                return false; // 如果对空栈pop则不合法
            }
        }
    }
    return stackSize == 0;    // 保证左右括号数量相同
}

int main() {
    string seq; cin >> seq;
    cout << "isLegal = " << isLegal(seq) << endl;
}