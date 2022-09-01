#include <bits/stdc++.h>
using namespace std;
#define cout std::cout
#define print_word_analyse 1
#define print_debug 1
const string _type[] = {"Empty", "Error", "identifier", "integer", "float", "operator", "seperator", "keyword"};
const string _keyword[] = {"alignas", "alignof", "and", "and_eq", "asm", "atomic_cancel", "atomic_commit", "atomic_noexcept", "auto", "bitand",
                           "bitor", "bool", "break", "case", "catch", "char", "char8_t", "char16_t", "char32_t", "class", "compl", "concept", "const", "consteval",
                           "constexpr", "constinit", "const_cast", "continue", "co_await", "co_return", "co_yield", "decltype", "default", "delete", "do", "double",
                           "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false", "float", "for", "friend", "goto", "if", "inline", "int", "long", "mutable",
                           "namespace", "new", "noexcept", "not", "not_eq", "nullptr", "operator", "or", "or_eq", "private", "protected", "public", "reflexpr",
                           "register", "reinterpret_cast", "return", "requires", "return", "short", "signed", "sizeof", "static", "static_assert", "static_cast", "struct",
                           "switch", "synchronized", "template", "this", "thread_local", "throw", "true", "try", "typedef", "typeid", "typename", "union", "unsigned", "using",
                           "virtual", "void", "volatile", "wchar_t", "while", "xor", "xor_eq"};
const string _operator[] = {"=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "++", "--", "+", "-", "*", "/", "%", "~", "&", "|", "^", "<<", ">>",
                            "!", "&&", "||", ".", "->", ":", "::", "?", "//", "/*", "*/"};
