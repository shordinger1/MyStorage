#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<ll, ll> P;
typedef complex<long double> cp;
const double pi = acos(-1);
ll tot = 0;
class bignum
{
public:

    bignum();
    bignum(double n);
    bignum(int n);
    bignum(ll n);
    bignum(string n);
    bignum(const bignum &n);
    ~bignum() { val.clear(); }

    void operator=(const bignum &T);
    bignum operator+(const bignum &T) const;
    bignum operator-(const bignum &T) const;
    bignum operator*(const bignum &T) const;
    bignum operator/(const bignum &T) const;
    bignum operator%(const bignum &T) const;
    bignum operator^(const bignum &T) const;
    void operator++(int);
    void operator++();
    void operator--(int);
    void operator--();
    void operator+=(const bignum &T);
    void operator-=(const bignum &T);
    template <typename T>
    void operator+=(const T &t);
    template <typename T>
    void operator-=(const T &t);
    template <typename T>
    bignum operator+(const T &t) const;
    template <typename T>
    bignum operator-(const T &t) const;
    template <typename T>
    bignum operator*(const T &t) const;
    template <typename T>
    bignum operator/(const T &t) const;
    template <typename T>
    bignum operator%(const T &t) const;
    template <typename T>
    bignum operator^(const T &t) const;
    bignum sqrt() const;

    bool operator==(const bignum &T) const;
    bool operator>=(const bignum &T) const;
    bool operator<=(const bignum &T) const;
    bool operator>(const bignum &T) const;
    bool operator<(const bignum &T) const;
    template <typename T>
    bool operator==(const T &t) const;
    template <typename T>
    bool operator>=(const T &t) const;
    template <typename T>
    bool operator<=(const T &t) const;
    template <typename T>
    bool operator>(const T &t) const;
    template <typename T>
    bool operator<(const T &t) const;

    friend istream &operator>>(istream &in, bignum &T);
    friend ostream &operator<<(ostream &out, const bignum &T);
    friend bignum abs(const bignum &T);

private:
    const static ll len = 3;
    const static ll Mod = 1e3;
    vector<ll> val;
    bool minus;
    ll length() const;
    ll size() const;
    string toString() const;
};

bignum abs(const bignum &T)
{
    bignum res(T);
    res.minus = 0;
    return res;
}

bignum::bignum()
{
    minus = false;
}

bignum::bignum(int n)
{
    minus = 0;
    if (n == 0)
    {
        val.push_back(0);
        minus = 0;
        return;
    }
    if (n < 0)
        minus = 1, n = -n;
    while (n)
    {
        val.push_back(n % Mod);
        n /= Mod;
    }
}

bignum::bignum(ll n)
{
    minus = 0;
    if (n == 0)
    {
        val.push_back(0);
        minus = 0;
        return;
    }
    if (n < 0)
        minus = 1, n = -n;
    while (n)
    {
        val.push_back(n % Mod);
        n /= Mod;
    }
}
ll trance(string v)
{
    reverse(v.begin(), v.end());
    ll res = 0;
    for (auto it : v)
    {
        res = res * 10 + ll(it - '0');
    }
    return res;
}
bignum::bignum(string n)
{
    if (n.empty())
    {
        val.push_back(0);
        minus = 0;
        return;
    }
    if (n[0] == '-')
        minus = true, n = n.substr(1);
    else
        minus = false;
    reverse(n.begin(), n.end());
    for (int it = 0; it < n.length(); it += len)
    {
        val.push_back(trance(n.substr(it, len)));
    }
}

bignum::bignum(double t)
{
    minus = 0;
    ll n = t;
    if (n == 0)
        val.push_back(0);
    if (n < 0)
        minus = 1, n = -n;
    while (n)
    {
        val.push_back(n % Mod);
        n /= Mod;
    }
}

bignum::bignum(const bignum &t)
{
    minus = t.minus;
    val = t.val;
}

