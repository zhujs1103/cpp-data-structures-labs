#include <iostream>
#include <queue>
#include <stack>
#include <vector>
using namespace std;

// 二叉树结点结构
struct BiTNode {
    char data;          // 数据域
    BiTNode* left;      // 左孩子指针
    BiTNode* right;     // 右孩子指针
    
    BiTNode(char val) : data(val), left(nullptr), right(nullptr) {}
};

// 二叉树类
class BiTree {
private:
    BiTNode* root;      // 根结点指针
    
    // 私有递归辅助函数
    BiTNode* createTreeRecursive() {
        char ch;
        cin >> ch;
        
        if (ch == '#') {  // '#'表示空结点
            return nullptr;
        }
        
        BiTNode* node = new BiTNode(ch);
        node->left = createTreeRecursive();
        node->right = createTreeRecursive();
        
        return node;
    }
    
    void preOrderRecursive(BiTNode* node) {
        if (node) {
            cout << node->data << " ";
            preOrderRecursive(node->left);
            preOrderRecursive(node->right);
        }
    }
    
    void inOrderRecursive(BiTNode* node) {
        if (node) {
            inOrderRecursive(node->left);
            cout << node->data << " ";
            inOrderRecursive(node->right);
        }
    }
    
    void postOrderRecursive(BiTNode* node) {
        if (node) {
            postOrderRecursive(node->left);
            postOrderRecursive(node->right);
            cout << node->data << " ";
        }
    }
    
    int getDepthRecursive(BiTNode* node) {
        if (!node) return 0;
        int leftDepth = getDepthRecursive(node->left);
        int rightDepth = getDepthRecursive(node->right);
        return max(leftDepth, rightDepth) + 1;
    }
    
    bool findPath(BiTNode* node, char target, vector<char>& path) {
        if (!node) return false;
        
        path.push_back(node->data);
        
        if (node->data == target) return true;
        
        if (findPath(node->left, target, path) || findPath(node->right, target, path)) {
            return true;
        }
        
        path.pop_back();
        return false;
    }
    
    void destroyRecursive(BiTNode* node) {
        if (node) {
            destroyRecursive(node->left);
            destroyRecursive(node->right);
            delete node;
        }
    }
    
public:
    // 构造函数 - 递归方式创建
    BiTree() : root(nullptr) {}
    
    // 1. 二叉树的建立（递归方式）
    void createTree() {
        cout << "请按前序遍历顺序输入二叉树（空结点用#表示）：" << endl;
        root = createTreeRecursive();
        cout << "二叉树创建完成！" << endl;
    }
    
    // 2. 前序遍历（递归）
    void preOrder() {
        cout << "前序遍历：";
        preOrderRecursive(root);
        cout << endl;
    }
    
    // 3. 中序遍历（递归）
    void inOrder() {
        cout << "中序遍历：";
        inOrderRecursive(root);
        cout << endl;
    }
    
    // 4. 后序遍历（递归）
    void postOrder() {
        cout << "后序遍历：";
        postOrderRecursive(root);
        cout << endl;
    }
    
    // 5. 层序遍历（非递归）
    void levelOrder() {
        cout << "层序遍历：";
        if (!root) {
            cout << "空树" << endl;
            return;
        }
        
        queue<BiTNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            BiTNode* current = q.front();
            q.pop();
            cout << current->data << " ";
            
            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
        cout << endl;
    }
    
    // 6. 求二叉树的深度
    int getDepth() {
        return getDepthRecursive(root);
    }
    
    // 7. 求指定结点到根的路径
    void findPathToRoot(char target) {
        vector<char> path;
        if (findPath(root, target, path)) {
            cout << "结点 " << target << " 到根的路径：";
            for (char c : path) {
                cout << c << " ";
            }
            cout << endl;
        } else {
            cout << "未找到结点 " << target << endl;
        }
    }
    
    // 8. 二叉树的销毁
    void destroy() {
        destroyRecursive(root);
        root = nullptr;
        cout << "二叉树已销毁" << endl;
    }
    
    // 9. 其他自定义操作：统计结点总数
    int countNodes() {
        return countNodesRecursive(root);
    }
    
    int countNodesRecursive(BiTNode* node) {
        if (!node) return 0;
        return 1 + countNodesRecursive(node->left) + countNodesRecursive(node->right);
    }
    
    // 析构函数
    ~BiTree() {
        destroy();
    }
    
