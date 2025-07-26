/**
 * @file forloop_perf.cpp
 * @author sean2077 (sean2077@163.com)
 * @brief C++ for循环中 i++ 与 ++i 的性能分析
 * @version 0.1
 * @date 2025-07-23
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <chrono>
#include <iostream>
#include <vector>

// 演示基本类型的情况
void test_builtin_types() {
    std::cout << "=== 基本类型测试 (int) ===\n";

    const int N = 100000000;
    auto start = std::chrono::high_resolution_clock::now();

    // 测试 i++
    int sum1 = 0;
    for (int i = 0; i < N; i++) { // 后置递增
        sum1 += i;
    }

    auto mid = std::chrono::high_resolution_clock::now();

    // 测试 ++i
    int sum2 = 0;
    for (int i = 0; i < N; ++i) { // 前置递增
        sum2 += i;
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto time1 = std::chrono::duration_cast<std::chrono::milliseconds>(mid - start);
    auto time2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - mid);

    std::cout << "i++ 时间: " << time1.count() << "ms\n";
    std::cout << "++i 时间: " << time2.count() << "ms\n";
    std::cout << "结果相等: " << (sum1 == sum2 ? "是" : "否") << "\n\n";
}

// 自定义类，演示重载操作符的情况
class Counter {
private:
    int value;

public:
    Counter(int v = 0) : value(v) {}

    // 前置递增：返回引用，效率更高
    Counter& operator++() {
        ++value;
        return *this;
    }

    // 后置递增：需要创建临时对象，理论上效率较低
    Counter operator++(int) {
        Counter temp = *this; // 创建临时对象
        ++value;
        return temp; // 返回临时对象
    }

    bool operator<(int n) const { return value < n; }

    int get_value() const { return value; }
};

void test_custom_class() {
    std::cout << "=== 自定义类测试 ===\n";

    const int N = 10000000;
    auto start = std::chrono::high_resolution_clock::now();

    // 测试后置递增
    int sum1 = 0;
    for (Counter i(0); i < N; i++) { // 后置递增
        sum1 += i.get_value();
    }

    auto mid = std::chrono::high_resolution_clock::now();

    // 测试前置递增
    int sum2 = 0;
    for (Counter i(0); i < N; ++i) { // 前置递增
        sum2 += i.get_value();
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto time1 = std::chrono::duration_cast<std::chrono::milliseconds>(mid - start);
    auto time2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - mid);

    std::cout << "i++ 时间: " << time1.count() << "ms\n";
    std::cout << "++i 时间: " << time2.count() << "ms\n";
    std::cout << "结果相等: " << (sum1 == sum2 ? "是" : "否") << "\n\n";
}

// 测试迭代器的情况
void test_iterators() {
    std::cout << "=== 迭代器测试 ===\n";

    std::vector<int> vec(10000000);
    for (int i = 0; i < vec.size(); ++i) {
        vec[i] = i;
    }

    auto start = std::chrono::high_resolution_clock::now();

    // 测试后置递增
    long sum1 = 0;
    for (auto it = vec.begin(); it != vec.end(); it++) { // 后置递增
        sum1 += *it;
    }

    auto mid = std::chrono::high_resolution_clock::now();

    // 测试前置递增
    long sum2 = 0;
    for (auto it = vec.begin(); it != vec.end(); ++it) { // 前置递增
        sum2 += *it;
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto time1 = std::chrono::duration_cast<std::chrono::milliseconds>(mid - start);
    auto time2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - mid);

    std::cout << "it++ 时间: " << time1.count() << "ms\n";
    std::cout << "++it 时间: " << time2.count() << "ms\n";
    std::cout << "结果相等: " << (sum1 == sum2 ? "是" : "否") << "\n\n";
}

// 汇编代码演示
void show_assembly_example() {
    std::cout << "=== 汇编代码对比 ===\n";
    std::cout << "对于基本类型，编译器通常生成相同的汇编代码：\n\n";

    std::cout << "// for (int i = 0; i < n; i++)   和   for (int i = 0; i < n; ++i)\n";
    std::cout << "// 在 -O2 优化下通常生成相同的汇编：\n";
    std::cout << "mov  eax, 0        ; i = 0\n";
    std::cout << "loop_start:\n";
    std::cout << "cmp  eax, edx      ; 比较 i 和 n\n";
    std::cout << "jge  loop_end      ; 如果 i >= n 跳出循环\n";
    std::cout << "                   ; 循环体代码\n";
    std::cout << "inc  eax           ; i = i + 1\n";
    std::cout << "jmp  loop_start    ; 跳回循环开始\n";
    std::cout << "loop_end:\n\n";
}

void performance_summary() {
    std::cout << "=== 性能总结 ===\n";
    std::cout << "1. 基本类型 (int, double, char等):\n";
    std::cout << "   - 编译器优化后，i++ 和 ++i 性能完全相同\n";
    std::cout << "   - 返回值在for循环中被忽略，不会产生临时对象\n\n";

    std::cout << "2. 自定义类和复杂对象:\n";
    std::cout << "   - 理论上 ++i 更快（避免临时对象）\n";
    std::cout << "   - 实际上编译器通常能优化掉差异\n";
    std::cout << "   - 但最佳实践仍推荐使用 ++i\n\n";

    std::cout << "3. STL 迭代器:\n";
    std::cout << "   - 对于简单迭代器（如指针），性能相同\n";
    std::cout << "   - 对于复杂迭代器，++it 理论上更快\n";
    std::cout << "   - 编译器优化通常能消除差异\n\n";

    std::cout << "4. 编译器优化:\n";
    std::cout << "   - -O2 或 -O3 优化级别下差异几乎消失\n";
    std::cout << "   - 现代编译器非常智能，能识别并优化这些模式\n";
    std::cout << "   - Release版本中性能差异可忽略\n\n";
}

int main() {
    std::cout << "C++ 中 i++ 与 ++i 的性能分析\n";
    std::cout << "编译建议: g++ -O2 或 clang++ -O2\n\n";

    test_builtin_types();
    test_custom_class();
    test_iterators();
    show_assembly_example();
    performance_summary();

    std::cout << "结论: 在现代编译器的优化下，for循环中使用 i++ 或 ++i\n";
    std::cout << "性能差异微乎其微，选择哪个主要看个人和团队习惯。\n";

    return 0;
}
