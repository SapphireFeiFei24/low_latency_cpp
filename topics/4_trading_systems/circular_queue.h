#pragma once
class MyCircularQueue {
public:
    MyCircularQueue(int k) {
        _data.resize(k);
        capacity = k;
    }

    bool enQueue(int value) {
        if (size == capacity) {
            return false;
        }
        int idx = (head + size) % capacity;
        _data[idx] = value;
        size += 1;
        return true;
    }

    bool deQueue() {
        if (size == 0) {
            return false;
        }
        head = (head + 1) % capacity;
        size -= 1;
        return true;
    }

    int Front() {
        if (size == 0){
            return -1;
        }
        return _data[head];
    }

    int Rear() {
        if (size == 0){
            return -1;
        }
        int idx = (head + size - 1) % capacity;
        return _data[idx];
    }

    bool isEmpty() {
        return size == 0;
    }

    bool isFull() {
        return size == capacity;
    }
private:
    std::vector<int> _data;
    int head = 0;
    int size = 0;
    int capacity = 0;
};

/**
 * Your MyCircularQueue object will be instantiated and called as such:
 * MyCircularQueue* obj = new MyCircularQueue(k);
 * bool param_1 = obj->enQueue(value);
 * bool param_2 = obj->deQueue();
 * int param_3 = obj->Front();
 * int param_4 = obj->Rear();
 * bool param_5 = obj->isEmpty();
 * bool param_6 = obj->isFull();
 */