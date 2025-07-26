/**
 * @file func_declare.cpp
 * @author sean2077 (sean2077@163.com)
 * @brief 区分"成员函数指针作为成员变量"和"返回成员函数指针的成员函数"
 * @version 0.1
 * @date 2025-07-23
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <iostream>

class Calculator {
private:
    // 成员函数指针作为成员变量
    int (Calculator::*operation)(int, int);
    // 返回成员函数指针的成员函数
    int (Calculator::* getOperation(char op))(int, int);

public:
    int add(int a, int b) { return a + b; }

    int subtract(int a, int b) { return a - b; }

    void setOperation(char op) {
        operation = getOperation(op); // 调用返回函数指针的函数
    }

    int calculate(int a, int b) {
        if (operation) {
            return (this->*operation)(a, b); // 调用成员函数指针
        }
        return 0;
    }
};

int (Calculator::* Calculator::getOperation(char op))(int, int) {
    switch (op) {
    case '+': return &Calculator::add;
    case '-': return &Calculator::subtract;
    default:  return nullptr;
    }
}

int main() {
    Calculator calc;
    calc.setOperation('+');
    std::cout << "Addition: " << calc.calculate(5, 3) << std::endl;
    calc.setOperation('-');
    std::cout << "Subtraction: " << calc.calculate(5, 3) << std::endl;
    calc.setOperation('*'); // Invalid operation
    std::cout << "Invalid operation: " << calc.calculate(5, 3) << std::endl;
    return 0;
}
