#include <iostream>
using namespace std;

// 链表节点，表示一个皇后的位置
struct QueenNode {
    int row;        // 行
    int col;        // 列
    QueenNode* next; // 指向下一个皇后节点
    
    QueenNode(int r, int c) : row(r), col(c), next(nullptr) {}
};

// 八皇后问题求解类
class EightQueens {
private:
    QueenNode* head; // 链表头节点
    int solutionCount; // 解决方案计数
    
public:
    EightQueens() : head(nullptr), solutionCount(0) {}
    
    ~EightQueens() {
        clearList();
    }
    
    // 清空链表
    void clearList() {
        while (head != nullptr) {
            QueenNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
    
    // 检查当前位置是否安全
    bool isSafe(int row, int col) {
        QueenNode* current = head;
        
        // 检查所有已放置的皇后
        while (current != nullptr) {
            // 检查同一列
            if (current->col == col) {
                return false;
            }
            
            // 检查对角线 (行差 == 列差)
            if (abs(current->row - row) == abs(current->col - col)) {
                return false;
            }
            
            current = current->next;
        }
        
        return true;
    }
    
    // 递归解决八皇后问题
    void solve(int row) {
        // 如果已经放置了8个皇后，打印解决方案
        if (row == 8) {
            printSolution();
            return;
        }
        
        // 尝试在当前行的每一列放置皇后
        for (int col = 0; col < 8; col++) {
            if (isSafe(row, col)) {
                // 放置皇后
                addQueen(row, col);
                
                // 递归处理下一行
                solve(row + 1);
                
                // 回溯，移除当前皇后
                removeLastQueen();
            }
        }
    }
    
    // 添加皇后到链表
    void addQueen(int row, int col) {
        QueenNode* newNode = new QueenNode(row, col);
        
        // 如果链表为空，新节点作为头节点
        if (head == nullptr) {
            head = newNode;
        } else {
            // 否则找到链表末尾并添加
            QueenNode* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
    }
    
    // 移除链表末尾的皇后（回溯）
    void removeLastQueen() {
        if (head == nullptr) {
            return;
        }
        
        // 如果只有一个节点
        if (head->next == nullptr) {
            delete head;
            head = nullptr;
            return;
        }
        
        // 找到倒数第二个节点
        QueenNode* current = head;
        while (current->next != nullptr && current->next->next != nullptr) {
            current = current->next;
        }
        
        // 删除最后一个节点
        delete current->next;
        current->next = nullptr;
    }
    
    // 打印解决方案
    void printSolution() {
        solutionCount++;
        cout << "解决方案 " << solutionCount << ":" << endl;
        
        // 创建棋盘
        char board[8][8];
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                board[i][j] = '.';
            }
        }
        
        // 在棋盘上标记皇后的位置
        QueenNode* current = head;
        while (current != nullptr) {
            board[current->row][current->col] = 'Q';
            current = current->next;
        }
        
        // 打印棋盘
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                cout << board[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
    
    // 获取解决方案数量
    int getSolutionCount() {
        return solutionCount;
    }
    
    // 启动求解过程
    void findSolutions() {
        solutionCount = 0;
        clearList();
        solve(0);
        cout << "总共找到 " << solutionCount << " 种解决方案" << endl;
    }
};

// 测试主函数
int main() {
    EightQueens solver;
    
    cout << "八皇后问题解决方案（链表实现）：" << endl;
    cout << "==============================" << endl;
    
    solver.findSolutions();
    
    return 0;
}