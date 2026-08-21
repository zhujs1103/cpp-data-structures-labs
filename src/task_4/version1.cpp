#include <iostream>
#include <cmath>
using namespace std;

// 多项式项的结构体
struct Term {
    float coef;  // 系数
    int expn;    // 指数
    Term* next;  // 下一项指针
    
    Term(float c, int e) : coef(c), expn(e), next(nullptr) {}
};

class Polynomial {
private:
    Term* head;  // 链表头节点
    
public:
    // 构造函数
    Polynomial() {
        head = new Term(0, -1);  // 头节点，不存储实际数据
    }
    
    // 析构函数
    ~Polynomial() {
        clear();
        delete head;
    }
    
    // 清空多项式
    void clear() {
        Term* current = head->next;
        while (current != nullptr) {
            Term* temp = current;
            current = current->next;
            delete temp;
        }
        head->next = nullptr;
    }
    
    // 输入多项式
    void input() {
        clear();
        cout << "请输入多项式的项数: ";
        int n;
        cin >> n;
        
        cout << "请按指数降序输入各项的系数和指数（系数 指数）:" << endl;
        for (int i = 0; i < n; i++) {
            float coef;
            int expn;
            cin >> coef >> expn;
            
            // 跳过系数为0的项
            if (fabs(coef) < 1e-6) continue;
            
            insertTerm(coef, expn);
        }
    }
    
    // 插入项（按指数降序排列）
    void insertTerm(float coef, int expn) {
        Term* newTerm = new Term(coef, expn);
        Term* current = head;
        
        // 找到插入位置
        while (current->next != nullptr && current->next->expn > expn) {
            current = current->next;
        }
        
        // 处理指数相同的情况（合并同类项）
        if (current->next != nullptr && current->next->expn == expn) {
            current->next->coef += coef;
            delete newTerm;
            // 如果合并后系数为0，删除该项
            if (fabs(current->next->coef) < 1e-6) {
                Term* temp = current->next;
                current->next = temp->next;
                delete temp;
            }
        } else {
            newTerm->next = current->next;
            current->next = newTerm;
        }
    }
    
    // 输出多项式
    void output() {
        if (head->next == nullptr) {
            cout << "0";
            return;
        }
        
        Term* current = head->next;
        bool firstTerm = true;
        
        while (current != nullptr) {
            // 处理正负号显示
            if (!firstTerm && current->coef > 0) {
                cout << " + ";
            } else if (!firstTerm && current->coef < 0) {
                cout << " - ";
            }
            
            // 输出系数和指数
            float absCoef = fabs(current->coef);
            
            if (current->expn == 0) {
                cout << absCoef;
            } else if (current->expn == 1) {
                if (fabs(absCoef - 1.0) < 1e-6) {
                    cout << "x";
                } else {
                    cout << absCoef << "x";
                }
            } else {
                if (fabs(absCoef - 1.0) < 1e-6) {
                    cout << "x^" << current->expn;
                } else {
                    cout << absCoef << "x^" << current->expn;
                }
            }
            
            firstTerm = false;
            current = current->next;
        }
        cout << endl;
    }
    
    // 多项式相加
    Polynomial add(const Polynomial& other) const {
        Polynomial result;
        Term* p1 = this->head->next;
        Term* p2 = other.head->next;
        
        while (p1 != nullptr && p2 != nullptr) {
            if (p1->expn > p2->expn) {
                result.insertTerm(p1->coef, p1->expn);
                p1 = p1->next;
            } else if (p1->expn < p2->expn) {
                result.insertTerm(p2->coef, p2->expn);
                p2 = p2->next;
            } else {
                float sumCoef = p1->coef + p2->coef;
                if (fabs(sumCoef) >= 1e-6) {
                    result.insertTerm(sumCoef, p1->expn);
                }
                p1 = p1->next;
                p2 = p2->next;
            }
        }
        
        // 处理剩余项
        while (p1 != nullptr) {
            result.insertTerm(p1->coef, p1->expn);
            p1 = p1->next;
        }
        
        while (p2 != nullptr) {
            result.insertTerm(p2->coef, p2->expn);
            p2 = p2->next;
        }
        
        return result;
    }
    
