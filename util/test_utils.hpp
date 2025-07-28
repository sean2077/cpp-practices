#pragma once
#include <chrono>
#include <iostream>
#include <tuple>

#include "print_utils.hpp"

template<typename Method, typename Case>
void runTest(const std::string& method_name, Method method, const Case& tcase) {
    // 移除 tuple 最后一个元素，仅将前 n-1 个元素传给 method
    constexpr size_t tuple_size = std::tuple_size<Case>::value;
    static_assert(tuple_size >= 1, "Tuple must have at least one element");
    std::chrono::duration<float, std::milli> duration;
    auto result = std::apply(
        [&](auto&&... params) {
            // 只取前 n-1 个参数
            return [&]<std::size_t... I>(std::index_sequence<I...>) {
                auto start = std::chrono::high_resolution_clock::now();
                auto result = method(std::get<I>(std::forward_as_tuple(params...))...);
                auto end = std::chrono::high_resolution_clock::now();
                duration = end - start;
                return result;
            }(std::make_index_sequence<tuple_size - 1>{});
        },
        tcase);

    // 若tcase最后一个元素为wanted，可打印对比
    if constexpr (std::tuple_size<Case>::value > 0) {
        constexpr size_t last = std::tuple_size<Case>::value - 1;
        auto wanted = std::get<last>(tcase);
        // 使用 ANSI 转义序列为对错符号加颜色
        const char* correct = "\033[32m✔\033[0m"; // 绿色
        const char* wrong = "\033[31m✖\033[0m";   // 红色
        std::cout << method_name << ": " << result << " (Time: " << duration.count() << " ms) "
                  << (result == wanted ? correct : wrong) << std::endl;
    } else {
        std::cout << method_name << ": " << result << " (Time: " << duration.count() << " ms)" << std::endl;
    }
}

template<typename Method, typename Case>
void runTests(const std::vector<std::pair<std::string, Method>>& methods, const std::vector<Case>& test_cases) {
    for (size_t i = 0; i < test_cases.size(); ++i) {
        auto& tcase = test_cases[i];
        std::cout << "---\ncase " << i + 1 << "/" << test_cases.size() << ": " << tcase << std::endl;

        for (auto&& [method_name, method] : methods) {
            runTest(method_name, method, tcase);
        }
    }
}
