// #include<stdio.h>
// #include<stdlib.h>
// #include<string.h>
// #include<windows.h>

// enum STM{
//     ID_MAX=20,
//     S_NAME_MAX=20,
//     CLASS_MAX=10,
// };

// // struct class{
// //     char c_name[10];//课程名
// //     int c_sc;//课程成绩
// //     struct class* next;//指向下一课程信息
// // };
// typedef struct Student{
//     char ID[ID_MAX];//学生学号
//     char s_name[S_NAME_MAX];//学生名字
//     // struct class* p;//指向学生成绩表
//     char class[CLASS_MAX][10];//课程名
//     int class_sc[CLASS_MAX];//课程成绩
//     struct Student* next;//指向下一学生
// }ST;


// void window()
// {
//     printf("*******************************************\n");
//     printf("*****1.录入学生信息*****2.添加到文件中 ******\n");
//     printf("*******************************************\n");
//     printf("*****3.查看学生不及格信息*****4.退出 ********\n");
//     printf("*******************************************\n");
// }

// ST* Malloc_ST()
// {
//     ST* SD1=(ST*)malloc(sizeof(ST));
//     memset(SD1->class_sc,-1,4*CLASS_MAX);
//     SD1->next=NULL;
//     return SD1;
// }

// void next_step(int* step)
// {
//     printf("请选择操作:");
//     scanf("%d",step);
//     getchar();
// }

// ST* SR_student(ST* SDF)
// {
//     ST* Head=SDF;
//     char m[4]="是";
//     char x[4]={0};
//     do
//     {
//         system("CLS");
//         SDF->next=Malloc_ST();
//         SDF=SDF->next;
//         //数据输入
//         printf("请输入学生ID:");
//         gets(SDF->ID);
//         printf("请输入学生姓名:");
//         gets(SDF->s_name);
//         printf("请问有多少条成绩数据？");
//         int i=0,max;
//         scanf("%d",&max);getchar();
//         char k[4]={0};
//         do
//         {
//             printf("请输入成绩：\n");
//             scanf("%s%d",SDF->class[i],&(SDF->class_sc[i]));getchar();
//             i++;
//         }while(i<max);
//         system("CLS");
//         printf("请问是否继续输入学生信息：");
//         gets(x);
//     }while(!strcmp(x,m));
//     return Head->next;
// }

// void add_file(ST* SDF)
// {
//     system("CLS");
//     FILE* F=fopen("学生.txt","a");
//     if(F==NULL)
//     {
//         puts("打开失败");
//     }
//     else 
//     {
//         while(SDF!=NULL)
//         {   
//             int i=0;
//             fprintf(F,"ID:%s\n姓名:%s\n成绩表:\n",SDF->ID,SDF->s_name);
//             while(SDF->class_sc[i]!=-1)
//             {
//                 fprintf(F,"%s %d\n",SDF->class[i],SDF->class_sc[i]);
//                 i++;
//             }
//             fprintf(F,"\n%s\n","**********************");
//             SDF=SDF->next;
//         }
//         printf("添加成功");Sleep(1000);
//         fclose(F);
//         F=NULL;
//     }
// }

