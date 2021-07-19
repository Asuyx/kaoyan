// 2.2 - 列表
// 这一节的主要内容都在这里
// 因为和向量比较相似，就不单独设计example了

#include <iostream>
#include "list.h"
using namespace std;

int main() {
    srand(time(0));
    showSections({
        []() -> void { // 构造函数
            auto L = List<int>(range(1, 8)); // 基于向量
            cout << L << endl;
            auto L2 = L;                     // 复制构造函数
            cout << L2 << endl;
        },
        []() -> void { // 循秩访问
            auto L = List<int>(range(1, 8, 2));
            cout << "L    = " << L << endl;
            cout << "L[2] = " << L[2]->value << endl;
            L[2]->value = 8;
            cout << "L[2] <- 8" << endl;
            cout << "L[2] = " << L[2]->value << endl;
            cout << "L    = " << L << endl;
        },
        []() -> void { // push(pop)_back(front)
            List<int> L;
            cout << "L    = " << L << endl;
            for (int i = 0; i < 3; ++i) {
                L.push_back(i);
                cout << "push back  = " << i << endl;
                cout << "L    = " << L << endl;
            }
            for (int i = 0; i < 3; ++i) {
                cout << "pop back   = " << L.pop_back() << endl;
                cout << "L    = " << L << endl;
            }
            for (int i = 0; i < 3; ++i) {
                L.push_front(i);
                cout << "push front = " << i << endl;
                cout << "L    = " << L << endl;
            }
            for (int i = 0; i < 3; ++i) {
                cout << "pop front  = " << L.pop_front() << endl;
                cout << "L    = " << L << endl;
            }
        },
        []() -> void { // 插入、删除、查找
            List<int> L(range(1, 6));
            cout << "L    = " << L << endl;
            for (int i = 4; i < 7; ++i) {
                Rank r = rand() % L.size();
                cout << "insert(" << i << " @ " << r << " as pred )" << endl;
                L.insertAsPred(i, L[r]);
                cout << "L    = " << L << endl;
            }
            for (int i = 5; i < 8; ++i) {
                Rank r = rand() % L.size();
                cout << "insert(" << i << " @ " << r << " as succ )" << endl;
                L.insertAsSucc(i, L[r]);
                cout << "L    = " << L << endl;
            }
            for (int i = 3; i < 6; ++i) {
                auto p = L.find(i);
                cout << "find(" << i << ")" << endl;
                cout << "context = " << "[" << p->pred->value << " - " << p->value << " - " << p->succ->value << "]" << endl;
            }
        },
        []() -> void { // 排序
            auto V = range(1, 17);
            V.shuffle();
            List<int> L(V);
            cout << "L    = " << L << endl;
            L.mergeSort();
            cout << "     sorted" << endl;
            cout << "L    = " << L << endl;
            Vector<int> VL(1<<20, 1<<20, [](Rank r) -> int {
                return rand()*rand();
            });
            L = VL;
            auto judgeSorted = [&L]() -> bool { // 验证列表是否有序
                for (auto p = L[1]; p != L.tail(); p = p->succ) {
                    if (p->pred->value > p->value) { return false; }
                }
                return true;
            };
            cout << "sorted? = " << judgeSorted() << endl;
            auto t = calculateTime([&L]() -> void {
                L.mergeSort();
            });
            cout << "sort t  = " << t << endl; // 注意这里的时间是显著高于例2.4里向量的排序时间的
            cout << "sorted? = " << judgeSorted() << endl;
        }
    });
    return 0;
}