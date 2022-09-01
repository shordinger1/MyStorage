#include "model.cpp"
using namespace std;
NLP *nlp;
int main()
{
	string FilePath = "C:\\Users\\ACMer\\Desktop\\ACM\\ML\\temp\\";
	nlp->seg_train(FilePath + "sentences.txt");
	printf("例句训练完成\n");
	nlp->sp_train(FilePath + "dict_words.txt");
	printf("关键词训练完成\n");
	nlp->sp_disable_word(FilePath + "stop_words.txt");
	printf("停词表输入完毕\n");
	FilePath = FilePath + "sentences.txt";
	ifstream input(FilePath);
	string line;
	vector<string> word;
	input >> line;
	while (line != "" && line != "\n")
	{
		printf("开始训练\n");
		for (auto it : nlp->get(line))
		{
			cout << it.first << " ";
		}
		input >> line;
	}
	input.close();
	return 0;
}