/*
......................阿弥陀佛......................
                       _oo0oo_                     
                      o8888888o                    
                     88\" . \"88                  
                      (| -_- |)                    
                      0\  =  /0                   
                   ___/‘---’\___                  
                  .' \|       |/ '.               
                 / \\|||  :  |||// \            
                / _||||| -卍-|||||_ \             
               |   | \\\  -  /// |   |          
               | \_|  ''\---/''  |_/ |           
               \  .-\__  '-'  ___/-. /           
             ___'. .'  /--.--\  '. .'___          
         .\"" ‘<  ‘.___\_<|>_/___.’>’ \"".      
       | | :  ‘- \‘.;‘\ _ /’;.’/ - ’ : | |       
         \  \ ‘_.   \_ __\ /__ _/   .-’ /  /   
    =====‘-.____‘.___ \_____/___.-’___.-’=====    
                       ‘=---=’                     
....................佛祖保佑 ,永无BUG...............
*/
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<ll, ll> P;
const ll inf = 0x3f3f3f3f;
const int mod = 1e9 + 7;
void File_reader()
{
    ios::sync_with_stdio(false);
    cout.tie(NULL);
    cin.tie(NULL);
    freopen("in.txt", "w", stdout);
}
int main()
{
    srand(time(NULL));
    int t = 1000;
    File_reader();
    //cout << 1000 << '\n';
    //while (t--)
    //{
    int n = rand() % 20 + 1, m = rand() % 20 + 1;
    if (n < m)
        swap(n, m);
    cout << n << ' ' << m << '\n';
    for (int i = 1; i <= n; i++)
    {
        cout << rand() % 100 + 1 << ' ';
    }
    cout << '\n';
    for (int i = 1; i <= n; i++)
    {
        cout << rand() % 100 + 1 << ' ';
    }
    cout << '\n';
    for (int i = 1; i <= m; i++)
    {
        cout << rand() % 200 + 1 << ' ';
    }
    cout << '\n';
    //}
}