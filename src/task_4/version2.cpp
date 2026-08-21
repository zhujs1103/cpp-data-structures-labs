#include <iostream>
#include <cmath>
using namespace std;

// Polynomial term structure
struct Term {
    float coef;  // coefficient
    int expn;    // exponent
    Term* next;  // next term pointer
    
    Term(float c, int e) : coef(c), expn(e), next(nullptr) {}
};

class Polynomial {
private:
    Term* head;  // linked list head node
    
public:
    // Constructor
    Polynomial() {
        head = new Term(0, -1);  // head node, doesn't store actual data
    }
    
    // Copy constructor (deep copy)
    Polynomial(const Polynomial& other) {
        head = new Term(0, -1);
        Term* current = head;
        Term* otherCurrent = other.head->next;
        
        while (otherCurrent != nullptr) {
            current->next = new Term(otherCurrent->coef, otherCurrent->expn);
            current = current->next;
            otherCurrent = otherCurrent->next;
        }
    }
    
    // Assignment operator
    Polynomial& operator=(const Polynomial& other) {
        if (this != &other) {
            clear();
            Term* current = head;
            Term* otherCurrent = other.head->next;
            
            while (otherCurrent != nullptr) {
                current->next = new Term(otherCurrent->coef, otherCurrent->expn);
                current = current->next;
                otherCurrent = otherCurrent->next;
            }
        }
        return *this;
    }
    
    // Destructor
    ~Polynomial() {
        clear();
        delete head;
    }
    
    // Clear polynomial
    void clear() {
        Term* current = head->next;
        while (current != nullptr) {
            Term* temp = current;
            current = current->next;
            delete temp;
        }
        head->next = nullptr;
    }
    
    // Input polynomial
    void input() {
        clear();
        cout << "Please enter the number of polynomial terms: ";
        int n;
        cin >> n;
        
        if (n <= 0) {
            cout << "Invalid number of terms!" << endl;
            return;
        }
        
        cout << "Please enter the coefficient and exponent of each term (coefficient exponent):" << endl;
        for (int i = 0; i < n; i++) {
            float coef;
            int expn;
            cin >> coef >> expn;
            
            // Skip terms with coefficient 0 (as shown in image 3)
            if (fabs(coef) < 1e-6) {
                cout << "Skipping term with coefficient 0." << endl;
                continue;
            }
            
            insertTerm(coef, expn);
        }
    }
    
    // Insert term (arranged in descending order of exponents)
    void insertTerm(float coef, int expn) {
        Term* newTerm = new Term(coef, expn);
        Term* prev = head;
        Term* current = head->next;
        
        // Find insertion position (descending order by exponent)
        while (current != nullptr && current->expn > expn) {
            prev = current;
            current = current->next;
        }
        
        // Handle same exponents (combine like terms)
        if (current != nullptr && current->expn == expn) {
            current->coef += coef;
            delete newTerm;
            
            // If coefficient becomes 0 after combination, delete the term
            if (fabs(current->coef) < 1e-6) {
                prev->next = current->next;
                delete current;
            }
        } else {
            newTerm->next = current;
            prev->next = newTerm;
        }
    }
    
    // Output polynomial
    void output() const {
        if (head->next == nullptr) {
            cout << "0";
            return;
        }
        
        Term* current = head->next;
        bool firstTerm = true;
        
        while (current != nullptr) {
            // Handle coefficient sign and display
            if (firstTerm) {
                if (current->coef < 0) {
                    cout << "-";
                }
            } else {
                if (current->coef > 0) {
                    cout << " + ";
                } else {
                    cout << " - ";
                }
            }
            
            // Output coefficient and exponent
            float absCoef = fabs(current->coef);
            
            // Handle different cases for coefficient and exponent values
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
    
    // Polynomial addition (as shown in image 4)
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
                // Same exponents: add coefficients (handle case where sum is 0)
                float sumCoef = p1->coef + p2->coef;
                if (fabs(sumCoef) >= 1e-6) {
                    result.insertTerm(sumCoef, p1->expn);
                }
                p1 = p1->next;
                p2 = p2->next;
            }
        }
        
        // Add remaining terms from first polynomial
        while (p1 != nullptr) {
            result.insertTerm(p1->coef, p1->expn);
            p1 = p1->next;
        }
        
        // Add remaining terms from second polynomial
        while (p2 != nullptr) {
            result.insertTerm(p2->coef, p2->expn);
            p2 = p2->next;
        }
        
