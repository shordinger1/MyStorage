#include "segment.cpp"
#include "speechpart.cpp"
class NLP
{
private:
    Segment *fwd, *rev;
    SpeechPart *sp;

public:
    NLP()
    {
        fwd = new Segment(0);
        rev = new Segment(1);
        sp = new SpeechPart();
    }

    void seg_train(string FilePath)
    {
        ifstream input(FilePath);
        string line;
        while (input >> line)
        {
            fwd->train(line);
            rev->train(line);
        }
        input.close();
    }

    void sp_train(string FilePath)
    {
        ifstream input(FilePath,ios::out);
        string line;
        while (input >> line)
        {
            sp->train(line, "/");
        }
        input.close();
    }

    void sp_disable_word(string FilePath)
    {
        ifstream input(FilePath);
        string line;
        vector<string> word;
        while (input >> line)
        {
            word.push_back(line);
        }
        sp->disable(word);
        input.close();
    }

    //返回一个数组表示每一个
    vector<pair<string,Pi>> get(string line)
    {
        vector<string> res1 = fwd->get(line), res2 = rev->get(line);
        if (res1.size() > res2.size())
            res1 = res2;
        return sp->get(res1);
    }
};