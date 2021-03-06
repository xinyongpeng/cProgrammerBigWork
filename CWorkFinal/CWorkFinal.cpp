// CWorkFinal.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#pragma comment(linker, "/STACK:1024000000,1024000000")
char checkedList[1600100] = { '\n' }, blackList[1000000][20];   //定义黑名单数组和被检测的内容的数组，
//每个可容纳100000个ip地址，网址未知                      
int checkedListLen = 1, blackListRow = 0;    //记录最终checkedlist长度和blackList的行数
int treeNode[1600100][38] = {};       //树的节点，记录状态
int output[1600100] = {};      //记录结束状态的对应的blackList中的行数 ,注：储存的是第几行，如blackList[0]是第一行
int condition = 1;           //表示状态
int fail[16001000];

void openFile();                                                                     //打开文件
void insert();                                                                       //构造goto表
int transform(char ch);               //转化“.”和“\n”,“.”是10，“\n”是11,a到z分别为12至37，“\”是38
void getFail();
void match();                                                                        //匹配函数



int main()
{
	openFile();
	insert();
	getFail();
	match();
	system("pause");
    return 0;
}

void openFile()                                                                       //打开文件，并将其内容储存在两个数组中
{
	int blackch, checkedch;                                                              //读取文件内容的ASCII码
    int blackListLine = 0;                                             //记录checkedList的列数，方便循环
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
		checkedList[checkedListLen] = checkedch;
		checkedch = fgetc(checkedFile);
		checkedListLen++;
	}
	checkedList[checkedListLen] = '\n';
	checkedListLen++;
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

int transform(char ch)
{
	if (ch == '.')
		return 10;
	else if (ch == '\n')
		return 11;
	else if (ch == '\\')
		return 38;
	else if ((ch > 'a') && (ch < 'z'))
		return ch - 'a' + 12;
	else
		return ch - '0';
}


void insert()
{
	int now = 0;                                                     //记录状态
	for (int row = 0; row < blackListRow; row++)
	{
		for (int line = 0; line < strlen(blackList[row]); line++)
		{
			int ch = transform(blackList[row][line]);             //记录字符串编码
			if (!treeNode[now][ch])
			{
				treeNode[now][ch] = condition;
				now = condition;
				condition++;
			}
			else
				now = treeNode[now][ch];
			if ((ch == 11) && (now != 1))
			{
				output[now] = row + 1;
			}
		}
		now = 0;
	}
}

void getFail() 
{
	int point[16001000];                                          //跳转指针的状态，深度
	int head = 0, tail = 1;                                      //队列头尾	                                            
	point[1] = 0;                                                //状态1的跳转状态为0
	while (head != tail)                                         //有队列时
	{
		head++;
	    int k = point[head];                                     //k为head的跳转状态
		for (int i = 0; i < 37; i++)
		{
			if (treeNode[k][i] != 0)                             //字典树上的k状态下的i分支有字符时
			{
				int now = fail[k];                               //now为k状态的下一跳转状态
				while (treeNode[now][i] == 0 && now != 0)         //当字典树上的now状态的i分支无字符且now不为0状态时
					now = fail[now];                             //now的跳转状态就为它自己,循环直到now的跳转状态为0
				if (treeNode[now][i] != 0)                       //当字典树上的now状态的i分支有字符时
					fail[treeNode[k][i]] = treeNode[now][i];     //treeNode[k][i]跳转到treeNode[now][i]
				else
					fail[treeNode[k][i]] = 0;                    //否则，treeNode[k][i]的最终跳转状态为0
				if (k == 0)                                      //如果k为0状态
					fail[treeNode[k][i]] = 0;                    //treeNode[k][i]的跳转状态就为0
				point[++tail] = treeNode[k][i];                      //tail的跳转状态为k状态的i分支
			}
		}
	}
}

void match()
{
	int now = 0;                                                  //记录当前状态 
	int i = 0, k = 1;                                             //用于循环的变量 
	printf("\nMembers of blacklist:\n");
	for (i = 0; i < checkedListLen; i++)                          //依次读取字符 
	{
		int ch = transform(checkedList[i]);                      //读取字符，转换成约定的对应数字 
		while (treeNode[now][ch] == 0)                              //如果匹配不成功，则调用fail数组 
		{			
			if (now == 0)
				break;
			now = fail[now];
		}
		now = treeNode[now][ch];                                   //如果匹配成功，则跳到下一个状态
		if (output[now] != 0)
		{
			int r = output[now];                                   //匹配成功的排数
			for (k = 1; k < strlen(blackList[r - 1]); k++)
				printf("%c", blackList[r - 1][k]);
		}
	}
}