void bignum::operator=(const bignum &t)
{
    minus = t.minus;
    val = t.val;
}
bignum bignum::operator+(const bignum &num) const
{
    return (*this) - (bignum(0) - num);
}
bignum bignum::operator-(const bignum &num) const
{
    ll temp = 0;
    bignum res;
    if (*this == 0)
    {
        res = num;
        res.minus = !res.minus;
        return res;
    }
    else if (num == 0)
        return (*this);
    bignum a(*this), b(num);
    if (a < b)
        swap(a, b), res.minus = 1;
    else
        res.minus = 0;
    bool low = false;
    for (int i = 0; i < max(a.length(), b.length()); i++)
    {
        ll ans = low ? -temp : temp;
        if (i < a.length())
            ans += a.minus ? -a.val[i] : a.val[i];
        if (i < b.length())
            ans -= b.minus ? -b.val[i] : b.val[i];
        low = (ans < 0 ? 1 : 0);
        ans = abs(ans);
        res.val.push_back(low ? (Mod - ans % Mod) : (ans % Mod));
        temp = low ? (ans / Mod + 1) : (ans / Mod);
    }
    while (temp)
    {
        if (temp == Mod / 10 && low == 1)
            break;
        res.val.push_back(low ? (Mod - temp % Mod) : (temp % Mod));
        temp = low ? (temp / Mod + 1) : (temp / Mod);
    }
    while (*(res.val.end() - 1) == 0)
        res.val.pop_back();
    if (res.length() == 0)
        return bignum(0);
    return res;
}
ll v;
ll rev[1 << 21];
cp p[1 << 21], q[1 << 21];
void fft(cp *a, ll inv)
{
    for (int i = 0; i < v; i++)
    {
        if (i < rev[i])
            swap(a[i], a[rev[i]]);
    }
    for (ll mid = 1; mid < v; mid *= 2)
    {
        cp temp(cos(pi / mid), inv * sin(pi / mid));
        for (int i = 0; i < v; i += mid * 2)
        {
            cp omega(1, 0);
            for (int j = 0; j < mid; j++, omega *= temp)
            {
                cp x = a[i + j], y = omega * a[i + j + mid];
                a[i + j] = x + y, a[i + j + mid] = x - y;
            }
        }
    }
}
bignum bignum::operator*(const bignum &num) const
{
    bignum ans, a(*this), b(num);
    ans.minus = minus ^ num.minus;
    int n = a.length() - 1, m = b.length() - 1;
    v = 2;
    while (v <= n + m)
        v *= 2;
    ll bit = 0;
    while ((1 << bit) < v)
        bit++;
    for (int i = 0; i < v; i++)
    {
        rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (bit - 1));
    }
    for (int i = 0; i <= n; i++)
    {
        p[i] = a.val[i];
    }
    for (int i = n + 1; i < v; i++)
        p[i] = 0;
    for (int i = 0; i <= m; i++)
    {
        q[i] = b.val[i];
    }
    for (int i = m + 1; i < v; i++)
        q[i] = 0;
    fft(p, 1);
    fft(q, 1);
    for (int i = 0; i < v; i++)
        p[i] *= q[i];
    fft(p, -1);
    ll temp = 0;
    for (int i = 0; i <= n + m; i++)
    {
        ll now = ll(p[i].real() / v + 0.5) + temp;
        ans.val.push_back(now % Mod);
        temp = now / Mod;
    }
    while (temp)
        ans.val.push_back(temp % Mod), temp /= Mod;
    return ans;
}
bignum bignum::operator/(const bignum &num) const
{
    bignum n = abs(num), m = abs(*this), res(0);
    if (abs(*this) < abs(num))
        return bignum(0);
    if (n == 0)
    {
        cout << "div 0 exception\n";
        return num;
    }
    else if (n == 1)
    {
        n = (*this);
        n.minus = minus ^ num.minus;
        return n;
    }
    else if (n == 2)
    {
        bool flag = false;
        for (auto it = val.rbegin(); it < val.rend(); it++)
        {
            ll now = (*it) + Mod * flag;
            res.val.push_back(now / 2);
            flag = now & 1;
        }
        reverse(res.val.begin(), res.val.end());
        n.minus = minus ^ num.minus;
        return res;
    }
    else if (n == 10)
    {
        string v = m.toString();
        v.pop_back();
        if (minus ^ num.minus)
            v = '-' + v;
        return bignum(v);
    }
    bignum temp(1);
    //cout << m << '\n';
    bignum len = max(0ll, m.size() - n.size() - 1);
    temp = bignum(10) ^ len;
    n = n * temp;
    while (m >= n * 10)
        n = n * 10, temp = temp * 10;
    while (m >= num)
    {
        cout << m << '\n';
        while (m >= n)
            m = m - n, res = res + temp;
        n = n / 10;
        temp = temp / 10;
    }
    res.minus = minus ^ num.minus;
    return res;
}
bignum bignum::operator%(const bignum &num) const
{
    bignum a = abs(*this), b = abs(num);
    bignum res = a - ((a / b) * b);
    res.minus = minus ^ num.minus;
    return res;
}
bignum bignum::operator^(const bignum &num) const
{
    bignum res(1), a(*this), n(num);
    if (n < 0)
    {
        cout << "Unexpected input\n";
        return res;
    }
    while (n > 0)
    {
        //cout << n << ' ' << a << '\n';
        if (n.val[0] & 1)
            res = res * a;
        a = a * a;
        n = n / 2;
    }
    return res;
}
template <typename T>
bignum bignum::operator+(const T &t) const
{
    bignum num(t);
    return (*this) + num;
}
template <typename T>
bignum bignum::operator-(const T &t) const
{
    bignum num(t);
    return (*this) - num;
}
template <typename T>
bignum bignum::operator*(const T &t) const
{
    bignum num(t);
    return (*this) * num;
}
template <typename T>
bignum bignum::operator/(const T &t) const
{
    bignum num(t);
    return (*this) / num;
}
template <typename T>
bignum bignum::operator%(const T &t) const
{
    //cout << toString() << ' ' << t << '\n';
    bignum num(t);
    return (*this) % num;
}
template <typename T>
bignum bignum::operator^(const T &t) const
{
    bignum num(t);
    return (*this) ^ num;
}
bignum bignum::sqrt() const
{
    bignum num(*this);
    bignum res, l(1), r = num;
    if (num < 0)
    {
        cout << "Unexpected input\n";
    }
    while (l < r)
    {
        bignum mid((l + r) / 2);
        //cout << mid << '\n';
        if ((mid * mid) > num)
            r = mid - 1;
        else if ((mid * mid) == num)
            return mid;
        else
            l = mid + 1;
    }
    if ((l * l) > num)
        l = l - 1;
    return l;
}
bool bignum::operator==(const bignum &num) const
{
    if ((num.length() == 1) && (length() == 1) && num.val[0] == 0 && val[0] == 0)
        return 1;
    return toString() == num.toString();
}
bool bignum::operator>=(const bignum &num) const
{
    if (!minus)
    {
        if (num.minus)
            return 1;
        if (num.size() < size())
            return 1;
        else if (num.size() > size())
            return 0;
    }
    else
    {
        if (!num.minus)
            return 0;
        if (num.size() < size())
            return 0;
        else if (num.size() > size())
            return 1;
    }
    if (toString() == num.toString())
        return 1;
    return !((abs(*this).toString() > abs(num).toString()) ^ !minus);
}
bool bignum::operator<=(const bignum &num) const
{
    return (*this) < num || (*this) == num;
}
bool bignum::operator>(const bignum &num) const
{
    return !((*this) <= num);
}
bool bignum::operator<(const bignum &num) const
{
    return !((*this) >= num);
}
template <typename T>
bool bignum::operator==(const T &t) const
{
    bignum num(t);
    return (*this) == num;
}
template <typename T>
bool bignum::operator>=(const T &t) const
{
    bignum num(t);
    return (*this) >= num;
}
template <typename T>
bool bignum::operator<=(const T &t) const
{
    bignum num(t);
    return (*this) <= num;
}
template <typename T>
bool bignum::operator>(const T &t) const
{
    bignum num(t);
    return (*this) > num;
}
template <typename T>
bool bignum::operator<(const T &t) const
{
    bignum num(t);
    return (*this) < num;
}