const char _seperator[] = " ,;\"\'{}[]()<>\\#";
const int max_size = 16;
int count_line = 0;
struct trie
{
    bool exist[256];
    bool end;
    // bool follow;
    trie *nxt[256];
    trie()
    {
        memset(exist, 0, sizeof(exist));
        end = 0;
        // follow = 0;
    }
    bool check(char cr)
    {
        return exist[cr];
    }
    void insert(char cr)
    {
        exist[cr] = 1;
        nxt[cr] = new trie();
        // follow = 1;
    }
    trie *next(char cr)
    {
        return nxt[cr];
    }
} * __keyword_tree__, *__operator_tree__;
void insert_tree(string str, trie *root)
{
    trie *now = root;
    for (auto it : str)
    {
        if (!now->check(it))
            now->insert(it);
        now = now->next(it);
    }
    now->end = 1;
}
bool check_tree(string str, trie *root)
{
    trie *now = root;
    for (auto it : str)
    {
        if (now->check(it))
            now = now->next(it);
        else
            return 0;
    }
    return now->end;
}
vector<pair<string, int>> token(string line)
{
    int ptr = 0, len = line.size();
    int type = 0;
    int type_flag = 0;
    int preview_flag = 1;
    vector<pair<string, int>> res;
    string now;
    int count_char = 0;
    int count_err = 0;
    while (ptr < len)
    {
        // cout << ptr << ' ' << type << ' ' << now << ' ' << line[ptr]<<'\n';
        count_err++;
        if (count_char == max_size)
        {

            if (type == 4)
            {
                cout << "Error token in line " << count_line << ", row " << count_err << ": float is too long for gcc\n";
            }
            else
                cout << "Error token in line " << count_line << ", row " << count_err << ": it is too long for our analyse\n";
            res.push_back({now, type});
            type = 0;
            count_char = 0;
            now.clear();
        }
        auto cr = line[ptr];
        if (line[ptr] & 0x80)
        {
            cout << "Error token in line " << count_line << ", row " << count_err << ": shity doesn't support chinese because author can't understand how to write chinese analyser\n";
            while (line[ptr] & 0x80)
                ptr++, count_err++;
            ptr++, count_err++;
            type = 0;
            continue;
        }
        if (preview_flag)
            now.push_back(cr);
        count_char = now.size();
        preview_flag = 1;
        switch (type)
        {
        case 0: // empty
        {
            if ((cr >= 'a' && cr <= 'z') || (cr >= 'A' && cr <= 'Z') || cr == '_')
            {
                type = 2;
                break;
            }
            else if ((cr >= '0' && cr <= '9'))
            {
                type = 3;
                break;
            }
            // else if(cr=='0')//可以塞一个16进制进去
            //{
            //     res.push_back({now, 1});
            //     now.clear();
            //     type = 0;
            //     break;
            // }
            else
            {
                int ptr = 0;
                bool flag = 0;
                for (; ptr < 30; ptr++)
                {
                    auto it = _operator[ptr];
                    if (cr == it[0])
                    {
                        type = 5;
                        flag = 1;
                        break;
                    }
                }
                if (flag)
                    break;
                ptr = 0;
                for (; ptr < 17; ptr++)
                {
                    auto it = _seperator[ptr];
                    if (cr == it)
                    {
                        res.push_back({now, 6});
                        now.clear();
                        type = 0;
                        // preview_flag = 1;
                        flag = 1;
                        break;
                    }
                }
                if (flag)
                    break;
                res.push_back({now, 1});
                now.clear();
                type = 0;

                break;
            }
            break;
        }
        case 2: // ident
        {
            if ((cr >= 'a' && cr <= 'z') || (cr >= 'A' && cr <= 'Z') || cr == '_' || (cr >= '0' && cr <= '9'))
            {
                break;
            }
            now.pop_back();
            if (check_tree(now, __keyword_tree__))
            {
                type = 7;
            }
            res.push_back({now, type});
            now.clear();
            now.push_back(cr);
            type = 0;
            preview_flag = 0;
            break;
        }
        case 3:
        {
            if (cr >= '0' && cr <= '9')
                break;
            else if (cr == '.')
            {
                type = 4;
                type_flag = 1;
            }
            else if (cr == 'E')
            {
                type = 4;
                type_flag = 2;
            }
            else if ((cr >= 'a' && cr <= 'z') || (cr >= 'A' && cr <= 'Z') || cr == '_')
            {
                cout << "Error token in line " << count_line << ", row " << count_err << ": identifier can't use number as prefix\n";
                now.pop_back();
                res.push_back({now, type});
                now.clear();
                now.push_back(cr);
                type = 0;
                preview_flag = 0;
            }
            else
            {
                now.pop_back();
                res.push_back({now, type});
                now.clear();
                now.push_back(cr);
                type = 0;
                preview_flag = 0;
            }
            break;
        }
        case 4:
        {
            if (cr >= '0' && cr <= '9')
            {
                if (type_flag == 1)
                {
                    type_flag = 4;
                }
                else if (type_flag == 2)
                {
                    type_flag = 3;
                }
            }
            else if (cr == 'E')
            {
                if (type_flag == 4)
                    type_flag = 2;
                else
                {
                    res.push_back({now, 1});
                    type = 0;
                    now.clear();
                    break;
                }
            }
            else if (cr == '+' || cr == '-')
            {
                if (type_flag == 2)
                {
                    type_flag = 3;
                }
                else
                {
                    res.push_back({now, 1});
                    type = 0;
                    now.clear();
                    break;
                }
            }
            else if ((cr >= 'a' && cr <= 'z') || (cr >= 'A' && cr <= 'Z') || cr == '_')
            {
                cout << "identifier can't use number as prefix\n";
                now.pop_back();
                res.push_back({now, type});
                now.clear();
                now.push_back(cr);
                type = 0;
                preview_flag = 0;
            }
            else
            {
                now.pop_back();
                res.push_back({now, type});
                type = 0;
                now.clear();
                now.push_back(cr);
                preview_flag = 0;
                break;
            }

            break;
        }
        case 5:
        {
            if (check_tree(now, __operator_tree__))
            {
                if (now == "//")
                    return res;
                res.push_back({now, type});
                now.clear();
                type = 0;
            }
            else
            {
                now.pop_back();
                res.push_back({now, type});
                now.clear();
                now.push_back(cr);
                type = 0;
                preview_flag = 0;
            }
            break;
        }
        default:
            break;
        }
        if (preview_flag)
            ptr++;
    }
    if (print_debug)
        cout << '\n';
    return res;
}

