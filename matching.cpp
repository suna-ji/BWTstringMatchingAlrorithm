#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <cstring>
#include <vector>
#include <cstdio>
#include <map>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
using namespace std;
#define _CRT_SECURE_NO_WARNINGS
#define N 10009//my genome�� ���� ó���� ������ �����ؼ� 3����� �ø�������
static int start_arr[4];
static int l;//������ ����->��������� search�ϱ� ���ؼ� ���������� �����ߴ�
static string find_genome1;
static int mis;
//static int insert_pos;
static string find_pattern;
static string mismatch_pattern;
/*
��������!!!!
���� N�� 1009��� ����
�׷� (1009-L)/L-overlap+2
�ε� �̶� overlap���� ������ ���������� �ȵǰ�
overlap���� �������� �������� 3�̻��̿����Ѵ�
*/
static inline string &rtrim(string &s) {
	s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
	return s;
}

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

class Mapping {
public:
	int index_pre;//���� ������ �ִ� index
	int index_post;
	int LF_pre;//LF Mapping�ϱ� ���� index
	int LF_post;
	char elem_pre;
	char elem_post;
	int sp_pre;
	int sp_post;
};
vector<Patt> *mix_pattern;
vector<Mapping> *map_table = new vector<Mapping>(N + 1);
void search(vector<Mapping> *tem_table, int index, int end, int p_n, string Pattern);
void pattern_maker();
void main()
{
	vector<BWT_T> *bwt_T_v = new vector<BWT_T>(N + 1);
	vector<BWT_T> *first_column_v = new vector<BWT_T>(N + 1);

	//����� ��µ� string �ʱ�ȭ
	string init = "*";
	for (int i = 0; i < N; i++)
		find_genome1.insert(i, init);

	//���ϸ����
	pattern_maker();

	//BWT(T)���ϱ�
	ifstream BWT_elem("c:\\BWT_elem.txt");
	if (BWT_elem.is_open())//������Ʈ36���� ���� bwt_T�� �о�ͼ� BWT(T)�����ϱ�
	{
		string tem;
		for (int i = 0; i <= N; i++)
		{
			getline(BWT_elem, tem);
			(*bwt_T_v)[i].elem = tem[0];
			//(*bwt_T_v)[i].index = N - i;
			if ((*bwt_T_v)[i].elem == 'A')
				(*bwt_T_v)[i].sp = 0;
			else if ((*bwt_T_v)[i].elem == 'C')
				(*bwt_T_v)[i].sp = 1;
			else if ((*bwt_T_v)[i].elem == 'G')
				(*bwt_T_v)[i].sp = 2;
			else if ((*bwt_T_v)[i].elem == 'T')
				(*bwt_T_v)[i].sp = 3;
			else if ((*bwt_T_v)[i].elem == '$')
				(*bwt_T_v)[i].sp = 4;
		}
	}
	BWT_elem.close();
	//BWT(T)�� �̿��ؼ� first column���ϱ�
	int A = 0;
	int C = 0;
	int G = 0;
	int T = 0;
	for (int i = 0; i <= N; i++)
	{
		if ((*bwt_T_v)[i].sp == 0)
			A++;
		else if ((*bwt_T_v)[i].sp == 1)
			C++;
		else if ((*bwt_T_v)[i].sp == 2)
			G++;
		else if ((*bwt_T_v)[i].sp == 3)
			T++;
	}
	start_arr[0] = 1;
	start_arr[1] = 1 + A;
	start_arr[2] = start_arr[1] + C;
	start_arr[3] = start_arr[2] + G;
	(*first_column_v)[0].elem = '$';
	(*first_column_v)[0].sp = 4;
	for (int i = 1; i <= A; i++)
	{
		(*first_column_v)[i].elem = 'A';
		(*first_column_v)[i].sp = 0;
	}
	for (int i = A + 1; i <= A + C; i++)
	{
		(*first_column_v)[i].elem = 'C';
		(*first_column_v)[i].sp = 1;
	}
	for (int i = A + C + 1; i <= A + C + G; i++)
	{
		(*first_column_v)[i].elem = 'G';
		(*first_column_v)[i].sp = 2;
	}
	for (int i = A + C + G + 1; i <= N; i++)
	{
		(*first_column_v)[i].elem = 'T';
		(*first_column_v)[i].sp = 3;
	}

	ifstream BWT_index("c:\\BWT_index.txt");
	if (BWT_index.is_open())//������Ʈ36���� ���� bwt_T�� �о�ͼ� BWT(T)�����ϱ�
	{
		string tem;
		for (int i = 0; i <= N; i++)
		{
			getline(BWT_index, tem);
			(*first_column_v)[i].index = stoi(tem);
		}
	}
	BWT_index.close();
	cout << "bwt�ϼ�" << endl;
	/*
	LF Mapping�ϱ����� ���̺� �����
	->�� ������ ������ �־���ϸ�
	���̺��� 0���� �տ��� ���� index������ ������ �־���Ѵ�
	*/
	(*map_table)[0].elem_pre = (*first_column_v)[0].elem;
	(*map_table)[0].index_pre = (*first_column_v)[0].index;
	(*map_table)[0].LF_pre = 0;
	int lf[4] = { 0 };
	int lf_specify;
	for (int i = 1; i <= N; i++)//�ϴ� table[][0]���� ä���
	{
		if ((*first_column_v)[i].sp == 0)
			lf_specify = 0;
		else if ((*first_column_v)[i].sp == 1)
			lf_specify = 1;
		else if ((*first_column_v)[i].sp == 2)
			lf_specify = 2;
		else if ((*first_column_v)[i].sp = 3)
			lf_specify = 3;
		(*map_table)[i].elem_pre = (*first_column_v)[i].elem;
		(*map_table)[i].index_pre = (*first_column_v)[i].index;
		(*map_table)[i].LF_pre = lf[lf_specify];
		(*map_table)[i].sp_pre = (*first_column_v)[i].sp;
		if ((*first_column_v)[i].sp == 0)
			lf[0]++;
		else if ((*first_column_v)[i].sp == 1)
			lf[1]++;
		else if ((*first_column_v)[i].sp == 2)
			lf[2]++;
		else if ((*first_column_v)[i].sp == 3)
			lf[3]++;

	}
	int lf_p[5] = { 0 };
	for (int i = 0; i <= N; i++)//���� table[][1]ä���
	{
		if ((*bwt_T_v)[i].elem == 'A')
			lf_specify = 0;
		else if ((*bwt_T_v)[i].elem == 'C')
			lf_specify = 1;
		else if ((*bwt_T_v)[i].elem == 'G')
			lf_specify = 2;
		else if ((*bwt_T_v)[i].elem == 'T')
			lf_specify = 3;
		else if ((*bwt_T_v)[i].elem == '$')
			lf_specify = 4;
		(*map_table)[i].elem_post = (*bwt_T_v)[i].elem;
		(*map_table)[i].LF_post = lf_p[lf_specify];
		(*map_table)[i].sp_post = (*bwt_T_v)[i].sp;
		if ((*bwt_T_v)[i].elem == 'A')
			lf_p[0]++;
		else if ((*bwt_T_v)[i].elem == 'C')
			lf_p[1]++;
		else if ((*bwt_T_v)[i].elem == 'G')
			lf_p[2]++;
		else if ((*bwt_T_v)[i].elem == 'T')
			lf_p[3]++;
	}
	vector<BWT_T>().swap(*bwt_T_v);
	vector<BWT_T>().swap(*first_column_v);

	cout << "map�ϼ�" << endl;
	

	//���� ã��
	ofstream result("C:\\result.txt");
	for (int i = 0; i < 30; i++) {
		find_pattern.insert(i, init);
		mismatch_pattern.insert(i, init);
	}

	int size = (N - 30)/ 20 + 2;
	for (int i = 0; i < size; i++)
	{
		l = (*mix_pattern)[i].pattern.size() - 1;
		int in;
		int end;
		if ((*mix_pattern)[i].pattern[l] == 'A') {
			in = start_arr[0];
			end = in + A - 1;
		}
		else if ((*mix_pattern)[i].pattern[l] == 'C')
		{
			in = start_arr[1];
			end = in + C - 1;
		}
		else if ((*mix_pattern)[i].pattern[l] == 'G')
		{
			in = start_arr[2];
			end = in + G - 1;
		}
		else if ((*mix_pattern)[i].pattern[l] == 'T')
		{
			in = start_arr[3];
			end = in + T - 1;
		}
		
		mis = 0;
		mismatch_pattern.replace(0, (*mix_pattern)[i].pattern.size(), (*mix_pattern)[i].pattern);
		find_pattern.replace(0, (*mix_pattern)[i].pattern.size(), (*mix_pattern)[i].pattern);
		if ((*mix_pattern)[i].pattern.size() < 30)//������ ���°� ����ó��
		{
			mismatch_pattern.erase((*mix_pattern)[i].pattern.size(), 30 - (*mix_pattern)[i].pattern.size());
			find_pattern.erase((*mix_pattern)[i].pattern.size(), 30 - (*mix_pattern)[i].pattern.size());
		}
	    search(map_table, in, end, l, (*mix_pattern)[i].pattern);
	}
	result << find_genome1 << endl;
	cout << "end" << endl;
}
void search(vector<Mapping> *tem_table, int index, int end, int p_n, string Pattern)
{
	
	int sub_table_size = end - index+1;
	if (p_n == 1)
	{
		int find_num = 0;
		for (int i = 0; i <= end; i++)
		{
			if ((*tem_table)[i].elem_post == Pattern[0])
			{
				find_num++;
				int find_position = start_arr[(*tem_table)[i].sp_post] + (*tem_table)[i].LF_post;
				int rfposi = (*map_table)[find_position].index_pre;
				find_genome1.replace(rfposi, find_pattern.size(),find_pattern);
				return;

			}
		}
		if (find_num == 0)
		{
			if (mis == 0)
			{
				mis++;
				string st_A = "A";
				string st_C = "C";
				string st_G = "G";
				string st_T = "T";

				if (Pattern[p_n - 1] == 'A')
				{
					
					mismatch_pattern.replace(p_n - 1, 1, st_C);
					search(tem_table, index, end, p_n, mismatch_pattern);//�״��sub_table�� ������ �ȵǾ����Ƿ� ���ĺ��ٲ㼭 ��������					
					mismatch_pattern.replace(p_n - 1, 1, st_G);
					search(tem_table, index, end, p_n, mismatch_pattern);
					mismatch_pattern.replace(p_n - 1, 1, st_T);
					search(tem_table, index, end, p_n, mismatch_pattern);
					return;
				}
				else if (Pattern[p_n - 1] == 'C')
				{
					
					mismatch_pattern.replace(p_n - 1, 1, st_A);
					search(tem_table, index, end, p_n, mismatch_pattern);//�״��sub_table�� ������ �ȵǾ����Ƿ� ���ĺ��ٲ㼭 ��������
					mismatch_pattern.replace(p_n - 1, 1, st_G);
					search(tem_table, index, end, p_n, mismatch_pattern);
					mismatch_pattern.replace(p_n - 1, 1, st_T);
					search(tem_table, index, end, p_n, mismatch_pattern);
					return;
				}
				else if (Pattern[p_n - 1] == 'G')
				{
					mismatch_pattern.replace(p_n - 1, 1, st_A);
					search(tem_table, index, end, p_n, mismatch_pattern);//�״��sub_table�� ������ �ȵǾ����Ƿ� ���ĺ��ٲ㼭 ��������
					mismatch_pattern.replace(p_n - 1, 1, st_C);
					search(tem_table, index, end, p_n, mismatch_pattern);
					mismatch_pattern.replace(p_n - 1, 1, st_T);
					search(tem_table, index, end, p_n, mismatch_pattern);
					return;
				}
				else
				{
					mismatch_pattern.replace(p_n - 1, 1, st_A);
					search(tem_table, index, end, p_n, mismatch_pattern);//�״��sub_table�� ������ �ȵǾ����Ƿ� ���ĺ��ٲ㼭 ��������
					mismatch_pattern.replace(p_n - 1, 1, st_C);
					search(tem_table, index, end, p_n, mismatch_pattern);
					mismatch_pattern.replace(p_n - 1, 1, st_G);
					search(tem_table, index, end, p_n, mismatch_pattern);
					return;
				}
			}
			else
			{
				return;
			}
			
		}
		
	}
	
	vector<Mapping> *sub_table = new vector<Mapping>(sub_table_size);
	int in;
	int sum = 0;
	for (int i = index; i <= end; i++)
	{
		if ((*tem_table)[i].elem_post == Pattern[p_n - 1])
		{
			(*sub_table)[sum].elem_pre = (*tem_table)[i].elem_post;
			(*sub_table)[sum].LF_pre = (*tem_table)[i].LF_post;
			(*sub_table)[sum].index_pre = start_arr[(*tem_table)[i].sp_post] + (*tem_table)[i].LF_post;
			in = (*sub_table)[sum].index_pre;
			(*sub_table)[sum].sp_pre = (*tem_table)[i].sp_pre;
			int t = in;
			(*sub_table)[sum].elem_post = (*map_table)[t].elem_post;
			(*sub_table)[sum].LF_post = (*map_table)[t].LF_post;
			(*sub_table)[sum].index_post = (*map_table)[t].index_post;
			(*sub_table)[sum].sp_post = (*map_table)[t].sp_post;
			sum++;
		}

	}

	if (sum == 0)
	{
		if (mis == 0)
		{
			mis++;
			string st_A = "A";
			string st_C = "C";
			string st_G = "G";
			string st_T = "T";
			
			if (Pattern[p_n - 1] == 'A')
			{
				mismatch_pattern.replace(p_n - 1, 1, st_C);
				search(tem_table, index, end, p_n, mismatch_pattern);//�״��sub_table�� ������ �ȵǾ����Ƿ� ���ĺ��ٲ㼭 ��������					
				mismatch_pattern.replace(p_n - 1, 1, st_G);
				search(tem_table, index, end, p_n, mismatch_pattern);
				mismatch_pattern.replace(p_n - 1, 1, st_T);
				search(tem_table, index, end, p_n, mismatch_pattern);
				return;
			}
			else if (Pattern[p_n - 1] == 'C')
			{
				mismatch_pattern.replace(p_n - 1, 1, st_A);
				search(tem_table, index, end, p_n, mismatch_pattern);//�״��sub_table�� ������ �ȵǾ����Ƿ� ���ĺ��ٲ㼭 ��������
				mismatch_pattern.replace(p_n - 1, 1, st_G);
				search(tem_table, index, end, p_n, mismatch_pattern);
				mismatch_pattern.replace(p_n - 1, 1, st_T);
				search(tem_table, index, end, p_n, mismatch_pattern);
				return;
			}
			else if (Pattern[p_n - 1] == 'G')
			{
				mismatch_pattern.replace(p_n - 1, 1, st_A);
				search(tem_table, index, end, p_n, mismatch_pattern);//�״��sub_table�� ������ �ȵǾ����Ƿ� ���ĺ��ٲ㼭 ��������
				mismatch_pattern.replace(p_n - 1, 1, st_C);
				search(tem_table, index, end, p_n, mismatch_pattern);
				mismatch_pattern.replace(p_n - 1, 1, st_T);
				search(tem_table, index, end, p_n, mismatch_pattern);
				return;
			}
			else
			{
				mismatch_pattern.replace(p_n - 1, 1, st_A);
				search(tem_table, index, end, p_n, mismatch_pattern);//�״��sub_table�� ������ �ȵǾ����Ƿ� ���ĺ��ٲ㼭 ��������
				mismatch_pattern.replace(p_n - 1, 1, st_C);
				search(tem_table, index, end, p_n, mismatch_pattern);
			    mismatch_pattern.replace(p_n - 1, 1, st_G);
				search(tem_table, index, end, p_n, mismatch_pattern);
				return;
			}
		}
		else
		{
			goto AA;
			return;
		}
    }
	search(sub_table, 0, sum - 1, p_n - 1, Pattern);
	AA:return;
	
}

void pattern_maker()
{
	vector<Patt> *Pattern;
	int size = (N -30) / 20 + 2;//20������ pattern�� 5�� ��ġ�ڴ� ->15���� �ݺ�//3���̷� 1�� ��ģ��->2���� �ݺ�
	Pattern = new vector<Patt>(size);
	string my_genome;
	string Text;
	ifstream Text36("C:\\Text36.txt");//������Ʈ36���� ������ mygenome�ڸ���
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
		for (int j = index; j < index +30; j++)
		(*Pattern)[a].pattern += my_genome[j];
		a++;
		index += 20;
	}
	for (int j = index; j <= N; j++)
		(*Pattern)[a].pattern += my_genome[j];
	(*Pattern)[a].pattern.erase((*Pattern)[a].pattern.length() - 1, 1);
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