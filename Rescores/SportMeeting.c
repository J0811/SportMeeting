#include <stdio.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h> 
#include <setjmp.h>
#include <Windows.h>
#define N 1000    		//整场运动会可参加运动员上限 
#define M 8 			//每个项目最多可报人数 
#define personlimit 3	//每人最多可报数量	
#define ddlperson 4	 //项目立项底线人数 
#define bool int      //这里用define定义了布尔类型
#define true 1         
#define false 0
#define length 20  1

jmp_buf   jb;
char password[3][20];
bool judge1 = true;     //是否截至报名
FILE *fp;
FILE *fr;
FILE *fq;
FILE *fs;
FILE *g_ptoSTUDENT ;
FILE *p_fold_Allsports;

struct University//运动会举办信息的结构体 
{
	char name[20];  //举办方名字 
	int  number;    //届数 
	int  month;		//月份
	int  bday;      //日 
}university; 
				
typedef struct   //运动员信息的结构体 
{
    char      g_sStuNum[50];        //学号 
    char      g_sStuName[50] ;		//名字 
    char      g_sDormitory[50] ;	//学院 
    char      g_sPhoneNumber[50];	//电话号码 
    char      g_cSex[50];			//性别 
    char      g_sRegSport[50];		//报名项目 
    int       g_iHaveAtenSportNum;	//参加项目数 
    double    g_dMark ;				//成绩 
    int       g_iScore ;			//得分 

}Player;
Player Players[N],temp[M];
Player deletable[N],tempPlayers[N],addable[N],addtempPlayers[N];		//在运动员管理内操作 

typedef struct{		//项目信息结构体 
	char Sports_Name[20];				
	int sex;
	int type;
	int Group_OR_Population;
	int eachtime;

}Sports_Single;
Sports_Single boysport[1000],girlsport[1000],fieldsport[1000],tracksport[1000],allsport[1000],Deleted_Sports[1000];
int numofbS=0,numofgS = 0,numofDorm=0,numofAS=0,numofRS=0,numofFS=0;
int h=0;
char Dorm[20][20];//存储参赛单位

typedef struct		//表示秩序册项目时间的结构体 
{
	int hour;
	int mint;
	int starttime1;
	int starttime2;
	int endtime1;
	int endtime2;
	int num1;		//num1为竞赛组数或田赛人数 
	int num2;		//num2为竞赛每组时间 或田赛每人时间 
}Time;

//首次使用时 确认 举办 单位				
 int Beginning()
{      
	if((fp = fopen("Beginning.txt","r"))==NULL) 
	{
		fp=fopen("Beginning.txt","a+");
		printf("请输入举办单位\n");
		scanf("%s",university.name);
		printf("请输入届数\n")    ;
		scanf("%d",&university.number);
		printf("请输入开始月份\n"); 
		scanf("%d",&university.month);
		printf("请输入开始于几号\n"); 
		scanf("%d",&university.bday);
		fprintf(fp,"%s\n%d\n%d\n%d",university.name,university.number,university.month,university.bday);
		fclose(fp);
	}
}

char *getfileall(char *fname)
{
    FILE *fp;
    char *str;
    char txt[1000];
    int filesize;
    //打开一个文件
    if ((fp=fopen(fname,"r"))==NULL){
        printf("打开文件%s错误\n",fname);
        return NULL;
    }
    //将文件指针移到末尾
    fseek(fp,0,SEEK_END);
    filesize = ftell(fp);//通过ftell函数获得指针到文件头的偏移字节数。
    
    str=(char *)malloc(filesize);//动态分配str内存
//    str=malloc(filesize);//动态分配str内存
    str[0]=0;//字符串置空
//    memset(str,filesize*sizeof(char),0);//清空数组,字符串置空第二种用法
    rewind(fp);
    
    while((fgets(txt,1000,fp))!=NULL){//循环读取1000字节,如果没有数据则退出循环
        strcat(str,txt);//拼接字符串
    }
    fclose(fp);
    return str;
}

