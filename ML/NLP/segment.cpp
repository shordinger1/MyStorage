/*
@param 
author:"包淳嘉"
time:2021/4/14
A model used for NLP problem in financial system inorder to split the sentence and sign the main word 

group member:bao chunjia,huang qixuan,zhou xin,wu ke
teacher:zhao xiaomin

*/

#include <bits/stdc++.h>
#define _CSI_ const static int
#define _CSD_ const static double
using namespace std;
typedef long long ll;
typedef pair<ll, ll> Pi;
typedef pair<ll, double> Pd;
typedef vector<pair<ll, vector<Pd>>> Graph;
typedef vector<ll> Vec;
class Segment
{
public:
    //_CSI_ max_word_len = 20;        //最大词长度
    //_CSI_ max_sentence_len = 505;   //最大句子长度
    _CSI_ max_word_count = 1e5 + 5; //最大汉字数量
    //_CSI_ debug_mode = 0;
    //_CSI_ hash_code = 92083; //hash乘数
    //_CSI_ hash_mod = 517619; //hash膜数
    // _CSD_ eps = 1e-5;
    bool trained = 0;
    map<ll, int> word_set;    //存储单个汉字对应的离散化下标
    map<ll, string> word_map; //存储单个汉字的utf8对应的汉字本身
    ll word_count = 0;
    //double word_cnt[max_word_count]; //出现的词频率

    //以下为HMM专用
    //[0=>'B',1=>'M',2=>'E',3=>'S'];
    double trans[4][4];
    double emit[max_word_count][4];
    double observe[4];
    bool rev;

    //ofstream cout;

    struct Trie
    {
        ll word;  //UTF-8编码值
        int deep; //词处for的位置
        int cnt;  //词频
        map<ll, Trie *> nxt;
        map<ll, bool> exist;
        Trie() : word(-1), deep(-1), cnt(-1) { nxt.clear(); }
        Trie(ll word, ll deep) : word(word), deep(deep), cnt(0) { nxt.clear(); }
    } *root;

    //~Segment()
    //{
    //    for (auto it : word_map)
    //        cout << it.first << ' ' << it.second << '\n';
    //}

    //保存trie树
    void save_Trie(string curr, Trie *forward)
    {
        ofstream ofs("trie.out", ios::app);

        for (auto i : forward->nxt)
        {
            Trie *temp = i.second;
            string CStr = curr + word_map[temp->word];
            //curr = curr + CStr;
            if (temp->cnt > 0)
            {
                ofs << CStr << "\t" << temp->cnt << "次" << endl;
                //cout << curr << endl;
            }
            ofs.flush();
            ofs.close();
            save_Trie(CStr, temp);
        }
        return;
    }

    //为了贯彻爱与和平，防止邪恶的编码问题阻止我们前进，我们决定把他们全部变成ll。
    Vec trance_to_ll(string str)
    {
        Vec res;
        ll num = str.size();
        ll i = 0;
        //cout << str << '\n';
        while (i < num)
        {
            ll size = 1;
            //cout << bitset<32>((0xff)&int(str[i])) << ' ';
            ll ptr = (0xff) & ll(str[i]);
            if (str[i] & 0x80)
            {
                char temp = str[i];
                temp <<= 1;
                do
                {
                    temp <<= 1;
                    i++;
                    size++;
                    ptr <<= 8;
                    ptr |= (0xff) & ll(str[i]);
                    //cout << ((0xff)&int(str[i])) << ' ';
                } while (temp & 0x80);
            }
            //cout << ptr << '\n';
            word_map[ptr] = str.substr(i - size + 1, size);
            if (word_set[ptr] == 0)
            {
                word_count++;
                word_set[ptr] = word_count;
            }
            res.push_back(ptr);
            i++;
        }
        if (rev)
            reverse(res.begin(), res.end());
        return res;
    }

    //获得已登录的词语的有向无环图
    Graph get_graph(Vec graph)
    {
        //cout << "start getting:\n";
        ll len = graph.size();
        Graph res(len);
        string str;
        for (ll i = 0; i < len; i++)
        {
            pair<ll, vector<Pd>> v(graph[i],vector<Pd>());
            ll ptr = graph[i];
            Trie *now = root;
            //str.append(word_map[ptr]);

            //cout << i << ' ';
            ll tot = 0;
            ll _ptr = i;
            if (!now->exist[ptr])
            {
                res[i]=v;
                v.second.clear();
                continue;
            }
            while (now->exist[ptr])
            {
                now = now->nxt[ptr];
                tot += now->cnt;
                _ptr++;
                if(_ptr==len)
                    break;
                ptr = graph[_ptr];
            }
            ptr = graph[i];
            now = root->nxt[ptr];
            //cout << tot << ' ';
            _ptr = i;
            while (1)
            {
                if (now->cnt > 0)
                {
                    v.second.push_back({_ptr, log10((1.0 * now->cnt) / (1.0 * tot))});
                    //cout << str << ':' << (1.0 * now->cnt) / (1.0 * tot) << ' ';
                }
                _ptr++;
                if(_ptr==len)
                    break;
                ptr = graph[_ptr];
                if (!now->exist[ptr])
                    break;
                now = now->nxt[ptr];
                //str.append(word_map[ptr]);
            }
            //cout << '\n';
            //str = "";
            res[i]=v;
            v.second.clear();
        }
        //cout << "end getting...\n";
        return res;
    }

