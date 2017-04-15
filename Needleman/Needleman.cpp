#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>

using namespace std;

const int len = 5;

int find_acgt(string a, int x);  //map ACGT in the sequence to the ACGT scoring matrix

void needle(string a, string b);

int main()
{
	string a;
	string b;

	ifstream i1("a.txt");
	string line;

	if (i1)
	{
		while (getline(i1, line))
		{
			a += line;
		}
	}

	ifstream i2("b.txt");
	if (i2)
	{
		while (getline(i2, line))
		{
			b += line;
		}
	}

	//	cin >> a;
	//	cin >> b;
	needle(a, b);

	system("pause");
	return 0;
}

int find_acgt(string a, int x)
{
	switch (a[x - 1])
	{
	case '_': return 0;
	case 'A': return 1;
	case 'C': return 2;
	case 'G': return 3;
	case 'T': return 4;
	}
}

void needle(string a, string b)
{
	//first step : find the largest matching score


	int match_num;      //matching score

	string c;
	// the ACGT scoring matrix
	//_ = 0, A = 1, C = 2, G = 3, T = 4
	int score[len][len];
	for (int i = 0; i < len; i++)
		for (int j = 0; j < len; j++)
		{
			if (i == 0 && j == 0)
				score[i][j] = 0;
			else if (i == j)
				score[i][j] = 2;
			else
				score[i][j] = -1;
		}

	transform(a.begin(), a.end(), a.begin(), toupper);
	transform(b.begin(), b.end(), b.begin(), toupper);

	//get the length of the sequence
	int len_a = a.size();
	int len_b = b.size();

	len_a++;
	len_b++;
	//construct the sequence matrix
	int ** m = new int *[len_a];
	for (int i = 0; i < len_a; i++)
		m[i] = new int[len_b];

	m[0][0] = 0;
	//initialize the first row
	for (int i = 1; i < len_b; i++)
		m[0][i] = m[0][i - 1] + score[0][find_acgt(b, i)];
	//initialize the first column
	for (int j = 1; j < len_a; j++)
		m[j][0] = m[j - 1][0] + score[find_acgt(a, j)][0];

	//initialize the row and column
	for (int j = 1; j < len_a; j++)
		for (int i = 1; i < len_b; i++)
		{
			//match mismatch
			if ((m[j - 1][i - 1] + score[find_acgt(a, j)][find_acgt(b, i)]) >= (m[j][i - 1] + score[0][find_acgt(b, i)]) && (m[j - 1][i - 1] + score[find_acgt(a, j)][find_acgt(b, i)]) >= (m[j - 1][i] + score[find_acgt(a, j)][0]))
				m[j][i] = (m[j - 1][i - 1] + score[find_acgt(a, j)][find_acgt(b, i)]);
			//delete
			if ((m[j][i - 1] + score[0][find_acgt(b, i)]) >= (m[j - 1][i - 1] + score[find_acgt(a, j)][find_acgt(b, i)]) && (m[j][i - 1] + score[0][find_acgt(b, i)]) >= (m[j - 1][i] + score[find_acgt(a, j)][0]))
				m[j][i] = (m[j][i - 1] + score[0][find_acgt(b, i)]);
			//insert
			if ((m[j - 1][i] + score[find_acgt(a, j)][0]) >= (m[j][i - 1] + score[0][find_acgt(b, i)]) && (m[j - 1][i] + score[find_acgt(a, j)][0]) >= (m[j - 1][i - 1] + score[find_acgt(a, j)][find_acgt(b, i)]))
				m[j][i] = (m[j - 1][i] + score[find_acgt(a, j)][0]);
		}

	match_num = m[len_a - 1][len_b - 1];   //get the largest matching score

	//second step to backtrack
	int j = len_a - 1;
	int i = len_b - 1;

	string aa;
	string bb;

	c = a[j];
	aa.insert(0, c);
	c = b[i];
	bb.insert(0, c);
	while (i || j)
	{
		if (j == 0)
		{
			aa.insert(0, "_");
			c = b[--i];
			bb.insert(0, c);
			continue;
		}
		if (i == 0)
		{
			c = a[--j];
			aa.insert(0, c);
			bb.insert(0, "_");
			continue;
		}
		//if ai == bj backtrack to top left corner
		if (find_acgt(a, j) == find_acgt(b, i))
		{
			c = a[--j];
			aa.insert(0, c);
			c = b[--i];
			bb.insert(0, c);
		}
		//ai != bj backtrack to the top left corner, top, left which has the largest matching score
		//if they are same, left corner > top > left
		else if (m[j - 1][i - 1] >= m[j - 1][i] && m[j - 1][i - 1] >= m[j][i - 1])
		{
			c = a[--j];
			aa.insert(0, c);
			c = b[--i];
			bb.insert(0, c);
		}
		//top
		else if (m[j - 1][i] > m[j - 1][i - 1] && m[j - 1][i] >= m[j][i - 1])
		{
			c = a[--j];
			aa.insert(0, c);
			bb.insert(0, "_");
		}
		//left
		else
		{
			aa.insert(0, "_");
			c = b[--i];
			bb.insert(0, c);
		}
	}

	//output the largest matching score, new matching sequence aa, bb
	cout << match_num << endl;
	cout << aa << endl << bb << endl;

	//output to txt document
	ofstream f1;
	f1.open("aa.txt");
	f1 << aa;
	f1.close();

	ofstream f2;
	f2.open("bb.txt");
	f2 << bb;
	f2.close();


	delete[]m;
}
