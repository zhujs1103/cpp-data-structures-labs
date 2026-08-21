#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

// 三元组节点定义
template<class T>
struct Triple {
    int row;
    int col;
    T data;
    Triple<T>* next;
    
    Triple(int r, int c, T d, Triple<T>* n = nullptr) 
        : row(r), col(c), data(d), next(n) {}
};

// 稀疏矩阵类
template<class T>
class SparseMatrix {
private:
    Triple<T>* head;  // 头节点
    int rows, cols;   // 矩阵行数和列数
    int nonZeroCount; // 非零元素个数

public:
    // 构造函数
    SparseMatrix(int r, int c) : rows(r), cols(c), nonZeroCount(0) {
        head = new Triple<T>(-1, -1, T()); // 头节点，不存储实际数据
    }
    
    // 析构函数
    ~SparseMatrix() {
        clear();
        delete head;
    }
    
    // 清空矩阵
    void clear() {
        Triple<T>* current = head->next;
        while (current != nullptr) {
            Triple<T>* temp = current;
            current = current->next;
            delete temp;
        }
        head->next = nullptr;
        nonZeroCount = 0;
    }
    
    // 插入元素
    void insert(int row, int col, T data) {
        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            throw out_of_range("行或列索引越界");
        }
        
        if (data == T(0)) {
            return; // 不存储0值元素
        }
        
        Triple<T>* newNode = new Triple<T>(row, col, data);
        
        // 插入到合适位置（按行优先，同行按列排序）
        Triple<T>* prev = head;
        Triple<T>* current = head->next;
        
        while (current != nullptr) {
            if (current->row > row || (current->row == row && current->col > col)) {
                break;
            }
            if (current->row == row && current->col == col) {
                // 更新现有元素
                current->data = data;
                delete newNode;
                return;
            }
            prev = current;
            current = current->next;
        }
        
        // 插入新节点
        newNode->next = current;
        prev->next = newNode;
        nonZeroCount++;
    }
    
    // 获取元素值
    T get(int row, int col) const {
        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            throw out_of_range("行或列索引越界");
        }
        
        Triple<T>* current = head->next;
        while (current != nullptr) {
            if (current->row == row && current->col == col) {
                return current->data;
            }
            if (current->row > row || (current->row == row && current->col > col)) {
                break;
            }
            current = current->next;
        }
        
        return T(0); // 未找到，返回0
    }
    
    // 转置矩阵
    SparseMatrix<T> transpose() const {
        SparseMatrix<T> result(cols, rows); // 转置后行列互换
        
        Triple<T>* current = head->next;
        while (current != nullptr) {
            result.insert(current->col, current->row, current->data);
            current = current->next;
        }
        
        return result;
    }
    
    // 矩阵相加
    SparseMatrix<T> add(const SparseMatrix<T>& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw invalid_argument("矩阵维度不匹配，无法相加");
        }
        
        SparseMatrix<T> result(rows, cols);
        
        Triple<T>* p1 = head->next;
        Triple<T>* p2 = other.head->next;
        
        while (p1 != nullptr && p2 != nullptr) {
            int pos1 = p1->row * cols + p1->col;
            int pos2 = p2->row * cols + p2->col;
            
            if (pos1 < pos2) {
                result.insert(p1->row, p1->col, p1->data);
                p1 = p1->next;
            } else if (pos1 > pos2) {
                result.insert(p2->row, p2->col, p2->data);
                p2 = p2->next;
            } else {
                T sum = p1->data + p2->data;
                if (sum != T(0)) {
                    result.insert(p1->row, p1->col, sum);
                }
                p1 = p1->next;
                p2 = p2->next;
            }
        }
        
        // 处理剩余元素
        while (p1 != nullptr) {
            result.insert(p1->row, p1->col, p1->data);
            p1 = p1->next;
        }
        
        while (p2 != nullptr) {
            result.insert(p2->row, p2->col, p2->data);
            p2 = p2->next;
        }
        
        return result;
    }
    
    // 矩阵相乘
    SparseMatrix<T> multiply(const SparseMatrix<T>& other) const {
        if (cols != other.rows) {
            throw invalid_argument("矩阵维度不匹配，无法相乘");
        }
        
        SparseMatrix<T> result(rows, other.cols);
        SparseMatrix<T> otherT = other.transpose(); // 转置第二个矩阵以提高效率
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < other.cols; j++) {
                T sum = T(0);
                
                // 计算结果的第i行第j列
                Triple<T>* p1 = getRow(i);
                Triple<T>* p2 = otherT.getRow(j);
                
                while (p1 != nullptr && p2 != nullptr && p1->row == i && p2->row == j) {
                    if (p1->col < p2->col) {
                        p1 = p1->next;
                    } else if (p1->col > p2->col) {
                        p2 = p2->next;
                    } else {
                        sum += p1->data * p2->data;
                        p1 = p1->next;
                        p2 = p2->next;
                    }
                }
                
                if (sum != T(0)) {
                    result.insert(i, j, sum);
                }
            }
        }
        
        return result;
    }
    
    // 获取指定行向量
    Triple<T>* getRow(int row) const {
        Triple<T>* current = head->next;
        while (current != nullptr && current->row < row) {
            current = current->next;
        }
        
        Triple<T>* rowHead = nullptr;
        Triple<T>* rowTail = nullptr;
        
        while (current != nullptr && current->row == row) {
            Triple<T>* newNode = new Triple<T>(current->row, current->col, current->data);
            if (rowHead == nullptr) {
                rowHead = rowTail = newNode;
            } else {
                rowTail->next = newNode;
                rowTail = newNode;
            }
            current = current->next;
        }
        
        return rowHead;
    }
    
    // 打印矩阵
    void print() const {
        cout << "稀疏矩阵 (" << rows << "×" << cols << ")，非零元素个数: " << nonZeroCount << endl;
        
        Triple<T>* current = head->next;
        while (current != nullptr) {
            cout << "(" << current->row << ", " << current->col << ") = " << current->data << endl;
            current = current->next;
        }
    }
    
    // 打印完整矩阵（包括0元素）
    void printFull() const {
        cout << "完整矩阵:" << endl;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << get(i, j) << "\t";
            }
            cout << endl;
        }
    }
    
    // 获取非零元素个数
    int getNonZeroCount() const {
        return nonZeroCount;
    }
    
    // 获取行数
    int getRows() const {
        return rows;
    }
    
    // 获取列数
    int getCols() const {
        return cols;
    }
};

