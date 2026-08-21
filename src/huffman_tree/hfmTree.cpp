#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <algorithm>
#include <bitset>
#include <memory>
#include <iomanip>

using namespace std;

// 哈夫曼树结点结构
struct HuffmanNode {
    char ch;               // 字符
    int freq;             // 频率
    string code;          // 哈夫曼编码
    HuffmanNode* left;    // 左孩子指针
    HuffmanNode* right;   // 右孩子指针
    HuffmanNode* parent;  // 双亲指针
    
    HuffmanNode(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr), parent(nullptr) {}
    HuffmanNode(int f) : ch('\0'), freq(f), left(nullptr), right(nullptr), parent(nullptr) {}
};

// 哈夫曼树类
class HuffmanTree {
private:
    HuffmanNode* root;                       // 哈夫曼树根结点
    map<char, int> frequency;                // 字符频率统计
    map<char, string> huffmanCode;           // 哈夫曼编码表
    vector<HuffmanNode*> allNodes;           // 存储所有结点，方便内存管理
    string originalText;                     // 原始字符串
    
public:
    // 构造函数
    HuffmanTree() : root(nullptr) {}
    
    // 析构函数 - 释放所有动态分配的内存
    ~HuffmanTree() {
        clear();
    }
    
    // 1. 初始化：统计字符频率并建立哈夫曼树
    void init(const string& text) {
        originalText = text;
        frequency.clear();
        huffmanCode.clear();
        
        // 统计字符频率
        for (char c : text) {
            frequency[c]++;
        }
        
        // 建立哈夫曼树
        buildHuffmanTree();
        
        // 生成编码表
        generateCodes();
    }
    
    // 2. 建立哈夫曼树
    void buildHuffmanTree() {
        // 使用最小堆（优先队列）
        auto cmp = [](HuffmanNode* a, HuffmanNode* b) {
            return a->freq > b->freq;
        };
        priority_queue<HuffmanNode*, vector<HuffmanNode*>, decltype(cmp)> minHeap(cmp);
        
        // 创建叶子结点并加入优先队列
        for (const auto& pair : frequency) {
            HuffmanNode* node = new HuffmanNode(pair.first, pair.second);
            minHeap.push(node);
            allNodes.push_back(node);
        }
        
        // 构建哈夫曼树
        while (minHeap.size() > 1) {
            // 取出两个频率最小的结点
            HuffmanNode* left = minHeap.top();
            minHeap.pop();
            
            HuffmanNode* right = minHeap.top();
            minHeap.pop();
            
            // 创建新结点，频率为两个结点之和
            HuffmanNode* parent = new HuffmanNode(left->freq + right->freq);
            parent->left = left;
            parent->right = right;
            left->parent = parent;
            right->parent = parent;
            
            // 将新结点加入优先队列
            minHeap.push(parent);
            allNodes.push_back(parent);
        }
        
        // 设置根结点
        if (!minHeap.empty()) {
            root = minHeap.top();
        }
    }
    
    // 3. 生成哈夫曼编码表
    void generateCodes() {
        if (!root) return;
        
        // 深度优先遍历生成编码
        generateCodeDFS(root, "");
    }
    
    void generateCodeDFS(HuffmanNode* node, string code) {
        if (!node) return;
        
        // 如果是叶子结点，保存编码
        if (!node->left && !node->right) {
            node->code = code;
            huffmanCode[node->ch] = code;
            return;
        }
        
        // 左子树编码加"0"
        if (node->left) {
            generateCodeDFS(node->left, code + "0");
        }
        
        // 右子树编码加"1"
        if (node->right) {
            generateCodeDFS(node->right, code + "1");
        }
    }
    
    // 4. 编码
    string encoding(const string& text) {
        string encodedStr = "";
        for (char c : text) {
            if (huffmanCode.find(c) != huffmanCode.end()) {
                encodedStr += huffmanCode[c];
            }
        }
        return encodedStr;
    }
    
