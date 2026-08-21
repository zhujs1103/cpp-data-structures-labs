#include <iostream>
#include <cstring>
#include <stdexcept>

// Address book data structure
struct DataType {
    int ID;           // ID number
    char name[10];    // Name
    char ch;          // Gender
    char phone[13];   // Phone number
    char addr[31];    // Address
    
    // Constructor
    DataType(int id = 0, const char* n = "", char c = 'M', 
             const char* p = "", const char* a = "") : ID(id), ch(c) {
        strncpy(name, n, 9);
        name[9] = '\0';
        strncpy(phone, p, 12);
        phone[12] = '\0';
        strncpy(addr, a, 30);
        addr[30] = '\0';
    }
    
    // Overload output operator
    friend std::ostream& operator<<(std::ostream& os, const DataType& data) {
        os << "ID: " << data.ID << ", Name: " << data.name 
           << ", Gender: " << data.ch << ", Phone: " << data.phone 
           << ", Address: " << data.addr;
        return os;
    }
    
    // Overload comparison operators (compare by ID)
    bool operator==(const DataType& other) const {
        return ID == other.ID;
    }
    
    bool operator<(const DataType& other) const {
        return ID < other.ID;
    }
};

// Linked list node class
template<typename T>
class ListNode {
public:
    T data;
    ListNode<T>* next;
    
    ListNode(const T& item, ListNode<T>* nextPtr = nullptr) 
        : data(item), next(nextPtr) {}
};

// Address Book Management Class (Singly Linked List with Head Node)
class AddressBook {
private:
    ListNode<DataType>* head;  // Head node
    int length;               // List length
    
