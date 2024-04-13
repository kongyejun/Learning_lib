
//90坦克大战
//开始界面(650*650像素)
//胜利设计
//设计玩家控制的坦克
//出生点设计
//随机地图设计

#include<graphics.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<assert.h>

#pragma comment(lib,"Winmm.lib")
#include<mmsystem.h>

enum basic {
	width = 650,//开始画面宽度
	length = 650,//开始画面长度
	p_size = 25,//基本图像单位
	enemy_num = 8,//敌人数,
	blood = 5,
	W_wall = 1,//瓦墙类代码
	S_wall = 2,//钢墙类代码
	STRH_wall = 3,//大本营类型代码
	P_wall = 4,//玩家类型
	E_wall = 5,//敌人类型
	Mag = 1,//伤害
	P_fire_stop=4,//玩家开火后坦克行动冷却时间
	G_width=800,//游戏画面宽度
	G_length=650,//游戏画面长度
};
#define PI 3.1415

//开始界面的矩形设置
int R1[4] = { 104,279,264,339 };
int R2[4] = { 404,279,564,339 };
//初始化玩家,大本营血条
int P_Blood;
int PS_Blood;
int cont;//全局敌人数
short G_Map[width / p_size][length / p_size] = { 0 };

struct Unity
{
	Unity* father;
	int xy[2];
	int S;//总计
	int H;//曼哈顿路径长度
	int G;//移动距离
};
struct QO1
{
	Unity* o[100];
	int max;
};
struct QO2
{
	Unity* o[200];
	int max;
};
class bullet
{
private:
	int z = 0;//伤害
	int C = 0;
	IMAGE BN, BS, BW, BE, BZ;
	bool PD = 0;
public:
	int xpos = 0;
	int ypos = 0;
	int s = 0;//运行距离
	void init(int x, int y, int c,bool source)
	{
		xpos = x, ypos = y;
		z = Mag;//伤害计算
		C = c;
		PD = source;
		loadimage(&BN, L"子弹.png");
		loadimage(&BS, L"子弹S.png");
		loadimage(&BW, L"子弹L.png");
		loadimage(&BE, L"子弹R.png");
	}
	int sports()//C表示炮筒朝向
	{
		int i = 0;
		if (s < 6)
		{
			switch (C)
			{
			case 0:(i = display(xpos, ypos - 1)) == 0 ? 0 : s = 7; ypos -= 1; break;
			case 1:(i = display(xpos + 1, ypos)) == 0 ? 0 : s = 7; xpos += 1; break;
			case 2:(i = display(xpos, ypos + 1)) == 0 ? 0 : s = 7; ypos += 1; break;
			case 3:(i = display(xpos - 1, ypos)) == 0 ? 0 : s = 7; xpos -= 1; break;
			}
			s++;
		}
		else return 1;//表示可以开火
		return i;
	}

	int display(int var1, int var2)//x，y
	{
		if (!(var2 >= 0 && var2 < (width / p_size)) && !(var1 < (length / p_size) && var1 >= 0))
			return 1;//导弹出界
		int i = IS_hunrt(var1, var2);//判断导弹的状态
		if (!i)//当导弹任在空地运行时
		{
			switch (C)
			{
			case 0:putimage(var1 * p_size, var2 * p_size, &BN); break;
			case 1:putimage(var1 * p_size, var2 * p_size, &BE); break;
			case 2:putimage(var1 * p_size, var2 * p_size, &BS); break;
			case 3:putimage(var1 * p_size, var2 * p_size, &BW); break;
			}
		}
		else
		{
			loadimage(&BZ, L"爆炸.png");
			putimage(var1 * p_size, var2 * p_size, &BZ);
		}
		return i;//返回导弹的信息
	}
	int IS_hunrt(int var1, int var2)
	{
		switch (G_Map[var2][var1])
		{
		case 0:return 0;//在空地飞行，炮弹未销毁
		case 1:G_Map[var2][var1] = 0; return 1;//撞到瓦墙，销毁瓦墙
		case 2:return 2;//撞到钢墙
		case 3:P_Blood -= z; return 3;//击中大本营(舍弃了大本营血量)(可以自己设计)
		case 4:P_Blood -= z; return 4;//击中玩家
		case 5:if (PD)cont--; return 5;//击中电脑(只有玩家生效)
		}
		return -1;
	}
};

class Stronghold
{
private:
	IMAGE stronghold;
public:
	int xpos = 0;
	int ypos = 0;
	void init_xy(short** G_Map)
	{
		xpos = rand() % ((length / p_size) - 1);
		ypos = rand() % ((width / p_size) - 1);
		loadimage(&stronghold, L"大本营.jpg");
		*((short*)G_Map + (length / p_size) * (ypos + 1) + xpos) = STRH_wall;
		*((short*)G_Map + (length / p_size) * ypos + xpos) = STRH_wall;
		*((short*)G_Map + (length / p_size) * ypos + (xpos + 1)) = STRH_wall;
		*((short*)G_Map + (length / p_size) * (ypos + 1) + (xpos + 1)) = STRH_wall;
	}
	void put()
	{
		putimage(xpos * p_size, ypos * p_size, &stronghold);
	}
};