    /*
    @param:
    graph: 有向无环图，也就是存for的词语的词语拓扑路线
    len:句子的长度 
    */
    Vec dp(Graph graph, ll len)
    {
        bool *included = new bool[len + 1]; //判断是否已经登陆
        Pd *path = new Pd[len + 1];         //存储前导位置和while前概率
        Vec temp;
        for (int i = 0; i < len; i++)
        {
            path[i].second = -1e9;
            included[i] = 0;
        }
        for (ll i = 0; i < len; i++)
        {
            //cout << i << ':' << word_map[graph[i].first] << ' ';
            if (!included[i] && !path[i].first && (!graph[i].second.size() || (graph[i].second.size() == 1 && graph[i].second[0].first == i)))
            {
                temp.push_back(word_set[graph[i].first]);
                //cout << word_map[graph[i].first];
                //cout << '\n';
                continue;
            }
            if (!temp.empty())
            {
                //cout << ":\n";
                Vec _path = HMM(temp);
                //cout << "!";
                ll pre = i - temp.size() - 1;
                for (ll j = 0; j < _path.size(); j++)
                {
                    //cout<<j<<':'<<_path[j]<<' ';
                    int now = pre + _path[j];
                    path[now] = Pd(pre, 0);
                    pre += _path[j];
                }
                //cout << '\n';
                path[i] = Pd(pre, 0);
                temp.clear();
            }
            if (graph[i].second.size() == 0)
                continue;
            for (auto it : graph[i].second)
            {
                //cout << it.first << ' ';
                ll to = it.first;
                if (path[i].second < -1e5)
                {
                    path[to] = Pd(i - 1, it.second);
                }
                else if (path[to].second < path[i].second + it.second) //由于对数函数是单调递增的，因此可以视作原来的单调性
                {
                    path[to] = Pd(i - 1, path[i].second + it.second); //取对数之后求和即可，防止精度溢出.
                }
            }
            //cout << '\n';
            //cout << path[i].first << ' ';
            for (ll j = i; j <= (*(graph[i].second.end() - 1)).first; j++)
                included[j] = 1;
        }
        if (!temp.empty())
        {
            //cout << ":\n";
            Vec _path = HMM(temp);
            ll pre = len - temp.size() - 1;
            for (ll j = 0; j < _path.size(); j++)
            {
                //cout<<j<<':'<<_path[j]<<' ';
                int now = pre + _path[j];
                path[now] = Pd(pre, 0);
                pre += _path[j];
            }
            //cout<<'\n';
            temp.clear();
        }
        Vec ans;
        ans.push_back(len - 1);
        ll ptr = len - 1;
        while (ptr >= 0)
        {
            ptr = path[ptr].first;
            //cout << ptr << ' ';
            ans.push_back(ptr);
        }
        //cout << '\n';
        ans.pop_back();
        //ans.push_back(0);
        reverse(ans.begin(), ans.end());
        delete included;
        delete path;
        return ans;
    }

    //注意返回的是单个词的长度
    Vec HMM(Vec str)
    {
        //cout << "HMM start:\n";
        Vec res;
        if (!trained)
        {
            init();
            trained = 1;
        }
        int len = str.size();
        int **pre = new int *[len];
        double **graph = new double *[len];
        for (int i = 0; i < len; i++)
        {
            graph[i] = new double[4];
            pre[i] = new int[4];
        }
        //cout << 0 << ':';
        for (int i = 0; i < 4; i++)
        {
            graph[0][i] = observe[i] + emit[str[0]][i];
            //cout << graph[0][i] << ' ';
        }
        //cout << '\n';
        int state = 0;
        for (int i = 1; i < len; i++)
        {
            state = 0;
            //cout << i << ':';
            for (int j = 0; j < 4; j++) //now
            {
                double now = trans[0][j] + emit[str[i]][j] + graph[i - 1][0];
                pre[i][j] = 0;
                for (int k = 1; k < 4; k++) //from
                {
                    if (now < trans[k][j] + emit[str[i]][j] + graph[i - 1][k])
                    {
                        now = trans[k][j] + emit[str[i]][j] + graph[i - 1][k];
                        pre[i][j] = k;
                    }
                }
                //cout << now << ' ';
                graph[i][j] = now;
                if (j && graph[i][j] > graph[i][j - 1])
                    state = j;
            }
            //cout << '\n';
        }
        int ptr = 0;
        for (int i = len - 1; i >= 0; i--)
        {
            ptr++;
            //cout << state << ' ';
            if (state == 3 || state == 0)
                res.push_back(ptr), ptr = 0;
            state = pre[i][state];
        }
        //cout << '\n';
        //cout << res.size() << '\n';
        if (ptr)
            res.push_back(ptr);
        reverse(res.begin(), res.end());
        for (int i = 0; i < len; i++)
        {
            delete graph[i];
            delete pre[i];
        }
        delete graph;
        delete pre;
        return res;
    }

