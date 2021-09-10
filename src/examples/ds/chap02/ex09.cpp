// 2.2 - 列表（静态列表）

#include <iostream>
#include "static_list.h"
using namespace std;

int main() {
    srand(time(0));
    showSections({
        []() -> void { // 构造函数
            auto L = StaticList<int>(range(1, 8)); // 基于向量
            cout << L << endl;
            auto L2 = L;                     // 复制构造函数
            cout << L2 << endl;
        },
        []() -> void { // 循秩访问
            auto L = StaticList<int>(range(1, 8, 2));
            cout << "L    = " << L << endl;
            cout << "L[2] = " << L[2] << endl;
            L[2] = 8;
            cout << "L[2] <- 8" << endl;
            cout << "L[2] = " << L[2] << endl;
            cout << "L    = " << L << endl;
        },
        []() -> void { // push(pop)_back(front)
            StaticList<int> L;
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
            StaticList<int> L(range(1, 6));
            cout << "L    = " << L << endl;
            for (int i = 4; i < 7; ++i) {
                Rank r = rand() % L.size();
                cout << "insert(" << i << " @ " << r << " as pred )" << endl;
                L.insertAsPred(i, L.forward(L.head(),r+1));
                cout << "L    = " << L << endl;
            }
            for (int i = 5; i < 8; ++i) {
                Rank r = rand() % L.size();
                cout << "insert(" << i << " @ " << r << " as succ )" << endl;
                L.insertAsSucc(i, L.forward(L.head(),r+1));
                cout << "L    = " << L << endl;
            }
            for (int i = 3; i < 6; ++i) {
                auto p = L.find(i);
                cout << "find(" << i << ")" << endl;
                cout << "context = " << "[" << L.get(L.getPred(p)) << " - " << L.get(p) << " - " << L.get(L.getSucc(p)) << "]" << endl;
            }
        },
        []() -> void { // 排序
            auto V = range(1, 17);
            V.shuffle();
            StaticList<int> L(V);
            cout << "L    = " << L << endl;
            L.mergeSort();
            cout << "     sorted" << endl;
            cout << "L    = " << L << endl;
            const int list_size = 1<<22;
            Vector<int> VL(list_size, list_size, [](Rank r) -> int {
                return rand()*rand();
            });
            L = VL;
            auto judgeSorted = [](const StaticList<int>& L) -> bool {
                for (Rank r = L.getSucc(L.head()); r != L.tail(); r = L.getSucc(r)) { 
                    if (L.get(L.getPred(r)) > L.get(r)) {
                        return false;
                    }
                }
                return true;
            }; // 判断静态列表是否有序的函数
            auto ranks = range(2, list_size+2);// 直接用向量V生成后，V的元素次序和L的元素次序是一样的
            ranks.shuffle();                   // 因此，需要用一个辅助向量ranks，对V中的元素进行重排
            L.getSucc(L.head()) = ranks[0];
            L.getPred(L.tail()) = ranks[list_size-1];
            for (int i = 0; i < list_size; ++i) {
                L.getPred(ranks[i]) = i == 0 ? L.head() : ranks[i-1];
                L.getSucc(ranks[i]) = i == list_size - 1 ? L.tail() : ranks[i+1];
            }
            auto testSort = [&](function<void(StaticList<int>&)> sortFunction) -> void {
                StaticList<int> Ltest(L);
                cout << "sorted? = " << judgeSorted(Ltest) << endl;
                auto t = calculateTime([&]() -> void {
                    sortFunction(Ltest);
                });
                cout << "sort t  = " << t << endl;
                cout << "sorted? = " << judgeSorted(Ltest) << endl;     
            };
            testSort([](StaticList<int>& L) -> void {
                L.mergeSort(); // 这里的时间和例2.8差不多
            });
            testSort([](StaticList<int>& L) -> void {
                // 直接利用向量做排序，速度更快，但会损失稳定性（即相等元素的逻辑次序被打乱）
                mergeSortDirectly<int>(L, less_equal<int>());
            });
        }
    });
    return 0;
}
