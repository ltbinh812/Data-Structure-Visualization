#include <iostream>
#include <vector>

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

struct LinkedList {
    Node* head = nullptr;

    void Initialize(std::vector<int> values) {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }

        if (values.empty()) return;
        head = new Node(values[0]);
        Node* curr = head;
        for (size_t i = 1; i < values.size(); ++i) {
            curr->next = new Node(values[i]);
            curr = curr->next;
        }
    }

    void Insert(int val, int pos) {
        if(pos < 0) return;
        Node* newNode = new Node(val);

        if (pos == 0 || !head) {
            newNode->next = head;
            head = newNode;
            return;
        }

        Node* curr = head;
        for (int i = 0; i < pos - 1 && curr->next; ++i) {
            curr = curr->next;
        }

        newNode->next = curr->next;
        curr->next = newNode;
    }

    void DeleteAt(int pos) {
        if (pos < 0 || !head) return;

        if (pos == 0) {
            Node* temp = head;
            head = head->next;
            delete temp;
            return;
        }

        Node* curr = head;
        for (int i = 0; i < pos - 1 && curr->next; ++i) {
            curr = curr->next;
        }

        if (!curr->next) return;

        Node* target = curr->next;
        curr->next = target->next;
        delete target;
    }

    void DeleteKey(int key) {
        if (!head) return;
        if (head->data == key) {
            Node* temp = head;
            head = head->next;
            delete temp;
            return;
        }
        Node* curr = head;
        while (curr->next && curr->next->data != key) 
            curr = curr->next;
        
        if (curr->next) {
            Node* target = curr->next;
            curr->next = target->next;
            delete target;
        }
    }

    void Update(int oldVal, int newVal) {
        Node* curr = head;
        while (curr) {
            if (curr->data == oldVal) {
                curr->data = newVal;
                return;
            }
            curr = curr->next;
        }
    }

    Node* Search(int key) {
        Node* curr = head;
        while (curr) {
            if (curr->data == key) return curr;
            curr = curr->next;
        }
        return nullptr;
    }
};