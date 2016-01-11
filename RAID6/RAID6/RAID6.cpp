#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

void ConstructBlock(char* block, int P)
{
	for (int i = 0; i < P-1; i++)
	{
		char label = char('A'+i);
		bool skip = false;
		int ptr = i;
		for (int j = P; j >= 0; j--)
		{
			if (!skip)
			{
				block[ptr*(P+1)+j] = label;
				ptr = (ptr+1)%(P-1);
				if (ptr == 0) skip = true;
			}
			else
			{
				skip = false;
			}
		}
	}
	int ptr = P - 2;
	for (int i = 1; i < P; i++)
	{
		block[ptr*(P+1)+i] = char('A'+P-1);
		ptr--;
	}
}

void Recover(int err_id1, int err_id2, char* block, int P)
{
	cout<<"Recovering...\n";
	if ((err_id1 != P) && (err_id2 != P)) 
	{
		int* count = (int *) malloc(sizeof(int)*(P-1));
		for (int i = 0; i < P-1; i++) count[i] = P;
		for (int i = P-2; i >= 0; i--)
		{
			int id = int(block[i*(P+1)+err_id1]-'A');
			if (id != P-1) count[id]--;
		}
		for (int i = P-2; i >= 0; i--)
		{
			int id = int(block[i*(P+1)+err_id2]-'A');
			if (id != P-1) count[id]--;
		}
		bool* flag = (bool *) malloc(sizeof(bool)*(P-1));
		for (int i = 0; i < P-1; i++) flag[i] = false;
		for (int i = 0; i < P-1; i++)
		{
			int recover_pattern;
			for (recover_pattern = 0; recover_pattern < P-1; recover_pattern++)
			{
				if ((count[recover_pattern] == P-1)&&(!flag[recover_pattern]))
				{
					flag[recover_pattern] = true;
					break;
				}
			}
            //digonal
			char tmp = char('A'+recover_pattern);
			bool find = false;
			int pos;
			for (int i = 0; i < P-1; i++)
			{
				if (block[i*(P+1)+err_id1] == tmp)
				{
					pos = i;
					find = true;
					break;
				}
			}
			if (find)
			{
				cout<<"Recover ("<<pos<<", "<<err_id1<<") using:";
				for (int i = 0; i < P-1; i++)
				{
					for (int j = 0; j < P+1; j++)
					{
						if ((j != err_id1) && (block[i*(P+1)+j] == tmp))
						{
							cout<<" ("<<i<<", "<<j<<")";
						}
					}
				}
				cout<<endl;
			}
			else
			{
				for (int i = 0; i < P-1; i++)
				{
					if (block[i*(P+1)+err_id2] == tmp)
					{
						pos = i;
						break;
					}
				} 
				cout<<"Recover ("<<pos<<", "<<err_id2<<") using:";
				for (int i = 0; i < P-1; i++)
				{
					for (int j = 0; j < P+1; j++)
					{
						if ((j != err_id2) && (block[i*(P+1)+j] == tmp))
						{
							cout<<" ("<<i<<", "<<j<<")";
						}
					}
				}
				cout<<endl;
			}
			//row
            if (find)
			{
				cout<<"Recover ("<<pos<<", "<<err_id2<<") using:";
				for (int i = 0; i < P ; i++)
				{
					if (i != err_id2)
					{
						cout<<" ("<<pos<<", "<<i<<")";
					}
				}
				int temp = int(block[pos*(P+1)+err_id2] - 'A');
				if (temp != P-1) count[temp]++;
				cout<<endl;
			}
			else
			{
				cout<<"Recover ("<<pos<<", "<<err_id1<<") using:";
				for (int i = 0; i < P ; i++)
				{
					if (i != err_id1)
					{
						cout<<" ("<<pos<<", "<<i<<")";
					}
				}
				int temp = int(block[pos*(P+1)+err_id1] - 'A');
				if (temp != P-1) count[temp]++;
				cout<<endl;				
			}
		}
	}
	else
	{
		int err = err_id1 + err_id2 - P;
		cout<<"Recover in parallel...\n";
		for (int i = 0; i < P-1; i++)
		{
			cout<<"Recover ("<<err<<", "<<i<<") using:";
			for (int j = 0; j < P; j++)
			{
				if (j != err)
				{
					cout<<" ("<<j<<", "<<i<<")";
				}
			}
			cout<<endl;
		}
		cout<<"Recover in parallel...\n";
		for (int i = 0; i < P-1; i++)
		{
			cout<<"Recover ("<<P<<", "<<i<<") using:";  
			bool skip = false;
			int ptr = i;
			for (int j = P; j >= 0; j--)
			{
				if (!skip)
				{	
					if (j != P) cout<<" ("<<j<<", "<<ptr<<")";
					ptr = (ptr+1)%(P-1);
					if (ptr == 0) skip = true;
				}
				else
				{
					skip = false;
				}
			}
			cout<<endl;
		}
	}
}

void PrintCutLine()
{
	cout<<"\n----------------------------------------------------\n";
}

void PrintPattern(char* block, int P)
{
	cout<<"Data Pattern\n\n";
	for (int i = 0; i < P-1; i++)
	{
		cout<<'D'<<i<<'\t';
	}
	cout<<"RP\tDP\n";
	for (int i = 0; i < P-1; i++)
	{
		for (int j = 0; j < P+1; j++)
		{
			cout<<block[i*(P+1)+j]<<'\t';
		}
		cout<<endl;
	}
}

int main()
{
	int P;
	cout<<"\n\nPlease input P:";
	cin>>P;
	char* block = (char*) malloc(sizeof(char)*(P-1)*(P+1));
	ConstructBlock(block, P);
	PrintCutLine();
	PrintPattern(block, P);
	PrintCutLine();
	int err_id1, err_id2;
	cout<<"Please input the id of two failed disk(0~P, P-1 --> RP, P --> DP):";
	cin>>err_id1>>err_id2;
	PrintCutLine();
	Recover(err_id1, err_id2, block, P);
	PrintCutLine();
	system("pause");
}