    //HMM的初始化
    void init()
    {
        //cout << "init start:\n";
        ll cnt = 0;
        for (int i = 0; i < 4; i++)
        {
            cnt += observe[i];
            //cout << observe[i] << ' ';
        }
        //cout << '\n';
        for (int i = 0; i < 4; i++)
        {

            observe[i] = log10(observe[i] / double(cnt));
            ll tot = 0;
            for (int j = 0; j < 4; j++)
            {
                tot += trans[i][j];
            }
            for (int j = 0; j < 4; j++)
            {
                if (tot == 0)
                    trans[i][j] = -1e5;
                else
                    trans[i][j] = log10(trans[i][j] / double(tot));
            }
        }
        int p[4] = {0, 0, 0, 0};
        ll tot = 0;
        for (int i = 1; i <= word_count; i++)
        {
            tot = 0;
            for (int j = 0; j < 4; j++)
            {
                tot += emit[i][j];
                p[j] += emit[i][j];
            }
            for (int j = 0; j < 4; j++)
            {
                if (tot == 0)
                    emit[i][j] = -1e5;
                else
                    emit[i][j] = log10(emit[i][j] / double(tot));
            }
        }
        tot = 0;
        for (int i = 0; i < 4; i++)
            tot += p[i];
        for (int i = 0; i < 4; i++)
            emit[0][i] = log10(p[i] / double(tot));
    }

    //插入单个词语
    ll insert(Vec vec, ll pre)
    {
        if (vec.size() == 0)
            return pre;
        Trie *now = root;
        ll len = vec.size();
        ll state = 0;
        if (len == 1)
        {
            state = 3;
        }
        observe[state]++;
        for (ll deep = 1; deep <= len; deep++)
        {
            auto ptr = vec[deep - 1];
            if (!now->exist[ptr])
            {
                now->exist[ptr] = 1;
                now->nxt[ptr] = new Trie(ptr, deep);
            }
            now = now->nxt[ptr];
            if (deep == len)
            {
                if (deep == 1)
                    state = 3;
                else
                    state = 2;
            }
            else if (deep == 1)
                state = 0;
            else
                state = 1;
            if (pre != -1)
            {
                trans[pre][state]++;
            }
            emit[word_set[ptr]][state]++;
            pre = state;
        }
        now->cnt++;
        return state;
    }

public:
    vector<string> get(string str)
    {
        vector<string> res;
        Vec word = trance_to_ll(str);
        Vec vec = dp(get_graph(word), word.size());
        ll pre = 0;
        for (auto it : vec)
        {
            string sub = "";
            if (rev)
                for (int i = it; i >= pre; i--)
                    sub.append(word_map[word[i]]);
            else
                for (int i = pre; i <= it; i++)
                    sub.append(word_map[word[i]]);
            res.push_back(sub);
            pre = it + 1;
        }
        if (rev)
            reverse(res.begin(), res.end());
        return res;
    }
    void train(string line)
    {
        ll pre = -1;
        stringstream str;
        str << line;
        while (str >> line)
        {
            pre = insert(trance_to_ll(line), pre);
        }
        str.flush();
    }

    void save()
    {
        cout << "start saving" << '\n';
        save_Trie("", root);
        cout << "end saving" << '\n';
    }

    Segment(bool rev) : rev(rev)
    {
        root = new Trie();
        //cout = ofstream("cout.out", ios::app);
    }

    ~Segment()
    {
        //cout.close();
    }
};

Segment *fwd, *rev;

int main()
{
    system("chcp 65001");
    freopen("pku_training.utf8", "r", stdin);
    string line;
    fwd = new Segment(0), rev = new Segment(1);
    while (getline(cin, line))
    {
        if (line == "finished")
            break;
        fwd->train(line);
        rev->train(line);
    }
    //sg.save();
    cout << "finish training:\n";
    int t = clock();
    while (cin >> line)
    {
        cout << line << '\n';
        vector<string> res1 = fwd->get(line);
        vector<string> res2 = rev->get(line);
        if (res2.size() < res1.size())
            res1 = res2;
        for (auto it : res1)
            cout << it << "  ";
        cout << '\n';
    }
    cout << clock() - t << '\n';
    delete fwd;
    delete rev;
    return 0;
}

