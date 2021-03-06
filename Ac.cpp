// ConsoleApplication1.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;
const int maxn = 1e7 + 5;
const int MAX = 10000000;
char whitelist[1600100] = { '\n' };
char blackList[1000000][20];
int whitelistLen = 1;
int blackListRow = 0;
int cnt;
struct node {
	node *next[100000];
	node *fail;
	int sum;
};
node *root;

char key[70];
node *q[MAX];
int head, tail;
node *newnode;
char pattern[maxn];
int N;
int change(char ch)
{
	if (ch == '.')
		return 10;
	else if (ch == '\n')
		return 11;
	
	else
		return ch - '0';
}
void Insert(char *s)
{
	node *p = root;
	for (int i = 0; i < 26; i++) {
		p->next[i] = NULL;
	}

	for (int i = 0; s[i]; i++)
	{
		int x = change(s[i]);
		if (p->next[x] == NULL)
		{

			newnode = new node[sizeof(node)];
			newnode = (struct node *)malloc(sizeof(struct node));
			for (int j = 0; j<26; j++) newnode->next[j] = NULL;
			newnode->sum = 0; newnode->fail = 0;
			p->next[x] = newnode;
		}
		p = p->next[x];
	}
	p->sum++;
}
void openFile()                                                                       //打开文件，并将其内容储存在两个数组中
{
	int blackch, checkedch;                                                              //读取文件内容的ASCII码
	int blackListLine = 0;                                             //记录whitelist的列数，方便循环
	char blacklistFileName[60], checkedFileName[60];                                  //两个文件名
	FILE *balckListFile;                                                              //定义两个文件指针
	FILE *checkedFile;
	printf("Please input the blacklist file:\n");
	scanf_s("%s", blacklistFileName, 60);
	printf("Please input the checked file:\n");
	scanf_s("%s", checkedFileName, 60);
	if (fopen_s(&balckListFile, blacklistFileName, "r"))
	{
		printf("blacklist file opening failed!\n");
		system("pause");
		exit(0);
	}
	if (fopen_s(&checkedFile, checkedFileName, "r"))
	{
		printf("checked file opening failed!\n");
		system("pause");
		exit(0);
	}

	checkedch = fgetc(checkedFile);
	while (checkedch != EOF)
	{
		whitelist[whitelistLen] = checkedch;
		checkedch = fgetc(checkedFile);
		whitelistLen++;
	}
	whitelist[whitelistLen] = '\n';
	whitelistLen++;
	fclose(checkedFile);
	blackch = fgetc(balckListFile);
	while (blackch != EOF)
	{
		if (blackListLine == 0)
		{
			blackList[blackListRow][blackListLine] = '\n';
			blackListLine++;
		}
		blackList[blackListRow][blackListLine] = blackch;
		if (blackch == '\n')
		{
			blackListRow++;
			blackListLine = 0;
		}
		else
			blackListLine++;
		blackch = fgetc(balckListFile);
	}
	fclose(balckListFile);
}

void build_fail_pointer()
{
	head = 0;
	tail = 1;
	q[head] = root;
	node *p;
	node *temp;
	while (head < tail)
	{
		temp = q[head++];
		for (int i = 0; i <= 25; i++)
		{
			if (temp->next[i])
			{
				if (temp == root)
				{
					temp->next[i]->fail = root;
				}
				else
				{
					p = temp->fail;
					while (p)
					{
						if (p->next[i])
						{
							temp->next[i]->fail = p->next[i];
							break;
						}
						p = p->fail;
					}
					if (p == NULL) temp->next[i]->fail = root;
				}
				q[tail++] = temp->next[i];
			}
		}
	}
}
bool ac_automation(char *ch)
{
	node *p = root;
	int len = strlen(ch);
	for (int i = 0; i < len; i++)
	{
		int x = change(ch[i]);
		while (!p->next[x] && p != root)
			p = p->fail;
		p = p->next[x];
		if (!p)
			p = root;
		node *temp = p;
		while (temp != root)
		{
			if (temp->sum >= 0)
			{
				cnt += temp->sum;
				temp->sum = -1;
				return true;
			}
			else break;
			temp = temp->fail;
		}
	}
	return false;
}


int main()
{
	//先打开文件
	//读取文件中的字符
	//将之转化为对应的能够用字典树存储起来
	//
	openFile();
	Insert(whitelist);
	for (int row = 0; row < blackListRow; row++) {
		//对于blackList中存储的每一行元素都代表一个ip地址
		//构造一个字典树
		//然后构造fail指针
		build_fail_pointer();
		//然后去进行模式匹配
		//一次只能匹配一个模式串
		if (ac_automation(blackList[blackListRow]))
			printf("%s\n", blackList[blackListRow]);

	}
	system("pause");
	// int T;
	// scanf("%d",&T);
	// while(T--)
	// {
	//     root=(struct node *)malloc(sizeof(struct node));
	//     for(int j=0;j<26;j++) root->next[j] = 0;
	//     root->fail = 0;
	//     root->sum = 0;
	//     scanf("%d",&N);
	//     getchar();
	//     for(int i = 1; i <= N; i++)
	//     {
	//         gets(key);
	//         Insert(key);
	//     }
	//     gets(pattern);
	//     cnt = 0;
	//     build_fail_pointer();
	//     ac_automation(pattern);
	//     printf("%d\n",cnt);
	// }
	return 0;
}
