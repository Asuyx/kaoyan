// 例2.10 - 静态列表的优势性举例

#include <iostream>
#include "utility.h"
using namespace std;

int n = 1<<25;

// 可以看出，在n比较大的时候，一次分配数组内存所花的时间仍然可以忽略不计，但连续n次分配内存的时间很多
int main() {
    cout << "malloc n items within 1 time  : " << calculateTime([=]() -> void {
        auto x = new char[n];
    }) << endl;
    cout << "malloc n items within n times : " << calculateTime([=]() -> void {
        while (n--) {
            auto x = new char;
        }
    }) << endl;
    return 0;
}