        return result;
    }
    
    // Polynomial subtraction (as shown in image 4)
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
                // Same exponents: subtract coefficients (handle case where difference is 0)
                float diffCoef = p1->coef - p2->coef;
                if (fabs(diffCoef) >= 1e-6) {
                    result.insertTerm(diffCoef, p1->expn);
                }
                p1 = p1->next;
                p2 = p2->next;
            }
        }
        
        // Add remaining terms from first polynomial
        while (p1 != nullptr) {
            result.insertTerm(p1->coef, p1->expn);
            p1 = p1->next;
        }
        
        // Subtract remaining terms from second polynomial
        while (p2 != nullptr) {
            result.insertTerm(-p2->coef, p2->expn);
            p2 = p2->next;
        }
        
        return result;
    }
    
    // Calculate polynomial value at x
    float evaluate(float x) const {
        float result = 0.0;
        Term* current = head->next;
        
        while (current != nullptr) {
            result += current->coef * pow(x, current->expn);
            current = current->next;
        }
        
        return result;
    }
    
    // Derivative
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
    
    // Polynomial multiplication (as shown in image 4)
    Polynomial multiply(const Polynomial& other) const {
        Polynomial result;
        
        Term* p1 = this->head->next;
        while (p1 != nullptr) {
            Term* p2 = other.head->next;
            while (p2 != nullptr) {
                // Multiply coefficients and add exponents
                float productCoef = p1->coef * p2->coef;
                int sumExpn = p1->expn + p2->expn;
                
                // Insert the product term (insertTerm will handle combining like terms)
                result.insertTerm(productCoef, sumExpn);
                
                p2 = p2->next;
            }
            p1 = p1->next;
        }
        
        return result;
    }
    
    // Check if polynomial is empty
    bool isEmpty() const {
        return head->next == nullptr;
    }
};

// Test function
int main() {
    Polynomial poly1, poly2, result;
    int choice;
    float x;
    
    cout << "=== Univariate Polynomial Calculator ===" << endl;
    
    do {
        cout << "\nMenu:" << endl;
        cout << "1. Input Polynomial 1" << endl;
        cout << "2. Input Polynomial 2" << endl;
        cout << "3. Output Polynomial 1" << endl;
        cout << "4. Output Polynomial 2" << endl;
        cout << "5. Polynomial Addition" << endl;
        cout << "6. Polynomial Subtraction" << endl;
        cout << "7. Polynomial Multiplication" << endl;
        cout << "8. Calculate Polynomial Value at x" << endl;
        cout << "9. Calculate Polynomial Derivative" << endl;
        cout << "0. Exit" << endl;
        cout << "Please choose an operation: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                cout << "Input Polynomial 1:" << endl;
                poly1.input();
                break;
                
            case 2:
                cout << "Input Polynomial 2:" << endl;
                poly2.input();
                break;
                
            case 3:
                if (poly1.isEmpty()) {
                    cout << "Polynomial 1 is empty. Please input polynomial first." << endl;
                } else {
                    cout << "Polynomial 1: ";
                    poly1.output();
                }
                break;
                
            case 4:
                if (poly2.isEmpty()) {
                    cout << "Polynomial 2 is empty. Please input polynomial first." << endl;
                } else {
                    cout << "Polynomial 2: ";
                    poly2.output();
                }
                break;
                
            case 5:
                if (poly1.isEmpty() || poly2.isEmpty()) {
                    cout << "Please input both polynomials first." << endl;
                } else {
                    result = poly1.add(poly2);
                    cout << "Polynomial 1: ";
                    poly1.output();
                    cout << "Polynomial 2: ";
                    poly2.output();
                    cout << "Addition Result: ";
                    result.output();
                }
                break;
                
            case 6:
                if (poly1.isEmpty() || poly2.isEmpty()) {
                    cout << "Please input both polynomials first." << endl;
                } else {
                    result = poly1.subtract(poly2);
                    cout << "Polynomial 1: ";
                    poly1.output();
                    cout << "Polynomial 2: ";
                    poly2.output();
                    cout << "Subtraction Result: ";
                    result.output();
                }
                break;
                
            case 7:
                if (poly1.isEmpty() || poly2.isEmpty()) {
                    cout << "Please input both polynomials first." << endl;
                } else {
                    result = poly1.multiply(poly2);
                    cout << "Polynomial 1: ";
                    poly1.output();
                    cout << "Polynomial 2: ";
                    poly2.output();
                    cout << "Multiplication Result: ";
                    result.output();
                }
                break;
                
            case 8:
                if (poly1.isEmpty() && poly2.isEmpty()) {
                    cout << "Please input at least one polynomial first." << endl;
                } else {
                    cout << "Please enter the value of x: ";
                    cin >> x;
                    if (!poly1.isEmpty()) {
                        cout << "Polynomial 1 value at x=" << x << ": " << poly1.evaluate(x) << endl;
                    }
                    if (!poly2.isEmpty()) {
                        cout << "Polynomial 2 value at x=" << x << ": " << poly2.evaluate(x) << endl;
                    }
                }
                break;
                
            case 9:
                if (poly1.isEmpty() && poly2.isEmpty()) {
                    cout << "Please input at least one polynomial first." << endl;
                } else {
                    if (!poly1.isEmpty()) {
                        cout << "Derivative of Polynomial 1: ";
                        result = poly1.derivative();
                        result.output();
                    }
                    if (!poly2.isEmpty()) {
                        cout << "Derivative of Polynomial 2: ";
                        result = poly2.derivative();
                        result.output();
                    }
                }
                break;
                
            case 0:
                cout << "Program ended. Thank you for using!" << endl;
                break;
                
            default:
                cout << "Invalid choice, please try again!" << endl;
        }
    } while (choice != 0);
    
    return 0;
}