class Tank_P
{
private:
	//yunyong
	int xpos = 0;//x坐标
	int ypos = 0;//y坐标
	IMAGE TN, TE, TW, TS;
	ExMessage msg = { 0 };
	bullet D;
	
public:
	int Towards = 0;//0表示北边，1表示东边，2表示南边，3表示西边
	int stop;//熄火
	//移动设置
	void put_image_p()//打印图片
	{
		switch (Towards)
		{
		case 0:putimage(xpos * p_size, ypos * p_size, &TN); break;//北
		case 1:putimage(xpos * p_size, ypos * p_size, &TE); break;//东
		case 2:putimage(xpos * p_size, ypos * p_size, &TS); break;//南
		case 3:putimage(xpos * p_size, ypos * p_size, &TW); break;//西
		default: break;
		}
	}

	void set_blocks(short** map)
	{
		*((short*)map + (length / p_size) * ypos + xpos) = P_wall;
	}
	void clear_blocks(short** map)
	{
		*((short*)map + (length / p_size) * ypos + xpos) = 0;
	}

	void IF_H_ob(char I, short** map, int* pos)//障碍检测
	{
		if (*((short*)map + (length / p_size) * ypos + xpos) != 0)
		{
			switch (I)
			{
			case'+':(*pos) = (*pos) - 1; break;
			case'-':(*pos) = (*pos) + 1; break;
			default:break;
			}
		}
	}
	void move(short** map)//传入二维地图
	{
		clear_blocks(map);
		//方法二：
		if (((GetKeyState('w') & 0x8000) || (GetKeyState('W') & 0x8000)) && stop == 0)
		{
			ypos -= ypos - 1 < 0 ? 0 : 1;
			Towards = 0; IF_H_ob('-', map, &ypos);
		}
		else if (((GetKeyState('s') & 0x8000) || (GetKeyState('S') & 0x8000)) && stop == 0)
		{
			ypos += ypos + 1 >= (width / p_size) ? 0 : 1, Towards = 2;
			IF_H_ob('+', map, &ypos);
		}
		else if ((GetKeyState('d') & 0x8000) || (GetKeyState('D') & 0x8000) && stop == 0)
		{
			xpos += xpos + 1 >= (length / p_size) ? 0 : 1, Towards = 1;
			IF_H_ob('+', map, &xpos);
		}
		else if ((GetKeyState('a') & 0x8000) || (GetKeyState('A') & 0x8000) && stop == 0)
		{
			xpos -= xpos - 1 < 0 ? 0 : 1; Towards = 3;
			IF_H_ob('-', map, &xpos);
		}
		set_blocks(map);
	}

	int fire(short** map)
	{
		if ((GetKeyState('j') & 0x8000 || GetKeyState('J') & 0x8000) && D.s > 5)
		{
			D.s = 0;
			D.init(xpos, ypos, Towards,1);
			stop = P_fire_stop;
		}
		return D.sports();
	}

	void init_xy(int x, int y)
	{
		xpos = x, ypos = y;
		stop = 0;
		D.s = 8;//大于7表示可以开火，等于0表示刚开过火
		loadimage(&TN, L"玩家.jpg");
		rotateimage(&TE, &TN, -PI / 2);
		rotateimage(&TS, &TN, PI);
		rotateimage(&TW, &TN, (PI / 2));
	}
	int get_D_xpos()
	{
		return D.xpos;
	}
	int get_D_ypos()
	{
		return D.ypos;
	}
	int getxpos()
	{
		return xpos;
	}
	int getypos()
	{
		return ypos;
	}
};

class Tank_C
{
private:
	bullet D;
	IMAGE CN, CE, CS, CW;
	QO1* open;
	int path[150];
	int T = 0;//遍历path用
public:
	int Towards = 0;//0表示北边，1表示东边，2表示南边，3表示西边
	int xpos = 0, ypos = 0;
	int stop = 0;
	short key_map[width / p_size][length / p_size] = { 0 };
	QO2* close;

	void init(short** map, int x, int y)
	{
		T = 0; cont++;
		xpos = rand() % (length / p_size), ypos = rand() % (width / p_size);
		D.s = 8;//导弹设置为未运行(s>6)
		Towards = 0;//初始朝向
		stop = rand() % 5;//当stop为0时才能开始行动

		//使用前需要将初始化变量
		loadimage(&CS, L"电脑.jpg");
		rotateimage(&CN, &CS, PI);
		rotateimage(&CE, &CN, -(PI / 2));
		rotateimage(&CW, &CN, (PI / 2));
		//随机出生点----不合适故废除
		//while (G_Map[ypos][xpos]) { xpos = rand() % (length / p_size), ypos = rand() % (width / p_size); }
		//在一定距离外生成
		while (G_Map[ypos][xpos] || !(abs(xpos - x) > 8 && abs(ypos - y) > 8))
		{
			xpos = rand() % (length / p_size), ypos = rand() % (width / p_size);
		}
		//设置脚下方块
		set_blocks();
		//初始化队列
		open = (QO1*)malloc(sizeof(QO1));
		close = (QO2*)malloc(sizeof(QO2));
		if (close) memset(close, 0, sizeof(QO2));
		else assert(close);
		if (open) memset(open, 0, sizeof(QO1));
		else assert(open);
		A_move(x, y);
	}

	void set_blocks()
	{
		G_Map[ypos][xpos] = E_wall;
	}

	void clear_blocks()
	{
		G_Map[ypos][xpos] = 0;
	}

	void put_image()
	{
		switch (Towards)
		{
		case 0:putimage(xpos * p_size, ypos * p_size, &CN); break;//北
		case 1:putimage(xpos * p_size, ypos * p_size, &CE); break;//东
		case 2:putimage(xpos * p_size, ypos * p_size, &CS); break;//南
		case 3:putimage(xpos * p_size, ypos * p_size, &CW); break;//西
		default: break;
		}
	}
	bool IS_out(int i, int j)
	{
		if (xpos + j<0 || xpos + j>(length / p_size));
		if (ypos + i<0 || ypos + i>(width / p_size));
		if ((i + j) == 1 || (i + j) == -1)
			return 1;
		return 0;
	}

