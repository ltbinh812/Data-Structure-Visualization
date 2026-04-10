#include <string>
#include <unordered_map>
#include <vector>

struct TrieNode {
    std::unordered_map<char, TrieNode*> children;
    int countEnd = 0;    
    int countPrefix = 0; 
};

class Trie {
private:
    TrieNode* root;
    void clearMemory(TrieNode* node) {
        if (!node) return; 
        TrieNode* temp = node;
        delete node;
        for (auto& pair : temp->children) clearMemory(pair.second);
    }

public:
    void Clear() {
        root = new TrieNode();
    }

    void Initialize(const std::vector<std::string>& words) {
        root = new TrieNode();
        for (const std::string& word : words) Insert(word);
    }

    void Insert(const std::string& word) {
        TrieNode* curr = root;
        for (char c : word) {
            if (curr->children.find(c) == curr->children.end()) {
                curr->children[c] = new TrieNode();
            }
            curr = curr->children[c];
            curr->countPrefix++; 
        }
        curr->countEnd++; 
    }

    bool Search(const std::string& word) {
        TrieNode* curr = root;
        for (char c : word) {
            if (curr->children.find(c) == curr->children.end()) return false;
            curr = curr->children[c];
        }
        return curr->countEnd > 0; 
    }

    void Delete(const std::string& word) {
        if (!Search(word)) return; 

        TrieNode* curr = root;
        for (char c : word) {
            TrieNode* nextNode = curr->children[c];
            nextNode->countPrefix--; 

            if (nextNode->countPrefix == 0) {
                clearMemory(nextNode); 
                curr->children.erase(c);
                return; 
            }
            curr = nextNode;
        }
        curr->countEnd--; 
    }
};