    // Deep copy function
    void deepCopy(const AddressBook& other) {
        head = new ListNode<DataType>(DataType());  // Create head node
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
    // Constructor
    AddressBook() : head(new ListNode<DataType>(DataType())), length(0) {}
    
    // Copy constructor
    AddressBook(const AddressBook& other) {
        deepCopy(other);
    }
    
    // Assignment operator overload
    AddressBook& operator=(const AddressBook& other) {
        if (this != &other) {
            clear();
            deepCopy(other);
        }
        return *this;
    }
    
    // Destructor
    ~AddressBook() {
        clear();
        delete head;
    }
    
    // Clear the address book
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
    
    // Get address book length
    int getLength() const {
        return length;
    }
    
    // Check if address book is empty
    bool isEmpty() const {
        return length == 0;
    }
    
    // Search contact by ID
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
    
    // Search contact by Name
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
    
    // Add contact (Tail insertion method)
    void addContact(const DataType& contact) {
        // Check if ID already exists
        if (searchByID(contact.ID) != nullptr) {
            throw std::invalid_argument("ID already exists, cannot add duplicate contact");
        }
        
        ListNode<DataType>* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        
        current->next = new ListNode<DataType>(contact);
        length++;
    }
    
    // Delete contact by ID
    bool deleteByID(int id) {
        if (isEmpty()) {
            throw std::runtime_error("Address book is empty, cannot delete");
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
    
    // Delete contact by Name
    bool deleteByName(const char* name) {
        if (isEmpty()) {
            throw std::runtime_error("Address book is empty, cannot delete");
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
    
    // Modify contact information
    bool modifyContact(int id, const DataType& newData) {
        DataType* contact = searchByID(id);
        if (contact != nullptr) {
            *contact = newData;
            return true;
        }
        return false;
    }
    
    // Display all contacts
    void displayAll() const {
        if (isEmpty()) {
            std::cout << "Address book is empty" << std::endl;
            return;
        }
        
        std::cout << "Total contacts in address book: " << length << std::endl;
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
    
    // Sort by ID (Bubble sort)
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
                    // Swap nodes
                    current->next = next->next;
                    next->next = current;
                    prev->next = next;
                    
                    // Update pointers
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

// Menu display function
void displayMenu() {
    std::cout << "\n========== Address Book Management System ==========" << std::endl;
    std::cout << "1. Add Contact" << std::endl;
    std::cout << "2. Delete Contact by ID" << std::endl;
    std::cout << "3. Delete Contact by Name" << std::endl;
    std::cout << "4. Search Contact by ID" << std::endl;
    std::cout << "5. Search Contact by Name" << std::endl;
    std::cout << "6. Modify Contact Information" << std::endl;
    std::cout << "7. Display All Contacts" << std::endl;
    std::cout << "8. Sort by ID" << std::endl;
    std::cout << "9. Clear Address Book" << std::endl;
    std::cout << "0. Exit System" << std::endl;
    std::cout << "====================================================" << std::endl;
    std::cout << "Please choose an operation: ";
}

// Test function
int main() {
    AddressBook addressBook;
    int choice;
    
    // Add some test data
    try {
        addressBook.addContact(DataType(1, "Zhang", 'M', "13800138000", "Haidian District, Beijing"));
        addressBook.addContact(DataType(2, "Li", 'F', "13900139000", "Pudong New Area, Shanghai"));
        addressBook.addContact(DataType(3, "Wang", 'M', "13600136000", "Tianhe District, Guangzhou"));
    } catch (const std::exception& e) {
        std::cout << "Error initializing test data: " << e.what() << std::endl;
    }
    
    do {
        displayMenu();
        std::cin >> choice;
        
        try {
            switch (choice) {
                case 1: {
                    // Add contact
                    int id;
                    char name[10], ch, phone[13], addr[31];
                    
                    std::cout << "Please enter contact information:" << std::endl;
                    std::cout << "ID: ";
                    std::cin >> id;
                    std::cout << "Name: ";
                    std::cin >> name;
                    std::cout << "Gender(M/F): ";
                    std::cin >> ch;
                    std::cout << "Phone: ";
                    std::cin >> phone;
                    std::cout << "Address: ";
                    std::cin.ignore();  // Clear buffer
                    std::cin.getline(addr, 30);
                    
                    DataType newContact(id, name, ch, phone, addr);
                    addressBook.addContact(newContact);
                    std::cout << "Contact added successfully!" << std::endl;
                    break;
                }
                
                case 2: {
                    // Delete by ID
                    int id;
                    std::cout << "Please enter the ID of the contact to delete: ";
                    std::cin >> id;
                    
                    if (addressBook.deleteByID(id)) {
                        std::cout << "Contact deleted successfully!" << std::endl;
                    } else {
                        std::cout << "Contact with this ID not found" << std::endl;
                    }
                    break;
                }
                
                case 3: {
                    // Delete by Name
                    char name[10];
                    std::cout << "Please enter the name of the contact to delete: ";
                    std::cin >> name;
                    
                    if (addressBook.deleteByName(name)) {
                        std::cout << "Contact deleted successfully!" << std::endl;
                    } else {
                        std::cout << "Contact with this name not found" << std::endl;
                    }
                    break;
                }
                
                case 4: {
                    // Search by ID
                    int id;
                    std::cout << "Please enter the ID of the contact to search for: ";
                    std::cin >> id;
                    
                    DataType* contact = addressBook.searchByID(id);
                    if (contact != nullptr) {
                        std::cout << "Contact found: " << *contact << std::endl;
                    } else {
                        std::cout << "Contact with this ID not found" << std::endl;
                    }
                    break;
                }
                
                case 5: {
                    // Search by Name
                    char name[10];
                    std::cout << "Please enter the name of the contact to search for: ";
                    std::cin >> name;
                    
                    DataType* contact = addressBook.searchByName(name);
                    if (contact != nullptr) {
                        std::cout << "Contact found: " << *contact << std::endl;
                    } else {
                        std::cout << "Contact with this name not found" << std::endl;
                    }
                    break;
                }
                
                case 6: {
                    // Modify contact
                    int id;
                    std::cout << "Please enter the ID of the contact to modify: ";
                    std::cin >> id;
                    
                    DataType* oldContact = addressBook.searchByID(id);
                    if (oldContact != nullptr) {
                        char name[10], ch, phone[13], addr[31];
                        
                        std::cout << "Original information: " << *oldContact << std::endl;
                        std::cout << "Please enter new information:" << std::endl;
                        std::cout << "Name: ";
                        std::cin >> name;
                        std::cout << "Gender(M/F): ";
                        std::cin >> ch;
                        std::cout << "Phone: ";
                        std::cin >> phone;
                        std::cout << "Address: ";
                        std::cin.ignore();
                        std::cin.getline(addr, 30);
                        
                        DataType newContact(id, name, ch, phone, addr);
                        if (addressBook.modifyContact(id, newContact)) {
                            std::cout << "Contact modified successfully!" << std::endl;
                        }
                    } else {
                        std::cout << "Contact with this ID not found" << std::endl;
                    }
                    break;
                }
                
                case 7: {
                    // Display all contacts
                    addressBook.displayAll();
                    break;
                }
                
                case 8: {
                    // Sort by ID
                    addressBook.sortByID();
                    std::cout << "Sorting completed!" << std::endl;
                    addressBook.displayAll();
                    break;
                }
                
                case 9: {
                    // Clear address book
                    addressBook.clear();
                    std::cout << "Address book cleared!" << std::endl;
                    break;
                }
                
                case 0: {
                    std::cout << "Thank you for using the Address Book Management System!" << std::endl;
                    break;
                }
                
                default: {
                    std::cout << "Invalid choice, please try again!" << std::endl;
                    break;
                }
            }
        } catch (const std::exception& e) {
            std::cout << "Operation error: " << e.what() << std::endl;
        }
        
    } while (choice != 0);
    
    return 0;
}