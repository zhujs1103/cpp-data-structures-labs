#include <iostream>
#include <vector>
#include <stdexcept>
using namespace std;

template<class T>
struct Triple {
    int row;
    int col;
    T data;
    Triple<T>* next;
    
    Triple(int r, int c, T d, Triple<T>* n = nullptr) 
        : row(r), col(c), data(d), next(n) {}
};

template<class T>
class SparseMatrix {
private:
    Triple<T>* head;
    int rows, cols;
    int nonZeroCount;

public:
    SparseMatrix(int r, int c) : rows(r), cols(c), nonZeroCount(0) {
        head = new Triple<T>(-1, -1, T());
    }
    
    ~SparseMatrix() {
        clear();
        delete head;
    }
    
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
    
    void insert(int row, int col, T data) {
        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            throw out_of_range("Row or column index out of range");
        }
        
        if (data == T(0)) {
            return;
        }
        
        Triple<T>* newNode = new Triple<T>(row, col, data);
        
        Triple<T>* prev = head;
        Triple<T>* current = head->next;
        
        while (current != nullptr) {
            if (current->row > row || (current->row == row && current->col > col)) {
                break;
            }
            if (current->row == row && current->col == col) {
                current->data = data;
                delete newNode;
                return;
            }
            prev = current;
            current = current->next;
        }
        
        newNode->next = current;
        prev->next = newNode;
        nonZeroCount++;
    }
    
    T get(int row, int col) const {
        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            throw out_of_range("Row or column index out of range");
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
        
        return T(0);
    }
    
    SparseMatrix<T> transpose() const {
        SparseMatrix<T> result(cols, rows);
        
        Triple<T>* current = head->next;
        while (current != nullptr) {
            result.insert(current->col, current->row, current->data);
            current = current->next;
        }
        
        return result;
    }
    
    SparseMatrix<T> add(const SparseMatrix<T>& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw invalid_argument("Matrix dimensions do not match for addition");
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
    
    SparseMatrix<T> multiply(const SparseMatrix<T>& other) const {
        if (cols != other.rows) {
            throw invalid_argument("Matrix dimensions do not match for multiplication");
        }
        
        SparseMatrix<T> result(rows, other.cols);
        SparseMatrix<T> otherT = other.transpose();
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < other.cols; j++) {
                T sum = T(0);
                
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
    
    Triple<T>* getRow(int row) const {
        Triple<T>* current = head->next;
        while (current != nullptr && current->row < row) {
            current = current->next;
        }
        
        if (current != nullptr && current->row == row) {
            return current;
        }
        else {
        return nullptr;
        }
    }
    
    void print() const {
        cout << "Sparse Matrix (" << rows << "x" << cols << "), Non-zero elements: " << nonZeroCount << endl;
        
        Triple<T>* current = head->next;
        while (current != nullptr) {
            cout << "(" << current->row << ", " << current->col << ") = " << current->data << endl;
            current = current->next;
        }
    }
    
    void printFull() const {
        cout << "Full Matrix:" << endl;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << get(i, j) << "\t";
            }
            cout << endl;
        }
    }
    
    int getNonZeroCount() const {
        return nonZeroCount;
    }
    
    int getRows() const {
        return rows;
    }
    
    int getCols() const {
        return cols;
    }
};

int main() {
    try {
        cout << "=== Sparse Matrix Test ===" << endl;
        
        SparseMatrix<int> matrix1(3, 3);
        matrix1.insert(0, 0, 1);
        matrix1.insert(0, 2, 2);
        matrix1.insert(1, 1, 3);
        matrix1.insert(2, 0, 4);
        matrix1.insert(2, 2, 5);
        
        cout << "Matrix 1:" << endl;
        matrix1.print();
        matrix1.printFull();
        cout << endl;
        
        SparseMatrix<int> matrix2(3, 3);
        matrix2.insert(0, 1, 2);
        matrix2.insert(1, 0, 1);
        matrix2.insert(1, 2, 3);
        matrix2.insert(2, 1, 4);
        
        cout << "Matrix 2:" << endl;
        matrix2.print();
        matrix2.printFull();
        cout << endl;
        
        cout << "Matrix 1 Transpose:" << endl;
        SparseMatrix<int> transposed = matrix1.transpose();
        transposed.printFull();
        cout << endl;
        
        cout << "Matrix 1 + Matrix 2:" << endl;
        SparseMatrix<int> sum = matrix1.add(matrix2);
        sum.printFull();
        cout << endl;
        
        cout << "Matrix 1 x Matrix 2:" << endl;
        SparseMatrix<int> product = matrix1.multiply(matrix2);
        product.printFull();
        cout << endl;
        
        cout << "=== Edge Case Tests ===" << endl;
        
        matrix1.insert(1, 1, 0);
        cout << "After inserting 0 value, non-zero count: " << matrix1.getNonZeroCount() << endl;
        
        matrix1.insert(0, 0, 10);
        cout << "After updating (0,0) element:" << endl;
        cout << "matrix1[0,0] = " << matrix1.get(0, 0) << endl;
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    
    return 0;
}