vector<pair<string, int>> word_analyse;
//词法分析代码到此结束
const string _follow[] = {};
const string _first[] = {};
map<string, bool> _value_list, _function_list, _type_list;
const string _pre_type[] = {"empty", "expression", "type"};
int analyse_point = -1, analyse_row = 0;
int line_count = 1;
struct node
{
    vector<node *> grammar;
    string type;
    node() { type = "Empty"; }
    node(string type) : type(type){};
    void insert(string type) { grammar.push_back(new node(type)); }
    node *end()
    {
        return *(grammar.end() - 1);
    }
    ~node()
    {
        int sz = grammar.size();
        for (int i = 0; i < sz; i++)
            delete grammar[i];
    }
} * __grammar_root__;
pair<string, int> get_now() { return word_analyse[analyse_point]; }
pair<string, int> get_next()
{
    analyse_point++, analyse_row++;
    if (analyse_point >= word_analyse.size())
        return make_pair(string("END"), 0);
    if (word_analyse[analyse_point].first == "\n")
        line_count++, analyse_point++, analyse_row = 1;
    if (analyse_point >= word_analyse.size())
        return make_pair(string("END"), 0);
    if (print_debug)
        cout << word_analyse[analyse_point].first << ' ' << word_analyse[analyse_point].second << endl;
    return word_analyse[analyse_point];
}
inline void Error_token(string massage)
{
    if (print_debug)
        cout << "Error token in line " << line_count << " row " << analyse_row << " :" << massage << '\n';
}
inline int try_sign(pair<string, int> now, string sign, node *root)
{
    if (now.first != sign)
    {
        Error_token("expect \"" + sign + "\" here but not found");
        return 1;
    }
    else
        root->insert(sign);
    return 0;
}
int get_for(node *root);
int get_block(pair<string, int> now, node *root);
int get_while(node *root);
int get_if(node *root);
int get_expression(node *root);
int get_definition(node *root, string type);
int get_comment(node *root, int type);
int get_sentences(node *root, int flag)
{
    int pre = 0;
    while (1)
    {
        auto now = get_next();
        if (print_debug)
            cout << now.first << ' ' << now.second << '\n';
        if (now.first == "for")
        {
            if (pre != 0)
                Error_token("expect " + _pre_type[pre] + " here");
            root->insert("for loop sentences");
            if (get_for(root->end()))
                Error_token("bad for_loop found");
            pre = 0;
        }
        else if (now.first == "while")
        {
            if (pre != 0)
                Error_token("expect " + _pre_type[pre] + " here");
            root->insert("while loop sentences");
            if (get_while(root->end()))
                Error_token("bad while_loop found");
            pre = 0;
        }
        else if (now.first == "if")
        {
            if (pre != 0)
                Error_token("expect " + _pre_type[pre] + " here");
            root->insert("if sentences");
            if (get_if(root->end()))
                Error_token("bad if sentences");
        }
        else if (_type_list[now.first])
        {
            if (pre != 0)
                Error_token("expect " + _pre_type[pre] + " here");
            root->insert("value/function definition");
            if (get_definition(root->end(), now.first))
                Error_token("bad definition sentences");
        }
        else if (now.first == "//")
        {
            if (pre != 0)
                Error_token("warning:the comment may crash the code block");
            root->insert("comment");
            get_comment(root->end(), 0);
        }
        else if (now.first == "/*")
        {
            if (pre != 0)
                Error_token("warning:the comment may crash the code block");
            root->insert("comment");
            get_comment(root->end(), 1);
        }
        else if (now.first == "\"" || now.first == "\'")
        {
            root->insert("const string");
            get_comment(root->end(), 2);
        }
        else if (now.first == "#")
        {
            now = get_next();
            if (now.first == "END")
            {
                Error_token("expect macro define here");
            }
            if (now.first == "define")
            {
                root->insert("macro define");
                now = get_next();
                if (now.second != 2)
                {
                    Error_token("expect identifier here");
                    continue;
                }
                root->end()->insert("defined:");
                _value_list[now.first] = 1;
                root->end()->insert(now.first);
                now = get_next();
                if (now.first == "END")
                {
                    Error_token("expect macro define here");
                    continue;
                }
                root->end()->insert(now.first);
            }
            else if (now.first == "include")
            {
                now = get_next();
                root->insert("macro include");
                if (try_sign(now, "<", root->end()))
                    continue;
                if (get_sentences(root->end(), 3))
                    continue;
                now = get_now();
                if (try_sign(now, ">", root->end()))
                    continue;
            }
        }
        else if (now.first == "typedef")
        {
            root->insert("macro typedef");
            if (get_sentences(root->end(), 1))
                Error_token("invalid macro typedef");
        }
        else if (now.first == "=")
        {
            if (print_debug)
                cout << "getting expression\n";
            if (!(pre <= 1))
            {
                Error_token("expressions not support these grammar");
            }
            root->insert("=");
            root->insert("expression(value)");
            if (get_expression(root->end()))
                Error_token("Error expressions found");
        }
        else if (now.first != "END") //摆烂用代码，不要使用他
        {
            if (now.first != "\n")
                root->insert(_type[now.second] + ":" + now.first);
        }

        else
            break;
        now = get_now();
        if (((now.first == ";" || now.first == "]" || now.first == "}" || now.first == ")" || now.first == ">" || now.first == ":") && flag == 1) || now.first == "END") //摆烂用代码，不要使用他
            return 0;
        else if ((now.first == "}") && flag == 2)
            return 0;
        else if (now.first == ">" && flag == 3)
            return 0;
    }
    return 0;
}
map<string, int> priority_type;
void expression(node *root, vector<string>::iterator begin, vector<string>::iterator end, int deep);
int get_expression(node *root)
{
    vector<string> vec;
    auto now = get_next();
    while (1)
    {
        cout << now.first << ':' << now.second << '\n';
        if ((now.second != 2) && (now.second != 3) && (now.second != 4) && (now.second != 5) && (now.first != "(") && (now.first != ")"))
            break;
        vec.push_back(now.first);
        now = get_next();
    }
    if (vec.empty())
        return 1;
    expression(root, vec.begin(), vec.end() - 1, 0);
    return 0;
}
void expression(node *root, vector<string>::iterator begin, vector<string>::iterator end, int deep)
{
    cout.flush();
    if (begin == end)
    {
        root->insert(*begin);
        return;
    }
    bool flag = 0;
    auto pre = begin;
    for (auto it = begin; it <= end; it++)
    {
        if (priority_type.find(*it) != priority_type.end())
            if (priority_type[*it] == (deep - 1))
            {
                root->insert("operator: " + *it);
                root->insert("expression");
                if (it != begin)
                    expression(root->end(), pre, it - 1, deep + 1);
                flag = 1;
                pre = it + 1;
            }
    }
    if (!flag)
        expression(root, begin, end, deep + 1);
    else
    {
        root->insert("expression");
        expression(root->end(), pre, end, deep + 1);
    }
}
int get_definition(node *root, string type)
{
    root->insert("type definition");
    auto now = get_next();
    if (type == "const")
    {
        root->insert("type limitation:const");
        if (print_debug)
            cout << now.first << '\n';
        if (!_type_list[now.first])
        {
            Error_token("const can not use as a single type");
            return 1;
        }
        type = now.first;
        now = get_next();
    }
    if (type == "class" || type == "struct")
    {
        if (now.second != 2)
        {
            return 1;
        }
        root->insert(type + " definition");
        now = get_next();
        if (get_block(now, root->end()))
            return 1;
        return 0; // todo
    }
    else if (type == "vector" || type == "template" || type == "pair" || type == "map" || type == "complex")
    {

        root->insert("type list");
        if (try_sign(now, "<", root->end()))
            return 1;
        root->end()->insert("type list contains:");
        now = get_next();
        if (now.first == ">")
        {
            Error_token("expect type list here");
            return 1;
        }
        bool flag = 0;
        while (1)
        {
            if (_type_list[now.first])
            {
                string tmp = now.first;
                if (now.first == "long")
                {
                    if (now.first == "long")
                    {
                        tmp = "long long";
                        now = get_next();
                    }
                    else if (now.first == "double")
                    {
                        tmp = "long double";
                        now = get_next();
                    }
                }
                if (flag)
                {
                    Error_token("expect \",\" or \">\" here but not found");
                    return 1;
                }
                else
                    root->end()->insert("type:" + tmp), flag = 1;
            }
            else if (now.first == ",")
            {
                if (!flag)
                {
                    Error_token("expect type here but not found");
                }
                else
                    root->end()->insert(","), flag = 0;
            }
            else if (now.first == ">")
            {
                if (!flag)
                {
                    Error_token("expect type here but not found");
                }
                // now = get_next();
                break;
            }
            now = get_next();
        }
        now = get_now();
        if (try_sign(now, ">", root->end()))
            return 1;
        now = get_next();
    }
    else if (type == "long")
    {
        if (now.first == "long")
        {
            type = "long long";
            now = get_next();
        }
        else if (now.first == "double")
        {
            type = "long double";
            now = get_next();
        }
    }
    // cout << "a:" << now.first << '\n';
    if (now.first == "(")
    {
        root->insert("warning, you are trying to redefine the constructor, but shity not suppose to support it");
        if (get_sentences(root->end(), 1))
            return 1;
        now = get_now();
        if (try_sign(now, ")", root))
            return 1;
    }
    if (now.second != 2)
    {
        if (now.second == 7)
        {
            if (now.first == "operator")
            {
                root->insert("redefined operator of type:" + type);
                now = get_next();
            }
            else
            {
                Error_token("key word can't be used as an identifier");
                return 1;
            }
        }
        else
        {
            Error_token("expect identifier here but not fount");
            return 1;
        }
    }
    // cout << "b:" << now.first << '\n';

    string name = now.first;
    root->insert("id:" + now.first);
    now = get_next();
    if (print_debug)
        cout << now.first << '\n';
    if (now.first == "[")
    {
        root->insert("(array definition)[");
        now = get_next();
        if (now.second == 2)
        {
            if (!_value_list[now.first])
            {
                Error_token("undefined value used");
                return 1;
            }
            else
                root->insert(now.first);
        }
        else if (now.second == 3)
        {
            root->insert(now.first);
        }
        else if (now.second == 4)
        {
            Error_token("float can't be used as an array len");
            return 1;
        }
        else
        {
            Error_token("unexpected token:" + now.first);
            return 1;
        }
        now = get_next();
        if (try_sign(now, "]", root))
            return 1;
    }
    else if (now.first == "(")
    {
        _function_list[name] = 1;
        if (name == "main")
            root->insert("(main function definition)(");
        else
            root->insert("(function definition)(");
        now = get_next();
        root->insert("type list");
        while (_type_list[now.first])
        {
            if (get_definition(root->end(), now.first))
            {
                Error_token("function arg define error");
                return 1;
            }
            now = get_next();
            if (now.first == ",")
                root->end()->insert(",");
            else if (now.first == ")")
                break;
        }
        root->insert(")");
        // now = get_next();
        //  cout << now.first <<'\n' ;
        if (now.first == "const")
        {
            root->insert("function limitation const detect");
            now = get_next();
        }
        root->insert("(function body)");
        if (get_block(now, root->end()))
            return 1;
        return 0;
    }
    _value_list[name] = 1;
    return 0;
}
int get_comment(node *root, int type)
{
    if (type == 1)
    {
        // root->insert("/*");
        while (1)
        {
            auto now = get_next();
            if (now.first == "*/")
                return 0;
            root->insert(now.first);
        }
    }
    else if (type == 0)
    {
        // root->insert("//");
        while (1)
        {
            analyse_point++;
            if (word_analyse[analyse_point].first == "\n")
                break;
            auto now = word_analyse[analyse_point];
            root->insert(now.first);
        }
        return 0;
    }
    else if (type == 2)
    {
        // if(print_debug)
        //     cout << "const string\n";
        // root->insert("\"");
        while (1)
        {
            auto now = get_next();
            if (now.first == "\"" || now.first == "\'")
                return 0;
            root->insert(now.first);
        }
    }
    return 0;
}
//给if,for,while等需要语句块的语句使用
int get_block(pair<string, int> now, node *root)
{
    if (print_debug)
        cout << "getting block: " << now.first << '\n';
    if (now.first == "{")
    {
        root->insert("{");
        root->insert("sentences");
        if (get_sentences(root->end(), 2))
            return 1;
        now = get_now();
        if (try_sign(now, "}", root))
            return 1;
    }
    else
    {
        root->insert("sentences");
        if (get_sentences(root->end(), 2))
            return 1;
    }
    return 0;
}

