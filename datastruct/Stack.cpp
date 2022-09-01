#include <bits/stdc++.h>
using namespace std;
template <class T>
class Stack
{
private:
    struct node
    {
        node *father;
        T data;
        node() { father = nullptr; };
        node(T data) : data(data) { father = nullptr; };
    } * button;
    size_t size;

public:
    Stack()
    {
        button = new node();
        size = 0;
    }
    void push(T data)
    {
        size++;
        node *now = new node(data);
        now->father = button;
        button = now;
    }

    void pop()
    {
        if(size==0)
            return;
        size--;
        node *temp = button;
        button = button->father;
        delete temp;
    }
};