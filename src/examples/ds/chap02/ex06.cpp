// 2.1 - 向量
// 其他例子中没展示的成员函数功能演示

#include <iostream>
#include "vector.h"
using namespace std;

int main() {
    showSections({
        []() -> void { // range函数
            cout << range(1, 8) << endl;
            cout << range(1, 8, 2) << endl;
            cout << range(1, 8, -1) << endl;
            cout << range(8, 1, -3) << endl;
        },
        []() -> void { // 循秩访问
            auto V = range(1, 8, 2);
            cout << "V    = " << V << endl;
            cout << "V[2] = " << V[2] << endl;
            V[2] = 8;
            cout << "V[2] <- 8" << endl;
            cout << "V[2] = " << V[2] << endl;
            cout << "V    = " << V << endl;
        },
        []() -> void { // push(pop)_back
            Vector<int> V;
            cout << "V    = " << V << endl;
            for (int i = 0; i < 3; ++i) {
                V.push_back(i);
                cout << "push = " << i << endl;
                cout << "V    = " << V << endl;
            }
            for (int i = 0; i < 3; ++i) {
                cout << "pop  = " << V.pop_back() << endl;
                cout << "V    = " << V << endl;
            }
        },
        []() -> void { // 批量插入元素
            auto V1 = range(1, 8, 2);   
            auto V2 = range(2, 9, 2);
            cout << "V1   = " << V1 << endl;
            cout << "V2   = " << V2 << endl;
            V1.insert(V2, 2);
            cout << "V1.insert(V2, 2)" << endl;
            cout << "V1   = " << V1 << endl;
            V2.insert(V1, 0);
            cout << "V2.insert(V1, 0)" << endl;
            cout << "V2   = " << V2 << endl;  
        },
        []() -> void { // 批量查找元素
            Vector<int> V(16, 10, [](int r) -> int {
                if (r % 2 == 1) {
                    return 1;
                } else {
                    return r;
                }
            });
            cout << "V       = " << V << endl;
            for (int i = 1; i <= 3; ++i) {
                cout << "find(" << i << ") = " << V.find(i) << endl;
                cout << "find[" << i << "] = " << V.findAll([i](Rank r, const int& e) -> bool {
                    return i == e;
                }) << endl;
            }
        },
        []() -> void { // 批量删除元素    
            auto V = range(1, 10);
            cout << "V    = " << V << endl;
            V.removeAll([](Rank r, const int& e) -> bool {
                return e % 2 == 0;
            });
            cout << "deleted even-value elements" << endl;
            cout << "V    = " << V << endl;
            V.removeAll([](Rank r, const int& e) -> bool {
                return r % 2 == 1;
            });
            cout << "deleted odd-rank elements" << endl;
            cout << "V    = " << V << endl;
        },
        []() -> void { // 折半查找
            auto V = range(1, 20, 2);
            cout << "V    = " << V << endl;
            for (int i : {0, 1, 7, 8, 19, 100}) {
                cout << "find(" << i << ") : \t" << V.find(i) << "\t" << V.binarySearch(i) << endl;
            }
        }
    });
    return 0;
}