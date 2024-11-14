#include "../../Server/Set/Heap/Heap.hpp"
#include "../../Server/Set/HashMap/HashTable.hpp"
#include <assert.h>
#include <vector>
#include <iostream>
#include <random>

using std::cin;
using std::cout;

int main() {

    std::mt19937 gen(3463);
    std::uniform_int_distribution dist(1, 100);

    // Test insert heap -->insert 5 nodes

    std::vector<HNode> vec(10);
    Heap heap;
    for (int i = 0; i < 10; ++i) {
        int a = dist(gen);
        vec[i] = HNode("key", 1, 1);
        heap.insertHeap(&vec[i], a);
    }

    for (int i = 0; i < 10; ++i) {
        cout << heap[i].expire << " ";
        // cout << heap[i].hnode->pos_ << " \n ";
    }


    cout << "+++++++++++++++++++++++++\n\n";

    // Test Update heap --> change 2 nodes 

    cout << heap[0].expire << "\n";
    cout << heap[9].expire << "\n";

    heap.updateHeap(9, 0);
    heap.updateHeap(1, 1000);

    cout << heap[0].expire << "\n";
    cout << heap[9].expire << "\n";

    // Test del heap --> del 3 nodes

    heap.delHeap();

    cout << heap[0].expire << "\n";

    for (int i = 0; i < 9; ++i) {
        cout << heap[i].expire << " ";
        cout << heap[i].hnode->pos_ << " \n";
    }

    // Test Heap empty

    cout << "\n" <<  heap.empty() << "\n";

    std::vector<int> arr;
    for (int i = 0; i < 9; ++i) {
        arr.push_back(heap[0].expire);
        HNode* node = heap.delHeap();
        if (i != 0) {
            assert(arr[i - 1] <= arr[i]);
        }
    }


    cout << heap.empty() << "\n";
}