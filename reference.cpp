#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include <cstdio>
#include <map>
using namespace std;
#define _CRT_SECURE_NO_WARNINGS
#define N 10009//my genome�� ���� ó���� ������ �����ؼ� 3����� �ø�������

class BWT {
public:
	int index;
	string suffix;
};

class Patt {
public:
	string pattern;
};

class BWT_T {
public:
	int index;
	char elem;
	int sp;
	int get_sp()
	{
		return sp;
	}
};


void main()
{
	vector<BWT> *bwt_v = new vector<BWT>(N + 1);
	vector<BWT_T> *bwt_T_v = new vector<BWT_T>(N + 1);
	vector<BWT_T> *first_column_v = new vector<BWT_T>(N + 1);
	//����� ��µ� string �ʱ�ȭ

	ofstream fout_input("C:\\10009.txt");
	if (!fout_input)
	{
		cout << "������ �� �� �����ϴ�.\n";
	}

	int num;
	srand((unsigned)time(NULL));
	for (int i = 0; i < N; i++)
	{
		num = rand() % 4;
		if (num == 0)
			fout_input << 'A';
		else if (num == 1)
			fout_input << 'C';
		else if (num == 2)
			fout_input << 'G';
		else
			fout_input << 'T';
	}
	fout_input.close();
	ifstream input_txt;
	input_txt.open("C:\\10009.txt");
	string Text;
	if (input_txt.is_open())
	{
		getline(input_txt, Text);
	}
	input_txt.close();
	
	//my_genome�� ���� Text(ref)�����Ǿ���->4��5õ���� mismatch������̴�
	int n1, n2;
	for (int i = 0; i < 5; i++)
	{
		if (N > 32767)
		{
			n1 = rand();
			n2 = rand() % (N / 32767);
		}
		else
		{
			n1 = rand() % (N + 1);//0~N
			n2 = 1;
		}
		num = rand() % 4;
		if (num == 0)
		{
			Text[n1*n2] = 'A';
		}
		else if (num == 1)
		{
			Text[n1*n2] = 'C';
		}
		else if (num == 2)
		{
			Text[n1*n2] = 'G';
		}
		else
		{
			Text[n1*n2] = 'T';
		}
	}

	Text[N] = '$';
	
	//suffices�����ϱ�
	
	int n;
	int m = 0;
	for (int i = 0; i <= N; i++) {
		n = N - i;
		(*bwt_v)[i].index = n;
		for (int j = 0; j <= i; j++)
		{
			(*bwt_v)[i].suffix += Text[n];
			n++;
		}
		for (int j = i + 1; j <= N; j++)
		{
		  
			(*bwt_v)[i].suffix += Text[m++];
		}
		m = 0;
	}
	cout << "suffixes�����Ϸ�" << endl;
	cout << "ä��� �Ϸ�" << endl;
	//sort�ϱ�
	int k;
	char tem[N + 1];
	for (int i = 0; i <= N; i++)
	{
		k = i;
		for (int j = i + 1; j <= N; j++)
		{
			//if (strcmp((*bwt_v)[k].suffix, (*bwt_v)[j].suffix) > 0)//�տ��� �� ũ�ٸ�->�ڷ� ��������
			if ((*bwt_v)[k].suffix.compare((*bwt_v)[j].suffix) > 0)
				k = j;
		}
		for (int j = 0; j <= N; j++)
			tem[j] = (*bwt_v)[i].suffix[j];
		for (int j = 0; j <= N; j++)
			(*bwt_v)[i].suffix[j] = (*bwt_v)[k].suffix[j];//���� bwt[i].suffix[j] = k�� �ϸ� 2��° �ٲ� �� ���� �ȵ��� �ߺ��ȴ�
		int tt = (*bwt_v)[i].index;
		(*bwt_v)[i].index = (*bwt_v)[k].index;
		for (int j = 0; j <= N; j++)
			(*bwt_v)[k].suffix[j] = tem[j];
		(*bwt_v)[k].index = tt;
		//������ strcpy_s�� �����Ϸ��� �ߴµ� buffer is too small && 0 ���� ���� �ٲ���
	}
	cout << "sort�Ϸ�" << endl;
	ofstream BWT_elem("c:\\10009_elem.txt");
	for (int i = 0; i <= N; i++)
	{
		BWT_elem << (*bwt_v)[i].suffix[N] << endl;
	}
	BWT_elem.close();

	ofstream BWT_index("c:\\10009_index.txt");
	for (int i = 0; i <= N; i++)
	{
		BWT_index << (*bwt_v)[i].index << endl;
	}
	BWT_index.close();


}