	//A*算法-----实现Ai寻路
	bool A_move(int x, int y)//返回0表示无路径
	{
		memset(path, 0, sizeof(int) * 150);
		memset(key_map, 0, sizeof(short) * (length / p_size) * (width / p_size));
		//(1)放入起点
		Unity* now = (Unity*)malloc(sizeof(Unity));//开辟空间存储‘起点’数据
		if (!now)assert(now);//如果开辟失败报错
		
		now->xy[0] = xpos, now->xy[1] = ypos, now->G = 0;
		now->father = NULL;
		(open->o[(open->max)++]) = now;//加入到open表中，并时max+1

		bool is_continue = true;
		int  i = 0, index = 0;//i用于辅助统计(遍历)，index用于数据位置		
		while (is_continue)
		{

			//判断open表是否为空，如果为空表示无路径
			if (open->max == 0)return 0;

			//寻找到S最小的格子
			for (i = 1, now = open->o[0], index = 0; i < open->max; i++)
			{
				if (open->o[i]->S < now->S)
				{
					now = open->o[i];
					index = i;
				}
				else if (open->o[i]->S == now->S)//当总代价一样时,比较曼哈顿距离
				{
					if (open->o[i]->H < now->H)
					{
						now = open->o[i];
						index = i;
					}
				}
			}
			//测试时的写法
			//for (i = 1, now = open->o[0], index = 0; i < open->max; i++) { if (open->o[i]->S < now->S) { now = open->o[i]; index = i; } else if (open->o[i]->S == now->S) { if (open->o[i]->H < now->H) { now = open->o[i]; index = i;}}}
			
			//因为取出了中心节点为此需要将open中的max（数据量）-1
			open->max -= 1;
			//将被访问节点放入close中,并标记以访问，从open表中删除
			open->o[index] = open->o[open->max];//采用尾值覆盖的方法
			key_map[now->xy[1]][now->xy[0]] = 1;
			close->o[close->max] = now; close->max += 1;

			//遍历周围的四个方位	
			for (int H = -1; H < 2; H++)//按行展开
			{
				for (int L = -1; L < 2; L++)
				{
					//判断是否为东，南，西，北（正方向）
					if (abs(H + L) == 1)
					{
						//判断是否出边界
						if (now->xy[0] + L < (length / p_size) && now->xy[1] + H < (width / p_size) && (now->xy[0] + L >= 0) && (now->xy[1] + H >= 0))
						{
							//判断是否为已访问
							if (key_map[now->xy[1] + H][now->xy[0] + L] == 0 && is_not_in_open(now->xy[0] + L, now->xy[1] + H))
							{
								open->o[open->max] = (Unity*)malloc(sizeof(Unity));
								open->o[open->max]->father = now;//指向上一步
								open->o[open->max]->xy[0] = now->xy[0] + L;
								open->o[open->max]->xy[1] = now->xy[1] + H;
								open->o[open->max]->S = -1;//预标记为无效路径(如果该点真有效，则之后会对其覆盖)

								if (G_Map[now->xy[1] + H][now->xy[0] + L] == 0)//如果为空地
								{
									open->o[open->max]->G = now->G + 1;
									open->o[open->max]->H = abs(now->xy[0] + L - x) + abs(now->xy[1] + H - y);
									open->o[open->max]->S = (open->o[open->max]->G) + (open->o[open->max]->H);
									open->max += 1;
								}
								else if (G_Map[now->xy[1] + H][now->xy[0] + L] == 1)//如果为瓦墙则想要通过它必须要多消费3点代价
								{
									open->o[open->max]->G = now->G + 1;
									open->o[open->max]->H = abs(now->xy[0] + L - x) + abs(now->xy[1] + H - y) + 3;
									open->o[open->max]->S = (open->o[open->max]->G) + (open->o[open->max]->H);
									open->max += 1;
								}
								//如果是终点则结束寻找
								else if (G_Map[now->xy[1] + H][now->xy[0] + L] == G_Map[y][x])
								{
									H = 2, L + 2;//终止寻找四周方格
									is_continue = false;//终止整个搜索
								}
								//虽然还有其他情况（该点被其他坦克占用/该点为不可摧毁钢墙/不是目标类型）\
								  但都可以不考虑,不加入到open表中，自然也就不会走这条路了\
								  并且由于不会进行max+=1操作，所以下一次操作会将该无效方格覆盖
							}
						}
					}
				}
			}
		}
		//一旦寻找到终点，则此刻的open表中的最后一位将为终点
		Unity* F = open->o[open->max];
		path[F->father->G + 1] = -1;
		for (; F->father != NULL; F = F->father)
		{
			//0表示北边，1表示东边，2表示南边，3表示西边
			switch (F->father->xy[0] - F->xy[0])
			{
			case -1:path[F->father->G] = 1; break;
			case 0://表示南北方向
				if (F->father->xy[1] - F->xy[1] == -1)path[F->father->G ] = 2;
				else path[F->father->G] = 0;
				break;
			case 1:path[F->father->G] = 3; break;
			}
		}

		//释放动态内存
		for (i = 0; i <= open->max; i++)
		{
			free(open->o[i]);
			open->o[i] = NULL;
		}
		open->max = 0;
		for (i = 0; i <= close->max; i++)
		{
			free(close->o[i]);
			close->o[i] = NULL;
		}
		close->max = 0;
		now = NULL; F = NULL; 
		T = 0;
		return 1;
	}

