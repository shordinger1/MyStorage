#include <bits/stdc++.h>
using namespace std;
int main()
{
    int t;
    cin >> t;
    while (t--)
    {
        long long n, m;
        cin >> n >> m;
        if (m == 1)
            cout << n + 1 << '\n';
        else
            cout << 3 << '\n';
    }
}