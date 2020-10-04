#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstdio>
#include <map>
using namespace std;
#define N 30009
static string find_genome;
/*ID:2016112128 NAME:������*/
class Patt {
public:
	string pattern;
};
void pattern_maker();
vector<Patt> *mix_pattern;
int main()
{
	string init = "*";
	for (int i = 0; i <= N; i++)
		find_genome.insert(i, init);

	ifstream input_txt;
	input_txt.open("C:\\30009.txt");
	string Text;
	int i = 0;
	if (input_txt.is_open())
	{
		getline(input_txt, Text);
	}
	input_txt.close();
	int num;
	srand(time(NULL));
	//my_genome�� ���� Text(ref)�����Ǿ���->4��5õ���� mismatch������̴�
	int n1, n2;
	for (int i = 0; i < 100; i++)
	{
		if (N > 32767)
		{
			n1 = rand();
			n2 = rand() % (N / 32767);
			num = rand() % 4;
			if (num == 0)
				Text[n1] = 'A';
			else if (num == 1)
				Text[n1] = 'C';
			else if (num == 2)
				Text[n1] = 'G';
			else
				Text[n1] = 'T';
		}
		else
		{
			n1 = rand() / 10000;
			num = rand() % 4;
			if (num == 0)
				Text[n1] = 'A';
			else if (num == 1)
				Text[n1] = 'C';
			else if (num == 2)
				Text[n1] = 'G';
			else
				Text[n1] = 'T';
		}

	}
	cout << "ref�����Ϸ�" << endl;
	pattern_maker();
	cout << "pattern_maker" << endl;
	int size = (N - 20) / 15 + 2;
	clock_t time;
	time = clock();
	for (int i= 0; i< size; i++)
	{
		int l = (*mix_pattern)[i].pattern.size()- 1;
		int mismatch = 2;
		for (int j= 0; j <= N - l + 1; j++)
		{
			int missed = 0;
			for (int k = 0; k < l; k++)
			{
				char t = Text[j + k];
				char p = (*mix_pattern)[i].pattern[k];
				if (t != p) {
					missed++;
				}
				if (missed > mismatch)
				{
					break;
				}
			}
			if (missed <= mismatch) {

				find_genome.replace(j, (*mix_pattern)[i].pattern.size(), (*mix_pattern)[i].pattern);

			}
		}
		
	}
	time = clock() - time;
	printf("time: %d (%f seconds).\n", time, ((float)time) / CLOCKS_PER_SEC);
	cout << find_genome;

	
}

void pattern_maker()
{
	vector<Patt> *Pattern;
	int size = (N - 20) / 15 + 2;//30������ pattern�� ���� ���ڿ�1��
	Pattern = new vector<Patt>(size);
	string my_genome;
	string Text;
	ifstream Text36("C:\\30009.txt");//������Ʈ36���� ������ mygenome�ڸ���
	if (!Text36)
	{
		cout << "������ �� �� �����ϴ�.\n";
	}
	int num;
	srand((unsigned)time(NULL));
	if (Text36.is_open())
	{
		getline(Text36, my_genome);
	}
	Text36.close();
	int index = 0;
	int a = 0;
	for (int i = 0; i < size - 1; i++)
	{
		for (int j = index; j < index + 20; j++)
			(*Pattern)[a].pattern += my_genome[j];
		a++;
		index += 15;
	}
	for (int j = index; j <= N; j++)
		(*Pattern)[a].pattern += my_genome[j];
	
	cout << "Pattern �ڸ��� �Ϸ�" << endl;
	//���� �ڸ� ���ϵ� ���� ������Ѵ�

	mix_pattern = new vector<Patt>(size);
	vector<int> over(size);
	int flag;
	for (int i = 0; i < size; i++)
	{
		while (1)
		{
			over[i] = rand() % size;
			flag = 0;
			for (int j = 0; j < i; ++j)
			{
				if (over[j] == over[i])
				{
					flag = 1;
					break;
				}
			}
			if (!flag)
				break;
		}
		(*mix_pattern)[over[i]].pattern = ((*Pattern)[i].pattern);
	}
	
	vector<Patt>().swap(*Pattern);
	cout << "������¿Ϸ�" << endl;

}