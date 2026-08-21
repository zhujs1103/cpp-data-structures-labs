#include <iostream>
#include <stdexcept>  // 用于异常处理

template <typename T>
class StackNode {
public:
    T data;
    StackNode<T>* next;
    
    // 构造函数
    StackNode(const T& value) : data(value), next(nullptr) {}
};


template <typename T>
class LinkedStack {
private:
    StackNode<T>* top;  // 栈顶指针
    int size;           // 栈中元素个数
    
public:
    // 构造函数
    LinkedStack() : top(nullptr), size(0) {}
    
    // 析构函数
    ~LinkedStack() {
        clear();
    }
    
    // 拷贝构造函数
    LinkedStack(const LinkedStack& other) : top(nullptr), size(0) {
        copyFrom(other);
    }
    
    // 赋值运算符
    LinkedStack& operator=(const LinkedStack& other) {
        if (this != &other) {
            clear();
            copyFrom(other);
        }
        return *this;
    }
    
    // 入栈操作
    void push(const T& value) {
        StackNode<T>* newNode = new StackNode<T>(value);
        newNode->next = top;  // 新节点指向原栈顶
        top = newNode;        // 更新栈顶指针
        size++;
    }
    
    // 出栈操作
    void pop() {
        if (isEmpty()) {
            throw std::underflow_error("栈为空，无法执行出栈操作");
        }
        
        StackNode<T>* temp = top;
        top = top->next;  // 栈顶指针下移
        delete temp;
        size--;
    }
    
    // 获取栈顶元素
    T& peek() {
        if (isEmpty()) {
            throw std::underflow_error("栈为空，无法获取栈顶元素");
        }
        return top->data;
    }
    
    // 常量版本获取栈顶元素
    const T& peek() const {
        if (isEmpty()) {
            throw std::underflow_error("栈为空，无法获取栈顶元素");
        }
        return top->data;
    }
    
    // 判断栈是否为空
    bool isEmpty() const {
        return top == nullptr;
    }
    
    // 获取栈大小
    int getSize() const {
        return size;
    }
    
    // 清空栈
    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }
    
    // 打印栈内容（用于调试）
    void display() const {
        if (isEmpty()) {
            std::cout << "栈为空" << std::endl;
            return;
        }
        
        std::cout << "栈内容（从栈顶到栈底）: ";
        StackNode<T>* current = top;
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
    
private:
    // 从其他栈复制数据
    void copyFrom(const LinkedStack& other) {
        if (other.isEmpty()) {
            return;
        }
        
        // 使用临时栈来保持顺序
        LinkedStack<T> temp;
        StackNode<T>* current = other.top;
        
        // 先按原顺序压入临时栈
        while (current != nullptr) {
            temp.push(current->data);
            current = current->next;
        }
        
        // 再从临时栈弹出，恢复到正确顺序
        while (!temp.isEmpty()) {
            push(temp.peek());
            temp.pop();
        }
    }
};


int main() {
    // 创建整型链栈
    LinkedStack<int> stack;
    
    // 测试基本操作
    std::cout << "=== 链栈基本操作演示 ===" << std::endl;
    
    // 入栈操作
    stack.push(10);
    stack.push(20);
    stack.push(30);
    stack.display();  // 输出: 栈内容（从栈顶到栈底）: 30 20 10
    
    std::cout << "栈顶元素: " << stack.peek() << std::endl;  // 输出: 30
    std::cout << "栈大小: " << stack.getSize() << std::endl; // 输出: 3
    
    // 出栈操作
    stack.pop();
    stack.display();  // 输出: 栈内容（从栈顶到栈底）: 20 10
    
    // 测试拷贝构造函数
    LinkedStack<int> stack2 = stack;
    std::cout << "拷贝后的栈: ";
    stack2.display();  // 输出: 栈内容（从栈顶到栈底）: 20 10
    
    // 测试赋值运算符
    LinkedStack<int> stack3;
    stack3 = stack;
    std::cout << "赋值后的栈: ";
    stack3.display();  // 输出: 栈内容（从栈顶到栈底）: 20 10
    
    // 清空栈
    stack.clear();
    std::cout << "清空后栈是否为空: " << (stack.isEmpty() ? "是" : "否") << std::endl;
    
    // 异常处理示例
    try {
        stack.pop();  // 尝试对空栈执行出栈操作
    } catch (const std::underflow_error& e) {
        std::cout << "捕获异常: " << e.what() << std::endl;
    }
    
    // 字符串栈示例
    LinkedStack<std::string> stringStack;
    stringStack.push("Hello");
    stringStack.push("World");
    stringStack.push("!");
    
    std::cout << "\n字符串栈: ";
    stringStack.display();  // 输出: 栈内容（从栈顶到栈底）: ! World Hello
    
    return 0;
}