// void searched_sc(ST* SDF)
// {
//     ST* H=SDF;
//     int i=0;
//     //导入外部文件数据
//     char DZ[50]={0};
//     printf("请输入外部数据源地址：");
//     gets(DZ);
//     if(strcmp(DZ,"无"))
//     {
//         FILE* F=fopen(DZ,"r");
//         if(!F)
//         {
//             printf("不存在该文件\n");
//         }
//         else 
//         {   //使SDF指向末尾(先判断SDF是否为空)
//             while(SDF->next!=NULL){SDF=SDF->next;}
//             char M[23]={0},try_ID[CLASS_MAX]={0};
//             //添加进来
//             for(;1;i=0)
//             {
//                 fseek(F,strlen("ID:"),SEEK_CUR);
//                 if(-1==fscanf(F,"%s%*c",try_ID)){break;}//判断文件是否到底
//                 SDF->next=Malloc_ST();
//                 SDF=SDF->next;
//                 strcpy(SDF->ID,try_ID);
//                 fseek(F,strlen("姓名:"),SEEK_CUR);
//                 fscanf(F,"%s%*c",SDF->s_name);
//                 fscanf(F,"%s%*c",M);//\n--->\r\n
//                 do
//                 {
//                     fscanf(F,"%s%*c",M);
//                     if(!strcmp(M,"**********************")){break;}
//                     else
//                     {
//                         strcpy(SDF->class[i],M);
//                         fscanf(F,"%d%*c",&(SDF->class_sc[i]));
//                         i++;
//                     }
//                 } while (1);
//             }
//         }
//     }

//     //统计  
//     system("CLS");
//     printf("以下是学生不及格信息:\n\n");
//     H=H->next;
//     for(i=0;H!=NULL;H=H->next,i=0)
//     {   
//         int x=0;
//         while(H->class_sc[i]!=-1)
//         {
//             if(H->class_sc[i]<60)
//             {
//                 if(x==0)
//                     printf("%s:不及格成绩有\n",H->s_name),x=1;
//                 Sleep(100);
//                 printf("\t\033[34m%s\033[0m--",H->class[i]);
//                 printf("\033[31m%d\033[0m\n",H->class_sc[i]);//\033[31m表示告诉控制台将后续字符设置为‘红色’
//                                                             //\033[30m表示将后续字符颜色改为默认值
//             }
//             i++;
//         }
//         printf("\n");
//     } 
//     system("pause");

// }

// int main()
// {
//     int step=0;
//     ST* SDF=Malloc_ST();
//     while(step!=4)
//     {   //界面
//         system("CLS");// 清屏
//         window();
//         next_step(&step);
//         switch(step)
//         {   //输入学生信息
//             case 1:SDF=SR_student(SDF);break;
//             //打开文件并添加到文件中，以追加的方式‘a’
//             case 2:add_file(SDF);break;
//             //统计学生不及格的科目，课程名称等信息----(需要把外部的数据导入进来)
//             case 3:searched_sc(SDF);break;
//             //退出
//             case 4:printf("Bye Bye"),Sleep(1000);return 0;//注意Sleep要大写S
//         }
//     }
// }


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>

#define ID_MAX 20
#define S_NAME_MAX 20
#define CLASS_MAX 10

typedef struct Student{
    char ID[ID_MAX];
    char s_name[S_NAME_MAX];
    char class[CLASS_MAX][10];
    int class_sc[CLASS_MAX];
    struct Student* next;
} ST;

void window()
{
    printf("*******************************************\n");
    printf("*****1.录入学生信息*****2.添加到文件中 ******\n");
    printf("*******************************************\n");
    printf("*****3.查看学生不及格信息*****4.退出 ********\n");
    printf("*******************************************\n");
}

ST* Malloc_ST()
{
    ST* SD1 = (ST*)malloc(sizeof(ST));
    memset(SD1->class_sc, -1, sizeof(int) * CLASS_MAX);
    SD1->next = NULL;
    return SD1;
}

void next_step(int* step)
{
    printf("请选择操作:");
    scanf("%d", step);
    getchar();
}

ST* SR_student(ST* SDF)
{
    ST* Head = SDF;
    char m[4] = "是";
    char x[4] = {0};
    do
    {
        system("CLS");
        SDF->next = Malloc_ST();
        SDF = SDF->next;
        
        printf("请输入学生ID:");
        fgets(SDF->ID, ID_MAX, stdin);
        SDF->ID[strcspn(SDF->ID, "\n")] = '\0';
        
        printf("请输入学生姓名:");
        fgets(SDF->s_name, S_NAME_MAX, stdin);
        SDF->s_name[strcspn(SDF->s_name, "\n")] = '\0';
        
        printf("请问有多少条成绩数据？");
        int i=0, max;
        scanf("%d", &max);
        getchar();
        char k[4] = {0};
        do
        {
            printf("请输入成绩：\n");
            scanf("%s%d", SDF->class[i], &(SDF->class_sc[i]));
            getchar();
            i++;
        } while(i < max);
        
        system("CLS");
        printf("请问是否继续输入学生信息：");
        fgets(x, 4, stdin);
        x[strcspn(x, "\n")] = '\0';
    } while(strcmp(x, m));
    
    return Head->next;
}