	void move()
	{
		clear_blocks();
		if (path[T] == -1)
			fire(1);
		else if (stop == 0)
		{
			switch (path[T++])
			{
			case 0:Towards = 0;
				if (G_Map[ypos - 1][xpos] == 0) {ypos -= 1; stop = 5;}
				else { fire(1); T -= 1; }
				break;
			case 1:Towards = 1;
				if (G_Map[ypos][xpos + 1] == 0) { xpos += 1; stop = 5;}
				else { fire(1); T -= 1; }
				break;
			case 2:Towards = 2;
				if (G_Map[ypos + 1][xpos] == 0) { ypos += 1; stop = 5; }
				else { fire(1); T -= 1; }
				break;
			case 3:Towards = 3;
				if (G_Map[ypos][xpos - 1] == 0) { xpos -= 1; stop = 5; }
				else { fire(1); T -= 1; }
				break;
			}
		}
		set_blocks();
		put_image(); 
	}

	bool is_not_in_open(int X,int Y)
	{
		int i = 0;
		for (; i < open->max&&!(open->o[i]->xy[0] == X && open->o[i]->xy[1] == Y); i++);
		if (i == open->max)return 1;
		else return 0;
	}

	int fire(bool F)
	{
		if (F && D.s > 6 && stop == 0)
		{
			D.s = 0;
			D.init(xpos, ypos, Towards, 0);//参数为x，y坐标，炮口朝向，炮弹来源是否是玩家
			stop = 8;
		}
		return D.sports();
	}
};


