#include <vector>
#include <algorithm>

class Heap {
private:
    std::vector<int> data;
    bool isMinHeap;

    bool compare(int a, int b) {
        return isMinHeap ? (a < b) : (a > b);
    }

    void HeapifyUp(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (compare(data[i], data[p])) {
                std::swap(data[i], data[p]);
                i = p;
            } else break;
        }
    }

    void HeapifyDown(int i) {
        int n = data.size();
        while (true) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int best = i;

            if (left < n && compare(data[left], data[best])) 
                best = left;
            if (right < n && compare(data[right], data[best])) 
                best = right;

            if (best != i) {
                std::swap(data[i], data[best]);
                i = best;
            } else break;
        }
    }

    void BuildHeap() {
        int n = data.size();
        for (int i = n / 2 - 1; i >= 0; i--) {
            HeapifyDown(i);
        }
    }

public:
    void Initialize(const std::vector<int>& arr, bool minHeap = true) {
        data = arr;
        isMinHeap = minHeap;
        BuildHeap(); 
    }

    void Insert(int val) {
        data.push_back(val);
        HeapifyUp(data.size() - 1);
    }

    void Update(int i, int val) {
        if (i < 0 || i >= data.size()) return;
        int oldVal = data[i];
        data[i] = val;
        
        if (compare(val, oldVal)) HeapifyUp(i);
        else HeapifyDown(i);
    }

    int Get() {
        if (data.empty()) return -1;
        int top = data[0];
        data[0] = data.back();
        data.pop_back();
        if (!data.empty()) HeapifyDown(0);
        return top;
    }
};