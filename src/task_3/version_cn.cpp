#include <iostream>
#include <cstring>
#include <stdexcept>

// 通讯录数据结构
struct DataType {
    int ID;           // 编号
    char name[10];    // 姓名
    char ch;          // 性别
    char phone[13];   // 电话
    char addr[31];    // 地址
    
    // 构造函数
    DataType(int id = 0, const char* n = "", char c = 'M', 
             const char* p = "", const char* a = "") : ID(id), ch(c) {
        strncpy(name, n, 9);
        name[9] = '\0';
        strncpy(phone, p, 12);
        phone[12] = '\0';
        strncpy(addr, a, 30);
        addr[30] = '\0';
    }
    
    // 重载输出运算符
    friend std::ostream& operator<<(std::ostream& os, const DataType& data) {
        os << "ID: " << data.ID << ", 姓名: " << data.name 
           << ", 性别: " << data.ch << ", 电话: " << data.phone 
           << ", 地址: " << data.addr;
        return os;
    }
    
    // 重载比较运算符（按ID比较）
    bool operator==(const DataType& other) const {
        return ID == other.ID;
    }
    
    bool operator<(const DataType& other) const {
        return ID < other.ID;
    }
};

// 链表节点类
template<typename T>
class ListNode {
public:
    T data;
    ListNode<T>* next;
    
    ListNode(const T& item, ListNode<T>* nextPtr = nullptr) 
        : data(item), next(nextPtr) {}
};

// 通讯录管理类（带头结点的单链表）
class AddressBook {
private:
    ListNode<DataType>* head;  // 头结点
    int length;               // 链表长度
    
    // 深拷贝函数
    void deepCopy(const AddressBook& other) {
        head = new ListNode<DataType>(DataType());  // 创建头结点
        ListNode<DataType>* current = head;
        ListNode<DataType>* otherCurrent = other.head->next;
        
        while (otherCurrent != nullptr) {
            current->next = new ListNode<DataType>(otherCurrent->data);
            current = current->next;
            otherCurrent = otherCurrent->next;
        }
        length = other.length;
    }
    
public:
    // 构造函数
    AddressBook() : head(new ListNode<DataType>(DataType())), length(0) {}
    
    // 拷贝构造函数
    AddressBook(const AddressBook& other) {
        deepCopy(other);
    }
    
    // 赋值运算符重载
    AddressBook& operator=(const AddressBook& other) {
        if (this != &other) {
            clear();
            deepCopy(other);
        }
        return *this;
    }
    
    // 析构函数
    ~AddressBook() {
        clear();
        delete head;
    }
    
    // 清空通讯录
    void clear() {
        ListNode<DataType>* current = head->next;
        while (current != nullptr) {
            ListNode<DataType>* temp = current;
            current = current->next;
            delete temp;
        }
        head->next = nullptr;
        length = 0;
    }
    
    // 获取通讯录长度
    int getLength() const {
        return length;
    }
    
    // 判断通讯录是否为空
    bool isEmpty() const {
        return length == 0;
    }
    
    // 按ID查找联系人
    DataType* searchByID(int id) {
        ListNode<DataType>* current = head->next;
        while (current != nullptr) {
            if (current->data.ID == id) {
                return &(current->data);
            }
            current = current->next;
        }
        return nullptr;
    }
    
    // 按姓名查找联系人
    DataType* searchByName(const char* name) {
        ListNode<DataType>* current = head->next;
        while (current != nullptr) {
            if (strcmp(current->data.name, name) == 0) {
                return &(current->data);
            }
            current = current->next;
        }
        return nullptr;
    }
    
    // 添加联系人（尾插法）
    void addContact(const DataType& contact) {
        // 检查ID是否已存在
        if (searchByID(contact.ID) != nullptr) {
            throw std::invalid_argument("ID已存在，无法添加重复的联系人");
        }
        
        ListNode<DataType>* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        
        current->next = new ListNode<DataType>(contact);
        length++;
    }
    