    // 5. 译码
    string decoding(const string& encodedStr) {
        string decodedStr = "";
        HuffmanNode* current = root;
        
        for (char bit : encodedStr) {
            if (bit == '0') {
                current = current->left;
            } else {
                current = current->right;
            }
            
            // 到达叶子结点
            if (!current->left && !current->right) {
                decodedStr += current->ch;
                current = root; // 重置到根结点
            }
        }
        
        return decodedStr;
    }
    
    // 6. 打印哈夫曼树（树形结构）
    void printTree() {
        if (!root) {
            cout << "树为空！" << endl;
            return;
        }
        
        cout << "\n哈夫曼树结构：" << endl;
        printTreeHelper(root, "", true);
    }
    
    void printTreeHelper(HuffmanNode* node, string prefix, bool isLeft) {
        if (!node) return;
        
        cout << prefix;
        cout << (isLeft ? "├── " : "└── ");
        
        if (node->ch == '\0') {
            cout << "频率:" << node->freq << endl;
        } else {
            cout << "'" << node->ch << "' 频率:" << node->freq 
                 << " 编码:" << node->code << endl;
        }
        
        // 递归打印左右子树
        if (node->left || node->right) {
            printTreeHelper(node->left, prefix + (isLeft ? "│   " : "    "), true);
            printTreeHelper(node->right, prefix + (isLeft ? "│   " : "    "), false);
        }
    }
    
    // 7. 打印编码表
    void printCodeTable() {
        cout << "\n哈夫曼编码表：" << endl;
        cout << "-----------------------------------" << endl;
        cout << left << setw(10) << "字符" 
             << setw(10) << "频率" 
             << setw(15) << "编码" << endl;
        cout << "-----------------------------------" << endl;
        
        for (const auto& pair : huffmanCode) {
            cout << left << setw(10) << pair.first 
                 << setw(10) << frequency[pair.first] 
                 << setw(15) << pair.second << endl;
        }
        cout << "-----------------------------------" << endl;
    }
    
    // 8. 计算压缩效果
    void analyzeCompression(const string& original, const string& encoded) {
        int originalBits = original.length() * 8;  // 原始位长度（假设每个字符8位）
        int encodedBits = encoded.length();         // 编码后位长度
        
        cout << "\n压缩效果分析：" << endl;
        cout << "原始文本长度: " << original.length() << " 字符" << endl;
        cout << "原始文本位长度: " << originalBits << " 位" << endl;
        cout << "编码后位长度: " << encodedBits << " 位" << endl;
        cout << "压缩率: " << fixed << setprecision(2) 
             << (1.0 - (double)encodedBits / originalBits) * 100 << "%" << endl;
        cout << "节省空间: " << originalBits - encodedBits << " 位" << endl;
    }
    
    // 9. 二进制编码方式（选作）
    string binaryEncoding(const string& text) {
        string encodedStr = encoding(text);
        string binaryStr = "";
        
        // 将编码字符串转换为二进制形式
        for (size_t i = 0; i < encodedStr.length(); i += 8) {
            string byteStr = encodedStr.substr(i, 8);
            if (byteStr.length() < 8) {
                // 不足8位补0
                byteStr.append(8 - byteStr.length(), '0');
            }
            bitset<8> bits(byteStr);
            binaryStr += bits.to_string() + " ";
        }
        
        return binaryStr;
    }
    
    // 清空内存
    void clear() {
        for (HuffmanNode* node : allNodes) {
            delete node;
        }
        allNodes.clear();
        frequency.clear();
        huffmanCode.clear();
        root = nullptr;
        originalText.clear();
    }
    
    // 获取字符频率
    map<char, int> getFrequency() const {
        return frequency;
    }
    
    // 获取编码表
    map<char, string> getHuffmanCode() const {
        return huffmanCode;
    }
};