    // 多项式相减
    Polynomial subtract(const Polynomial& other) const {
        Polynomial result;
        Term* p1 = this->head->next;
        Term* p2 = other.head->next;
        
        while (p1 != nullptr && p2 != nullptr) {
            if (p1->expn > p2->expn) {
                result.insertTerm(p1->coef, p1->expn);
                p1 = p1->next;
            } else if (p1->expn < p2->expn) {
                result.insertTerm(-p2->coef, p2->expn);
                p2 = p2->next;
            } else {
                float diffCoef = p1->coef - p2->coef;
                if (fabs(diffCoef) >= 1e-6) {
                    result.insertTerm(diffCoef, p1->expn);
                }
                p1 = p1->next;
                p2 = p2->next;
            }
        }
        
        // 处理剩余项
        while (p1 != nullptr) {
            result.insertTerm(p1->coef, p1->expn);
            p1 = p1->next;
        }
        
        while (p2 != nullptr) {
            result.insertTerm(-p2->coef, p2->expn);
            p2 = p2->next;
        }
        
        return result;
    }
    
    // 计算多项式在x处的值
    float evaluate(float x) const {
        float result = 0.0;
        Term* current = head->next;
        
        while (current != nullptr) {
            result += current->coef * pow(x, current->expn);
            current = current->next;
        }
        
        return result;
    }
    
    // 求导
    Polynomial derivative() const {
        Polynomial result;
        Term* current = head->next;
        
        while (current != nullptr) {
            if (current->expn > 0) {
                result.insertTerm(current->coef * current->expn, current->expn - 1);
            }
            current = current->next;
        }
        
        return result;
    }
    
    // 多项式相乘
    Polynomial multiply(const Polynomial& other) const {
        Polynomial result;
        
        Term* p1 = this->head->next;
        while (p1 != nullptr) {
            Term* p2 = other.head->next;
            while (p2 != nullptr) {
                float productCoef = p1->coef * p2->coef;
                int sumExpn = p1->expn + p2->expn;
                result.insertTerm(productCoef, sumExpn);
                p2 = p2->next;
            }
            p1 = p1->next;
        }
        
        return result;
    }
};

// 测试函数
int main() {
    Polynomial poly1, poly2, result;
    int choice;
    float x;
    
    do {
        cout << "\n=== 一元多项式计算器 ===" << endl;
        cout << "1. 输入多项式1" << endl;
        cout << "2. 输入多项式2" << endl;
        cout << "3. 输出多项式1" << endl;
        cout << "4. 输出多项式2" << endl;
        cout << "5. 多项式相加" << endl;
        cout << "6. 多项式相减" << endl;
        cout << "7. 多项式相乘" << endl;
        cout << "8. 计算多项式在x处的值" << endl;
        cout << "9. 求多项式导数" << endl;
        cout << "0. 退出" << endl;
        cout << "请选择操作: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                cout << "输入多项式1:" << endl;
                poly1.input();
                break;
                
            case 2:
                cout << "输入多项式2:" << endl;
                poly2.input();
                break;
                
            case 3:
                cout << "多项式1: ";
                poly1.output();
                break;
                
            case 4:
                cout << "多项式2: ";
                poly2.output();
                break;
                
            case 5:
                result = poly1.add(poly2);
                cout << "相加结果: ";
                result.output();
                break;
                
            case 6:
                result = poly1.subtract(poly2);
                cout << "相减结果: ";
                result.output();
                break;
                
            case 7:
                result = poly1.multiply(poly2);
                cout << "相乘结果: ";
                result.output();
                break;
                
            case 8:
                cout << "请输入x的值: ";
                cin >> x;
                cout << "多项式1在x=" << x << "处的值: " << poly1.evaluate(x) << endl;
                cout << "多项式2在x=" << x << "处的值: " << poly2.evaluate(x) << endl;
                break;
                
            case 9:
                cout << "多项式1的导数: ";
                result = poly1.derivative();
                result.output();
                cout << "多项式2的导数: ";
                result = poly2.derivative();
                result.output();
                break;
                
            case 0:
                cout << "程序结束，谢谢使用！" << endl;
                break;
                
            default:
                cout << "无效选择，请重新输入！" << endl;
        }
    } while (choice != 0);
    
    return 0;
}