// 测试函数
int main() {
    try {
        cout << "=== 稀疏矩阵测试 ===" << endl;
        
        // 创建第一个稀疏矩阵
        SparseMatrix<int> matrix1(3, 3);
        matrix1.insert(0, 0, 1);
        matrix1.insert(0, 2, 2);
        matrix1.insert(1, 1, 3);
        matrix1.insert(2, 0, 4);
        matrix1.insert(2, 2, 5);
        
        cout << "矩阵1:" << endl;
        matrix1.print();
        matrix1.printFull();
        cout << endl;
        
        // 创建第二个稀疏矩阵
        SparseMatrix<int> matrix2(3, 3);
        matrix2.insert(0, 1, 2);
        matrix2.insert(1, 0, 1);
        matrix2.insert(1, 2, 3);
        matrix2.insert(2, 1, 4);
        
        cout << "矩阵2:" << endl;
        matrix2.print();
        matrix2.printFull();
        cout << endl;
        
        // 测试转置
        cout << "矩阵1的转置:" << endl;
        SparseMatrix<int> transposed = matrix1.transpose();
        transposed.printFull();
        cout << endl;
        
        // 测试矩阵相加
        cout << "矩阵1 + 矩阵2:" << endl;
        SparseMatrix<int> sum = matrix1.add(matrix2);
        sum.printFull();
        cout << endl;
        
        // 测试矩阵相乘
        cout << "矩阵1 × 矩阵2:" << endl;
        SparseMatrix<int> product = matrix1.multiply(matrix2);
        product.printFull();
        cout << endl;
        
        // 测试边界情况
        cout << "=== 边界情况测试 ===" << endl;
        
        // 测试插入0值（应该被忽略）
        matrix1.insert(1, 1, 0);
        cout << "插入0值后，非零元素个数: " << matrix1.getNonZeroCount() << endl;
        
        // 测试更新现有元素
        matrix1.insert(0, 0, 10);
        cout << "更新(0,0)元素后:" << endl;
        cout << "matrix1[0,0] = " << matrix1.get(0, 0) << endl;
        
    } catch (const exception& e) {
        cerr << "错误: " << e.what() << endl;
    }
    
    return 0;
}