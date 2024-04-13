#define _CRT_SECURE_NO_WARNINGS
#include"game.h"
void title()
{
	printf("************************\n");
	printf("*****1.pkay  2.quit*****\n");
	printf("************************\n");
}
void game()
{   
	char f = '2';
	//储放棋盘数据
	char arr[ROW][COL] = { 0 };//ROW表示行  COL表示列
	//数据初始化
	Initarr(arr, ROW, COL);
	while (1)
	{ 
		//清空屏幕
		system("CLS");
		//打印棋盘
		QIPAN(arr, ROW, COL);
		//玩家1下棋
		player1(arr, ROW, COL);
		//打印棋盘
		system("CLS"); QIPAN(arr, ROW, COL);
		//玩家2下棋
		//player2(arr, ROW, COL);
		//电脑下棋
		computer(arr, ROW, COL);
		//游戏判断
		system("CLS"); QIPAN(arr, ROW, COL);
		f=seetlement(arr,ROW,COL);
		if (f== '#')
		{
			//printf("电脑胜利\n");
			printf("玩家2胜利\n");
			system("pause");
			break;
		}
		else if (f=='*')
		{
			printf("玩家1胜利\n");
			system("pause");
			break;
		}
		else if (f == '0')
		{
			printf("平局\n");
			system("pause");
			break;
		}
	}
	//打印是否继续游戏
	printf("三子棋游戏\n");
}
void text()
{
	srand((unsigned int)time(NULL));//时间戳  ，放在这里可以使每下完一步不会被重置
	int input = 0;
	printf("三子棋游戏\n");
	do
	{
		title();
		printf("请输入:>");
		scanf("%d", &input);
		switch (input)
		{
		case 1:
			printf("三子棋游戏\n\n");
			game();
			break;
		case 2:
			printf("退出游戏成功\n");
			break;
		default :
			printf("输入错误,请重新输入\n\n");
		    break;
		}
	} while (input !=2);
}
int main()
{
	system("title  井字棋");
	system("color 1f");
	text();
	return 0;
}