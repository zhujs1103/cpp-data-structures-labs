#include <iostream>
#include <stdexcept>

template <typename T>
class StackNode {
public:
    T data;
    StackNode<T>* next;

    StackNode(const T& value) : data(value), next(nullptr) {}
};

template <typename T>
class LinkedStack {
private:
    StackNode<T>* top;
    int size;

public:
    LinkedStack() : top(nullptr), size(0) {}

    ~LinkedStack() {
        clear();
    }

    LinkedStack(const LinkedStack& other) : top(nullptr), size(0) {
        copyFrom(other);
    }

    LinkedStack& operator=(const LinkedStack& other) {
        if (this != &other) {
            clear();
            copyFrom(other);
        }
        return *this;
    }

    void push(const T& value) {
        StackNode<T>* newNode = new StackNode<T>(value);
        newNode->next = top;
        top = newNode;
        size++;
    }

    void pop() {
        if (isEmpty()) {
            throw std::underflow_error("empty stack, can't pop");
        }

        StackNode<T>* temp = top;
        top = top->next;
        delete temp;
        size--;
    }

    //获取栈顶元素
    T& peek() {
        if(isEmpty()) {
            throw std::underflow_error("empty stack, can't get top");
        }
        return top->data;
    }

    //常量版本的获取栈顶元素
    const T& peek() const {
        if (isEmpty()) {
            throw std::underflow_error("empty stack, can't get top");
        }
        return top->data;
    }

    //判断栈是否为空
    bool isEmpty() const {
        return top == nullptr;
    }

    //获取栈元素个数
    int getSize() const {
        return size;
    }

    //清空栈
    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }

    //打印栈的内容
    void display() const {
        if (isEmpty()) {
            std::cout << "empty stack" << std::endl;
            return;
        }
        std::cout << "stack elements from top to bottom : ";
        StackNode<T>* current = top;
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

private:
    //从其它栈复制数据
    void copyFrom(const LinkedStack<T>& other) {
        if (other.isEmpty()) {
            return;
        }

        //using a temp stack to keep the order of data
        LinkedStack<T> temp;
        StackNode<T>* current = other.top;

        while (current != nullptr) {
            temp.push(current->data);
            current = current->next;
        }

        while (!temp.isEmpty()) {
            push(temp.peek());
            temp.pop();
        }
    }
};

int main() {
    // 创建整型链栈
    std::cout << "a test on int stack" << std::endl;
    LinkedStack<int> stack;

    stack.push(10);
    stack.push(20);
    stack.push(30);
    stack.display();

    std::cout << "top stack element: " << stack.peek() << std::endl;
    std::cout << "stack size: " << stack.getSize() << std::endl;

    stack.pop();
    stack.display();

    LinkedStack<int> stack2 = stack;
    std::cout<< "copy stack to satack2: ";
    stack2.display();

    LinkedStack<int> stack3;
    stack3 = stack;
    std::cout << "assign stack to stack3";
    stack3.display();

    stack.clear();
    std::cout << "clear the stack" << std::endl;
    stack.display();

    // 异常处理测试
    try{
        stack.pop();
    } catch (const std::underflow_error& e) {
        std::cout << "catching: " << e.what() << std::endl;
    }

    LinkedStack<std::string> stringStack;
    std::cout << "now push i love u by sequence" << std::endl;
    stringStack.push("i");
    stringStack.push("love");
    stringStack.push("u");

    std::cout<<"\nlet's see stringStack from top to bottom: ";
    stringStack.display();
    
    return 0;
}