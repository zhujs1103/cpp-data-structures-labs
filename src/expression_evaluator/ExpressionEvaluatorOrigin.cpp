#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <cctype>
#include <stdexcept>
#include <cmath>

using namespace std;

class ExpressionCalculator {
private:
    // 获取操作符优先级
    int getPriority(char op) {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return 0; // 括号或其他
    }
    
    // 执行计算
    int calculate(int a, int b, char op) {
        switch (op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': 
                if (b == 0) throw runtime_error("除数不能为零");
                return a / b;
            default: throw runtime_error("无效的操作符");
        }
    }
    
    // 处理栈顶的操作符
    void processOperator(stack<int>& numStack, stack<char>& opStack) {
        if (numStack.size() < 2 || opStack.empty()) {
            throw runtime_error("表达式不合法");
        }
        
        int b = numStack.top(); numStack.pop();
        int a = numStack.top(); numStack.pop();
        char op = opStack.top(); opStack.pop();
        
        int result = calculate(a, b, op);
        numStack.push(result);
    }

public:
    // 表达式求值主函数
    int evaluate(const string& expression) {
        stack<int> numStack;    // 操作数栈
        stack<char> opStack;    // 操作符栈
        
        int i = 0;
        int n = expression.length();
        
        while (i < n) {
            char ch = expression[i];
            
            // 跳过空格
            if (isspace(ch)) {
                i++;
                continue;
            }
            
            // 如果是数字，读取完整数字
            if (isdigit(ch)) {
                int num = 0;
                while (i < n && isdigit(expression[i])) {
                    num = num * 10 + (expression[i] - '0');
                    i++;
                }
                numStack.push(num);
            }
            // 如果是左括号，直接入栈
            else if (ch == '(') {
                opStack.push(ch);
                i++;
            }
            // 如果是右括号，处理直到遇到左括号
            else if (ch == ')') {
                while (!opStack.empty() && opStack.top() != '(') {
                    processOperator(numStack, opStack);
                }
                if (opStack.empty()) {
                    throw runtime_error("括号不匹配");
                }
                opStack.pop(); // 弹出左括号
                i++;
            }
            // 如果是操作符
            else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
                // 处理优先级高的操作符
                while (!opStack.empty() && getPriority(opStack.top()) >= getPriority(ch)) {
                    processOperator(numStack, opStack);
                }
                opStack.push(ch);
                i++;
            }
            else {
                throw runtime_error("无效字符: " + string(1, ch));
            }
        }
        
        // 处理剩余的操作符
        while (!opStack.empty()) {
            if (opStack.top() == '(') {
                throw runtime_error("括号不匹配");
            }
            processOperator(numStack, opStack);
        }
        
        if (numStack.size() != 1) {
            throw runtime_error("表达式不完整");
        }
        
        return numStack.top();
    }
};

// 测试函数
void testExpressionCalculator() {
    ExpressionCalculator calculator;
    
    // 测试用例
    vector<pair<string, int>> testCases = {
        {"14+((13-2)*2-11*5)*2", 14 + ((13-2)*2-11*5)*2},
        {"3+4*5", 3+4*5},
        {"(3+4)*5", (3+4)*5},
        {"10-20/5", 10-20/5},
        {"2*3+4*5", 2*3+4*5},
        {"((2+3)*4)+5", ((2+3)*4)+5},
        {"100/10/2", 100/10/2}
    };
    
    cout << "表达式求值测试结果:" << endl;
    cout << "==================" << endl;
    
    for (const auto& testCase : testCases) {
        try {
            int result = calculator.evaluate(testCase.first);
            int expected = testCase.second;
            
            cout << "表达式: " << testCase.first << endl;
            cout << "计算结果: " << result << endl;
            cout << "期望结果: " << expected << endl;
            cout << "状态: " << (result == expected ? "✓ 正确" : "✗ 错误") << endl;
            cout << "------------------" << endl;
        } catch (const exception& e) {
            cout << "表达式: " << testCase.first << endl;
            cout << "错误: " << e.what() << endl;
            cout << "------------------" << endl;
        }
    }
}

// 交互式测试
void interactiveTest() {
    ExpressionCalculator calculator;
    string expression;
    
    cout << "\n交互式测试模式 (输入 'quit' 退出)" << endl;
    cout << "=================================" << endl;
    
    while (true) {
        cout << "请输入表达式: ";
        getline(cin, expression);
        
        if (expression == "quit") {
            break;
        }
        
        if (expression.empty()) {
            continue;
        }
        
        try {
            int result = calculator.evaluate(expression);
            cout << "结果: " << result << endl;
        } catch (const exception& e) {
            cout << "计算错误: " << e.what() << endl;
        }
        cout << endl;
    }
}

int main() {
    // 运行预设测试
    testExpressionCalculator();
    
    // 运行交互式测试
    interactiveTest();
    
    return 0;
}
