#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
//日志相关定义
#define LOG_LEVE 1
#define OPEN_LOG 0
//地图相关定义
#define MAP_LENGTH 30
#define MAP_WIDTH  80
//定义物体参数
#define BLANK 0
#define WALL -2
#define FOOD -1

char* P_map;//地图一维指针
                                            /*65行辅助代码  实际贪吃蛇代码用110行*/
HANDLE hOutput, hOutBuf, hStdout ;  
//两个句柄，这里可以就理解为我们等会需要用的两个缓冲区
COORD coord = { 0,0 };  //位置坐标，用到的时候解释
DWORD bytes = 0;        //这个主要是要配合下面的一个函数
//定义日志等级
typedef enum{
    LOG_DEBUG=0,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
}E_LOG_LEVE;
//为等级添加颜色区别
const char* EM_LOGLevelGet(const int level)
{
    switch(level){
        case 0:return "DEBUG";
        case 1:return "\033[32mINFO\033[0m";//\033[颜色代码m  -----表示切换之后输出的颜色
        case 2:return "\033[35mWARN\033[0m";
        case 3:return "\033[31mERROR\033[0m";
    }
    return "UNKNOW";
}
//打印高等级日志
void EM_LOG(const int LEVE,const char *fmt,...)
{
    #ifdef OPEN_LOG
    va_list arg;
    va_start(arg,fmt);
    char* buff =(char*)malloc(1 + vsnprintf(NULL,0,fmt,arg));
    vsprintf(buff,fmt,arg);
    va_end(arg);
    if(LOG_LEVE <= LEVE){
        if(LEVE==LOG_DEBUG){
            printf("%s",buff);
        }else{
            printf("[%s]: %s",EM_LOGLevelGet(LEVE),buff);
        }
    }
    #endif
}

void initconsoleScreenBuffer(){
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE); // 获取标准输出的句柄
    hOutBuf = CreateConsoleScreenBuffer(  //创建缓冲区
        GENERIC_WRITE,
        FILE_SHARE_WRITE,
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    );
    hOutput = CreateConsoleScreenBuffer(  //创建缓冲区
        GENERIC_WRITE,
        FILE_SHARE_WRITE,
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    );
    //以下5行是设置这两个缓冲区的光标不可见
    CONSOLE_CURSOR_INFO cci;  //创建光标
    cci.bVisible = 0;    //光标可见度为0
    cci.dwSize = 1;      //光标大小为1，可省略
    SetConsoleCursorInfo(hOutput, &cci); //设置第一个缓冲区的光标参数
    SetConsoleCursorInfo(hOutBuf, &cci); //设置第二个缓冲区的光标参数
}
int main()
{
    initconsoleScreenBuffer();
//变量定义 
    srand(time(NULL));//随即种子初始化
    int food;
    char* hand;
    int key=0,move=0;
    char state;
    char map[MAP_LENGTH][MAP_WIDTH];//定义地图
    P_map=(char*)map;
//初始化地图
    //设置边界=='-2'
    int P_L,P_W;P_L=0,P_W=0;
    while(P_L<MAP_LENGTH){//-------------------------------------可以优化
        while(P_W<MAP_WIDTH){
            if(P_L==0||P_L==MAP_LENGTH-1||P_W==0||P_W==MAP_WIDTH-1){//对第一和最后一行以及边上两列处理
                //map[P_L][P_W]=-2;
                *P_map=WALL;//墙壁设为-2
            }else{
                *P_map=BLANK;//空地设为0
            }
            P_map+=1,++P_W;
        }
        P_W=0,++P_L;
    }
    //创建第一个食物
    P_map=(char*)map;
    while((food=rand()%(MAP_LENGTH*MAP_WIDTH),*(P_map+food))== WALL)EM_LOG(LOG_DEBUG,"重新初始化食物\n");
    *(P_map+food)=FOOD;
    //创建小蛇
    while((hand=(P_map+rand()%(MAP_LENGTH*MAP_WIDTH)),*hand != BLANK))EM_LOG(LOG_DEBUG,"重新初始化蛇\n");
    *hand=1;
//---------------------------------------------------------------------------------------------------------
    int i=0;
    for(move=0,state=BLANK;1;P_map=(char*)map,i=(i+1)%2){
        //数据收集
        if(_kbhit()){
            key=getch();
            if(key>'A'&&key<'Z'){key+=32;}//大小写转换
            if(key=='a'||key=='s'||key=='d'||key=='w'){
                switch(key){//有效按键检测&&动作更新
                    case 'a':move=-1;         break;
                    case 's':move=MAP_WIDTH; break;
                    case 'd':move=1;          break;
                    case 'w':move=-MAP_WIDTH;break;
                }
            }
        }
    //数据处理
        state=move==0?BLANK:*(hand+move);
        if(state==WALL||state>BLANK){
            EM_LOG(LOG_ERROR,"遇到%s挑战失败\n",state==WALL?"\033[31m墙壁\033[0m":"\033[31m蛇身\033[0m");break;}//如果是撞到墙或者自己就退出循环
        else if(state==FOOD){
            while((food=rand()%(MAP_LENGTH*MAP_WIDTH),*(P_map+food))!= BLANK);//生成新的食物
            *(P_map+food)=FOOD;
            *(hand+move)=(*hand)+1,hand+=move;
        }else{
            *(hand+move)=(*hand),hand+=move;
        }
    //反馈
        P_L=0;
        while(P_L<MAP_LENGTH){
            P_W=0;
            while(P_W<MAP_WIDTH){
                if((*P_map)>0&&P_map!=hand){*P_map-=1;}
                (*P_map)=(*P_map)==0?' ':(*P_map)==-2?'#':(*P_map)==-1?'@':'a'-1+*(P_map);//大于1的数字都打印为*
                P_W+=1;P_map+=1;
            }
            //EM_LOG(LOG_DEBUG,"%c",'\n');
            coord.Y=P_L;
            if(i==0){
                WriteConsoleOutputCharacterA(hOutBuf,map[P_L],MAP_WIDTH,coord,&bytes);
            }else{
                WriteConsoleOutputCharacterA(hOutput,map[P_L],MAP_WIDTH,coord,&bytes);
            }
            P_W=0;P_map-=MAP_WIDTH;
            while(P_W<MAP_WIDTH){
                (*P_map)=(*P_map)==' '?0:(*P_map)=='#'?-2:(*P_map)=='@'?-1:*(P_map)+1-'a';//大于1的数字都打印为*
                P_W+=1;P_map+=1;
            }
            P_L+=1;
        }
        if(i==0)SetConsoleActiveScreenBuffer(hOutBuf);
        else{SetConsoleActiveScreenBuffer(hOutput);}
        Sleep(100);
    }
    SetConsoleActiveScreenBuffer(hStdout); // 切换回默认控制台屏幕缓冲区
    system("pause");
    return 0;
}
