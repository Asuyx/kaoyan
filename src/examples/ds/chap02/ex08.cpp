// 2.2 - 列表（动态列表）
// 这一节的主要内容都在例2.8和例2.9中
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
            const int list_size = 1<<22;
            Vector<int> VL(list_size, list_size, [](Rank r) -> int {
                return rand()*rand();
            });
            L = VL;
            auto judgeSorted = [&L]() -> bool { // 验证列表是否有序
                for (auto p = L[1]; p != L.tail(); p = p->succ) {
                    if (p->pred->value > p->value) { return false; }
                }
                return true;
            };
            Vector<ListNode<int>*> pos;         // 直接生成的列表，相邻元素的内存地址是相邻的
            L.traverse([&pos](ListNode<int>* p) -> void { // 这会导致它和向量一样能利用计算机的快速缓存机制
                pos.push_back(p);                         // 所以需要用一个随机化的pos，将它的逻辑次序打乱
            });
            pos.shuffle();
            L.head()->succ = pos[0];
            L.tail()->pred = pos[list_size-1];
            for (int i = 0; i < list_size; ++i) {
                pos[i]->pred = i == 0 ? L.head() : pos[i-1];
                pos[i]->succ = i == list_size - 1 ? L.tail() : pos[i+1];
            }
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
