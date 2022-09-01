#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
ll fac[17];
int main()
{
    fac[0] = 1;
    for (int i = 1; i <= 17; i++)
        fac[i] = fac[i - 1] * i;
    for (int r1 = 1; r1 <= 17; r1++)
    {
        for (int l1 = 0; l1 < r1; l1++)
        {
            for (int r2 = 1; r2 <= 17; r2++)
            {
                for (int l2 = 0; l2 < r1; l2++)
                {
                    if((fac[r2]*fac[l1])%(fac[r1]*fac[l2])==0)
                        cout << r2 << ' ' << r1 << ' ' << l1 + 1 << ' ' << l2 + 1 << '\n';
                }
            }
        }
    }
}
