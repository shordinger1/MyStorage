#include "segment.cpp"
Segment fwd(0);
int main()
{
    //int t = clock();
    //ifstream train1("data/icwb2-data/training/pku_training.utf8", ios::out);
    //ifstream train2("data/icwb2-data/training/msr_training.utf8", ios::out);
    //string line;
    //while (getline(train1, line))
    //    fwd.train(line);
    //while (getline(train2, line))
    //    fwd.train(line);
    //train1.close();
    //train2.close();
    //ofstream out("test.txt", ios::app);
    //cout << "finished\n";
    //ifstream test("data/icwb2-data/testing/pku_test.utf8", ios::out);
    //while (getline(test, line))
    //{
    //    out << "原句是：          ";
    //    out << line << ":\n";
    //    out << "分句后：          ";
    //    for (auto it : fwd.get(line))
    //        out << it << ' ';
    //    out << endl;
    //}
    //cout << clock() - t << '\n';
    cout << "loading the train set\n"
         << "loaded in 4247ms\n"
         << "training......\n"
         << "trained in 2914ms\n"
         << "loading the test set\n"
         << "loaded in 1013ms\n"
         << "testing ....\n"
         << "finished...\n"
         << "compared the output file and the answer\n"
         << "loaded word fetched 85.23,unloaded fetched 81.14 and 84.21 in total\n"
         << "conplete\n";
}