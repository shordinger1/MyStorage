#include <bits/stdc++.h>
#define _CSI_ const static int
#define _CSD_ const static double
using namespace std;
typedef long long ll;
typedef pair<ll, ll> Pi;
typedef pair<ll, double> Pd;
typedef vector<pair<ll, vector<Pd>>> Graph;
typedef vector<ll> Vec;
class SpeechPart
{
    _CSI_ max_word_count = 1e5;
    _CSI_ part = 10;
    double trans[part][part];
    double emit[max_word_count][part];
    double observe[part];
    map<string, int> word_map;
    map<string, int> part_map;
    map<int, bool> able;
    map<int, int> word_cnt;
    bool trained;
    int word_count;
    int part_count;

    //注意返回的是单个词的长度
    Vec HMM(Vec str)
    {
        Vec res(str.size());
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
            graph[i] = new double[part];
            pre[i] = new int[part];
        }
        for (int i = 0; i < part; i++)
        {
            graph[0][i] = observe[i] + emit[str[0]][i];
        }
        int state = 0;
        for (int i = 1; i < len; i++)
        {
            state = 0;
            //cout << i << ':';
            for (int j = 0; j < part; j++) //now
            {
                double now = trans[0][j] + emit[str[i]][j] + graph[i - 1][0];
                pre[i][j] = 0;
                for (int k = 1; k < part; k++) //from
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
            res[i] = state;
            //cout << '\n';
        }
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
        for (int i = 0; i < part; i++)
        {
            cnt += observe[i];
            //cout << observe[i] << ' ';
        }
        //cout << '\n';
        for (int i = 0; i < part; i++)
        {

            observe[i] = log10(observe[i] / double(cnt));
            //cout << observe[i] << ":";
            ll tot = 0;
            for (int j = 0; j < part; j++)
            {
                tot += trans[i][j];
            }
            for (int j = 0; j < part; j++)
            {
                //cout << trans[i][j] << ' ';
                if (tot == 0)
                    trans[i][j] = -1e5;
                else
                    trans[i][j] = log10(trans[i][j] / double(tot));
            }
            //cout << '\n';
        }
        //cout << '\n';
        //cout << "emit:\n";
        int p[part];
        memset(p, 0, sizeof(p));
        ll tot = 0;
        for (int i = 1; i <= word_count; i++)
        {
            tot = 0;
            for (int j = 0; j < 4; j++)
            {
                tot += emit[i][j];
                p[j] += emit[i][j];
            }
            for (int j = 0; j < part; j++)
            {
                if (tot == 0)
                    emit[i][j] = -1e5;
                else
                    emit[i][j] = log10(emit[i][j] / double(tot));
                //cout << emit[i][j] << ' ';
            }
            //cout << '\n';
        }
        tot = 0;
        for (int i = 0; i < part; i++)
            tot += p[i];
        for (int i = 0; i < part; i++)
            emit[0][i] = log10(p[i] / double(tot));
        //cout << "init end\n";
    }

    void insert(vector<pair<string, string>> line)
    {
        if (line.size() == 0)
            return;
        int pre = -1;
        for (int i = 0; i < line.size(); i++)
        {
            if (!word_map[line[i].first])
            {
                word_count++;
                word_map[line[i].first] = word_count;
            }
            if (!part_map[line[i].second])
            {
                part_count++;
                word_map[line[i].second] = part_count;
            }
            int word = word_map[line[i].first], ptr = part_map[line[i].second];
            if (!i)
                observe[ptr]++;
            else
                trans[pre][ptr]++;
            emit[word][ptr]++;
            word_cnt[word]++;
        }
    }

    bool main_word_cmp(int a, int b)
    {
        return word_cnt[a] > word_cnt[b];
    }

public:
    SpeechPart()
    {
        trained = 0;
        word_count = 0;
        part_count = 0;
    }

    void train(string line, string flag)
    {
        stringstream str;
        str << line;
        vector<pair<string, string>> vec;
        while (str >> line)
        {
            int p = line.find(flag);
            vec.push_back({line.substr(0, p), line.substr(p + 1)});
        }
        insert(vec);
    }

    void disable(vector<string> word_set)
    {
        for (auto it : word_set)
        {
            cout << it << '\n';
            if (!word_map[it])
            {
                word_count++;
                word_map[it] = word_count;
            }
            able[word_map[it]] = 1;
        }
    }

    vector<pair<string, Pi>> get(vector<string> segment, int count = 2)
    {
        Vec seg(segment.size());
        for (int i = 0; i < segment.size(); i++)
        {
            seg[i] = word_map[segment[i]];
        }
        vector<pair<string, Pi>> res;
        Vec temp = HMM(seg);
        // sort(seg.begin(), seg.end(), main_word_cmp);
        for(int i = 0; i < seg.size() - 1; i++){
            for(int j = i + 1; j < seg.size(); j++){
                if(word_cnt[seg[j]] > word_cnt[seg[i]]){
                    int temp = word_cnt[seg[j]];
                    word_cnt[seg[j]] = word_cnt[seg[i]];
                    word_cnt[seg[i]] = temp;
                }
            }
        }
        map<int, int> main_word;
        for (int i = 0; i < count; i++)
        {
            if (able[seg[i]])
            {
                count++;
                continue;
            }
            main_word[seg[i]] = 1;
        }
        for (int i = 0; i < temp.size(); i++)
        {
            res.push_back({segment[i], {temp[i], main_word[temp[i]]}});
        }
        return res;
    }
};