int get_for(node *root)
{
    auto now = get_next();
    root->insert("for");
    if (try_sign(now, "(", root))
        return 1;
    root->insert("sentences");
    if (get_sentences(root->end(), 1))
        return 1;
    now = get_now();
    if (print_debug)
        cout << "for_loop:" << now.first << ' ' << now.second << '\n';
    if (now.first == ";")
    {
        root->insert("sentences");
        if (get_sentences(root->end(), 1))
            return 1;
        now = get_now();
        if (try_sign(now, ";", root))
            return 1;
        root->insert("sentences");
        if (get_sentences(root->end(), 1))
            return 1;
        now = get_now();
        if (try_sign(now, ")", root))
            return 1;
        now = get_next();
        if (get_block(now, root))
            return 1;
        return 0;
    }
    else if (now.first == ":")
    {
        root->insert("sentences");
        if (get_sentences(root->end(), 1))
            return 1;
        now = get_now();
        if (print_debug)
            cout << "for(a:b)" << now.first << ' ' << now.second << '\n';
        if (try_sign(now, ")", root))
            return 1;
        now = get_next();
        if (get_block(now, root))
            return 1;
        return 0;
    }
    return 0;
}

int get_while(node *root)
{
    auto now = get_next();
    root->insert("while");
    if (try_sign(now, "(", root))
        return 1;
    root->insert("sentences");
    if (get_sentences(root->end(), 1))
        return 1;
    now = get_now();
    if (try_sign(now, ")", root))
        return 1;
    now = get_next();
    if (get_block(now, root))
        return 1;
    return 0;
}