    // 非递归方式创建二叉树（解决思考问题1）
    void createTreeNonRecursive() {
        cout << "请输入前序遍历序列（空结点用#表示，以$结束）：" << endl;
        stack<BiTNode*> s;
        char ch;
        BiTNode* parent = nullptr;
        bool isLeft = true;
        
        cin >> ch;
        if (ch == '#' || ch == '$') {
            root = nullptr;
            return;
        }
        
        root = new BiTNode(ch);
        s.push(root);
        
        while (cin >> ch && ch != '$') {
            if (ch != '#') {
                BiTNode* newNode = new BiTNode(ch);
                if (isLeft) {
                    s.top()->left = newNode;
                } else {
                    s.top()->right = newNode;
                }
                s.push(newNode);
                isLeft = true;
            } else {
                if (!isLeft) {
                    s.pop();
                }
                isLeft = !isLeft;
            }
            
            // 如果右子树处理完毕，弹出栈顶
            while (!s.empty() && s.top()->right) {
                s.pop();
            }
        }
        cout << "非递归方式创建二叉树完成！" << endl;
    }
    
    // 搜索指定值的叶子结点（思考问题2）
    BiTNode* searchLeafNode(char value) {
        return searchLeafRecursive(root, value);
    }
    
    BiTNode* searchLeafRecursive(BiTNode* node, char value) {
        if (!node) return nullptr;
        
        // 如果是叶子结点且值匹配
        if (!node->left && !node->right && node->data == value) {
            return node;
        }
        
        BiTNode* leftResult = searchLeafRecursive(node->left, value);
        if (leftResult) return leftResult;
        
        return searchLeafRecursive(node->right, value);
    }
    
    // 输出从根到指定叶子的路径（思考问题3）
    void printPathToLeaf(BiTNode* leaf) {
        if (!leaf) {
            cout << "叶子结点为空" << endl;
            return;
        }
        
        vector<char> path;
        if (findPath(root, leaf->data, path)) {
            cout << "从根到叶子结点 " << leaf->data << " 的路径：";
            for (char c : path) {
                cout << c << " ";
            }
            cout << endl;
        }
    }
};

// 测试主函数
int main() {
    cout << "========== 二叉树测试 ==========" << endl;
    
    BiTree tree;
    int choice;
    char target;
    
    do {
        cout << "\n========== 菜单 ==========" << endl;
        cout << "1. 递归创建二叉树" << endl;
        cout << "2. 非递归创建二叉树（防止栈溢出）" << endl;
        cout << "3. 前序遍历" << endl;
        cout << "4. 中序遍历" << endl;
        cout << "5. 后序遍历" << endl;
        cout << "6. 层序遍历" << endl;
        cout << "7. 求二叉树深度" << endl;
        cout << "8. 查找结点到根的路径" << endl;
        cout << "9. 统计结点总数" << endl;
        cout << "10. 搜索叶子结点" << endl;
        cout << "11. 输出叶子结点路径" << endl;
        cout << "0. 退出" << endl;
        cout << "请选择操作：";
        cin >> choice;
        
        switch (choice) {
            case 1:
                tree.createTree();
                break;
                
            case 2:
                tree.createTreeNonRecursive();
                break;
                
            case 3:
                tree.preOrder();
                break;
                
            case 4:
                tree.inOrder();
                break;
                
            case 5:
                tree.postOrder();
                break;
                
            case 6:
                tree.levelOrder();
                break;
                
            case 7:
                cout << "二叉树深度：" << tree.getDepth() << endl;
                break;
                
            case 8:
                cout << "请输入要查找的结点值：";
                cin >> target;
                tree.findPathToRoot(target);
                break;
                
            case 9:
                cout << "二叉树结点总数：" << tree.countNodes() << endl;
                break;
                
            case 10:
                cout << "请输入要搜索的叶子结点值：";
                cin >> target;
                {
                    BiTNode* leaf = tree.searchLeafNode(target);
                    if (leaf) {
                        cout << "找到叶子结点：" << leaf->data << endl;
                    } else {
                        cout << "未找到该叶子结点" << endl;
                    }
                }
                break;
                
            case 11:
                cout << "请输入叶子结点值：";
                cin >> target;
                {
                    BiTNode* leaf = tree.searchLeafNode(target);
                    if (leaf) {
                        tree.printPathToLeaf(leaf);
                    } else {
                        cout << "该结点不是叶子结点或不存在" << endl;
                    }
                }
                break;
                
            case 0:
                tree.destroy();
                cout << "程序退出" << endl;
                break;
                
            default:
                cout << "无效选择，请重新输入！" << endl;
        }
    } while (choice != 0);
    
    return 0;
}