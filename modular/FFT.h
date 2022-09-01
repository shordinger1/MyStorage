#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<ll, ll> P;
typedef complex<double> cp;
const ll inf = 0x3f3f3f3f;
const int mod = 1e9 + 7;
class FFT
{
  const double pi = acos(-1);
  const static int maxn = 2e6 + 5;
  int n, m, v;
  int rev[maxn + maxn & (-maxn) + 10], res[maxn + maxn & (-maxn) + 10];
  cp p[maxn + maxn & (-maxn) + 10], q[maxn + maxn & (-maxn) + 10];
  public:
  FFT(int n, int m) : n(n), m(m)
  {
    v = 2;
    while (v < n + m)
      v *= 2;
    memset(rev, 0, sizeof(rev));
    memset(res, 0, sizeof(res));
    memset(p, 0, sizeof(p));
    memset(q, 0, sizeof(q));
  }

  void input()
  {
    for (int i = 0; i <= n;i++)
      cin >> p[i];
    for (int i = 0; i <= m;i++)
      cin >> q[i];
  }

  void display()
  {
    for (int i = 0; i <= n + m;i++)
    {
      cout << res[i] << ' ';
    }
    cout << endl;
  }

  void fft(cp *a, int n, int inv)
  {
    int bit = 0;
    while ((1 << bit) < n)
      bit++;
    for (int i = 0; i < n; i++)
    {
      rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (bit - 1));
      if (i < rev[i])
        swap(a[i], a[rev[i]]); //不加这条if会交换两次（就是没交换）
    }
    for (int mid = 1; mid < n; mid *= 2) //mid是准备合并序列的长度的二分之一
    {
      cp temp(cos(pi / mid), inv * sin(pi / mid)); //单位根，pi的系数2已经约掉了
      for (int i = 0; i < n; i += mid * 2)         //mid*2是准备合并序列的长度，i是合并到了哪一位
      {
        cp omega(1, 0);
        for (int j = 0; j < mid; j++, omega *= temp) //只扫左半部分，得到右半部分的答案
        {
          cp x = a[i + j], y = omega * a[i + j + mid];
          a[i + j] = x + y, a[i + j + mid] = x - y; //这个就是蝴蝶变换什么的
        }
      }
    }
  }
  void multi()
  {
    fft(p, v, 1);
    fft(q, v, 1);
    for (int i = 0; i < v; i++)
      p[i] *= q[i];
    fft(p, v, -1);
    for (int i = 0; i < v; i++)
      res[i] = (int)(p[i].real() / v + 0.5);
  }
};