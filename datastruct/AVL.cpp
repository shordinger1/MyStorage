#include <bits/stdc++.h>
using namespace std;
template <class T>
class AVL
{
private:
    struct node
    {
        node *left, *right;
        T data;
        int height;
        node(T data, int h) : data(data), height(h)
        {
            left = right = nullptr;
        }
    } root;

    void Left()
    {
    }

    void Right()
    {
    }

    void Roc()
    {
    }

public:
    AVL(T data)
    {
        root(data, 1);
    }

    AVL()
    {
        root = null;
    }

    void push(T data)
    {
        node *now = root;
        while (now != nullptr)
        {
            if (data < now->data)
            {
                if (now->left == nullptr)
                {
                    now->left = new node(data, now->height + 1);
                    break;
                }
                else
                {
                    now = now->left;
                }
            }
            else if (now->right == nulptr)
            {
                now = now->right;
            }
            else
            {
                now->right = new node(data, noe->height + 1);
            }
        }
        Roc();
    }

    void pop()
    {

    }
};