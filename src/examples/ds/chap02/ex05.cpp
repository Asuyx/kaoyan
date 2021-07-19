// 例2.5 - 唯一化

#include <iostream>
#include "vector.h"
using namespace std;

const int start_size = 16;
const int sup_element = 6;

int main() {
    srand(time(0));
    Vector<int> V1(start_size, start_size, [=](Rank r) -> int {
        return rand() % sup_element;
    });
    Vector<int> V2 = V1;   // 生成两个相同的向量
    // V1直接唯一化，V2排序后唯一化
    cout << "V      = " << V1 << endl;
    V1.duplicate();
    cout << "V(dup) = " << V1 << endl;
    V2.mergeSort();
    cout << "V(sort)= " << V2 << endl;
    V2.duplicateSorted();
    cout << "V(s-d) = " << V2 << endl;
    return 0;
}