void add_file(ST* SDF)
{
    system("CLS");
    FILE* F = fopen("学生.txt", "a");
    if(F == NULL)
    {
        puts("打开失败");
    }
    else
    {
        while(SDF != NULL)
        {
            int i = 0;
            fprintf(F, "ID:%s\n姓名:%s\n成绩表:\n", SDF->ID, SDF->s_name);
            while(SDF->class_sc[i] != -1)
            {
                fprintf(F, "%s %d\n", SDF->class[i], SDF->class_sc[i]);
                i++;
            }
            fprintf(F, "\n%s\n", "**********************");
            SDF = SDF->next;
        }
        printf("添加成功");
        Sleep(1000);
        fclose(F);
        F = NULL;
    }
}

void searched_sc(ST* SDF)
{
    ST* H = SDF;
    int i = 0;
    char DZ[50] = {0};
    
    printf("请输入外部数据源地址：");
    fgets(DZ, 50, stdin);
    DZ[strcspn(DZ, "\n")] = '\0';
    
    if(strcmp(DZ, "无"))
    {
        FILE* F = fopen(DZ, "r");
        if(!F)
        {
            printf("不存在该文件\n");
        }
        else
        {
            while(SDF->next != NULL)
            {
                SDF = SDF->next;
            }
            
            char M[23] = {0}, try_ID[CLASS_MAX] = {0};
            for(;1;i=0)
            {
                fseek(F, strlen("ID:"), SEEK_CUR);
                if(-1 == fscanf(F, "%s%*c", try_ID))
                {
                    break;
                }
                
                SDF->next = Malloc_ST();
                SDF = SDF->next;
                strcpy(SDF->ID, try_ID);
                
                fseek(F, strlen("姓名:"), SEEK_CUR);
                fscanf(F, "%s%*c", SDF->s_name);
                fscanf(F, "%s%*c", M);
                
                do
                {
                    fscanf(F, "%s%*c", M);
                    if(!strcmp(M, "**********************"))
                    {
                        break;
                    }
                    else
                    {
                        strcpy(SDF->class[i], M);
                        fscanf(F, "%d%*c", &(SDF->class_sc[i]));
                        i++;
                    }
                } while(1);
            }
            
            fclose(F);
        }
    }
    
    system("CLS");
    printf("以下是学生不及格信息:\n\n");
    H = H->next;
    for(i = 0; H != NULL; H = H->next, i = 0)
    {
        int x = 0;
        while(H->class_sc[i] != -1)
        {
            if(H->class_sc[i] < 60)
            {
                if(x == 0)
                {
                    printf("%s:不及格成绩有\n", H->s_name);
                    x = 1;
                }
                Sleep(100);
                printf("\t\033[34m%s\033[0m--", H->class[i]);
                printf("\033[31m%d\033[0m\n", H->class_sc[i]);
            }
            i++;
        }
        printf("\n");
    }
    
    system("pause");
}

int main()
{
    int step = 0;
    ST* SDF = Malloc_ST();
    
    while(step != 4)
    {
        system("CLS");
        window();
        next_step(&step);
        
        switch(step)
        {
            case 1:
                SDF = SR_student(SDF);
                break;
            case 2:
                add_file(SDF);
                break;
            case 3:
                searched_sc(SDF);
                break;
            case 4:
                printf("Bye Bye");
                Sleep(1000);
                return 0;
        }
    }
}