int backup(){
	time_t t;
    struct tm * lt;
    time (&t);//获取Unix时间戳。
    lt = localtime (&t);//转为时间结构。
	char *q;
	char *r;
	char *s;
	fp=fopen("Bakcup.txt","a");
	q=getfileall("PLAYERS.txt");
	r=getfileall("Dorm.txt");
	s=getfileall("AllSports.txt");
	fprintf(fp,"\n备份时间：%d/%d/%d %d:%d:%d\n运动员信息:\n%s\n\n比赛项目信息:\n%s\n\n参赛单位:\n%s\n------------------------------------",lt->tm_year+1900, lt->tm_mon+1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec,q,s,r );
	printf("运动员信息:\n%s\n\n比赛项目信息:\n%s\n\n参赛单位:\n%s\n",q,s,r);
	printf("\n备份成功！备份时间：%d/%d/%d %d:%d:%d\n",lt->tm_year+1900, lt->tm_mon+1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
	fclose(fp);
	printf("备份结束，按任意键返回主目录！");
	getch();
	longjmp(jb, 1);
}
//主函数一开始运行的打印块 
 int Screen()
{
	Beginning();
	fp=fopen("Beginning.txt","a+");
	fscanf(fp,"%s%d%d%d",university.name,&university.number,&university.month,&university.bday);
	printf("欢迎参加%s第%d届运动会\n", university.name,university.number);
	printf("举办时间是%d月%d号\n", university.month,university.bday);
	printf("按任意键继续操作");
	fclose(fp);
}

//学生端主菜单 
 int Student() 
{
	check();
	int choice; 
	    printf("           \n\n\t\t1.报名                  \n") ;
	    printf("           \n\n\t\t2.查成绩                \n") ;
	    printf("           \n\n\t\t3.退出到开始界面        \n") ;
	    printf("\n\t\t请输入数字进行选择\n") ;
	    scanf("%d",&choice);
	    switch(choice)
	    	{
			case 1 : 
			system("cls");
			if(judge1==false)
			{
			MessageBox(NULL,TEXT("报名已截止"), TEXT("提示"),MB_OK);system("cls");back();
			}
			else
			Register() ; break; 
            case 2 : 
            if((fp=fopen("Gradepublish.txt","r"))==NULL)
            {
            	MessageBox(NULL,TEXT("成绩未发布"), TEXT("提示"),MB_OK);system("cls");back(); 
			}
			else
			{
				system("cls");student_numSearch();
			}
			break; 
            case 3 : system("cls");main(); break;
            default : printf("无法识别，请重新输入！\n"); break;
            } 
}

int student_numSearch()
//管理员按学号查询// 
{ 
    system("cls");	//清屏// 
    int i,j,sum=0;
	char num[20];	//学生号码（形参）// 
	int tag=0;	//标志是否存在该学号的运动员// 
	printf("\n\n\t************按学号查询************\n");
	printf("\t输入运动员学号：");
	scanf("%s",num);
	
	//通过循环找到特定学号的结构体// 
	printf("\n\t学号\t姓名\t项目\t\t成绩\t得分\n");
	for(i=0;i<h;i++)
	{
		if(strcmp(Players[i].g_sStuNum,num)==0)
		{
            printf("\t%s\t%s\t%s\t%.2lf\t%d\n\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
			tag=1;	//找到该学号的信息，将标志值改变// 
			sum=sum+Players[i].g_iScore;
		}
	} 
		if(tag==1)
	{
		printf("\t%s的总得分是%d",num,sum);
	 } 
		if(tag==0)
	{
		printf("\n\t未找到该运动员,任意键返回成绩管理菜单\n");
		getch();
		return(Gradeop());
	} 	
	printf("\n\n\t任意键返回学生菜单\n");
    getch();
    system("cls");
	return(Student());
	//标志值未改变，未找到该学员信息// 
}


//报名函数 
int Register()
{ 
	int a,j=0;
	char number[50];
	if((fp=fopen("Allsports.txt","r"))==NULL)
	{
		printf("\n\n\t报名未开始!"); getch();return(main());
	}
	fclose(fp);
	if((fp=fopen("Dorm.txt","r"))==NULL)
	{
		printf("\n\n\t报名未开始!"); getch();return(main());
	}
	fclose(fp);
	int count,i;		//i循环 
    count = h;

	if(count==N)
	{
		printf("人数已满，无法继续报名,即将返回主界面！\n");
		system("pause");
		system("cls");
		return(main()) ; 
	}
    printf("已有报名人数：%d\n",count);
           
				//===================输入学号======================// 
			printf("请输入你的学号：\n");
			scanf("%s",number) ;
			for(a=0;a<count;a++)
			{
				if(strcmp(Players[a].g_sStuNum,number)==0)
				j=j+1;
			} 
			if(j>=personlimit)
			{
				printf("个人报名项目数量已达上限%d次！",personlimit);//限制每人报名项目数量上限 
				sleep(3);
				system("cls");
				return(Student()); 
			}	
			else
			{
				strcpy(Players[count].g_sStuNum,number);
			}
			
			
		//==================输入性别========================//
		int sex ; 
		printf("请选择你的性别：\n");
		printf(" 1.男   2.女\n") ;
 		sexcho:scanf("%d",&sex); 
		if(sex==1)
		strcpy(Players[count].g_cSex,"男");
		else if(sex==2)
		strcpy(Players[count].g_cSex,"女");
		else
		{
		printf("请选择正确的编号\n");goto sexcho;
		}
		
			//========================男子选择报名项目===================//
			if(sex==1) 
			{  
			    int RaceChoose=0 ;           //=====定义并初始化男子选择报名项目的选择标记值=====// 
				int LiebChoose=0;				//=======田径类别==========//
                     ManChoSpo :              //==========男子选择报名无条件转换标志点======// 
					 printf("请选择比赛类别:\n\t1、田赛\t2、径赛\n"); 
					 scanf("%d",&LiebChoose);
					 system("cls");
					 if(LiebChoose==1){
						printf("请选择参赛项目类别：\n"); 
						for(i=0;i<numofbS;i++){
							if(boysport[i].type==1)
							printf("\t\t%d.%s\n",i+1,boysport[i]);
							}
			        		 	scanf("%d",&RaceChoose);
			         			system("cls");
			        	if(RaceChoose>=1&&RaceChoose<=numofbS) 
							maxPlayers(boysport[RaceChoose-1],count,number);
						else							
			           		{
					  		 printf("无法识别，请再次输入") ;getch(); system("cls") ; goto ManChoSpo ;
					  		 }
					 }
					 else if(LiebChoose==2){
						printf("请选择参赛项目：\n"); 
			         	for(i=0;i<numofbS;i++){
							if(boysport[i].type==2){
								printf("\t\t%d.%s\n",i+1,boysport[i]);
							}
					}
			         scanf("%d",&RaceChoose);
			         system("cls");
			        if(RaceChoose>=1&&RaceChoose<=numofbS) 
					maxPlayers(boysport[RaceChoose-1],count,number);
					else							
			           {
					   printf("无法识别，请再次输入") ;getch(); system("cls") ; goto ManChoSpo ;
					   }
					 }
					 else{
						printf("无法识别，请再次输入") ;getch(); system("cls") ; goto ManChoSpo ;
					 }
			         
			}
 
			//========================女子选择报名项目===================//
			if(sex==2)
			{ 
			         int RaceChoose=0 ;           //======定义并初始化女子选择报名项目的选择标记值=======// 
					 int LiebChoose=0;
			         WomanChoSpo :                 //=======女子选择报名无条件转换标志点========// 
			         printf("请选择比赛类别:\n\t1、田赛\t2、径赛\n"); 
					 scanf("%d",&LiebChoose);
					 system("cls");
					 if(LiebChoose==1){
						printf("请选择参赛项目类别：\n"); 
			         for(i=0;i<numofbS;i++)
					{
						if(girlsport[i].type==1)
					printf("\t\t%d.%s\n",i+1,girlsport[i]);
					}
			         scanf("%d",&RaceChoose);
			         system("cls");
			        if(RaceChoose>=1&&RaceChoose<=numofbS) 
					maxPlayers(girlsport[RaceChoose-1],count,number);
					else							
			           {
					   printf("无法识别，请再次输入") ;getch(); system("cls") ; goto ManChoSpo ;
					   }
					 }
					 else if(LiebChoose==2){
						printf("请选择参赛项目：\n"); 
			         for(i=0;i<numofbS;i++)
					{if(girlsport[i].type==2)
					printf("\t\t%d.%s\n",i+1,girlsport[i]);
					}
			         scanf("%d",&RaceChoose);
			         system("cls");
			        if(RaceChoose>=1&&RaceChoose<=numofbS) 
					maxPlayers(girlsport[RaceChoose-1],count,number);
					else							
			           {
					   printf("无法识别，请再次输入") ;getch(); system("cls") ; goto ManChoSpo ;
					   }
					 }
					 else							
			           {
					   printf("无法识别，请再次输入") ;getch(); system("cls") ; goto ManChoSpo ;
					   }
			}
		

			//===================输入姓名=======================// 
			printf("请输入你的姓名：\n");
			scanf("%s",Players[count].g_sStuName) ;
		
		
			//===================选择参赛学院===================// 
		
		
		     int Dorchoice=0 ;     //设置一个参赛学院选择判断标志值// 
		     
			dormi:   //=======无条件转移到选择参赛学院的开始的标志点=======//	
		
		    printf("\t\n请选择你的参赛学院：\n") ;
			for(i=0;i<numofDorm;i++)
			{
				printf("\t\t%d.%s\n",i+1,Dorm[i]);
			}
			
			    scanf("%d",&Dorchoice) ;     //读取从键盘输入的选择标记值// 
			    system("cls");
			if(Dorchoice>=1&&Dorchoice<=numofDorm)   
			    strcpy(Players[count].g_sDormitory,Dorm[Dorchoice-1]) ; 
			else	
			{printf("无法识别，请再次输入")	  ;system("pause") ;system("pause") ; goto dormi ; } //=======无条件转移到选择参赛学院的开始=======//				
				
				
					
			//==================输入电话号码======================= // 
			printf("请输入你的电话号码：\n");
			scanf("%s",Players[count].g_sPhoneNumber) ;
			
			
						
			//========================男子选择报名项目===================//

			
	//询问报名者是否确认？确认后不可更改//
	 //system("cls") ;
     Players[count].g_dMark=0;
	 Players[count].g_iScore=0;
	 int EnsureChoice ;
	 ensure :
	 printf("\n\n%s\t%s\t%s\t%s\t%s\t%s\n\n",Players[count].g_sStuNum,Players[count].g_sStuName,Players[count].g_cSex,
				Players[count].g_sDormitory,Players[count].g_sRegSport,Players[count].g_sPhoneNumber);
	 printf("是否确认报名？确认后不可更改。") ;
	 printf("1.是         2.否\n") ;
	 scanf("%d",&EnsureChoice) ;
	 system("cls") ;
	 switch(EnsureChoice)
	 {
	 	case 1 :    
		 if((g_ptoSTUDENT = fopen("PLAYERS.txt","a+"))==NULL)      //====判断文件是否打开成功====// 
		{
			printf("数据录入失败，即将返回主界面！") ;}
    
	//===开始向文件中保存数据=====//
	fprintf(g_ptoSTUDENT,"%s\t",Players[count].g_sStuNum) ;      //  学号 //====利用格式化文件输出函数把Players[count].g_iStuNum写入文件中//  
	fprintf(g_ptoSTUDENT,"%s\t",Players[count].g_sStuName) ;   //=====姓名===// 
	fprintf(g_ptoSTUDENT,"%s\t",Players[count].g_cSex) ;        //====性别====//
	fprintf(g_ptoSTUDENT,"%s\t",Players[count].g_sPhoneNumber) ;      //===电话====// 
	fprintf(g_ptoSTUDENT,"%s\t",Players[count].g_sDormitory) ;        //====参赛学院==== 
	fprintf(g_ptoSTUDENT,"%s\t\t",Players[count].g_sRegSport) ;     ///====报名项目==== 
	fprintf(g_ptoSTUDENT,"%.2f\t",Players[count].g_dMark) ;     //===项目成绩===== 
	fprintf(g_ptoSTUDENT,"%d\n",Players[count].g_iScore) ;    //=====得分==== 
	fclose(g_ptoSTUDENT); break ;
	 	case 2 : printf("即将返回主界面！") ; system("pause") ; system("cls") ; return(main()) ;
	 	default : printf("无法识别，请重新确认！") ;getch(); system("cls") ; goto ensure ;
	 }
		h=0;
		return (main());
}



//限制项目的报名人数和重复报名 

int maxPlayers(char *x,int num,char *nb)    //*x导入一段字符串，h导入一个常数 
{ 
	int j=0; 
	int q=0;         
	int i=0;      
	int z;            //判断是否可以报名的标志 
    char y[50];
    char t[50];
    strncpy(y,x,sizeof(y));     	//将通过*x导入的结字符串赋给y[] 
    strncpy(t,nb,sizeof(t));   
    for(i=0;i<num;i++)      
	      {   
		    if(strcmp(Players[i].g_sRegSport,y)==0)     //循环找到与y相等的Players[k]. g_sRegsport ，并使j增加1 
	      	j=j+1;
	      	if((strcmp(Players[i].g_sRegSport,y)==0)&&(strcmp(Players[i].g_sStuNum,t)==0))
	      	q=q+1;
	      	
          }
	if(q==0)
	{
		if(j<M)
		{
		z=0;       //j<M表示当前项目人数未报满，令z=0作为可继续报名的标志 
		}
		else
		z=1;       //返回z=1,则判断人数已满，无法报名 		
	}
	else {
		printf("已报过该项目！按任意键返回主菜单");
		getch();
		system("cls");
		return(main());
	}

    if(z==0)          
	strcpy(Players[num].g_sRegSport,y);
    else if(z==1)
    	    {
		        printf("该项目报名人数已满,按任意键返回主菜单");
				getch();
				system("cls");
				return(main());
		     }
}





//管理员端报名的人数判断 
int Adm_maxPlayer(char *x,int num)
{
	int j=0;          
	int i=0;      
	int z;            //判断是否可以报名的标志 
    char y[50];
    strncpy(y,x,sizeof(y));     	//将通过*x导入的结字符串赋给y[] 
    for(i=0;i<num;i++)      
	{   
		if(strcmp(Players[i].g_sRegSport,y)==0)     //循环找到与y相等的Players[k]. g_sRegsport ，并使j增加1 
	    j=j+1;
    }

	if(j<M)
	{
	z=0;       //j<M表示当前项目人数为报满，令z=0作为可继续报名的标志 
	}
	else
	z=1;       //返回z=1,则判断人数已满，无法报名 
	return z;
}


//超级用户 
int Vip()
{
	char developer[20]={"123456"};	//超级用户默认密码 
	int k,j=0;			
	char c;			//获取密码输入 
	char  PassWord[20] ;
	char  null[20]={0};
	L:	
		strcpy(PassWord,null);
		printf("输入密码：") ;
			for(k=0;k<j;k++)
				{
					if(PassWord[k]!='\0')
					printf("*");
				}				
		for(j=0;j<20;j++)
		    {	
			c=getch();
			if(c=='\r') //回车键 
			{
				PassWord[k]='\0';
				break;
			}
		
			else if(c=='\b')
			{
			k=k-1;
			PassWord[k]='\0';
			system("cls");				
			goto L; 
			}
			else
			{
			PassWord[k]=c;
			k++;
			printf("*")	;
			}
		}
	if((strcmp(PassWord,developer))!=0)
	{
		printf("\n默认密码错误，返回开始页面！\n");
		getch(); 
		return(main());
	}
	system("cls");
	printf("\n\n\t超级用户只能将各管理员的密码重置为默认值，以协助管理员修改密码\n");
	int manager; 
	int choice; 
	int i;
	fp=fopen("password.txt","r");
	for(i=0;i<3;i++)
	{
		fscanf(fp,"%s",password[i]);
	} 
	fclose(fp);
cho: 	printf("            \n\n\t\t1.   项目管理员             \n") ;
		printf("            \n\n\t\t2.   运动员管理员            \n") ;
    	printf("            \n\n\t\t3.   成绩管理员             \n") ;
		printf("\n\t请选择要置零的管理员编号\n") ;
        scanf("%d",&choice); 
		
            switch(choice)
	        {
			case 1 : strcpy(password[0],"000000"); break; //重置为默认密码000000 
			case 2 : strcpy(password[1],"000000"); break;  //重置为默认密码000000 
            case 3 : strcpy(password[2],"000000"); break; //重置为默认密码000000 
            default : printf("无法识别，请重新输入！\n");getch();system("cls");goto cho; break;
            } 
            fp=fopen("password.txt","w");
			for(i=0;i<3;i++)
			{
				fprintf(fp,"%s\n",password[i]);
			}
			fclose(fp);
			printf("\n\t重置成功，任意键返回主菜单\n");
			getch();
			return main();
} 


//修改管理员密码 
int ChangeAdministratorPassword()
{
	int choose;
	printf("\n\n\t\t请选择要修改密码的管理模块\n\n");
	printf("            \n\n\t\t1.   项目管理密码修改             \n") ;
	printf("            \n\n\t\t2.   运动员管理密码修改           \n") ;
   	printf("            \n\n\t\t3.   成绩管理密码修改             \n") ;
	scanf("%d",&choose);
	ChangeAdPasw(choose);
}

//==========管理员更改密码函数======// 
ChangeAdPasw(int cho)                
{
	int i;//循环 
	char c; //获取密码输入 
	int choose;
	char previous[20],newp[20] , check[20] ;
	printf("\n\n\t请输入当前密码：") ;
	for(i=0;i<20;i++)
		{
			c=getch();
			if(c=='\r')
			{
				previous[i]='\0';
				break;
			}
			else
			{
			previous[i]=c;
			printf("*");
			}	
		}
	if(strcmp(previous,password[cho-1])==0)
	{
		ChanAdPa : 
		printf("\n\t请输入新密码：") ;
		for(i=0;i<20;i++)
		{
			c=getch();
			if(c=='\r')
			{
				newp[i]='\0';
				break;
			}
			else
			{
			newp[i]=c;
			printf("*");
			}	
		}
		printf("\n\t请再次输入新密码：");
		for(i=0;i<20;i++)
		{
			c=getch();
			if(c=='\r')
			{
				check[i]='\0';
				break;
			}
			else
			{
			check[i]=c;
			printf("*");
			}	
		}
		
		if(strcmp(newp,check)==0)
		{
			strcpy(password[cho-1],newp) ;
			printf("\n\t更改成功！\n") ;
			
					
		if((fp=fopen("password.txt","w+"))!=NULL)
		{
				for(i=0;i<3;i++)
			{
				fprintf(fp,"%s\n",password[i]);
			}
			fclose(fp);
		}
			system("pause") ;
			system("cls") ;
			return(Administrator()) ;
		}
		else
		{
			printf("\n\t两次密码不一致，请再次输入新密码或取消更改！") ;
			printf("\t1.再次输入     2.取消更改") ;
			scanf("%d",&choose);
			switch(choose)
			{
				case 1 : goto ChanAdPa ; break ;
				case 2 : return(main()) ;break;
			}
			system("pause") ;
			system("cls") ;
		}
	}
	else
	{
		printf("\n\n\t当前密码不正确，请尝试联系超级用户!");
		getch();
		return(main());
	}

}

//管理员模式 
 int Administrator()
{
	if((fp=fopen("password.txt","a+"))==NULL)
	{
		printf("Error!");
		exit(0);
	}
	fscanf(fp,"%s%s%s",password[0],password[1],password[2]);
	fclose(fp);
	int choice; 
	 L: printf("            \n\n\t\t1.各级管理              		 \n") ;
	    printf("            \n\n\t\t2.更改管理员密码                 \n") ;
	    printf("            \n\n\t\t3.退出到开始界面                 \n") ;
	    printf("\n\t请输入数字进行选择\n") ;
		scanf("%d",&choice);
	        switch(choice)
	        {
			case 1 : system("cls");Manage(); break; //登录管理员账号 
            case 2 : system("cls");ChangeAdministratorPassword(); break;
            case 3 : system("cls");main(); break;
            default : printf("无法识别，请重新输入！\n"); getch();system("cls");goto L; break;
            }
}


//首次使用时设置管理员密码  
int Developer()
{
	 if((fp = fopen("password.txt","r"))==NULL) 
		{
			int i,j;//循环 
			char c;//获取密码输入 
			fp=fopen("password.txt","a+");
			for(i=0;i<3;i++)
			{
				switch(i)
				{
					case 0:printf("\n请输入项目管理员密码\n");break; 
					case 1:printf("\n请输入运动员管理员密码\n");break;
					case 2:printf("\n请输入成绩管理员密码\n");break;
				 } 

				for(j=0;j<20;j++)
				{	
					c=getch();
					if(c=='\r')
					{
						password[i][j]='\0';
						break;
					}
					else
					{
						password[i][j]=c;
						printf("*");
					}
				}
				fprintf(fp,"%s\n",password[i]);
			}
			fclose(fp);
		}
}


//密码块有漏 
WaysToAd(int i)              //=======登录管理员模式的进入判别函数=======// 
{
	int k,j=0;			
	char c;			//获取密码输入 
	char  PassWord[20] ;
	char  null[20]={0};
	L:	
		strcpy(PassWord,null);
		printf("请输入密码：") ;
			for(k=0;k<j;k++)
				{
					if(PassWord[k]!='\0')
					printf("*");
				}				
		for(j=0;j<20;j++)
		    {	
			c=getch();
			if(c=='\r')
			{
				PassWord[k]='\0';
				break;
			}
		
			else if(c=='\b')
			{
			k=k-1;
			PassWord[k]='\0';
			system("cls");				
			goto L; 
			}
			else
			{
			PassWord[k]=c;
			k++;
			printf("*")	;
			}
		}

		if(strcmp(PassWord,password[i-1])==0)
		{
			printf("\n登录成功，按任意键继续");
			getch() ;           //======调用管理员函数======// 
	    }
		else
		{
			printf("\n密码错误，按任意键再次输入！");
			getch();
			system("cls") ;
			goto L;
		}
}

//管理员模式具体操作 
int Manage()
{
	system("cls");
	
	    int choice;  
	L:    
        printf("			\n\n\t管理员模式					\n") ;
		printf("            \n\n\t\t1.   项目管理             \n") ;
		printf("            \n\n\t\t2.   运动员管理            \n") ;
    	printf("            \n\n\t\t3.   成绩管理             \n") ;
    	printf("            \n\n\t\t4.   返回主菜单             \n") ;
    	printf("\n\t请输入数字进行选择\n") ;
    	scanf("%d",&choice); 
            switch(choice)
	        {
			case 1 : system("cls");  system("cls");Sportop();break; 
            case 2 : system("cls"); system("cls");Playerop() ; break; 
            case 3 : system("cls"); system("cls");Gradeop();break;
            case 4 : return(main());break;
            default : printf("无法识别，请重新输入！\n");getch();system("cls");goto L; break;
            } 
 
}

//项目管理菜单 
int Sportop()
{	
	numofbS=0;numofgS = 0;numofAS=0;numofRS=0;numofFS=0; 
	system("cls");
	int tag;
		if((fp = fopen("Allsports.txt","r"))!=NULL)
	{
	while(fscanf(fp,"%s%d%d%d%d",allsport[numofAS].Sports_Name,&allsport[numofAS].sex,
	&allsport[numofAS].type,&allsport[numofAS].Group_OR_Population,&allsport[numofAS].eachtime)!=EOF)
    {
    numofAS++;                                     
    } //总的项目计数
    fclose(fp) ; //关闭文件
	//-----------------------------------------------//	
	//其他的项目计数		
	for(tag=0;tag<numofAS;tag++)
	{	
		if(allsport[tag].sex == 1)//男子 
		{
			boysport[numofbS]=allsport[tag];
			numofbS++ ;	
		}
		else  
		{
			girlsport[numofgS]=allsport[tag];
			numofgS++ ;//女子
		}

		if(allsport[tag].type == 1)//径赛
		{
			tracksport[numofRS]=allsport[tag];
			numofRS++ ;
		}
		else 
		{
			fieldsport[numofFS]=allsport[tag];
			numofFS++ ;//田赛 
		}
	}
	}
	printf("\n\n-------项目管理-------\n\n\t1.参赛单位设置\n\n\t2.项目列表\n\n\t3.添加项目\n\n\t4.删除项目\n\n\t5.修改项目信息\n\n\t6.返回\n");
	int choose;
	scanf("%d",&choose);
	L:switch(choose)
	{
		case 1:ParticUnit();break;
		case 2:Search_Sports();break;
		case 3:Add_Sports();break;
		case 4:Delete_Sports();break;
		case 5:Change_Sports();break;
		case 6:return(Manage());break;
		default : printf("无法识别，请重新确认！") ; getch();system("cls") ; goto L ;break;
	}	
}

//参赛单位设置 
int ParticUnit()
{	
	system("cls");
	int max;//最大参赛单位数 
	int i;//循环
	char Unit[20][20];
	int choose;
	printf("\n\n\t请输入参赛单位数目(少于20)\n");	
	scanf("%d",&max);
	printf("\n\n\t请逐一输入参赛单位（回车确认单个单位）\n");
	for(i=0;i<max;i++)
	{
	scanf("%s",Unit[i]);	
	}
	printf("\n\n\t参赛单位设定后不得更改，是否确认\n");
	printf("\n\t1.是\t2.否\n");
L:	scanf("%d",&choose);
	if(choose==1)
	{
		fp=fopen("Dorm.txt","a+");
		for(i=0;i<max;i++)
		{
		fprintf(fp,"%s\n",Unit[i]);
		}
		fclose(fp);
		printf("\n\t已保存，任意键返回项目管理菜单!\n");
		getch();
		return(Sportop());
	}
	else if(choose==2)
	{
		printf("\n\n\t任意键返回项目管理菜单页面\n") ;
		getch();
		return(Sportop());
	}
	else
	{
		printf("\n\n\t请选择有效选项\n");
		goto L;
	}
}

//修改项目信息：径赛每组或田赛每人的用时 
int Change_Sports()
{
	system("cls");
	int Change_Save_typeChoice = 0;			//修改用时 
	int tag;//循环判断 
	int choose;			//选择 
	int confirm;		//确定执行操作 
edit:	printf("\n\n\t修改径赛每组或田赛每人的用时\n");
	for(tag=0;tag<numofAS;tag++)
	{
		printf("\t%d.%s\n",tag+1,allsport[tag].Sports_Name);
	} 
	printf("\n\t请选择要修改的项目序号:");
	scanf("%d",&choose);
	if(choose<1||choose>numofAS)
		{
			printf("\n请选择有效的序号\n");getch();system("cls");goto edit;
		}
	system("cls");
conf:	printf("\n\n\t确定修改该项目吗?\n");
	printf("\t%s\n",allsport[choose-1].Sports_Name);
	printf("\n\t1.是\t2.否\n");
	scanf("%d",&confirm);
	if(confirm==2)
	{
		printf("\n\t任意键返回项目管理菜单页面\n") ;
		getch();
		return(Sportop());
	}
	if(confirm==1)
	{
		if(allsport[choose-1].type==1)			//径赛 
		printf("\n\t请修改每组的用时\n");
		else if(allsport[choose-1].type==2)
		printf("\n\t请修改每人的用时\n");
		scanf("%d",&Change_Save_typeChoice);
		allsport[choose-1].eachtime=Change_Save_typeChoice;				//修改 
		p_fold_Allsports=fopen("Allsports.txt","w");		//写回文件 
		for(tag=0;tag<numofAS;tag++)
		fprintf(p_fold_Allsports,"%s\t\t%d\t%d\t%d\t%d\n",allsport[tag].Sports_Name,allsport[tag].sex,allsport[tag].type,
										allsport[tag].Group_OR_Population,allsport[tag].eachtime);	
		fclose(p_fold_Allsports) ;
		printf("\n\t修改成功,按任意键返回项目管理菜单页面");
		getch();
		return(Sportop());
	}
	else
	{
		printf("\n\t请选择有效选项\n");
		goto conf;
	}
}

//删除项目 
int Delete_Sports(){
		
	system("cls");
	int tag;//循环判断 
	int choose;
	int confirm;
del:printf("\n\n");
	for(tag=0;tag<numofAS;tag++)
	{
		printf("\t%d.%s\n",tag+1,allsport[tag].Sports_Name);
	} 
	printf("\n\n\t请选择要删除的项目序号:");
	scanf("%d",&choose);
	if(choose<1||choose>numofAS)
		{
			printf("\n\t请选择有效的序号\n");getch();system("cls");goto del;
		}
	system("cls");
conf:	printf("\n\n\t确定删除该项目吗?\n");
	printf("\t%s\n",allsport[choose-1].Sports_Name);
	printf("\n\t1.是\t2.否");
	scanf("%d",&confirm);
	if(confirm==1)				//确认删除 
	{
		for(tag=choose-1;tag<numofAS;tag++)
			allsport[tag]=allsport[tag+1];				//删除 
			
		numofAS--;
		
		p_fold_Allsports=fopen("Allsports.txt","w");		//写回文件 
		for(tag=0;tag<numofAS;tag++)
		fprintf(p_fold_Allsports,"%s\t\t%d\t%d\t%d\t%d\n",allsport[tag].Sports_Name,allsport[tag].sex,allsport[tag].type,
										allsport[tag].Group_OR_Population,allsport[tag].eachtime);	
		fclose(p_fold_Allsports) ;
		printf("\n\t删除成功,按任意键返回项目管理菜单页面");
		getch();
		return(Sportop());
	}
	else if(confirm==2)			//不删除 
	{
		return(Sportop());
	}
	else
	{
		printf("\t请选择有效选项\n");
		goto conf; 
	}		
} 


//项目列表 
int Search_Sports(){
	
	system("cls");
	printf("\n\n\t请选择查找类型\n\n\t1.径赛\n\n\t2.田赛\n\n\t3.男生项目\n\n\t4.女生项目\n\n\t5.返回\n");
	int choose;
	int tag;
	int num=1; 
	scanf("%d",&choose);
	if(choose<1||choose>5)
	{
		printf("\n\t请重新选择正确序号");getch(); return Search_Sports();
	}
	if(choose==5) return Sportop(); 
	for(tag=0;tag<numofAS;tag++)
	{	
		if(allsport[tag].type==choose)						//输出田赛或者径赛 
		{
			printf("\t%d.%s\t每组或人用时:%d分钟\n",num,allsport[tag].Sports_Name,allsport[tag].eachtime);num++;
		}
		if(choose==3)
		{
			if(allsport[tag].sex==1)						//输出男生项目 
			{
				printf("\t%d.%s\t每组或人用时:%d分钟\n",num,allsport[tag].Sports_Name,allsport[tag].eachtime);num++;
			}
		}
		if(choose==4)
		{		
			if(allsport[tag].sex==2)						//输出女生项目 
			{
				printf("\t%d.%s\t每组或人用时:%d分钟\n",num,allsport[tag].Sports_Name,allsport[tag].eachtime);num++;
			}
		}			
	}
	printf("\n\n\t按任意键返回查找项目主页面");
	getch();
	return(Search_Sports());

} 


//项目添加 
int Add_Sports()
{
 	Sports_Single temp;			//临时存储单个项目
	int   firstchoice;	//选择径赛或田赛 
	char  name[20] ;//暂时存储输如的项目名字
	int   sex_choice= 0 ; //暂时存储选择的项目的性别 
	char sex1[20]={"男子"};
	char sex2[20]={"女子"};
	char sex[20];
	int tag,Lab; //循环标志 
loop:system("cls");
	numofbS=0;numofgS = 0;numofDorm=0;numofAS=0;numofRS=0;numofFS=0;
	//-------------------------------------------------//
	//打开文件读取项目
	//===================计数项目用于更新数组================// 
	fp = fopen("Allsports.txt","r");
	while(fscanf(fp,"%s%d%d%d%d",allsport[numofAS].Sports_Name,&allsport[numofAS].sex,
	&allsport[numofAS].type,&allsport[numofAS].Group_OR_Population,&allsport[numofAS].eachtime)!=EOF)
    {
    numofAS++;                                     
    } //总的项目计数
    fclose(fp) ; //关闭文件
	//-----------------------------------------------//
	//其他的项目计数
	printf("\t当前总项目数为%d\n",numofAS);
	
	
	for(tag=0;tag<numofAS;tag++)
	{	printf("\t%s\t\t%d\t%d\t%d\t%d\n",allsport[tag].Sports_Name,allsport[tag].sex,allsport[tag].type,allsport[tag].Group_OR_Population,allsport[tag].eachtime);
		if(allsport[tag].sex == 1)//男子 
		{
			boysport[numofbS]=allsport[tag];
			numofbS++ ;	
		}
		else  
		{
			girlsport[numofgS]=allsport[tag];
			numofgS++ ;//女子
		}

		if(allsport[tag].type == 1)//径赛
		{
			tracksport[numofRS]=allsport[tag];
			numofRS++ ;
		}
		else 
		{
			fieldsport[numofFS]=allsport[tag];
			numofFS++ ;//田赛 
		}
	} 

	//------------输入新项目的信息------------------//
		printf("\n\t请选择比赛类别\n") ; 
    	printf("\t1.径赛\t\t2.田赛\n");
	re:	scanf("%d",&firstchoice);
		if(firstchoice==1)
		temp.type=1;
		else if(firstchoice==2)
		temp.type=2;
		else
		{
			printf("请重新输入\n");goto re;
		}
		printf("\n\t请输入比赛项目：") ;

//	char  Judge_name_type[10] ; //定义用于判断用户的输入项目与选择的项目类别是否一致的变量 
	scanf("%s",name) ;

	//---------------------------// 
	printf("\n\t请选择比赛项目的参赛人员性别：（输入数字）\n");
	printf("\t1.男子");
	printf("\t2.女子") ;
	printf("\t3.男子和女子	\n") ;
	scanf("%d",&sex_choice);
	
	if(temp.type==1)			 
	temp.Group_OR_Population=1;			//径赛设置只有一组
	else if (temp.type==2)
	temp.Group_OR_Population=0;			//表示田赛人数，初始为0 
	printf("\t请设定比赛项目需要花费的比赛时间\n(竞赛按每组，田赛按每人计算，请输入数字)：");
	scanf("%d",&temp.eachtime); 
	//---------------------------------------//
	//截取输入字符串的前两个字符

	
	p_fold_Allsports=fopen("AllSports.txt","a+") ;
	if(sex_choice==1)//选择了男子 
	{
		temp.sex=1;
		strcpy(sex,sex1);
		strcat(sex,name);
		strcpy(temp.Sports_Name,sex) ;//存储的项目名为 男子+项目
		for(tag=0;tag<numofAS;tag++)
	{
		if(strcmp(temp.Sports_Name,allsport[tag].Sports_Name)==0)
		{
			printf("\n\t该项目已添加，请重新添加项目");			
			getch();
			system("cls");
			goto loop;
		}
	}
		//未添加的项目成功添加 
		fprintf(p_fold_Allsports,"%s\t\t%d\t%d\t%d\t%d\n",temp.Sports_Name,temp.sex,temp.type,temp.Group_OR_Population,temp.eachtime);
		numofbS++;
	}


	if(sex_choice==2) 
	{//选择了女子 
		temp.sex=2;
		strcpy(sex,sex2);
		strcat(sex,name);
		strcpy(temp.Sports_Name,sex) ;			//存储的项目名为 女子+项目
		for(tag=0;tag<numofAS;tag++)
		{
		if(strcmp(temp.Sports_Name,allsport[tag].Sports_Name)==0)
		{
			printf("\n\t该项目已添加，请重新添加项目");			
			getch();
			system("cls");
			goto loop;
		}
		} 
		fprintf(p_fold_Allsports,"%s\t\t%d\t%d\t%d\t%d\n",temp.Sports_Name,temp.sex,temp.type,temp.Group_OR_Population,temp.eachtime);
		numofgS++;
	}

	if(sex_choice==3)
	{
		int repeat=0;
		//先操作男子 
		temp.sex=1;
		strcpy(sex,sex1);
		strcat(sex,name);
		strcpy(temp.Sports_Name,sex) ;//存储的项目名为 男子+项目
		for(tag=0;tag<numofAS;tag++)
		{
			if(strcmp(temp.Sports_Name,allsport[tag].Sports_Name)==0)		//男生项目已存储 
			{	
				printf("\n\t男生项目已添加，无需重复添加\n"); 
				repeat=1;
				break;
			}
		}
		if(repeat==0)
		{
		fprintf(p_fold_Allsports,"%s\t\t%d\t%d\t%d\t%d\n",temp.Sports_Name,temp.sex,temp.type,temp.Group_OR_Population,temp.eachtime);
		printf("\t男生项目添加成功\n");
		}		
						
		temp.sex=2;
		strcpy(sex,sex2);
		strcat(sex,name);
		strcpy(temp.Sports_Name,sex) ;			//存储的项目名为 女子+项目 
		for(Lab=0;Lab<numofAS;Lab++)
		{
			if(strcmp(temp.Sports_Name,allsport[Lab].Sports_Name)==0)		//女生项目已存储 ，直接返回项目输入 
			{
				printf("\n\t女生项目已添加，无需重复添加");	
				repeat=2;		
				break;		
			}
		}	
		if(repeat==0||repeat==1)
		{
			printf("\t女生项目添加成功\n");
			fprintf(p_fold_Allsports,"%s\t\t%d\t%d\t%d\t%d\n",temp.Sports_Name,temp.sex,temp.type,temp.Group_OR_Population,temp.eachtime);
		}		
	}


	fclose(p_fold_Allsports);  //关闭总项目文件 
	int continue_choice;//用于判定是否继续 
	printf("\n\n\t操作完毕!\n") ;
	con:	printf("\n\t是否继续增加项目?\n");
		printf("\t1.继续增加	2.退出\n");
		scanf("%d",&continue_choice) ;
		if(continue_choice==1) {goto loop;}
		else if(continue_choice==2)
		{
			return Sportop();
		}
		else
		{
			printf("\t请重新选择有效选项\n");
			getch();
			system("cls");
			goto loop;
		}	
} 

//运动员管理菜单 
int Playerop()
{
	system("cls");
	{//将文件里已有报名信息写入结构数组
	h=0; 
	if((fp=fopen("PLAYERS.txt","a+"))==NULL) 			
    {
    	printf("无法打开此文件\n");	
		system("pause")	;		 	 	
		exit(0); 
	}
    while(fscanf(fp,"%s%s%s%s%s%s%lf%d",Players[h].g_sStuNum,Players[h].g_sStuName,Players[h].g_cSex,Players[h].g_sPhoneNumber,Players[h].g_sDormitory,Players[h].g_sRegSport,&Players[h].g_dMark,&Players[h].g_iScore)!=EOF) 
   {
//  打印运动员信息 	print(h);
   h++;                                     
   }
   fclose(fp);
   }
	printf("\n\n-------运动员管理-------\n\n\t1.删除运动员\n\n\t2.修改报名项目\n\n\t3.运动员列表\n\n\t4.秩序册生成\n\n\t5.返回\n");
	int choose;
L:	scanf("%d",&choose);
	switch(choose)
	{

		case 1:Delete();break;
		case 2:Edit_Player_infomation();break;
		case 3:search_Player();break;
		case 4:Volume(); break;
		case 5:return(Manage());break;
		default : printf("无法识别，请重新确认！") ; getch();system("cls") ; goto L ;break;
	}
}


//秩序册 
int Volume()
{
	int startt1,startt2;
	int timelength ;
	Time ctimelist[numofRS];		//径赛时间表 
	Time ftimelist[numofFS];		//田赛时间表 
	system("cls");
	check();
	if(judge1==true)
	close();
	int i 	;		//循环及项目序号 
	int j	;		//循环及项目序号 
	fp=fopen("Beginning.txt","a+");
	fscanf(fp,"%s%d%d%d",university.name,&university.number,&university.month,&university.bday);
	printf("\n\n\t\t\t%s%d届运动会竞赛规程\n\n",university.name,university.number);
	printf("\t一.主办单位:%s\n",university.name);
	printf("\t二.承办单位:%s体育部\n",university.name);
	printf("\t三.大会口号:艰难困苦，风雨无阻，亦文亦武，共展宏图。\n");
	printf("\n\t四.报名规则\n\t1.报名时请学生认真填写个人信息，不得重复报名同个项目\n\t2.认真阅读比赛发布信息，不得报连续时间段的比赛，上下午不算作连续\n");
	printf("\t3.各参赛单位不设置报名人数限制，各个项目有报名人数限制\n");
	printf("\n\t五.得分制\n");
	printf("\t学生根据自己的比赛成绩获得相应得分\n");
	printf("\n\t六.工作人员\n");
	printf("\t运动会工作人员全由校内教职工负责，各项目裁判由体育老师担任\n"); 
    printf("\n\t七.运动会负责人信息\n");
    printf("\t1.电话：12345678901\n");
    printf("\t2.邮箱：123@stu.edu.cn\n");
    printf("\n\t八.注意事项\n");
    printf("\t1.运动会开始前，工作人员应认真检查场地及设备\n");
	printf("\t2.运动员应认真对待比赛，不得佩戴首饰，比赛前应做好准备活动，防止肌肉和韧带受伤\n");	
	printf("\t3.运动员比赛前不宜大量饮食，运动后适当饮水或葡萄糖等，不宜喝冷饮\n"); 
	printf("\t4.非运动员的学生及非工作人员教师不宜在比赛场地上穿行，应在指定地点观看比赛，运动员比赛结束后,\n也应迅速撤离比赛场地，到指定地点休息\n"); 
	printf("\n\t径赛\n");
	printf("\t项目\t\t\t组数\t\t时间\n");
	fclose(fp);
	for(i=0;i<numofRS;i++)
	{

		ctimelist[i].num1=1;		//暂时限定一组，若有多组应设为从人数判断 ！！！！！！！！！！！！！！！！！！！！！！！！！！！！ 
	
		timelength = ctimelist[i].num1 * tracksport[i].eachtime;
	
		if(i>0&&ctimelist[i-1].endtime1>=11)
			{
				startt1 = 14;
				startt2 = 0;			
			}
		else
		{
			startt1 = 9;
			startt2 = 0;
		}
	
		if(((i>=1)&&(ctimelist[i-1].endtime1<11))||((i>=1)&&(ctimelist[i-1].endtime1>=14)))
		{	
			ctimelist[i].starttime2=ctimelist[i-1].endtime2+15;
			if(ctimelist[i].starttime2>=60)
				{
					ctimelist[i].starttime1=ctimelist[i-1].endtime1+1;
					ctimelist[i].starttime2=ctimelist[i].starttime2-60;	
				}
			else 
				{	
					ctimelist[i].starttime1=ctimelist[i-1].endtime1;			
				}
				ctimelist[i].endtime2=ctimelist[i].starttime2+timelength;
			if(ctimelist[i].endtime2>=60)
			{
				ctimelist[i].endtime1=ctimelist[i].starttime1+1;
				ctimelist[i].endtime2=ctimelist[i].endtime2-60;
			}
			else
				{
					ctimelist[i].endtime1=ctimelist[i].starttime1;
				}
		}
		else
		{
			ctimelist[i].starttime1=startt1;
			ctimelist[i].starttime2=startt2;
			ctimelist[i].endtime2=ctimelist[i].starttime2+timelength;
			if(ctimelist[i].endtime2>=60)
				{
					ctimelist[i].endtime1=ctimelist[i].starttime1+1;
					ctimelist[i].endtime2=ctimelist[i].endtime2-60;
				}		
			else
				{
					ctimelist[i].endtime1=ctimelist[i].starttime1;
				}
		}
		if(ctimelist[i].starttime2<10)
		printf("\t%s\t\t1\t\t%d:0%d",tracksport[i],ctimelist[i].starttime1,ctimelist[i].starttime2);
		else
		printf("\t%s\t\t1\t\t%d:%d",tracksport[i],ctimelist[i].starttime1,ctimelist[i].starttime2);
		printf(" -- ");
		if(ctimelist[i].endtime2<10)
		printf("%d:0%d\n",ctimelist[i].endtime1,ctimelist[i].endtime2);
		else
		printf("%d:%d\n",ctimelist[i].endtime1,ctimelist[i].endtime2);
		
	}//for(径赛) 
	
	
	printf("\n\t田赛\n");
	printf("\t项目\t\t\t人数\t\t时间\n");
	for(i=0;i<numofFS;i++)
	{		

	ftimelist[i].num1=0;
	for(j=0;j<h;j++)
	{
		if(strcmp(Players[j].g_sRegSport,fieldsport[i].Sports_Name)==0)
		ftimelist[i].num1++;
	}
	
	timelength = ftimelist[i].num1 * fieldsport[i].eachtime;
	if(i>0&&ftimelist[i-1].endtime1>=11)
		{
			startt1 = 13;
			startt2 = 0;			
		}
	else
	{
		startt1 = 9;
		startt2 = 0;
	}
	
	if(((i>=1)&&(ftimelist[i-1].endtime1<11))||((i>=1)&&(ftimelist[i-1].endtime1>=13)))
	{	
		ftimelist[i].starttime2=ftimelist[i-1].endtime2+15;
		if(ftimelist[i].starttime2>=60)
		{
			ftimelist[i].starttime1=ftimelist[i-1].endtime1+1;
			ftimelist[i].starttime2=ftimelist[i].starttime2-60;	
		}
		else 
		{	
			ftimelist[i].starttime1=ftimelist[i-1].endtime1;			
		}
		ftimelist[i].endtime2=ftimelist[i].starttime2+timelength;
		if(ftimelist[i].endtime2>=60)
		{
			ftimelist[i].endtime1=ftimelist[i].starttime1+1;
			ftimelist[i].endtime2=ftimelist[i].endtime2-60;
		}
		else
		{
			ftimelist[i].endtime1=ftimelist[i].starttime1;
		}
	}
	else
	{
		ftimelist[i].starttime1=startt1;
		ftimelist[i].starttime2=startt2;
		ftimelist[i].endtime2=ftimelist[i].starttime2+timelength;
		if(ftimelist[i].endtime2>=60)
		{
			ftimelist[i].endtime1=ftimelist[i].starttime1+1;
			ftimelist[i].endtime2=ftimelist[i].endtime2-60;
		}
		else
		{
			ftimelist[i].endtime1=ftimelist[i].starttime1;
		}
	}
	if(ftimelist[i].starttime2<10)
	printf("\t%s\t\t%d\t\t%d:0%d",fieldsport[i],ftimelist[i].num1,ftimelist[i].starttime1,ftimelist[i].starttime2);
	else
	printf("\t%s\t\t%d\t\t%d:%d",fieldsport[i],ftimelist[i].num1,ftimelist[i].starttime1,ftimelist[i].starttime2);
	printf(" -- ");
	if(ftimelist[i].endtime2<10)
	printf("%d:0%d\n",ftimelist[i].endtime1,ftimelist[i].endtime2);
	else
	printf("%d:%d\n",ftimelist[i].endtime1,ftimelist[i].endtime2);
		
	} 
	
	back();

}


//修改运动员信息 
int Edit_Player_infomation()
{
	int tag1=0;
	int tag2=0;

	char num[20],sport[20];
	int i,j,tag;				//循环 
	int firstchoose;			//已报名项目序号 
	int secondchoose;			//修改的项目序号 
Editt:	system("cls");
	printf("\n\n\t1.修改\t2.返回\n");
	scanf("%d",&firstchoose);
	if(firstchoose==2) return(Playerop());
	if(firstchoose<1||firstchoose>2) {
		printf("\n\n\t按任意键后重新选择\n");
		getch();
		goto Editt;
	} 
	printf("\n\t请输入修改的运动员学号：");
	scanf("%s",&num);				//输入学号 
	
	//选择项目 
	for(j=0;j<numofAS;j++)				
		{
				printf("\t\t%d.%s\n\n",j+1,allsport[j]);
		}
      L:scanf("%d",&firstchoose);				//选择项目 	
       	if(firstchoose>=1&&firstchoose<=numofAS)		
			strcpy(sport,allsport[firstchoose-1].Sports_Name);				
		else	
			{printf("\n\t无法识别，请再次输入")	;getch();goto L ; }
			
			
	//查找对应的运动员信息 
	for(i=0;i<h;i++)
	{
		if(strcmp(Players[i].g_sStuNum,num)==0)
		    {
		    	tag1==1;		//表示有对应学号的信息 
		    	
		    	if(strcmp(Players[i].g_sRegSport,sport)==0)
		    	{tag2==1;		//表示有对应项目的信息 
				printf("%s\t%s\t%s\t%s\t%s\t%s\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
				Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_sPhoneNumber);
				P:
                printf("\n\t是否修改该运动员信息？\t1.是\t2.否\n");
                scanf("%d",&j);
				switch(j)
				{
					case 1:
					{     //确定修改 
					printf("\n\n\t修改报名项目为(选择序号)\n\n");
					if((strcmp(Players[i].g_cSex,"男"))==0)
					{
						for(tag=0;tag<numofbS;tag++)
						{
							printf("\t%d.%s\n\n",tag+1,boysport[tag].Sports_Name);
						}	
										
			editsport:	scanf("%d",&secondchoose);
						if(secondchoose<1||secondchoose>numofbS)		//输入的序号不合法 
						{
							printf("\n\n\t请重新选择\n");
							goto editsport;
						}
					//判断项目是否满员 
						if((Adm_maxPlayer(boysport[secondchoose-1].Sports_Name,h))==1)
						{
							printf("\n\n\t!!当前所选项目已经满员，任意键后返回!!\n");
							getch();
							goto Editt;
						}
						printf("\n\n\t是否将报名信息修改为\n");
						printf("%s\t%s\t%s\t%s\t%s\t%s\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
						Players[i].g_sDormitory,boysport[secondchoose-1].Sports_Name,Players[i].g_sPhoneNumber);
						printf("\n\t1.是\t2.否\n");
                		scanf("%d",&j);
                		if(j==1)
                		{
                			strcpy(Players[i].g_sRegSport,boysport[secondchoose-1].Sports_Name);
						}
						else if(j==2)
						{
						printf("\n\n\t取消修改\n");
						printf("\n\t按任意键返回运动员管理菜单\n");
						getch();
						Playerop();
						}
					}//if(男)
					 
					else if((strcmp(Players[i].g_cSex,"女"))==0)
					{
						for(tag=0;tag<numofgS;tag++)
						{
							printf("\t%d.%s\n\n",tag+1,girlsport[tag].Sports_Name);
						}					
			choseditsport:	scanf("%d",&secondchoose);
						if(secondchoose<1||secondchoose>numofbS)		//输入的序号不合法 
						{
							printf("\n\n\t请重新选择\n");
							goto choseditsport;
						}
						//判断项目是否满员 
						if((Adm_maxPlayer(girlsport[secondchoose-1].Sports_Name,h))==1)
						{
							printf("\n\n\t!!当前所选项目已经满员，任意键后返回!!\n");
							getch();
							goto Editt;
						}
						printf("\n\n\t是否将报名信息修改为\n");
						printf("%s\t%s\t%s\t%s\t%s\t%s\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
					Players[i].g_sDormitory,girlsport[secondchoose-1].Sports_Name,Players[i].g_sPhoneNumber);
					printf("\n\t1.是\t2.否\n");
                	scanf("%d",&j);
                	if(j==1)
                	{
                		strcpy(Players[i].g_sRegSport,girlsport[secondchoose-1].Sports_Name);
					}
					else if(j==2)
					{
						printf("\n\n\t取消修改\n");
						printf("\n\t按任意键返回运动员管理菜单\n");
						getch();
						Playerop();
					}
					}//if(女)
					
					//将数据重新写回文件 
					if((fp=fopen("PLAYERS.txt","w"))==NULL)
						{
							printf("无法打开此文件");
							exit(0);
						}
               		 for(i=0;i<h;i++)
					{
 						fprintf(fp,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
							Players[i].g_sPhoneNumber,Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
					}
					fclose(fp);
					system("cls");
					printf("\n\n\t已成功修改该报名项目信息\n");					
					printf("\n\t按任意键返回运动员管理菜单\n");
					getch();
					Playerop();
					break;
					} 
					
					case 2:{     //不修改
					if((fp=fopen("PLAYERS.txt","w"))==NULL)
						{
							printf("无法打开此文件");
							exit(0);
						}
					for(i=0;i<h;i++)
					{
 					fprintf(fp,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
						Players[i].g_sPhoneNumber,Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
					}
					fclose(fp);
					printf("\n\t按任意键返回运动员管理菜单\n");
					getch();
					Playerop();
					break;
					}					
					default:printf("\t请选择正确的序号\n");goto P;break;
				}                
				}
			}
		
	}//for
			
		if(tag1==0)
		{
			if((fp=fopen("PLAYERS.txt","w"))==NULL)
						{
							printf("无法打开此文件");
							exit(0);
						}
			for(i=0;i<h;i++)
				{
 					fprintf(fp,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
						Players[i].g_sPhoneNumber,Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
				}
			fclose(fp);
			printf("\t未找到匹配学号的学生信息\n");
			printf("\n\t按任意键返回运动员管理菜单\n");
			getch();
			Playerop();
		}
		if(tag1==1&&tag2==0)
		{
			if((fp=fopen("PLAYERS.txt","w"))==NULL)
						{
							printf("无法打开此文件");
							exit(0);
						}
			for(i=0;i<h;i++)
				{
 					fprintf(fp,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
						Players[i].g_sPhoneNumber,Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
				}
			fclose(fp);
			printf("\t未找到匹配项目的学生信息\n");
			printf("\n\t按任意键返回运动员管理菜单\n");
			getch();
			Playerop();
		}	

} 


//查找运动员 
int search_Player()
{
	 
	int firstchoose;		//选择比赛类别
	int secondchoose;        //选择项目 
	int tag; 		//循环判断 
first:system("cls");
	printf("\n\n\t按比赛类别查找");
	printf("\n\n\t1.径赛\n\n\t2.田赛\n\n\t3.返回\n");
	scanf("%d",&firstchoose);
	
	system("cls");
	printf("\n\t请选择项目序号\n\n");
	
	//选择径赛
	if(firstchoose==1)
	{		
		for(tag=0;tag<numofRS;tag++)
		{
			printf("\t%d.%s\n\n",tag+1,tracksport[tag].Sports_Name);
		}
		scanf("%d",&secondchoose);
		secondchoose=secondchoose-1;
		for(tag=0;tag<h;tag++)
		{
			if((strcmp(tracksport[secondchoose].Sports_Name,Players[tag].g_sRegSport))==0)
			printf("%s\t%s\t%s\t%s\t%s\t%s\n",Players[tag].g_sStuNum,Players[tag].g_sStuName,Players[tag].g_cSex,
				Players[tag].g_sDormitory,Players[tag].g_sRegSport,Players[tag].g_sPhoneNumber);
		}	
		printf("\n查找完毕\n");
		getch();
		goto first;
			
	}//if
	
	//选择田赛 
	else if(firstchoose==2)
	{
		for(tag=0;tag<numofFS;tag++)
		{
			printf("\t%d.%s\n\n",tag+1,fieldsport[tag].Sports_Name);
		}
		scanf("%d",&secondchoose);
		secondchoose=secondchoose-1;
		for(tag=0;tag<h;tag++)
		{
			if((strcmp(fieldsport[secondchoose].Sports_Name,Players[tag].g_sRegSport))==0)
			printf("%s\t%s\t%s\t%s\t%s\t%s\n",Players[tag].g_sStuNum,Players[tag].g_sStuName,Players[tag].g_cSex,
				Players[tag].g_sDormitory,Players[tag].g_sRegSport,Players[tag].g_sPhoneNumber);
		}	
		printf("\n查找完毕\n");
		getch();
		goto first;
		
	}//if
	else if(firstchoose==3)
	{
		return(Playerop());
	}
	else
	{
		printf("\n\n\t请重新选择");
		goto first;
	}
	


	
}




//运动员信息删除 
int Delete()
{
	int tag1=0;
	int tag2=0;
	if((fp=fopen("PLAYERS.txt","w"))==NULL)
	{
		printf("无法打开此文件");
		exit(0);
	}
	char num[20],sport[20];
	int i,j,c;
	system("cls");
	printf("\n\t请输入删除的运动员学号：");
	scanf("%s",&num);
	//选择项目 
	for(j=0;j<numofAS;j++)				
		{
				printf("\t\t%d.%s\n",j+1,allsport[j]);
		}
      L:scanf("%d",&c);
       	if(c>=1&&c<=numofAS)
			strcpy(sport,allsport[c-1].Sports_Name);				
		else	
			{printf("\n\t无法识别，请再次输入")	;getch();goto L ; }
	//查找 
	for(i=0;i<h;i++)
	{
		if(strcmp(Players[i].g_sStuNum,num)==0)
		    {
		    	tag1==1;		//表示有对应学号的信息 
		    	
		    	if(strcmp(Players[i].g_sRegSport,sport)==0)
		    	{tag2==1;		//表示有对应项目的信息 
				print(i);
				P:
                printf("\n\t是否删除该运动员信息？\t1.是\t2.否\n");
                scanf("%d",&j);
				switch(j)
				{
					case 1:     //确定删除 
					g_ptoSTUDENT=fopen("DELETE.txt","a+");				//另外保存被删除的信息 
					fprintf(g_ptoSTUDENT,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
							Players[i].g_sPhoneNumber,Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
					fclose(g_ptoSTUDENT);
						//删除 
					for(j=i;j<h;j++)		
					Players[j]=Players[j+1];
						//剩余数据写回文件 
               		 for(i=0;i<h-1;i++)
					{
 						fprintf(fp,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
							Players[i].g_sPhoneNumber,Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
					}
					fclose(fp);
					system("cls");
					printf("\n\n\t已成功删除该报名信息\n");					
					printf("\n\t按任意键返回运动员管理菜单\n");
					getch();
					Playerop();
					break;
					
					case 2:     //不删除 
					for(i=0;i<h;i++)
					{
 					fprintf(fp,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
						Players[i].g_sPhoneNumber,Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
					}
					fclose(fp);
					printf("\n\t按任意键返回运动员管理菜单\n");
					getch();
					Playerop();
					break;					
					default:printf("\t请选择正确的序号\n");goto P;break;
				}                
				}
			}
		
	}//for
			
		if(tag1==0)
		{
			for(i=0;i<h;i++)
				{
 					fprintf(fp,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
						Players[i].g_sPhoneNumber,Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
				}
			fclose(fp);
			printf("\t未找到匹配学号的学生信息\n");
			printf("\n\t按任意键返回运动员管理菜单\n");
			getch();
			Playerop();
		}
		if(tag1==1&&tag2==0)
		{
			for(i=0;i<h;i++)
				{
 					fprintf(fp,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
						Players[i].g_sPhoneNumber,Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
				}
			fclose(fp);
			printf("\t未找到匹配项目的学生信息\n");
			printf("\n\t按任意键返回运动员管理菜单\n");
			getch();
			Playerop();
		}	

} 

//成绩管理菜单
int Gradeop()
{
	{//将文件里已有报名信息写入结构数组
	h=0; 
	if((fp=fopen("PLAYERS.txt","a+"))==NULL) 			
    {
    	printf("无法打开此文件\n");	
		system("pause")	;		 	 	
		exit(0); 
	}
    while(fscanf(fp,"%s%s%s%s%s%s%lf%d",Players[h].g_sStuNum,Players[h].g_sStuName,Players[h].g_cSex,Players[h].g_sPhoneNumber,Players[h].g_sDormitory,Players[h].g_sRegSport,&Players[h].g_dMark,&Players[h].g_iScore)!=EOF) 
   {
//  打印运动员信息 	print(h);
   h++;                                     
   }
   fclose(fp);
   }
	system("cls") ;
	int FirstChioce;
	FirstMenu:
	printf("\n\n\t1.输入成绩\n");
	printf("\n\n\t2.计算得分\n");
	printf("\n\n\t3.更改成绩\n");
	printf("\n\n\t4.查看成绩\n");
	printf("\n\n\t5.删除成绩\n"); 
	printf("\n\n\t6.返回\n");
	printf("\n\t请输入数字进行选择\n") ;
	scanf("%d",&FirstChioce);
	system("cls") ;                //===清屏====// 
	switch(FirstChioce)
	{
		case 1 : getMark() ;    break ;                
		case 2 : getScore();	break ; 
		case 3 : changeMark();      break ;               
		case 4 : SearchMenu();      break ;                 
		case 5 : DeleteMark();       break ;   
		case 6 : Manage() ;      break ;                    
		default: printf("\n无法识别，请再次输入\n") ; goto FirstMenu ; break; 
	}
} 


//为所选运动员输入成绩 
int  getMark()
{
	
	if(h==0)
	{
		printf("\n\t当前没有学生信息\n");
		back(); 
	}
	
	char sport[20];
	int i,c;
	double j;
	L:
		for(i=0;i<numofAS;i++)
		{
			printf("\t%d.",i+1);
			printf("%s\n",allsport[i].Sports_Name);
		}
		printf("\n\n\t请输入项目序号：");
        scanf("%d",&c);      //选择需要输入成绩的项目 
        if(c<=numofAS&&c>0)
		{
			for(i=0;i<numofAS;i++)
			{
				if(i+1==c)
					strcpy(sport,allsport[i].Sports_Name);
			}
		}
		else 
			{printf("\n\n\t请选择正确的序号\n");goto L;}
			
		system("cls");
		
		for(i=0;i<h;i++)
		{
			if(strcmp(Players[i].g_sRegSport,sport)==0)     //若存在选择该项目的运动员，！！！！！！！！！！！输入成绩，要设置两种设置方式 
			{
				 
				printf("\n");
		        printf("%s\t%s\n",Players[i].g_sStuNum,Players[i].g_sStuName);
		        printf("\n请为该运动员输入成绩:");
		        scanf("%lf",&j);
		        Players[i].g_dMark = j;
		        printf("\n");
		        printf("%s\t%s\t%.2f\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_dMark);
		        printf("\n");
			}
		}
		if((fp=fopen("PLAYERS.txt","w"))==NULL)
		{
			printf("无法打开此文件");
			exit(0);
		}
	    for(i=0;i<h;i++)
		{
 			fprintf(fp,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
			Players[i].g_sPhoneNumber,Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
		}
		fclose(fp);
		int choosenext; 
		printf("\n\n\t输入完毕");
		printf("\n\n\t1.选择其他项目\n\n\t2.返回成绩管理菜单\n\n\t3.退出");
		waycho:	scanf("%d",&choosenext) ;
		if(choosenext==1)
		{
			system("cls");
			goto L;
		}
		else if(choosenext==2)
		{
			return(Gradeop());
		}
		else if(choosenext==3)
		{
			exit(0);
		}
		else 
		{
			printf("\n\n\t重新选择\n");
			goto waycho;
		}
}

//根据运动员的项目成绩给予得分
int getScore()
{
	if((fp=fopen("Gradepublish.txt","r"))==NULL) 
	{
		if((MessageBox(NULL,TEXT("是否发布成绩"), TEXT("提示"),MB_OKCANCEL))==1)
		{	
			fp=fopen("Gradepublish.txt","w");//成绩发布 
			fclose(fp);
			MessageBox(NULL,TEXT("成绩已发布"), TEXT("提示"),MB_OK); 
		}
		else
		{
			MessageBox(NULL,TEXT("成绩未发布"), TEXT("提示"),MB_OK); 
		}
	}
	else
	{
		printf("\n\t当前成绩及得分已发布，学生可查询成绩\n");
	}
	
	int i,j,k,m;
	i=j=k=m=0;
	int choosenext;
    char sport[50];
    fp=fopen("PLAYERS.txt","w");
    fclose(fp);	
    for(i=0;i<numofAS;i++)			//遍历所有项目 
	{
		k=0;
		for(j=0;j<h;j++)				
		{
			if(strcmp(Players[j].g_sRegSport,allsport[i].Sports_Name)==0)		//将同一项目的运动员信息放在一个临时数组 
				temp[k++] = Players[j];
		}
		if(allsport[i].type==1)				//项目为径赛 
			LowtoHighSort(temp,k);			//排序并写入文件 ，竞赛的成绩排序方式！！！！！！！！！！！ 
		else if(allsport[i].type==2)		//项目为田赛 
			HightoLowSort(temp,k);			//排序并写入文件 ，田赛的成绩排序方式！！！！！！！！！！！ 
			
	}
	//将文件里排序后的信息写入结构数组
	i=0; 
	fp=fopen("PLAYERS.txt","r");			
    while(fscanf(fp,"%s%s%s%s%s%s%lf%d",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,Players[i].g_sPhoneNumber,Players[i].g_sDormitory,Players[i].g_sRegSport,&Players[i].g_dMark,&Players[i].g_iScore)!=EOF) 
   	{
   		i++;                                     
   	}
   	fclose(fp); 
 L: k=0;
 	printf("\n\n\t\t请选择要打印的学生得分表\n\n ");
	for(i=0;i<numofAS;i++)				//打印可选项目信息 
	{
		printf("\t%d.",i+1);			 
		printf("%s\n",allsport[i].Sports_Name);
	}
		scanf("%d",&m);					//输入项目信息 
		if(m<=numofAS&&m>0)
		{
			strcpy(sport,allsport[m-1].Sports_Name);
		}
		else 
		{printf("\n\n\t请选择正确的序号\n");goto L;}
   	
   	//打印对应项目成绩和得分 
   	for(i=0;i<numofAS;i++)
	{
		if(strcmp(sport,allsport[i].Sports_Name)==0)			//找到选择的项目 
		{
			system("cls");
			printf("\n\n\t\t%s",allsport[i].Sports_Name);
			printf("成绩及得分\n\n");
			for(j=0;j<h;j++)
			{
				if(strcmp(Players[j].g_sRegSport,allsport[i].Sports_Name)==0)
					temp[k++] = Players[j];				//将该项目的运动员信息写入一个数组 
			}
			if(k<ddlperson)
			{
				printf("该项目参赛人数不足，无法统计成绩！");
				getch();
				return(Gradeop());				
			}
			else
				printScore(temp,k);						//打印 
		}
	}
	printf("\n\n\t1.选择其他项目\n\n\t2.返回成绩管理菜单\n\n\t3.退出\n");
waycho:	scanf("%d",&choosenext) ;
	if(choosenext==1)
	{
		system("cls");
		goto L;
	}
	else if(choosenext==2)
	{
		return(Gradeop());
	}
	else if(choosenext==3)
	{
		exit(0);
	}
	else 
	{
		printf("\n\n\t重新选择\n");
		goto waycho;
	}
} 

//对成绩进行高到低排序的函数，并写入文件 ，给分数 
int HightoLowSort(Player x[M],int y)
{
 	int i,j;
 	Player t;
 	for(i=0;i<y;i++)
 	{
 		for(j=0;j<y-1;j++)
		{
			if(x[j].g_dMark<x[j+1].g_dMark)
			{
				t=x[j];
				x[j]=x[j+1];
			 	x[j+1]=t;
			} 
		}	
	}
	if(y<4)
	{
		for(i=0;i<y;i++)
		{
				x[i].g_iScore=0;
		}		

	}
	if(y>=4&&y<=6)
	{
		for(i=0;i<y;i++)
		{
			if(i==0)
			{
				x[i].g_iScore=5;
			}
			else if(i==1)
			{
				x[i].g_iScore=3;
			}
			else if(i==2)
			{
				x[i].g_iScore=2;
			}
			else
			{
				x[i].g_iScore=0;
			}
		}		
	}
	if(y>6)
	{
		for(i=0;i<y;i++)
		{
			if(i==0)
			{
				x[i].g_iScore=7;
			}
			else if(i==1)
			{
				x[i].g_iScore=5;
			}
			else if(i==2)
			{
				x[i].g_iScore=3;
			}
			else if(i==3)
			{
				x[i].g_iScore=2;
			}
			else if(i==4)
			{
				x[i].g_iScore=1;
			}
		}		
	}
	if((g_ptoSTUDENT=fopen("PLAYERS.txt","a+"))==NULL)
	{
		printf("无法打开文件");
		exit(0);
	}
	for(i=0;i<y;i++)
	{
		fprintf(g_ptoSTUDENT,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",x[i].g_sStuNum,x[i].g_sStuName,x[i].g_cSex,
			x[i].g_sPhoneNumber,x[i].g_sDormitory,x[i].g_sRegSport,x[i].g_dMark,x[i].g_iScore);
	}
	fclose(g_ptoSTUDENT); 
}

//低到高排序 ，并写入文件，给分数 
int LowtoHighSort(Player x[M],int y)
{
	int i,j;	
 	Player t;

 	for(i=0;i<y;i++)
 	{
 		for(j=0;j<y-1;j++)
		{
			if(x[j].g_dMark>x[j+1].g_dMark)
			{
			t=x[j];
			x[j]=x[j+1];
			x[j+1]=t;
			} 
		}	
	}
	if(y<4)
	{
		for(i=0;i<y;i++)
		{
				x[i].g_iScore=0;
		}		
	}
	if(y>=4&&y<=6)
	{
		for(i=0;i<y;i++)
		{
			if(i==0)
			{
				x[i].g_iScore=5;
			}
			else if(i==1)
			{
				x[i].g_iScore=3;
			}
			else if(i==2)
			{
				x[i].g_iScore=2;
			}
			else
			{
				x[i].g_iScore=0;
			}
		}		
	}
	if(y>6)
	{
		for(i=0;i<y;i++)
		{
			if(i==0)
			{
				x[i].g_iScore=7;
			}
			else if(i==1)
			{
				x[i].g_iScore=5;
			}
			else if(i==2)
			{
				x[i].g_iScore=3;
			}
			else if(i==3)
			{
				x[i].g_iScore=2;
			}
			else if(i==4)
			{
				x[i].g_iScore=1;
			}
		}		
	}
	if((g_ptoSTUDENT=fopen("PLAYERS.txt","a+"))==NULL)
	{
		printf("无法打开文件");
		exit(0);
	}
	for(i=0;i<y;i++)
	{
		fprintf(g_ptoSTUDENT,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",x[i].g_sStuNum,x[i].g_sStuName,x[i].g_cSex,
			x[i].g_sPhoneNumber,x[i].g_sDormitory,x[i].g_sRegSport,x[i].g_dMark,x[i].g_iScore);
	}
	fclose(g_ptoSTUDENT); 
	
} 

//打印得分表 
int printScore(Player x[M],int y)
{
	int i;
	printf("\n\t%s\n",x[i].g_sRegSport);
	for(i=0;i<y;i++)
	{	
		printf("%s\t%s\t%.2lf\t%d\n\n",x[i].g_sStuNum,x[i].g_sStuName,
			x[i].g_dMark,x[i].g_iScore);
		
	} 

} 

//修改成绩
int changeMark()
{
	int choosenext;
	if(h==0)
	{
	printf("当前没有学生信息");
	back(); 
	}		
	Player targe[M];
	char sport[20];
	char number[20];
	int i,c;
	int k ;
	int x ;
	double j;
	editgrade:
		k=1;x=0;
		for(i=0;i<numofAS;i++)
		{
			printf("\t%d.",i+1);
			printf("%s\n",allsport[i].Sports_Name);
		}
		printf("请输入项目：");
        scanf("%d",&c);      //选择需要输入成绩的项目 
        if(c<=numofAS&&c>0)
		{
			for(i=0;i<numofAS;i++)
			{
				if(i+1==c)
					strcpy(sport,allsport[i].Sports_Name);
			}
		}
		else 
			{printf("请选择正确的序号\n");goto editgrade;}
		
		system("cls");
		
	S:	for(i=0;i<h;i++)
		{
				
			if(strcmp(Players[i].g_sRegSport,sport)==0)     //若存在选择该项目的运动员，则执行{}内语句 
			{
				targe[x++]=Players[i];
				printf("\t%d.",k++);
				printf("%s\t%s\t%.2f\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_dMark);
			}
		} 
		printf("输入要修改成绩的运动员序号：");
		scanf("%d",&c); 
		if(c<=k&&c>0)
		{
			for(i=0;i<k;i++)
			{
				if(i+1==c)
				strcpy(number,targe[i].g_sStuNum);
			}
		}
		else 
			{printf("请选择正确的序号\n");goto S;} 
		
	for(i=0;i<h;i++)
	{
		if(strcmp(Players[i].g_sRegSport,sport)==0)
			if(strcmp(Players[i].g_sStuNum,number)==0)
			{
				printf("\n");
			    printf("%s\t%s\t%.2f\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_dMark);
				printf("\n请为该运动员修改成绩成绩:");
			    scanf("%lf",&j);
			    Players[i].g_dMark = j;
			    printf("\n");
			    printf("%s\t%s\t%.2f\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_dMark);
			}
	}
	if((fp=fopen("PLAYERS.txt","w"))==NULL)
	{
		printf("无法打开此文件");
		exit(0);
	}
	for(i=0;i<h;i++)
	{
 		fprintf(fp,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
		Players[i].g_sPhoneNumber,Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
	}
	fclose(fp);
	printf("修改成功");
	printf("\n\n\t1.选择其他项目\n\n\t2.返回成绩管理菜单\n\n\t3.退出\n");
	waycho:	scanf("%d",&choosenext) ;
	if(choosenext==1)
	{
		system("cls");
		goto editgrade;
	}
	else if(choosenext==2)
	{
		return(Gradeop());
	}
	else if(choosenext==3)
	{
		exit(0);
	}
	else 
	{
		printf("\n\n\t重新选择\n");
		goto waycho;
	}
}

//删除成绩
int DeleteMark()
{
	int choosenext;
	if(h==0)
	{
	printf("当前没有学生信息");
	back(); 
	}
		
	Player targe[M];
	char sport[50];
	char number[50];
	int i,c;
	int k ;
	int x ;
	double j;
	deletegrade:
		k=1;
		x=0;
		for(i=0;i<numofAS;i++)
		{
			printf("\t%d.",i+1);
			printf("%s\n",allsport[i].Sports_Name);
		}
		printf("\n\t\t请输入项目：");
        scanf("%d",&c);      //选择需要输入成绩的项目 
        if(c<=numofAS&&c>0)
		{
			for(i=0;i<numofAS;i++)
			{
				if(i+1==c)
					strcpy(sport,allsport[i].Sports_Name);
			}
		}
		else 
			{printf("\n\t请选择正确的序号\n");goto deletegrade;}
		
		system("cls");
		
	S:	for(i=0;i<h;i++)
		{
				
			if(strcmp(Players[i].g_sRegSport,sport)==0)     //若存在选择该项目的运动员，则执行{}内语句 
			{
				targe[x++]=Players[i];
				printf("\t%d.",k++);
				printf("%s\t%s\t%.2f\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_dMark);
			}
		} 
		printf("\n\n\t输入要修改成绩的运动员序号：");
		scanf("%d",&c); 
		if(c<=k&&c>0)
		{
			for(i=0;i<k;i++)
			{
				if(i+1==c)
				strcpy(number,targe[i].g_sStuNum);
			}
		}
		else 
			{printf("\n\n\t请选择正确的序号\n");goto S;} 
		
	for(i=0;i<h;i++)
	{
		if(strcmp(Players[i].g_sRegSport,sport)==0)
			if(strcmp(Players[i].g_sStuNum,number)==0)
			{ 
			    Players[i].g_dMark = 0;
			    printf("\n");
			    printf("%s\t%s\t%.2f\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_dMark);
			}
	}
	
	if((fp=fopen("PLAYERS.txt","w"))==NULL)
	{
		printf("无法打开此文件");
		exit(0);
	}
	for(i=0;i<h;i++)
	{
 		fprintf(fp,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
		Players[i].g_sPhoneNumber,Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
	}
	fclose(fp);
	printf("\n\n\t修改成功\n");
	printf("\n\n\t1.选择其他项目\n\n\t2.返回成绩管理菜单\n\n\t3.退出\n");
	waycho:	scanf("%d",&choosenext) ;
	if(choosenext==1)
	{
		system("cls");
		goto deletegrade;
	}
	else if(choosenext==2)
	{
		return(Gradeop());
	}
	else if(choosenext==3)
	{
		exit(0);
	}
	else 
	{
		printf("\n\n\t重新选择\n");
		goto waycho;
	}
}


int numSearch()
//管理员按学号查询// 
{ 
    system("cls");	//清屏// 
    int i,j;
	char num[20];	//学生号码（形参）// 
	int tag=0;	//标志是否存在该学号的运动员// 
	int sum=0;
	printf("\n\n\t************按学号查询************\n");
	printf("\t输入运动员学号：");
	scanf("%s",num);
	
	//通过循环找到特定学号的结构体// 
	printf("\n\t学号\t姓名\t项目\t\t成绩\t得分\n");
	for(i=0;i<h;i++)
	{
		if(strcmp(Players[i].g_sStuNum,num)==0)
		{
            printf("\t%s\t%s\t%s\t%.2lf\t%d\n\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
			tag=1;	//找到该学号的信息，将标志值改变// 
			sum=sum+Players[i].g_iScore;
		}
	} 
	if(tag==1)
	{
		printf("%s的总得分是%d",num,sum);
	 } 
		if(tag==0)
	{
		printf("\n\t未找到该运动员,任意键返回成绩管理菜单\n");
		getch();
		return(Gradeop());
	} 	
	printf("\n\n\t任意键返回成绩管理菜单\n");
    getch();
	return(Gradeop());
	//标志值未改变，未找到该学员信息// 
}


//按比赛项目查询// 
int sportSearch()
{

	int i,c;
	int num;	//选择项目的序号//  
	int n=0;	//特定项目的结构体数组的下标//

	char sport[20]; 
P:	system("cls");	//清屏 

	printf("\n\n\t************按比赛项目查询************\n\n");
		
		for(i=0;i<numofAS;i++)
		{
			printf("\t%d.",i+1);
			printf("%s\n",allsport[i].Sports_Name);
		}
		printf("请输入需要查询的项目：");
        scanf("%d",&c);      //选择需要输入成绩的项目 
        if(c<=numofAS&&c>0)
		{
			for(i=0;i<numofAS;i++)
			{
				if(i+1==c)
					strcpy(sport,allsport[i].Sports_Name);
			}
		}
		else 
			{printf("\n\n\t请选择正确的序号\n");getch();goto P;}
			
 		system("cls");
		printf("\n\n\t\t%s\n\n",sport);
		printf("\n\t学号\t姓名\t项目\t\t成绩\t得分\n");
		for(i=0;i<h;i++)
		{
			if(strcmp(Players[i].g_sRegSport,sport)==0)     //若存在选择该项目的运动员，则执行{}内语句 
			{
				 
				printf("\n");
		        printf("\t%s\t%s\t%s\t%.2lf\t%d\n\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
		    }
		}
		printf("\n\n查询完毕，任意键返回成绩管理菜单\n");
		getch();
		return(Gradeop());
}
 
 
//按参赛学院查询 
int DorSearch()
{
	int i,c;
	int num;	//选择项目的序号// 
	int tag=0;	//查看序号输入是否错误的标志量// 
	int n=0;	//特定项目的结构体数组的下标// 
	int sum=0;
	char dorm[20];
	system("cls");	//清屏 

	printf("\n\n\t************按参赛学院查询************\n\n");
L:
		for(i=0;i<numofDorm;i++)
		{
			printf("\t%d.",i+1);
			printf("%s\n",Dorm[i]);
		}
		printf("请输入项目：");
        scanf("%d",&c);      //选择需要输入成绩的项目 
        if(c<=numofDorm&&c>0)
		{
			for(i=0;i<numofDorm;i++)
			{
				if(i+1==c)
					strcpy(dorm,Dorm[i]);
			}
		}
		else 
			{printf("请选择正确的序号\n");goto L;}
 		
		system("cls");	
 		printf("\n\t\t\t\t%s\n\n",dorm);
 		printf("\n\t学号\t姓名\t项目\t\t成绩\t得分\n");
 		for(i=0;i<h;i++){
			if(strcmp(Players[i].g_sDormitory,dorm)==0)     //若存在选择该项目的运动员，则执行{}内语句 
			{
				printf("\t%s\t%s\t%s\t%.2lf\t%d\n\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
				sum=sum+Players[i].g_iScore;
			}
		}
		printf("\n\t%s的总得分是%d\n",dorm,sum); 
		printf("\n\n查询完毕，任意键返回成绩管理菜单\n");
		getch();
		return(Gradeop());
} 

//查询菜单 
int SearchMenu(){
	int ch1;
        L:
		printf("1.按学号查找\n2.按比赛项目查找\n3.按参赛学院查找\n"); 
		printf("\n选择功能：");
	    scanf("%d",&ch1);
		switch(ch1)                                  //选择查找方式 
		{ 
		case 1:numSearch();break;
		case 2:sportSearch();break;
		case 3:DorSearch();break;
		default:printf("请选择有效的选项\n");goto L;break;
		} 
		system("cls");
}

//其他函数
int backinAdm()
{
	int j ;
		printf("\n\n\t1.返回主菜单\t2.返回管理员菜单\t3.退出\n"); 
		         L:	scanf("%d",&j);
			switch(j)
			{	case 1:h=0;return(main());break;
			 	case 2:return(Administrator()); break;
				case 3:exit(0); break;
				default:printf("\n\t请选择有效的选项\n");goto L;break;
		    }
} 

int back()                          //调用来判断是否返回主菜单 
{
	int j ;
		printf("\n\n\t1.返回主菜单\t2.退出\n"); 
		L:	scanf("%d",&j);
		switch(j)
		{	case 1:h=0;return(main());break;
			case 2:exit(0); break;
			default:printf("\n\t请选择有效的选项\n");goto L;break;
	    }
} 


 //程序运行时，通过运行以下代码来确认报名是否关闭 
int check()
{
	char ch ;
	ch = '1';
	if((g_ptoSTUDENT=fopen("Close.txt","a+"))==NULL)
		{
		printf("无法打开此文件\n");
		exit(0); 
	} 
	ch=fgetc(g_ptoSTUDENT);          //获取close函数存入 close.txt的值，赋给ch 
	if(ch=='0')           
	{
		judge1 = false;           //若ch='0',改变judge的值用于判断报名已关闭 
	}
	fclose(g_ptoSTUDENT);
}

//秩序册生成同时关闭报名 
int close()
{
    int x;   
    char ch;
	printf("\n\n\t\t是否关闭报名\n\n\t\t1.是\t\t2.否\n");
	scanf("%d",&x);
	if(x==1)  
	{
	ch='0';
	g_ptoSTUDENT=fopen("Close.txt","w+");
	fputc(ch,g_ptoSTUDENT);                    //关闭通道时，将ch赋值为'0'并存入close.txt 
	fclose(g_ptoSTUDENT); 
	printf("\n\n\t报名已关闭,秩序册已生成\n");
	getch();
	system("cls");
	}	
	else
	{
	system("cls");	
	back();
	}
	
} 

//打印运动员信息 
int print(int i)                  
{
	printf("%s\t%s\t%s\t%s\t%s\t%.2f\t%d\t%s\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
				Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore,Players[i].g_sPhoneNumber);
}

//-------------主函数---------------------------------------------------------------// 
int main()
{   
	system("cls"); //清屏 
	Screen();
    getch();
	system("cls");
	int tag;//循环判断 
	numofDorm=0;
	numofbS=0;numofgS = 0;numofAS=0;numofRS=0;numofFS=0;
	
	
	//读取参赛单位 
	if((fp = fopen("Dorm.txt","r"))!=NULL)
	{
		while(fscanf(fp,"%s",Dorm[numofDorm])!=EOF)
		{
			numofDorm++;
		}	
	}fclose(fp);
	//--------------读取文件里已有项目文件 
	if((fp = fopen("Allsports.txt","r"))!=NULL)
	{
	while(fscanf(fp,"%s%d%d%d%d",allsport[numofAS].Sports_Name,&allsport[numofAS].sex,
	&allsport[numofAS].type,&allsport[numofAS].Group_OR_Population,&allsport[numofAS].eachtime)!=EOF)
    {
    	numofAS++;                                     
    } //总的项目计数
    fclose(fp) ; //关闭文件
	//-----------------------------------------------//
	
	//其他的项目计数
		
	for(tag=0;tag<numofAS;tag++)
	{	
	//	printf("\t%s\t\t%d\t%d\t%d\t%d\n",allsport[tag].Sports_Name,allsport[tag].sex,allsport[tag].type,allsport[tag].Group_OR_Population,allsport[tag].eachtime);
		if(allsport[tag].sex == 1)//男子 
		{
			boysport[numofbS]=allsport[tag];
			numofbS++ ;	
		}
		else if(allsport[tag].sex == 2)
		{
			girlsport[numofgS]=allsport[tag];
			numofgS++ ;//女子
		}

		//径赛
		if(allsport[tag].type == 1)
		{
			tracksport[numofRS]=allsport[tag];
			numofRS++ ;
		}
		else if(allsport[tag].type == 2)
		{
			fieldsport[numofFS]=allsport[tag];
			numofFS++ ;//田赛 
		}
	}
	}
	
	{//将文件里已有报名信息写入结构数组
		h=0; 
		if((fp=fopen("PLAYERS.txt","a+"))==NULL) 			
    	{
    		printf("无法打开此文件\n");	
			system("pause")	;		 	 	
			exit(0); 
		}
    	while(fscanf(fp,"%s%s%s%s%s%s%lf%d",Players[h].g_sStuNum,Players[h].g_sStuName,Players[h].g_cSex,
		Players[h].g_sPhoneNumber,Players[h].g_sDormitory,Players[h].g_sRegSport,&Players[h].g_dMark,&Players[h].g_iScore)!=EOF) 
		{
	//  打印运动员信息 	print(h);
			h++;                                     
		}
		fclose(fp);
	}
	setjmp(jb);
    int choice=0; 
     	Developer(); 
   		system("cls");
			printf("\n\n\n\n\t------------运动会管理系统-------------\n\n");
    		printf("\n\n\t\t1.选手端\n\n\t\t2.管理员端\n\n\t\t3.备份\n\n\t\t4.退出\n\n");
           	printf("\n\n\t请输入数字进行选择\n");
		   	scanf("%d",&choice);
            switch(choice)
            {
				case 1 : system("cls");Student(); break; 
                case 2 : system("cls");WaysToAd(choice); Administrator(); break;
                case 3 : system("cls");backup(); break;
                case 4 : exit(0);break;
				case 0 : system("cls");Vip();break;
                default : printf("\t\n无法识别，请重新输入！\n"); break;
   	        }
}