void bignum::operator++(int)
{
    (*this) = (*this) + 1;
}
void bignum::operator++()
{
    (*this) = (*this) + 1;
}
void bignum::operator--(int)
{
    (*this) = (*this) - 1;
}
void bignum::operator--()
{
    (*this) = (*this) - 1;
}
void bignum::operator+=(const bignum &T)
{
    (*this) = (*this) + T;
}
void bignum::operator-=(const bignum &T)
{
    (*this) = (*this) - T;
}
template <typename T>
void bignum::operator+=(const T &t)
{
    (*this) = (*this) + bignum(t);
}
template <typename T>
void bignum::operator-=(const T &t)
{
    (*this) = (*this) - bignum(t);
}

istream &operator>>(istream &in, bignum &t)
{
    string v;
    in >> v;
    t = bignum(v);
    return in;
}
ostream &operator<<(ostream &out, const bignum &t)
{
    out << t.toString();
    return out;
}

ll bignum::length() const
{
    return val.size();
}

ll bignum::size() const
{
    return abs(*this).toString().size();
}

string bignum::toString() const
{
    string ans;
    if (val.size() == 0)
        return "0";
    //ll t=clock();
    for (auto it : val)
    {
        ll tot = 0;
        while (it)
        {
            ans.push_back(char(it % 10 + '0'));
            it /= 10;
            tot++;
        }
        while (tot < len)
            ans.push_back('0'), tot++;
    }
    while (*(ans.end() - 1) == '0')
        ans.pop_back();
    reverse(ans.begin(), ans.end());
    if (minus)
        ans = '-' + ans;
    if (ans.length() == 0)
        return "0";
    return ans;
}
void File_reader()
{
    ios::sync_with_stdio(false);
    cout.tie(NULL);
    cin.tie(NULL);
}
bignum jiecheng(ll l, ll r)
{
    //cout << l << ' ' << r << '\n';
    if (l == r)
        return l;
    ll mid = (l + r) >> 1;
    //cout << mid << ' ';
    return jiecheng(l, mid) * jiecheng(mid + 1, r);
}