int get_if(node *root)
{
    if (print_debug)
        cout << "getting if\n";
    auto now = get_next();
    root->insert("if");
    if (try_sign(now, "(", root))
        return 1;
    root->insert("sentences");
    if (get_sentences(root->end(), 1))
        return 1;
    now = get_now();
    if (try_sign(now, ")", root))
        return 1;
    now = get_next();
    if (get_block(now, root))
        return 1;
    // cout << "?\n";
    while (1)
    {
        now = get_next();
        if (now.first == "else")
        {
            now = get_next();
            if (now.first == "if")
            {
                root->insert("else if");
                now = get_next();
                if (try_sign(now, "(", root))
                    return 1;
                root->insert("sentences");
                if (get_sentences(root->end(), 1))
                    return 1;
                now = get_now();
                if (try_sign(now, ")", root))
                    return 1;
                now = get_next();
                if (get_block(now, root))
                    return 1;
            }
            else
            {
                root->insert("else");
                // now = get_next();
                if (get_block(now, root))
                    return 1;
                return 0;
            }
        }
        else
            return 0;
    }
    return 0;
}
string _init_type_list[] = {"bignum", "vector", "pair", "map", "template", "int", "float", "double", "char", "signed", "unsigned", "connst", "static", "long", "string", "class", "struct", "auto", "ll", "P", "cp", "complex", "void", "const"};
vector<string> _priority_[] = {{"(", "[", ")", "]", "->", ".", "::", "++", "--"},
                               {"!", "~", "++", "--"},
                               {"*", "/", "%"},
                               {"+", "-"},
                               {"<<", ">>"},
                               {"<", "<=", ">", ">="},
                               {"==", "!="},
                               {"&"},
                               {"^"},
                               {"|"},
                               {"&&"},
                               {"||"}};