    // 按ID删除联系人
    bool deleteByID(int id) {
        if (isEmpty()) {
            throw std::runtime_error("通讯录为空，无法删除");
        }
        
        ListNode<DataType>* prev = head;
        ListNode<DataType>* current = head->next;
        
        while (current != nullptr) {
            if (current->data.ID == id) {
                prev->next = current->next;
                delete current;
                length--;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }
    
    // 按姓名删除联系人
    bool deleteByName(const char* name) {
        if (isEmpty()) {
            throw std::runtime_error("通讯录为空，无法删除");
        }
        
        bool deleted = false;
        ListNode<DataType>* prev = head;
        ListNode<DataType>* current = head->next;
        
        while (current != nullptr) {
            if (strcmp(current->data.name, name) == 0) {
                ListNode<DataType>* temp = current;
                prev->next = current->next;
                current = current->next;
                delete temp;
                length--;
                deleted = true;
            } else {
                prev = current;
                current = current->next;
            }
        }
        return deleted;
    }
    
    // 修改联系人信息
    bool modifyContact(int id, const DataType& newData) {
        DataType* contact = searchByID(id);
        if (contact != nullptr) {
            *contact = newData;
            return true;
        }
        return false;
    }
    
    // 显示所有联系人
    void displayAll() const {
        if (isEmpty()) {
            std::cout << "通讯录为空" << std::endl;
            return;
        }
        
        std::cout << "通讯录中共有 " << length << " 个联系人：" << std::endl;
        std::cout << "==========================================" << std::endl;
        
        ListNode<DataType>* current = head->next;
        int index = 1;
        while (current != nullptr) {
            std::cout << index << ". " << current->data << std::endl;
            current = current->next;
            index++;
        }
        std::cout << "==========================================" << std::endl;
    }
    
    // 按ID排序（冒泡排序）
    void sortByID() {
        if (length <= 1) return;
        
        bool swapped;
        do {
            swapped = false;
            ListNode<DataType>* prev = head;
            ListNode<DataType>* current = head->next;
            ListNode<DataType>* next = current->next;
            
            while (next != nullptr) {
                if (current->data.ID > next->data.ID) {
                    // 交换节点
                    current->next = next->next;
                    next->next = current;
                    prev->next = next;
                    
                    // 更新指针
                    prev = next;
                    swapped = true;
                } else {
                    prev = current;
                    current = next;
                }
                next = current->next;
            }
        } while (swapped);
    }
};

// 菜单显示函数
void displayMenu() {
    std::cout << "\n========== 通讯录管理系统 ==========" << std::endl;
    std::cout << "1. 添加联系人" << std::endl;
    std::cout << "2. 按ID删除联系人" << std::endl;
    std::cout << "3. 按姓名删除联系人" << std::endl;
    std::cout << "4. 按ID查找联系人" << std::endl;
    std::cout << "5. 按姓名查找联系人" << std::endl;
    std::cout << "6. 修改联系人信息" << std::endl;
    std::cout << "7. 显示所有联系人" << std::endl;
    std::cout << "8. 按ID排序" << std::endl;
    std::cout << "9. 清空通讯录" << std::endl;
    std::cout << "0. 退出系统" << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << "请选择操作: ";
}

// 测试函数
int main() {
    AddressBook addressBook;
    int choice;
    
    // 添加一些测试数据
    try {
        addressBook.addContact(DataType(1, "张三", 'M', "13800138000", "北京市海淀区"));
        addressBook.addContact(DataType(2, "李四", 'F', "13900139000", "上海市浦东新区"));
        addressBook.addContact(DataType(3, "王五", 'M', "13600136000", "广州市天河区"));
    } catch (const std::exception& e) {
        std::cout << "初始化测试数据时出错: " << e.what() << std::endl;
    }
    
    do {
        displayMenu();
        std::cin >> choice;
        
        try {
            switch (choice) {
                case 1: {
                    // 添加联系人
                    int id;
                    char name[10], ch, phone[13], addr[31];
                    
                    std::cout << "请输入联系人信息:" << std::endl;
                    std::cout << "ID: ";
                    std::cin >> id;
                    std::cout << "姓名: ";
                    std::cin >> name;
                    std::cout << "性别(M/F): ";
                    std::cin >> ch;
                    std::cout << "电话: ";
                    std::cin >> phone;
                    std::cout << "地址: ";
                    std::cin.ignore();  // 清除缓冲区
                    std::cin.getline(addr, 30);
                    
                    DataType newContact(id, name, ch, phone, addr);
                    addressBook.addContact(newContact);
                    std::cout << "联系人添加成功!" << std::endl;
                    break;
                }
                
                case 2: {
                    // 按ID删除
                    int id;
                    std::cout << "请输入要删除的联系人ID: ";
                    std::cin >> id;
                    
                    if (addressBook.deleteByID(id)) {
                        std::cout << "联系人删除成功!" << std::endl;
                    } else {
                        std::cout << "未找到该ID的联系人" << std::endl;
                    }
                    break;
                }
                
                case 3: {
                    // 按姓名删除
                    char name[10];
                    std::cout << "请输入要删除的联系人姓名: ";
                    std::cin >> name;
                    
                    if (addressBook.deleteByName(name)) {
                        std::cout << "联系人删除成功!" << std::endl;
                    } else {
                        std::cout << "未找到该姓名的联系人" << std::endl;
                    }
                    break;
                }
                
                case 4: {
                    // 按ID查找
                    int id;
                    std::cout << "请输入要查找的联系人ID: ";
                    std::cin >> id;
                    
                    DataType* contact = addressBook.searchByID(id);
                    if (contact != nullptr) {
                        std::cout << "找到联系人: " << *contact << std::endl;
                    } else {
                        std::cout << "未找到该ID的联系人" << std::endl;
                    }
                    break;
                }
                
                case 5: {
                    // 按姓名查找
                    char name[10];
                    std::cout << "请输入要查找的联系人姓名: ";
                    std::cin >> name;
                    
                    DataType* contact = addressBook.searchByName(name);
                    if (contact != nullptr) {
                        std::cout << "找到联系人: " << *contact << std::endl;
                    } else {
                        std::cout << "未找到该姓名的联系人" << std::endl;
                    }
                    break;
                }
                
                case 6: {
                    // 修改联系人
                    int id;
                    std::cout << "请输入要修改的联系人ID: ";
                    std::cin >> id;
                    
                    DataType* oldContact = addressBook.searchByID(id);
                    if (oldContact != nullptr) {
                        char name[10], ch, phone[13], addr[31];
                        
                        std::cout << "原信息: " << *oldContact << std::endl;
                        std::cout << "请输入新信息:" << std::endl;
                        std::cout << "姓名: ";
                        std::cin >> name;
                        std::cout << "性别(M/F): ";
                        std::cin >> ch;
                        std::cout << "电话: ";
                        std::cin >> phone;
                        std::cout << "地址: ";
                        std::cin.ignore();
                        std::cin.getline(addr, 30);
                        
                        DataType newContact(id, name, ch, phone, addr);
                        if (addressBook.modifyContact(id, newContact)) {
                            std::cout << "联系人修改成功!" << std::endl;
                        }
                    } else {
                        std::cout << "未找到该ID的联系人" << std::endl;
                    }
                    break;
                }
                
                case 7: {
                    // 显示所有联系人
                    addressBook.displayAll();
                    break;
                }
                
                case 8: {
                    // 按ID排序
                    addressBook.sortByID();
                    std::cout << "排序完成!" << std::endl;
                    addressBook.displayAll();
                    break;
                }
                
                case 9: {
                    // 清空通讯录
                    addressBook.clear();
                    std::cout << "通讯录已清空!" << std::endl;
                    break;
                }
                
                case 0: {
                    std::cout << "感谢使用通讯录管理系统!" << std::endl;
                    break;
                }
                
                default: {
                    std::cout << "无效的选择，请重新输入!" << std::endl;
                    break;
                }
            }
        } catch (const std::exception& e) {
            std::cout << "操作出错: " << e.what() << std::endl;
        }
        
    } while (choice != 0);
    
    return 0;
}