// 菜单界面
void displayMenu() {
    cout << "\n========== 哈夫曼编/解码器 ==========" << endl;
    cout << "1. 输入字符串并初始化哈夫曼树" << endl;
    cout << "2. 显示字符频率统计" << endl;
    cout << "3. 显示哈夫曼编码表" << endl;
    cout << "4. 显示哈夫曼树结构" << endl;
    cout << "5. 编码字符串" << endl;
    cout << "6. 解码字符串" << endl;
    cout << "7. 分析压缩效果" << endl;
    cout << "8. 使用测试数据" << endl;
    cout << "9. 二进制编码" << endl;
    cout << "0. 退出" << endl;
    cout << "=====================================" << endl;
    cout << "请选择操作: ";
}

// 主函数
int main() {
    HuffmanTree huffmanTree;
    string inputText = "";
    string encodedText = "";
    
    int choice;
    do {
        displayMenu();
        cin >> choice;
        cin.ignore(); // 清除输入缓冲区
        
        switch (choice) {
            case 1: {
                cout << "请输入字符串: ";
                getline(cin, inputText);
                
                if (inputText.empty()) {
                    cout << "输入不能为空！" << endl;
                    break;
                }
                
                huffmanTree.init(inputText);
                cout << "哈夫曼树初始化完成！" << endl;
                break;
            }
            
            case 2: {
                map<char, int> freq = huffmanTree.getFrequency();
                if (freq.empty()) {
                    cout << "请先初始化哈夫曼树！" << endl;
                    break;
                }
                
                cout << "\n字符频率统计：" << endl;
                cout << "-----------------------------------" << endl;
                for (const auto& pair : freq) {
                    if (pair.first == ' ') {
                        cout << "空格: " << pair.second << " 次" << endl;
                    } else if (pair.first == '\n') {
                        cout << "换行: " << pair.second << " 次" << endl;
                    } else {
                        cout << "'" << pair.first << "': " << pair.second << " 次" << endl;
                    }
                }
                break;
            }
            
            case 3: {
                huffmanTree.printCodeTable();
                break;
            }
            
            case 4: {
                huffmanTree.printTree();
                break;
            }
            
            case 5: {
                if (inputText.empty()) {
                    cout << "请先输入字符串！" << endl;
                    break;
                }
                
                encodedText = huffmanTree.encoding(inputText);
                cout << "\n编码结果: " << encodedText << endl;
                cout << "编码长度: " << encodedText.length() << " 位" << endl;
                break;
            }
            
            case 6: {
                if (encodedText.empty()) {
                    cout << "请先编码字符串！" << endl;
                    break;
                }
                
                string decodedText = huffmanTree.decoding(encodedText);
                cout << "\n解码结果: " << decodedText << endl;
                break;
            }
            
            case 7: {
                if (inputText.empty() || encodedText.empty()) {
                    cout << "请先编码字符串！" << endl;
                    break;
                }
                
                huffmanTree.analyzeCompression(inputText, encodedText);
                break;
            }
            
            case 8: {
                // 使用测试数据
                inputText = "I love data Structure, I love Computer. I will try my best to study data Structure.";
                cout << "使用测试数据: " << inputText << endl;
                
                huffmanTree.init(inputText);
                encodedText = huffmanTree.encoding(inputText);
                
                cout << "\n哈夫曼树已建立！" << endl;
                cout << "编码结果: " << encodedText << endl;
                
                // 测试解码
                string decodedText = huffmanTree.decoding(encodedText);
                cout << "解码结果: " << decodedText << endl;
                cout << "解码是否正确: " << (inputText == decodedText ? "是" : "否") << endl;
                break;
            }
            
            case 9: {
                if (inputText.empty()) {
                    cout << "请先输入字符串！" << endl;
                    break;
                }
                
                string binaryStr = huffmanTree.binaryEncoding(inputText);
                cout << "\n二进制编码结果: " << binaryStr << endl;
                break;
            }
            
            case 0: {
                cout << "感谢使用哈夫曼编/解码器！" << endl;
                break;
            }
            
            default: {
                cout << "无效选择，请重新输入！" << endl;
                break;
            }
        }
        
        cout << endl;
        
    } while (choice != 0);
    
    return 0;
}