void start_window()
{
	cleardevice();
	//界面打印
	settextstyle(100, 0, L"楷体", 0, 0, FW_BLACK, true, 0, 0);
	settextcolor(YELLOW);
	outtextxy(40, 100, L"坦 克 大 战");
	//矩形对话框
	RECT r1[4] = { R1[0],R1[1],R1[2],R1[3] };
	RECT r2[4] = { R2[0],R2[1],R2[2],R2[3] };
	setfillcolor(RGB(105, 217, 78));
	solidrectangle(R1[0], R1[1], R1[2], R1[3]);
	solidrectangle(R2[0], R2[1], R2[2], R2[3]);
	settextstyle(25, 0, L"宋体");
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	drawtext(L"操作说明", r1, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	drawtext(L"开始游戏", r2, DT_CENTER | DT_SINGLELINE | DT_VCENTER);

}

bool LBC_cope(ExMessage msg)
{
	//处理点击操作说明
	if (msg.x >= R1[0] && msg.x <= R1[2] && msg.y >= R1[1] && msg.y <= R1[3])
	{
		IMAGE OPIN;
		loadimage(&OPIN, L"操作按钮说明.png");
		//Resize(&OPIN, 300, 211);
		putimage(68, 359, &OPIN);
	}
	else
	{
		//当鼠标离开时清空该区域
		clearrectangle(68, 359, 650, 650);
	}
	return true;
}

bool  LBS_cope(ExMessage msg)
{
	if ((msg.x >= R2[0] && msg.y >= R2[1]) && (msg.x <= R2[2] && msg.y <= R2[3]))
		return 0;
	return 1;
}
//界面控制
bool W_button()
{
	ExMessage msg;
	bool i = 1;//判断是否还循环在开始时界面
	while (i)
	{
		getmessage(&msg, EX_MOUSE | EX_KEY);
		switch (msg.message)
		{
		case WM_KEYDOWN://ESC键处理
			if (msg.vkcode == VK_ESCAPE)
				return 0;
		case WM_LBUTTONDOWN://鼠标右键处理
			i = LBS_cope(msg);
			break;
		case WM_MOUSEMOVE://鼠标移动处理
			i = LBC_cope(msg);
			break;
		default:continue;
		}
		flushmessage();//因为机器运行速度特别快，所以在1秒内会有n多杂糅数据待读，所以要清除
		Sleep(50);//休眠50毫秒（windows下以毫秒为单位）
	}
	return 1;
}

void map(int L, IMAGE* BJ)
{

	if (L)//判断是否需要设置障碍物
	{
		int x = 0, y = 0;
		for (int i = 0; i < 350; i++)
		{
			x = rand() % (length / p_size);
			y = rand() % (width / p_size);
			if (G_Map[y][x])//判断是否已经有障碍物
				i--;
			else
			{
				G_Map[y][x] = (rand() % 10 < 7) ? 1 : 2;//1/7的概率是钢墙，6/7的概率是瓦墙
			}
		}
	}
	else
	{
		putimage(0, 0, BJ);
		IMAGE S_W, T_W;//定义钢墙，瓦墙的图片结构的实例对象
		loadimage(&S_W, L"钢墙.png");
		loadimage(&T_W, L"瓦墙.png");
		for (int x = 0; x < length / p_size; x++)
		{
			for (int y = 0; y < width / p_size; y++)
			{
				if (G_Map[y][x])
				{
					if (G_Map[y][x] == S_wall)
					{
						putimage(x * p_size, y * p_size, &S_W);
					}
					else if (G_Map[y][x] == W_wall)
					{
						putimage(x * p_size, y * p_size, &T_W);
					}
				}
			}
		}
	}
}

bool is_near_player(int x, int y, int C)
{
	int i = 1;
	switch (C)
	{
	case 0:for (; i < 6 && G_Map[y - i][x] != 2; i++);
		if (i == 6)return 1; break;
	case 1:for (; i < 6 && G_Map[y][x + i] != 2; i++);
		if (i == 6)return 1; break;
	case 2:for (; i < 6 && G_Map[y + i][x] != 2; i++);
		if (i == 6)return 1; break;
	case 3:for (; i < 6 && G_Map[y][x - i] != 2; i++);
		if (i == 6)return 1; break;
	}
	return 0;

}


void set_enemy(Tank_C** List, short** G_Map, int x, int y)
{
	for (int i = 0; i < enemy_num; i++)
	{
		List[i] = new Tank_C;
		Tank_C* A = *(List + i);
		A->init(G_Map, x, y);
		A->put_image();
	}
}

int IS_over()
{
	if (cont == 0) { return 0; }
	else if (P_Blood == 0) { return 1; }
	return 2;
}


void enemy_move(Tank_C* base[enemy_num],Tank_P player)
{
	int p = 0;
	for (p = 0; p < enemy_num; p++)
	{
		if (base[p] != NULL)
		{
			if (base[p]->stop == 0)
			{
				//等待开发
				if ((p % 2) == 0) { base[p]->move(); }//大本营
				else { base[p]->A_move(player.getxpos(), player.getypos()); base[p]->move(); }//玩家
			}
			else { base[p]->stop -= 1; base[p]->put_image(); }

			if (player.getxpos() == base[p]->xpos)
			{

				base[p]->Towards = base[p]->ypos - player.getypos() < 0 ? 2 : 0;
				if (is_near_player(base[p]->xpos, base[p]->ypos, base[p]->Towards))
				{
					base[p]->put_image();
					base[p]->fire(1);
				}
			}
			else if (player.getypos() == base[p]->ypos)
			{
				base[p]->Towards = base[p]->xpos - player.getxpos() < 0 ? 1 : 3;
				if (is_near_player(base[p]->xpos, base[p]->ypos, base[p]->Towards))
				{
					base[p]->put_image();
					base[p]->fire(1);
				}
			}
		}
	}

}

void Health_Points(IMAGE* B)
{
	int i = 0;
	for (; i < P_Blood; i++)
	{
		putimage(35 + i * 65, 680, B, SRCPAINT);
	}
}

int play( IMAGE* BJ,IMAGE* B,IMAGE* XL)
{
	int x, y, k;
	//初始化大本营位置
	Stronghold S;
	S.init_xy((short**)G_Map);
	x = S.xpos, y = S.ypos;
	//初始化玩家,大本营血条
	P_Blood = blood;
	//PS_Blood = blood;
	//生成敌人
	int  p = 0;
	Tank_C* base[enemy_num] = { NULL };
	cont = 0;
	set_enemy(base,(short**) G_Map, S.xpos, S.ypos);
	//初始化坦克位置
	while ((abs(x - S.xpos) > 7 || abs(y - S.ypos) > 7) || G_Map[y][x]>=2)
	{
		x = rand() % (length / p_size);
		y = rand() % (width / p_size);
	}
	Tank_P player;
	player.init_xy(x, y);
	player.set_blocks((short**)G_Map);
	while ((k = IS_over()) == 2)
	{
		BeginBatchDraw();
		cleardevice();
		//绘制地图
		map(0, BJ);
		S.put();
		player.move((short**)G_Map);
		player.put_image_p();
		player.stop > 0 ? player.stop -= 1 : 0;
		putimage(0, 650, XL);
		Health_Points(B);
		//是否击中敌人
		if (player.fire((short**)G_Map) == 5)
		{
			for (p = 0; p < enemy_num; p++)
			{
				if (base[p] != NULL && base[p]->xpos == player.get_D_xpos() && base[p]->ypos == player.get_D_ypos())
				{
					base[p]->clear_blocks();
					free(base[p]);
					base[p] = NULL;
				}
			}
		}
		//敌人移动
		enemy_move(base,player);
		FlushBatchDraw();
		//system("pause");
		Sleep(100);
	}
	return k;
}

bool NEXT()
{

	ExMessage M;
	while (true)
	{
		getmessage(&M,EX_MOUSE);
		if (M.lbutton && (M.x > 54 && M.x < 250) && (M.y > 490 && M.y < 580))
		{
			return 0;
		}
		else if (M.lbutton && (M.x > 388 && M.x < 600) && (M.y > 499 && M.y < 564))
		{
			return 1;
		}
	}


}

int main()
{
	
	bool IS_NEXT = true;
	//开始于2023/06/19
	IMAGE BJ,XL;
	IMAGE pw, cw, B;
	loadimage(&pw, L"玩家胜利.png");
	loadimage(&cw, L"玩家失败.jpg");
	loadimage(&BJ, L"背景.jpg");
	loadimage(&XL, L"信息栏背景.png");
	loadimage(&B, L"心.jpg");
	srand((unsigned)time(NULL));//设置随机数种子
	//mciSendString(L"open 爆炸声.mp3 alias 爆炸声", NULL, 0, NULL);//声音设置（有兴趣再开发）
	//mciSendString(L"close 爆炸声", NULL, 0, NULL);
	while (IS_NEXT)
	{
		initgraph(width, length, EX_SHOWCONSOLE | EW_DBLCLKS);
		start_window();
		memset(G_Map, 0, sizeof(G_Map));
		if (W_button())
		{
			closegraph();
			initgraph(G_length,G_width, EX_SHOWCONSOLE | EW_DBLCLKS);
			//清除开始界面
			map(1, &BJ);
			//正式开始
			IS_NEXT = play(&BJ,&B,&XL);
			EndBatchDraw();
		}
		closegraph();
		initgraph(width, length, EX_SHOWCONSOLE | EW_DBLCLKS);
		//结束
		switch (IS_NEXT)
		{
		case 0:putimage(0, 0, &pw); break;
		case 1:putimage(0, 0, &cw); break;
		}
		IS_NEXT = NEXT();
		closegraph();
	}

	return 0;
	//第一次完成于2023/6/26
}



//void original_route(int x, int y)
//{
//	int num[4];
//	int X=xpos, Y=ypos;
//	int min = 0;
//	while (true)
//	{
//		min = 0;
//		memset(num, 1111, sizeof(int) * 4);
//		G_Map[Y + 1][X] < 2 ? num[0] = abs(y - Y - 1) + abs(x - X) : 0;//'s'---0
//		G_Map[Y][X + 1] < 2 ? num[1] = abs(y - Y) + abs(x - X - 1) : 0;//'d'---1
//		G_Map[Y - 1][X] < 2 ? num[2] = abs(y - Y + 1) + abs(x - X) : 0;//'w'---2
//		G_Map[Y][X - 1] < 2 ? num[3] = abs(y - Y) + abs(x - X + 1) : 0;//'a'---3
//		MOVE.M[MOVE.W] = 0;
//		for (int i = 0; i < 4; i++)
//		{
//			if (num[min] > num[i])
//			{
//				min = i;
//				MOVE.M[MOVE.W] = i;
//			}
//		}
//		switch (min)
//		{
//		case 0:Y += 1; break;
//		case 1:X += 1; break;
//		case 2:Y -= 1; break;
//		case 3:X -= 1; break;
//		}
//		MOVE.W++;
//		if (G_Map[Y + 1][X] == 3 || G_Map[Y][X + 1] == 3 || G_Map[Y - 1][X] == 3 || G_Map[Y][X - 1] == 3)
//		{
//			break;
//		}
//	}
//}
//void move(int x, int y)
//{
//	if (!stop)
//	{
//		/*int k = 0;
//		int next[3] = { 0 }, cont_p = 0, cont = 1000000;
//		for(int i=-1;i<2;i++)
//			for (int j = -1; j < 2; j++)
//			{
//				if (IS_out(i, j))
//				{
//					k = *((short*)map + (length / p_size) * (ypos + i) + xpos + j);
//					if (k == W_wall)
//					{
//						cont_p =3+ ((y - (ypos + i)) < 0 ? ypos + i - y : y - ypos - i) + ((x - xpos - j) < 0 ? xpos + j - x : x - xpos - j);
//						if (cont > cont_p)
//						{
//							next[0] = i, next[1] = j, next[2] = 1;
//							cont = cont_p;
//						}
//					}
//					else if (k == 0)
//					{
//						if (S[1] == ypos + i && S[0] == xpos + j)
//							cont_p =7+((y - (ypos + i)) < 0 ? ypos + i - y : y - ypos - i) + ((x - xpos - j) < 0 ? xpos + j - x : x - xpos - j);
//						else
//							cont_p = ((y - (ypos + i)) < 0 ? ypos + i - y : y - ypos - i) + ((x - xpos - j) < 0 ? xpos + j - x : x - xpos - j);
//						if (cont > cont_p)
//						{
//							next[0] = i, next[1] = j, next[2] = 0;
//							cont = cont_p;
//						}
//					}
//				}
//			}
//		if (next[2])
//			fire(map);
//		S[0] = xpos, S[1] = ypos;
//		xpos += next[1];
//		ypos += next[0];*/
//		clear_blocks();
//		//计算只绕开钢墙的路线
//		
//		int r = rand() % 100;
//		if (MOVE.R != MOVE.W)
//		switch (MOVE.M[MOVE.R++])
//		{
//		case 0:G_Map[ypos + 1][xpos] == 1 && r < 50 ? fire(true) : stop = rand() % 3 + 1; if (!G_Map[ypos + 1][xpos])ypos += 1; break;
//		case 1:G_Map[ypos][xpos + 1] == 1 && r < 50 ? fire(true) : stop = rand() % 3 + 1; if (!G_Map[ypos][xpos + 1])xpos += 1; break;
//		case 2:G_Map[ypos - 1][xpos] == 1 && r < 50 ? fire(true) : stop = rand() % 3 + 1; if (!G_Map[ypos - 1][xpos])ypos -= 1; break;
//		case 3:G_Map[ypos][xpos - 1] == 1 && r < 50 ? fire(true) : stop = rand() % 3 + 1; if (!G_Map[ypos][xpos - 1])xpos -= 1; break;
//		}
//		set_blocks();
//	}
//	else
//	{
//		fire(false);
//		stop--;
//	}
//}
//void move()
//{
//	if (!stop)
//	{
//		clear_blocks();
//		//计算只绕开钢墙的路线
//		int r = rand() % 100;
//		switch (MOVE.M[MOVE.R++])
//		{
//		case 0:G_Map[ypos + 1][xpos] == 1 && r < 50 ? fire(true) : stop = rand() % 3 + 1; if (!G_Map[ypos + 1][xpos])ypos += 1; break;
//		case 1:G_Map[ypos][xpos + 1] == 1 && r < 50 ? fire(true) : stop = rand() % 3 + 1; if (!G_Map[ypos][xpos + 1])xpos += 1; break;
//		case 2:G_Map[ypos - 1][xpos] == 1 && r < 50 ? fire(true) : stop = rand() % 3 + 1; if (!G_Map[ypos - 1][xpos])ypos -= 1; break;
//		case 3:G_Map[ypos][xpos - 1] == 1 && r < 50 ? fire(true) : stop = rand() % 3 + 1; if (!G_Map[ypos][xpos - 1])xpos -= 1; break;
//		}
//		set_blocks();
//	}
//	else
//	{
//		stop--;
//	}
//	fire(false);
//}
	////二叉堆方案
	////加入四周节点
	//if (G_Map[L->xy[1]][L->xy[0] + 1] < 2)//不是不可破坏方块
	//{
	//	if (!key_map[L->xy[1]][L->xy[0] + 1])//未访问过
	//	{
	//		now = (Unity*)malloc(sizeof(Unity));
	//		now->xy[0] = X + 1; now->xy[1] = Y;
	//		now->H = abs(x - X + 1) + abs(Y - y);
	//		now->G = abs(X + 1 - xpos) + abs(y - ypos);
	//		now->S = now->H + now->G;
	//		now->father = L;
	//		(open->o[(open->max)]) = now;
	//		//上浮排序
	//		i = open->max;
	//		while ((open->o[(i - 1) / 2])->S > now->S)
	//		{
	//			open->o[i] = open->o[(i - 1) / 2];
	//			open->o[(i - 1) / 2] = now;
	//			i = (i - 1) / 2;
	//		}
	//		key_map[open->o[0]->xy[1]][open->o[0]->xy[0] + 1] = 1;//标记已经访问过
	//	}
	//}
//#include<graphics.h>
//#include<stdio.h>
//#include<math.h>
//#define S(x,y) 0.5*x*y*y 
//
//int main()
//{
//	initgraph(600, 500, SHOWCONSOLE);
//	setbkcolor(WHITE);
//	cleardevice();
//	IMAGE view;
//	loadimage(&view, L"夜下的工作者.png", 600, 500, true);
//	putimage(0, 0, &view);
//
//	setlinecolor(GREEN);
//	setfillcolor(GREEN);
//	fillcircle(200, 250, 50);
//	fillcircle(400, 250, 50);
//
//	setlinestyle(PS_SOLID, 5);
//	setfillcolor(YELLOW);
//	fillrectangle(200, 200, 400, 300);
//
//
//	settextstyle(40, 0, L"楷书");
//	setbkcolor(YELLOW);
//	settextcolor(RED);
//	outtextxy(220, 230, L"开始游戏");
//
//	Sleep(1000);
//	setbkcolor(BLACK);
//	cleardevice();
//
//	////坠落小球1
//	//int a = 10; float x = 45, v = 0;
//	//while (TRUE)
//	//{
//	//	v += a;
//	//	x += v;
//	//	if (x > 485) { v = -(0.95 * v); x = 485; }
//	//	solidcircle(300, x, 15);
//	//	Sleep(100);
//	//	cleardevice();
//	//}
//
//	////旋转时钟
//	//float x =0;
//	//setorigin(200, 300);//设置逻辑坐标
//	//setaspectratio(1, -1);//反转坐标（x轴不变，y反转）
//	//for (; 1; x += 3.14/18)
//	//{
//	//	circle(0, 0, 50);
//	//	line(0, 0, (int)50*cos(x), (int)50*sin(x));
//	//	Sleep(100);
//	//	cleardevice();
//	//}
//
//	//可移动小球
//	int R[4] = { 400,400,475,450 };
//	ExMessage msg;
//	setbkcolor(BLACK);
//	cleardevice();
//	setfillcolor(YELLOW);
//	int x = 20, y = 20;
//	HWND Dive = GetHWnd();
//	while (true)
//	{
//		if(peekmessage(&msg,EM_MOUSE))
//		if (msg.message == WM_LBUTTONDOWN && msg.x >= 400 && msg.x <= 475 && msg.y >= 400 && msg.y <= 450)
//		{
//			int x = MessageBox(Dive, L"是否退出游戏！！", L"提示", MB_OKCANCEL);
//			if (x == IDOK)
//				break;
//		}
//		flushmessage(EM_MOUSE);//清除运行快导致的消息多
//
//		BeginBatchDraw();
//		setfillcolor(YELLOW);
//		rectangle(R[0], R[1], R[2], R[3]);
//		settextstyle(50, 0, L"宋体");
//		setbkcolor(WHITE);
//		settextcolor(GREEN);
//		outtextxy(400, 400, L"ESC");
//		setbkcolor(BLACK);
//		solidcircle(x, y, 20);
//		if (GetAsyncKeyState(VK_UP))//上键
//		{
//			y -= 10;
//		}
//		if (GetAsyncKeyState(VK_DOWN))//下键
//		{
//			y += 10;
//		}
//		if (GetAsyncKeyState(VK_LEFT))//左键
//		{
//			x -= 10;
//		}
//		if (GetAsyncKeyState(VK_RIGHT))//右键
//		{
//			x += 10;
//		}
//		// 绘制一个不规则图形
//		setlinecolor(RED);
//		setfillcolor(YELLOW);
//		moveto(100, 100);
//		lineto(200, 100);
//		lineto(200, 200);
//		lineto(150, 250);
//		lineto(100, 200);
//		lineto(100, 100);
//		
//
//		// 填充不规则图形
//		setfillcolor(BLUE);
//		floodfill(140, 150, RED);
//		FlushBatchDraw();
// 
//		Sleep(100);
//		cleardevice();
//
//	}
//	wchar_t name[10];
//	InputBox(name, 12, L"请输入名字", L"用户信息登记", L"李四");
//	settextcolor(YELLOW);
//	settextstyle(50, 0, L"宋体");
//	outtextxy(100, 200,name);
//	EndBatchDraw();
//	getchar();
//	closegraph();
//	return 0;
//}
//方法一：（弃用）
		//flushmessage();
		//if (peekmessage(&msg, EX_KEY))
		//{
		//	getmessage(&msg, EX_KEY);
		//	clearrectangle(xpos * p_size, ypos * p_size, (xpos+1) * p_size, (1+ypos) * p_size);
		//	switch (msg.vkcode)
		//	{
		//	case 0x57:
		//	case 'w':
		//		ypos -= ypos - 1 < 0 ? 0 : 1; Towards = 0; IF_H_ob('-', map, &ypos); break;//移动，朝向，碰撞检测,边界检查
		//	case 0x53:
		//	case 's':
		//		ypos += ypos + 1 > (width / p_size) ? 0 : 1, Towards = 2; IF_H_ob('+', map, &ypos); break;
		//	case 0x44:
		//	case 'd':
		//		xpos += xpos + 1 > (length / p_size) ? 0 : 1, Towards = 1; IF_H_ob('+', map, &xpos); break;
		//	case 0x41:
		//	case 'a':
		//		xpos -= xpos - 1 < 0 ? 0 : 1; Towards = 3; IF_H_ob('-', map, &xpos); break;
		//	default:break;
		//	}
		//}
//		//判断是否已经被访问过
		//	if (now->xy[0] + 1 <length/p_size &&key_map[now->xy[1]][now->xy[0] + 1] == 0)//右
		//	{
		//		if (G_Map[now->xy[1]][now->xy[0] + 1] == G_Map[y][x])break;
		//		//判断是不是不可摧毁阻碍
		//		if (G_Map[now->xy[1]][now->xy[0] + 1] < 2)
		//		{
		//			//更新数据,放入open表中
		//			open->o[index] = (Unity*)malloc(sizeof(Unity));
		//			open->o[index]->father = now;
		//			open->o[index]->xy[0] = now->xy[0] + 1; open->o[index]->xy[1] = now->xy[1];
		//			open->o[index]->G = now->G + 1;
		//			open->o[index]->H = abs(x - open->o[index]->xy[0]) + abs(open->o[index]->xy[1] - y);
		//			open->o[index]->S = open->o[index]->H + open->o[index]->G;
		//			open->max += 1;
		//			index = open->max;
		//		}
		//	}
		//	if (now->xy[0] - 1 >= 0 && key_map[now->xy[1]][now->xy[0] - 1] == 0)//左
		//	{
		//		if (G_Map[now->xy[1]][now->xy[0] - 1] == G_Map[y][x])break;
		//		if (G_Map[now->xy[1]][now->xy[0] - 1] < 2)
		//		{
		//			open->o[index] = (Unity*)malloc(sizeof(Unity));
		//			open->o[index]->father = now;
		//			open->o[index]->xy[0] = now->xy[0] - 1; open->o[index]->xy[1] = now->xy[1];
		//			open->o[index]->G = now->G + 1;
		//			open->o[index]->H = abs(x - open->o[index]->xy[0]) + abs(open->o[index]->xy[1] - y);
		//			open->o[index]->S = open->o[index]->H + open->o[index]->G;
		//			open->max += 1;
		//			index = open->max;
		//		}
		//	}
		//	if (now->xy[1] + 1 < width / p_size && key_map[now->xy[1] + 1][now->xy[0]] == 0)//下
		//	{
		//		if (G_Map[now->xy[1] + 1][now->xy[0]] == G_Map[y][x])break;
		//		if (G_Map[now->xy[1] + 1][now->xy[0]] < 2)
		//		{
		//			open->o[index] = (Unity*)malloc(sizeof(Unity));
		//			open->o[index]->father = now;
		//			open->o[index]->xy[0] = now->xy[0]; open->o[index]->xy[1] = now->xy[1] + 1;
		//			open->o[index]->G = now->G + 1;
		//			open->o[index]->H = abs(x - open->o[index]->xy[0]) + abs(open->o[index]->xy[1] - y);
		//			open->o[index]->S = open->o[index]->H + open->o[index]->G;
		//			open->max += 1;
		//			index = open->max;
		//		}
		//	}
		//	if (now->xy[1] - 1 >= 0 && key_map[now->xy[1] - 1][now->xy[0]] == 0)//上
		//	{
		//		if (G_Map[now->xy[1] - 1][now->xy[0]] == G_Map[y][x])break;
		//		if (G_Map[now->xy[1] - 1][now->xy[0]] < 2)
		//		{
		//			open->o[index] = (Unity*)malloc(sizeof(Unity));
		//			open->o[index]->father = now;
		//			open->o[index]->xy[0] = now->xy[0]; open->o[index]->xy[1] = now->xy[1] - 1;
		//			open->o[index]->G = now->G + 1;	
		//			open->o[index]->H = abs(x - open->o[index]->xy[0]) + abs(open->o[index]->xy[1] - y);
		//			open->o[index]->S = open->o[index]->H + open->o[index]->G;
		//			open->max += 1;
		//			index = open->max;
		//		}
		//	}
		//	if (now == open->o[index])
		//	{
		//		open->o[index] = open->o[open->max];
		//		open->o[open->max] = NULL;
		//	}
		//}
		//记录路线

