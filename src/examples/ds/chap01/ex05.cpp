// 问题1.5 - 角谷猜想
// 输入：正整数x
// 输出：对x反复进行操作：如果是奇数，乘3再加1；如果是偶数，除以2。直到x变为1为止。输出操作序列。

#include <iostream>
#include <list>
using namespace std;

// 算法1.5A
list<int>& f1(int x) {
    static list<int> ans; ans.clear();
	ans.push_back(x);
    do {
        if (x % 2) {
            ans.push_back(x = 3*x+1);
        } else {
            ans.push_back(x = x / 2);
        }
    } while (x != 1);
    return ans;
}

// 测试样例
int main() {
    int x; cin >> x;       // 对于int范围内的数据角谷猜想均正确，但较大的数可能会溢出
    cout << "op seq :" << endl;
    for (int y : f1(x)) {
        cout << y << endl;
    }
    return 0;
}
