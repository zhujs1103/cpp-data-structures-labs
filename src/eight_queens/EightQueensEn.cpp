#include <iostream>
#include <cmath>
using namespace std;

// Linked list node to represent a queen's position
struct QueenNode {
    int row;        // Row position
    int col;        // Column position
    QueenNode* next; // Pointer to next queen node
    
    QueenNode(int r, int c) : row(r), col(c), next(nullptr) {}
};

// Eight Queens Problem Solver Class
class EightQueens {
private:
    QueenNode* head;        // Head of the linked list
    int solutionCount;      // Counter for solutions
    
public:
    EightQueens() : head(nullptr), solutionCount(0) {}
    
    ~EightQueens() {
        clearList();
    }
    
    // Clear the linked list
    void clearList() {
        while (head != nullptr) {
            QueenNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
    
    // Check if position (row, col) is safe for a new queen
    bool isSafe(int row, int col) {
        QueenNode* current = head;
        
        // Check all previously placed queens
        while (current != nullptr) {
            // Check same column
            if (current->col == col) {
                return false;
            }
            
            // Check diagonal (absolute row difference equals absolute column difference)
            if (abs(current->row - row) == abs(current->col - col)) {
                return false;
            }
            
            current = current->next;
        }
        
        return true;
    }
    
    // Recursive function to solve Eight Queens problem
    void solve(int row) {
        // Base case: all 8 queens placed successfully
        if (row == 8) {
            printSolution();
            return;
        }
        
        // Try placing queen in each column of current row
        for (int col = 0; col < 8; col++) {
            if (isSafe(row, col)) {
                // Place queen at (row, col)
                addQueen(row, col);
                
                // Recursively solve for next row
                solve(row + 1);
                
                // Backtrack: remove the last placed queen
                removeLastQueen();
            }
        }
    }
    
    // Add a queen to the linked list
    void addQueen(int row, int col) {
        QueenNode* newNode = new QueenNode(row, col);
        
        // If list is empty, new node becomes head
        if (head == nullptr) {
            head = newNode;
        } else {
            // Find the end of list and append new node
            QueenNode* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
    }
    
    // Remove the last queen from linked list (backtracking)
    void removeLastQueen() {
        if (head == nullptr) {
            return;
        }
        
        // If only one node in list
        if (head->next == nullptr) {
            delete head;
            head = nullptr;
            return;
        }
        
        // Find the second last node
        QueenNode* current = head;
        while (current->next != nullptr && current->next->next != nullptr) {
            current = current->next;
        }
        
        // Delete the last node
        delete current->next;
        current->next = nullptr;
    }
    
    // Print the current solution
    void printSolution() {
        solutionCount++;
        cout << "Solution " << solutionCount << ":" << endl;
        
        // Initialize chess board
        char board[8][8];
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                board[i][j] = '.';
            }
        }
        
        // Mark queen positions on board
        QueenNode* current = head;
        while (current != nullptr) {
            board[current->row][current->col] = 'Q';
            current = current->next;
        }
        
        // Display the board
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                cout << board[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
    
    // Get total number of solutions found
    int getSolutionCount() {
        return solutionCount;
    }
    
    // Main function to initiate the solving process
    void findSolutions() {
        solutionCount = 0;
        clearList();
        solve(0);
        cout << "Total solutions found: " << solutionCount << endl;
    }
};

// Main test function
int main() {
    EightQueens solver;
    
    cout << "Eight Queens Problem Solutions:" << endl;
    cout << "============================================================" << endl;
    
    solver.findSolutions();
    
    return 0;
}