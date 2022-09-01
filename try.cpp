#include <bits/stdc++.h>
using namespace std;
int p[2][4] = {{150000, 90000, 75000, 45000}, {120000, 60000, 45000, 22500}};
int bonus[2] = {200, 100};
int res[2][5][51];
int main()
{
    system("chcp 65001");
    for (int x = 0; x <= 1; x++)
    {
        cout << (x == 0 ? "星门" : "其他") << '\n'<<"人数 ";
        for (int j = 100; j <= 150; j++)
            printf("%5d%s", j, (j == 150) ? "\n" : "");
        for (int i = 1; i <= 4; i++)
        {
            cout << "第" << i << "档";
            for (int j = 100; j <= 150; j++)
            {
                res[x][i][j - 100] = p[x][i - 1] / j + bonus[x];
                printf("%5d%s", res[x][i][j - 100], (j == 150) ? "\n" : "");
            }
        }
    }
}