void __init__()
{
    __keyword_tree__ = new trie();
    __operator_tree__ = new trie();
    int ptr = 0;
    for (; ptr < (sizeof(_keyword) / sizeof(_keyword[0])); ptr++)
    {
        auto str = _keyword[ptr];
        insert_tree(str, __keyword_tree__);
    }
    ptr = 0;
    for (; ptr < (sizeof(_operator) / sizeof(_operator[0])); ptr++)
    {
        auto str = _operator[ptr];
        insert_tree(str, __operator_tree__);
    }
    __grammar_root__ = new node("program body");
    for (int i = 0; i < sizeof(_init_type_list) / sizeof(_init_type_list[0]); i++)
    {
        _type_list[_init_type_list[i]] = 1;
    }
    for (int i = 0; i < sizeof(_priority_) / sizeof(_priority_[0]); i++)
    {
        for (auto it : _priority_[i])
        {
            priority_type[it] = i;
        }
    }
}

int show_grammar_tree(node *root, int deep, bool flag)
{
    if (flag || root->grammar.size())
    {
        cout << '\n';
        for (int i = 1; i <= deep; i++)
            cout << "    ";
    }
    else
        cout << ' ';

    if (root->grammar.size())
    {
        cout << root->type << " {";
        int pre = 1;
        for (auto it : root->grammar)
            pre = show_grammar_tree(it, deep + 1, pre);
        for (int i = 1; i <= deep; i++)
            cout << "    ";
        cout << "}" << endl;
        return 1;
    }
    else
        cout << root->type;
    return 0;
}

int main()
{
    __init__();
    freopen("b.c", "r", stdin);
    freopen("analyse.txt", "w", stdout);
    string line;
    while (getline(cin, line))
    {
        line.push_back(' ');
        count_line++;
        if (print_word_analyse)
            cout << line << '\n';
        for (auto it : token(line))
        {
            if (it.first == " ")
                continue;
            word_analyse.push_back(it);
            if (print_word_analyse)
            {
                cout << "\"" << it.first << "\""
                     << " : " << _type[it.second];
                if (it.second == 1)
                    cout << " token in line " << count_line << '\n';
                else
                    cout << '\n';
            }
        }
        word_analyse.push_back({"\n", 6});
    }
    get_sentences(__grammar_root__, 0);
    cout << "finished analyse" << endl;
    show_grammar_tree(__grammar_root__, 0, 1);
}
