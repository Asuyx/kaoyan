// 例2.12 - 分块表的插入、删除和重构
#define EXAMPLE_MODE
#include <iostream>
#include "listlist.h"
using namespace std;

const int n = 32;

int main() {
    ListList<int> L;
    cout << "L = " << L << endl;
    for (int i = 0; i < n; ++i) {     // 将L赋值为[n-1,n-2,...,0]
        L.insert(i, 0);               // 输出逐个插入到列表开头的全过程
        cout << "L = " << L << endl;  // 这个过程中，分块表的第一块会不断分裂，以进行重构
    }
    for (int i = 0; i < n; ++i) {
        // 删除的时候，优先删除没有被索引节点指向的数据，否则难以触发整体重构
        // 在整个删除的过程中，只会触发一次整体重构
        bool removed = false;
        for (int k = 0; k < n-i; ++k) {
            auto p1 = L.findPositionInIndexList(k);
            if (p1->value.p != L[k]) {
                L.remove(k);
                removed = true;
                break;
            }
        } // 如果没有符合条件的优先删除数据，则删除第一个节点
        if (!removed) {
            L.remove(0);
        }
        cout << "L = " << L << endl;
    }
    return 0;
}
