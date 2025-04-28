#include <iostream>
#include <vector>
#include <string>

struct Point {
    int x;
    int y;
};

int main() {
    // 🚫 Narrowing conversion — this will not compile!
    double pi = 3.14;
    int a = pi;     // allowed
    int b{pi};      // ❌ Uncommenting this line gives a compile error!

    // ✅ Safe initialization
    int good{5};
    std::cout << "good: " << good << "\n";

    // ✅ Struct initialization
    Point p1{10, 20};
    std::cout << "Point: (" << p1.x << ", " << p1.y << ")\n";

    // ✅ Vector initialization — brace style
    std::vector<int> nums{1, 2, 3};
    for (int n : nums) std::cout << n << " ";
    std::cout << "\n";

    // ⚠️ Vector constructor differences
    std::vector<int> v1(5, 100);   // 5 elements, all 100
    std::vector<int> v2{5, 100};   // 2 elements: 5 and 100

    std::cout << "v1: ";
    for (int x : v1) std::cout << x << " ";
    std::cout << "\nv2: ";
    for (int x : v2) std::cout << x << " ";
    std::cout << "\n";

    // ⚠️ Brace + auto — initializer_list!
    auto list = {1, 2, 3}; // list is initializer_list<int>
    std::cout << "auto list size: " << list.size() << "\n"; // .size() works!

    return 0;
}
