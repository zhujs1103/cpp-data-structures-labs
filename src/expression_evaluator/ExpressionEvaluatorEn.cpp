#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <stdexcept>
#include <vector>

using namespace std;

class ExpressionCalculator {
private:
    // Get operator priority
    int getPriority(char op) {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return 0; // Parentheses or others
    }
    
    // Perform calculation
    int calculate(int a, int b, char op) {
        switch (op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': 
                if (b == 0) throw runtime_error("Division by zero");
                return a / b;
            default: throw runtime_error("Invalid operator");
        }
    }
    
    // Process top operator in stack
    void processOperator(stack<int>& numStack, stack<char>& opStack) {
        if (numStack.size() < 2 || opStack.empty()) {
            throw runtime_error("Invalid expression");
        }
        
        int secondOperand = numStack.top(); numStack.pop();
        int firstOperand = numStack.top(); numStack.pop();
        char op = opStack.top(); opStack.pop();
        
        int result = calculate(firstOperand, secondOperand, op);
        numStack.push(result);
    }

    // Check if character is valid operator
    bool isValidOperator(char ch) {
        return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '(' || ch == ')';
    }

public:
    // Main evaluation function
    int evaluate(const string& expression) {
        stack<int> numStack;    // Operand stack
        stack<char> opStack;    // Operator stack
        
        int index = 0;
        int length = expression.length();
        
        while (index < length) {
            char currentChar = expression[index];
            
            // Skip spaces
            if (isspace(currentChar)) {
                index++;
                continue;
            }
            
            // If digit, parse complete number
            if (isdigit(currentChar)) {
                int number = 0;
                while (index < length && isdigit(expression[index])) {
                    number = number * 10 + (expression[index] - '0');
                    index++;
                }
                numStack.push(number);
            }
            // If left parenthesis, push to stack
            else if (currentChar == '(') {
                opStack.push(currentChar);
                index++;
            }
            // If right parenthesis, process until left parenthesis
            else if (currentChar == ')') {
                while (!opStack.empty() && opStack.top() != '(') {
                    processOperator(numStack, opStack);
                }
                if (opStack.empty()) {
                    throw runtime_error("Mismatched parentheses");
                }
                opStack.pop(); // Remove left parenthesis
                index++;
            }
            // If operator
            else if (currentChar == '+' || currentChar == '-' || 
                     currentChar == '*' || currentChar == '/') {
                // Process higher priority operators first
                while (!opStack.empty() && 
                       getPriority(opStack.top()) >= getPriority(currentChar)) {
                    processOperator(numStack, opStack);
                }
                opStack.push(currentChar);
                index++;
            }
            else {
                throw runtime_error("Invalid character: " + string(1, currentChar));
            }
        }
        
        // Process remaining operators
        while (!opStack.empty()) {
            if (opStack.top() == '(') {
                throw runtime_error("Mismatched parentheses");
            }
            processOperator(numStack, opStack);
        }
        
        if (numStack.size() != 1) {
            throw runtime_error("Incomplete expression");
        }
        
        return numStack.top();
    }
};

// Test function
void runTestCases() {
    ExpressionCalculator calculator;
    
    // Test cases with expected results
    vector<pair<string, int>> testCases = {
        {"14+((13-2)*2-11*5)*2", 14 + ((13-2)*2-11*5)*2},
        {"3+4*5", 3+4*5},
        {"(3+4)*5", (3+4)*5},
        {"10-20/5", 10-20/5},
        {"2*3+4*5", 2*3+4*5},
        {"((2+3)*4)+5", ((2+3)*4)+5},
        {"100/10/2", 100/10/2},
        {"5", 5},
        {"2+3", 5}
    };
    
    cout << "Expression Evaluation Test Results:" << endl;
    cout << "===================================" << endl;
    
    for (const auto& testCase : testCases) {
        try {
            string expression = testCase.first;
            int result = calculator.evaluate(expression);
            int expected = testCase.second;
            
            cout << "Expression: " << expression << endl;
            cout << "Computed: " << result << endl;
            cout << "Expected: " << expected << endl;
            cout << "Status: " << (result == expected ? "PASS" : "FAIL") << endl;
            cout << "-----------------------------------" << endl;
        } catch (const exception& e) {
            cout << "Expression: " << testCase.first << endl;
            cout << "Error: " << e.what() << endl;
            cout << "-----------------------------------" << endl;
        }
    }
}

// Interactive testing mode
void runInteractiveMode() {
    ExpressionCalculator calculator;
    string userExpression;
    
    cout << "\nInteractive Testing Mode (Type 'exit' to quit)" << endl;
    cout << "==============================================" << endl;
    
    while (true) {
        cout << "Enter expression: ";
        getline(cin, userExpression);
        
        if (userExpression == "exit" || userExpression == "quit") {
            break;
        }
        
        if (userExpression.empty()) {
            continue;
        }
        
        try {
            int result = calculator.evaluate(userExpression);
            cout << "Result: " << result << endl;
        } catch (const exception& e) {
            cout << "Calculation error: " << e.what() << endl;
        }
        cout << endl;
    }
}

// Demonstration function
void demonstrateAlgorithm() {
    ExpressionCalculator calculator;
    
    cout << "Algorithm Demonstration:" << endl;
    cout << "=======================" << endl;
    
    vector<string> demoExpressions = {
        "1+2*3",
        "(1+2)*3",
        "10/2-3",
        "2*3+4*5"
    };
    
    for (const auto& expr : demoExpressions) {
        cout << "Processing: " << expr << endl;
        try {
            int result = calculator.evaluate(expr);
            cout << "Final Result: " << result << endl;
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
        cout << "---------------------" << endl;
    }
}

int main() {
    cout << "Expression Calculator" << endl;
    cout << "======================================" << endl;
    
    // Run demonstration
    demonstrateAlgorithm();
    
    // Run automated tests
    runTestCases();
    
    // Run interactive mode
    runInteractiveMode();
    
    cout << "Program ended. Thank you for using Expression Calculator!" << endl;
    
    return 0;
}