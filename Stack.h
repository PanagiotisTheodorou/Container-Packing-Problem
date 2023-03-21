#pragma once

#include <iostream>

#define MAX_SIZE 100
using std::cout;

template <typename T>
struct Stack {
    T _data[MAX_SIZE];
    unsigned int _top = 0;

    bool isEmpty() {
        return _top == 0;
    }

    bool isFull() {
        return _top == MAX_SIZE;
    }

    void push(T newVal) {
        if (!isFull()) {
            _data[_top++] = newVal;
        }
    }

    void pop() {
        if (!isEmpty()) {
            _top--;
        }
    }

    T top() {
        if (!isEmpty()) {
            return _data[_top - 1];
        }
    }

    void print() {
        if (!isEmpty()) {
            cout << "Printing stack from the top\n";
            for (int i = _top - 1; i >= 0; i--) {
                cout << _data[i] << "\n";
            }
            cout << "-----------\n\n";
        }
        else cout << "Stack is empty\n\n";
    }

};