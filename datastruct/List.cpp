#include <bits/stdc++.h>
using namespace std;
template <typename T>
class List
{
private:
    struct node
    {
        node *left, *right;
        T data;
        node() { left = right = nullptr; };
        node(T data) : data(data) { left = right = nullptr; };
    } * begin, *end;
    size_t size;

    void push(node *left, node *right, T data)
    {
        size++;
        node *now;
        now = new node(data);
        now->left = left;
        left->right = right->left = now;
        now->right = right;
    }

    void pop(node *left, node *right)
    {
        if (empty())
            return;
        size--;
        left->right = right;
        delete right->left;
        right->left = left;
    }

public:
    List()
    {
        size = 0;
        begin = new node();
        end = new node();
        begin->right = end;
        begin->left = nullptr;
        end->left = begin;
        end->right = nullptr;
    }
    List(int n)
    {
        size = 0;
        begin = new node();
        end = new node();
        begin->right = end;
        begin->left = nullptr;
        end->left = begin;
        end->right = nullptr;
        for (int i = 2; i <= n; i++)
            push_end(i);
    }
    ~List()
    {
        node *now = begin;
        while (now->right != nullptr)
        {
            node *temp = now->right;
            delete now;
            now = temp;
        }
        delete end;
        //cout << "end test:..\n";
    }

    int Size()
    {
        return size;
    }

    void push_begin(T data)
    {
        push(begin, begin->right, data);
    }

    void push_end(T data)
    {
        push(end->left, end, data);
    }

    void pop_begin()
    {
        pop(begin, begin->right->right);
    }

    void pop_end()
    {
        pop(end->left->left, end);
    }

    void pop_n(int n)
    {
        if (n < 1 || n > size)
            return;
        node *temp = begin;
        size--;
        while (n)
        {
            temp = temp->right;
            n--;
        }
        temp->right->left = temp->left;
        temp->left->right = temp->right;
        delete temp;
    }

    T Nth(int n)
    {
        node *temp = begin;
        while (n)
        {
            temp = temp->right;
            n--;
        }
        return temp->data;
    }

    bool empty()
    {
        return (size == 0);
    }

    string print()
    {
        stringstream str;
        node *temp = begin->right;
        int cnt = 1;
        while (temp->right != nullptr)
        {
            cout << "the " << cnt << "th Element is:" << temp->data << '\n';
            temp = temp->right;
            cnt++;
        }
        return str.str();
    }
};
class Filter
{
private:
    int factor;

public:
    Filter(int n)
    {
        factor = n;
        cout << n << ' ';
    }
    ~Filter()
    {
        cout << '\n';
    }

    bool seive(List<int> &sv)
    {
        bool flag = 0;
        int cnt = 2;
        //sv.print();
        while (cnt <= sv.Size())
        {
            if (sv.Nth(cnt) % factor == 0)
                sv.pop_n(cnt), flag = 1, cnt--;
            cnt++;
        }
        return flag;
    }
};
int main()
{
    int n;
    cin >> n;
    List<int> test(n);
    List<int> ans;
    Filter *flt = new Filter(2);
    while (flt->seive(test))
        ans.push_end(test.Nth(1)), test.pop_begin(), flt = new Filter(test.Nth(1));
    while (!test.empty())
        ans.push_end(test.Nth(1)), test.pop_begin();
    ans.print();
}
