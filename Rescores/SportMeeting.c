#include <stdio.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h> 
#include <setjmp.h>
#include <Windows.h>
#define N 1000    		//�����˶���ɲμ��˶�Ա���� 
#define M 8 			//ÿ����Ŀ���ɱ����� 
#define personlimit 3	//ÿ�����ɱ�����	
#define ddlperson 4	 //��Ŀ����������� 
#define bool int      //������define�����˲�������
#define true 1         
#define false 0
#define length 20  1

jmp_buf   jb;
char password[3][20];
bool judge1 = true;     //�Ƿ��������
FILE *fp;
FILE *fr;
FILE *fq;
FILE *fs;
FILE *g_ptoSTUDENT ;
FILE *p_fold_Allsports;

struct University//�˶���ٰ���Ϣ�Ľṹ�� 
{
	char name[20];  //�ٰ췽���� 
	int  number;    //���� 
	int  month;		//�·�
	int  bday;      //�� 
}university; 
				
typedef struct   //�˶�Ա��Ϣ�Ľṹ�� 
{
    char      g_sStuNum[50];        //ѧ�� 
    char      g_sStuName[50] ;		//���� 
    char      g_sDormitory[50] ;	//ѧԺ 
    char      g_sPhoneNumber[50];	//�绰���� 
    char      g_cSex[50];			//�Ա� 
    char      g_sRegSport[50];		//������Ŀ 
    int       g_iHaveAtenSportNum;	//�μ���Ŀ�� 
    double    g_dMark ;				//�ɼ� 
    int       g_iScore ;			//�÷� 

}Player;
Player Players[N],temp[M];
Player deletable[N],tempPlayers[N],addable[N],addtempPlayers[N];		//���˶�Ա�����ڲ��� 

typedef struct{		//��Ŀ��Ϣ�ṹ�� 
	char Sports_Name[20];				
	int sex;
	int type;
	int Group_OR_Population;
	int eachtime;

}Sports_Single;
Sports_Single boysport[1000],girlsport[1000],fieldsport[1000],tracksport[1000],allsport[1000],Deleted_Sports[1000];
int numofbS=0,numofgS = 0,numofDorm=0,numofAS=0,numofRS=0,numofFS=0;
int h=0;
char Dorm[20][20];//�洢������λ

typedef struct		//��ʾ�������Ŀʱ��Ľṹ�� 
{
	int hour;
	int mint;
	int starttime1;
	int starttime2;
	int endtime1;
	int endtime2;
	int num1;		//num1Ϊ������������������ 
	int num2;		//num2Ϊ����ÿ��ʱ�� ������ÿ��ʱ�� 
}Time;

//�״�ʹ��ʱ ȷ�� �ٰ� ��λ				
 int Beginning()
{      
	if((fp = fopen("Beginning.txt","r"))==NULL) 
	{
		fp=fopen("Beginning.txt","a+");
		printf("������ٰ쵥λ\n");
		scanf("%s",university.name);
		printf("���������\n")    ;
		scanf("%d",&university.number);
		printf("�����뿪ʼ�·�\n"); 
		scanf("%d",&university.month);
		printf("�����뿪ʼ�ڼ���\n"); 
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
    //��һ���ļ�
    if ((fp=fopen(fname,"r"))==NULL){
        printf("���ļ�%s����\n",fname);
        return NULL;
    }
    //���ļ�ָ���Ƶ�ĩβ
    fseek(fp,0,SEEK_END);
    filesize = ftell(fp);//ͨ��ftell�������ָ�뵽�ļ�ͷ��ƫ���ֽ�����
    
    str=(char *)malloc(filesize);//��̬����str�ڴ�
//    str=malloc(filesize);//��̬����str�ڴ�
    str[0]=0;//�ַ����ÿ�
//    memset(str,filesize*sizeof(char),0);//�������,�ַ����ÿյڶ����÷�
    rewind(fp);
    
    while((fgets(txt,1000,fp))!=NULL){//ѭ����ȡ1000�ֽ�,���û���������˳�ѭ��
        strcat(str,txt);//ƴ���ַ���
    }
    fclose(fp);
    return str;
}

int backup(){
	time_t t;
    struct tm * lt;
    time (&t);//��ȡUnixʱ�����
    lt = localtime (&t);//תΪʱ��ṹ��
	char *q;
	char *r;
	char *s;
	fp=fopen("Bakcup.txt","a");
	q=getfileall("PLAYERS.txt");
	r=getfileall("Dorm.txt");
	s=getfileall("AllSports.txt");
	fprintf(fp,"\n����ʱ�䣺%d/%d/%d %d:%d:%d\n�˶�Ա��Ϣ:\n%s\n\n������Ŀ��Ϣ:\n%s\n\n������λ:\n%s\n------------------------------------",lt->tm_year+1900, lt->tm_mon+1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec,q,s,r );
	printf("�˶�Ա��Ϣ:\n%s\n\n������Ŀ��Ϣ:\n%s\n\n������λ:\n%s\n",q,s,r);
	printf("\n���ݳɹ�������ʱ�䣺%d/%d/%d %d:%d:%d\n",lt->tm_year+1900, lt->tm_mon+1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
	fclose(fp);
	printf("���ݽ������������������Ŀ¼��");
	getch();
	longjmp(jb, 1);
}
//������һ��ʼ���еĴ�ӡ�� 
 int Screen()
{
	Beginning();
	fp=fopen("Beginning.txt","a+");
	fscanf(fp,"%s%d%d%d",university.name,&university.number,&university.month,&university.bday);
	printf("��ӭ�μ�%s��%d���˶���\n", university.name,university.number);
	printf("�ٰ�ʱ����%d��%d��\n", university.month,university.bday);
	printf("���������������");
	fclose(fp);
}

//ѧ�������˵� 
 int Student() 
{
	check();
	int choice; 
	    printf("           \n\n\t\t1.����                  \n") ;
	    printf("           \n\n\t\t2.��ɼ�                \n") ;
	    printf("           \n\n\t\t3.�˳�����ʼ����        \n") ;
	    printf("\n\t\t���������ֽ���ѡ��\n") ;
	    scanf("%d",&choice);
	    switch(choice)
	    	{
			case 1 : 
			system("cls");
			if(judge1==false)
			{
			MessageBox(NULL,TEXT("�����ѽ�ֹ"), TEXT("��ʾ"),MB_OK);system("cls");back();
			}
			else
			Register() ; break; 
            case 2 : 
            if((fp=fopen("Gradepublish.txt","r"))==NULL)
            {
            	MessageBox(NULL,TEXT("�ɼ�δ����"), TEXT("��ʾ"),MB_OK);system("cls");back(); 
			}
			else
			{
				system("cls");student_numSearch();
			}
			break; 
            case 3 : system("cls");main(); break;
            default : printf("�޷�ʶ�����������룡\n"); break;
            } 
}

int student_numSearch()
//����Ա��ѧ�Ų�ѯ// 
{ 
    system("cls");	//����// 
    int i,j,sum=0;
	char num[20];	//ѧ�����루�βΣ�// 
	int tag=0;	//��־�Ƿ���ڸ�ѧ�ŵ��˶�Ա// 
	printf("\n\n\t************��ѧ�Ų�ѯ************\n");
	printf("\t�����˶�Աѧ�ţ�");
	scanf("%s",num);
	
	//ͨ��ѭ���ҵ��ض�ѧ�ŵĽṹ��// 
	printf("\n\tѧ��\t����\t��Ŀ\t\t�ɼ�\t�÷�\n");
	for(i=0;i<h;i++)
	{
		if(strcmp(Players[i].g_sStuNum,num)==0)
		{
            printf("\t%s\t%s\t%s\t%.2lf\t%d\n\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
			tag=1;	//�ҵ���ѧ�ŵ���Ϣ������־ֵ�ı�// 
			sum=sum+Players[i].g_iScore;
		}
	} 
		if(tag==1)
	{
		printf("\t%s���ܵ÷���%d",num,sum);
	 } 
		if(tag==0)
	{
		printf("\n\tδ�ҵ����˶�Ա,��������سɼ�����˵�\n");
		getch();
		return(Gradeop());
	} 	
	printf("\n\n\t���������ѧ���˵�\n");
    getch();
    system("cls");
	return(Student());
	//��־ֵδ�ı䣬δ�ҵ���ѧԱ��Ϣ// 
}


//�������� 
int Register()
{ 
	int a,j=0;
	char number[50];
	if((fp=fopen("Allsports.txt","r"))==NULL)
	{
		printf("\n\n\t����δ��ʼ!"); getch();return(main());
	}
	fclose(fp);
	if((fp=fopen("Dorm.txt","r"))==NULL)
	{
		printf("\n\n\t����δ��ʼ!"); getch();return(main());
	}
	fclose(fp);
	int count,i;		//iѭ�� 
    count = h;

	if(count==N)
	{
		printf("�����������޷���������,�������������棡\n");
		system("pause");
		system("cls");
		return(main()) ; 
	}
    printf("���б���������%d\n",count);
           
				//===================����ѧ��======================// 
			printf("���������ѧ�ţ�\n");
			scanf("%s",number) ;
			for(a=0;a<count;a++)
			{
				if(strcmp(Players[a].g_sStuNum,number)==0)
				j=j+1;
			} 
			if(j>=personlimit)
			{
				printf("���˱�����Ŀ�����Ѵ�����%d�Σ�",personlimit);//����ÿ�˱�����Ŀ�������� 
				sleep(3);
				system("cls");
				return(Student()); 
			}	
			else
			{
				strcpy(Players[count].g_sStuNum,number);
			}
			
			
		//==================�����Ա�========================//
		int sex ; 
		printf("��ѡ������Ա�\n");
		printf(" 1.��   2.Ů\n") ;
 		sexcho:scanf("%d",&sex); 
		if(sex==1)
		strcpy(Players[count].g_cSex,"��");
		else if(sex==2)
		strcpy(Players[count].g_cSex,"Ů");
		else
		{
		printf("��ѡ����ȷ�ı��\n");goto sexcho;
		}
		
			//========================����ѡ������Ŀ===================//
			if(sex==1) 
			{  
			    int RaceChoose=0 ;           //=====���岢��ʼ������ѡ������Ŀ��ѡ����ֵ=====// 
				int LiebChoose=0;				//=======�ﾶ���==========//
                     ManChoSpo :              //==========����ѡ����������ת����־��======// 
					 printf("��ѡ��������:\n\t1������\t2������\n"); 
					 scanf("%d",&LiebChoose);
					 system("cls");
					 if(LiebChoose==1){
						printf("��ѡ�������Ŀ���\n"); 
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
					  		 printf("�޷�ʶ�����ٴ�����") ;getch(); system("cls") ; goto ManChoSpo ;
					  		 }
					 }
					 else if(LiebChoose==2){
						printf("��ѡ�������Ŀ��\n"); 
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
					   printf("�޷�ʶ�����ٴ�����") ;getch(); system("cls") ; goto ManChoSpo ;
					   }
					 }
					 else{
						printf("�޷�ʶ�����ٴ�����") ;getch(); system("cls") ; goto ManChoSpo ;
					 }
			         
			}
 
			//========================Ů��ѡ������Ŀ===================//
			if(sex==2)
			{ 
			         int RaceChoose=0 ;           //======���岢��ʼ��Ů��ѡ������Ŀ��ѡ����ֵ=======// 
					 int LiebChoose=0;
			         WomanChoSpo :                 //=======Ů��ѡ����������ת����־��========// 
			         printf("��ѡ��������:\n\t1������\t2������\n"); 
					 scanf("%d",&LiebChoose);
					 system("cls");
					 if(LiebChoose==1){
						printf("��ѡ�������Ŀ���\n"); 
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
					   printf("�޷�ʶ�����ٴ�����") ;getch(); system("cls") ; goto ManChoSpo ;
					   }
					 }
					 else if(LiebChoose==2){
						printf("��ѡ�������Ŀ��\n"); 
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
					   printf("�޷�ʶ�����ٴ�����") ;getch(); system("cls") ; goto ManChoSpo ;
					   }
					 }
					 else							
			           {
					   printf("�޷�ʶ�����ٴ�����") ;getch(); system("cls") ; goto ManChoSpo ;
					   }
			}
		

			//===================��������=======================// 
			printf("���������������\n");
			scanf("%s",Players[count].g_sStuName) ;
		
		
			//===================ѡ�����ѧԺ===================// 
		
		
		     int Dorchoice=0 ;     //����һ������ѧԺѡ���жϱ�־ֵ// 
		     
			dormi:   //=======������ת�Ƶ�ѡ�����ѧԺ�Ŀ�ʼ�ı�־��=======//	
		
		    printf("\t\n��ѡ����Ĳ���ѧԺ��\n") ;
			for(i=0;i<numofDorm;i++)
			{
				printf("\t\t%d.%s\n",i+1,Dorm[i]);
			}
			
			    scanf("%d",&Dorchoice) ;     //��ȡ�Ӽ��������ѡ����ֵ// 
			    system("cls");
			if(Dorchoice>=1&&Dorchoice<=numofDorm)   
			    strcpy(Players[count].g_sDormitory,Dorm[Dorchoice-1]) ; 
			else	
			{printf("�޷�ʶ�����ٴ�����")	  ;system("pause") ;system("pause") ; goto dormi ; } //=======������ת�Ƶ�ѡ�����ѧԺ�Ŀ�ʼ=======//				
				
				
					
			//==================����绰����======================= // 
			printf("��������ĵ绰���룺\n");
			scanf("%s",Players[count].g_sPhoneNumber) ;
			
			
						
			//========================����ѡ������Ŀ===================//

			
	//ѯ�ʱ������Ƿ�ȷ�ϣ�ȷ�Ϻ󲻿ɸ���//
	 //system("cls") ;
     Players[count].g_dMark=0;
	 Players[count].g_iScore=0;
	 int EnsureChoice ;
	 ensure :
	 printf("\n\n%s\t%s\t%s\t%s\t%s\t%s\n\n",Players[count].g_sStuNum,Players[count].g_sStuName,Players[count].g_cSex,
				Players[count].g_sDormitory,Players[count].g_sRegSport,Players[count].g_sPhoneNumber);
	 printf("�Ƿ�ȷ�ϱ�����ȷ�Ϻ󲻿ɸ��ġ�") ;
	 printf("1.��         2.��\n") ;
	 scanf("%d",&EnsureChoice) ;
	 system("cls") ;
	 switch(EnsureChoice)
	 {
	 	case 1 :    
		 if((g_ptoSTUDENT = fopen("PLAYERS.txt","a+"))==NULL)      //====�ж��ļ��Ƿ�򿪳ɹ�====// 
		{
			printf("����¼��ʧ�ܣ��������������棡") ;}
    
	//===��ʼ���ļ��б�������=====//
	fprintf(g_ptoSTUDENT,"%s\t",Players[count].g_sStuNum) ;      //  ѧ�� //====���ø�ʽ���ļ����������Players[count].g_iStuNumд���ļ���//  
	fprintf(g_ptoSTUDENT,"%s\t",Players[count].g_sStuName) ;   //=====����===// 
	fprintf(g_ptoSTUDENT,"%s\t",Players[count].g_cSex) ;        //====�Ա�====//
	fprintf(g_ptoSTUDENT,"%s\t",Players[count].g_sPhoneNumber) ;      //===�绰====// 
	fprintf(g_ptoSTUDENT,"%s\t",Players[count].g_sDormitory) ;        //====����ѧԺ==== 
	fprintf(g_ptoSTUDENT,"%s\t\t",Players[count].g_sRegSport) ;     ///====������Ŀ==== 
	fprintf(g_ptoSTUDENT,"%.2f\t",Players[count].g_dMark) ;     //===��Ŀ�ɼ�===== 
	fprintf(g_ptoSTUDENT,"%d\n",Players[count].g_iScore) ;    //=====�÷�==== 
	fclose(g_ptoSTUDENT); break ;
	 	case 2 : printf("�������������棡") ; system("pause") ; system("cls") ; return(main()) ;
	 	default : printf("�޷�ʶ��������ȷ�ϣ�") ;getch(); system("cls") ; goto ensure ;
	 }
		h=0;
		return (main());
}



//������Ŀ�ı����������ظ����� 

int maxPlayers(char *x,int num,char *nb)    //*x����һ���ַ�����h����һ������ 
{ 
	int j=0; 
	int q=0;         
	int i=0;      
	int z;            //�ж��Ƿ���Ա����ı�־ 
    char y[50];
    char t[50];
    strncpy(y,x,sizeof(y));     	//��ͨ��*x����Ľ��ַ�������y[] 
    strncpy(t,nb,sizeof(t));   
    for(i=0;i<num;i++)      
	      {   
		    if(strcmp(Players[i].g_sRegSport,y)==0)     //ѭ���ҵ���y��ȵ�Players[k]. g_sRegsport ����ʹj����1 
	      	j=j+1;
	      	if((strcmp(Players[i].g_sRegSport,y)==0)&&(strcmp(Players[i].g_sStuNum,t)==0))
	      	q=q+1;
	      	
          }
	if(q==0)
	{
		if(j<M)
		{
		z=0;       //j<M��ʾ��ǰ��Ŀ����δ��������z=0��Ϊ�ɼ��������ı�־ 
		}
		else
		z=1;       //����z=1,���ж������������޷����� 		
	}
	else {
		printf("�ѱ�������Ŀ����������������˵�");
		getch();
		system("cls");
		return(main());
	}

    if(z==0)          
	strcpy(Players[num].g_sRegSport,y);
    else if(z==1)
    	    {
		        printf("����Ŀ������������,��������������˵�");
				getch();
				system("cls");
				return(main());
		     }
}





//����Ա�˱����������ж� 
int Adm_maxPlayer(char *x,int num)
{
	int j=0;          
	int i=0;      
	int z;            //�ж��Ƿ���Ա����ı�־ 
    char y[50];
    strncpy(y,x,sizeof(y));     	//��ͨ��*x����Ľ��ַ�������y[] 
    for(i=0;i<num;i++)      
	{   
		if(strcmp(Players[i].g_sRegSport,y)==0)     //ѭ���ҵ���y��ȵ�Players[k]. g_sRegsport ����ʹj����1 
	    j=j+1;
    }

	if(j<M)
	{
	z=0;       //j<M��ʾ��ǰ��Ŀ����Ϊ��������z=0��Ϊ�ɼ��������ı�־ 
	}
	else
	z=1;       //����z=1,���ж������������޷����� 
	return z;
}


//�����û� 
int Vip()
{
	char developer[20]={"123456"};	//�����û�Ĭ������ 
	int k,j=0;			
	char c;			//��ȡ�������� 
	char  PassWord[20] ;
	char  null[20]={0};
	L:	
		strcpy(PassWord,null);
		printf("�������룺") ;
			for(k=0;k<j;k++)
				{
					if(PassWord[k]!='\0')
					printf("*");
				}				
		for(j=0;j<20;j++)
		    {	
			c=getch();
			if(c=='\r') //�س��� 
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
		printf("\nĬ��������󣬷��ؿ�ʼҳ�棡\n");
		getch(); 
		return(main());
	}
	system("cls");
	printf("\n\n\t�����û�ֻ�ܽ�������Ա����������ΪĬ��ֵ����Э������Ա�޸�����\n");
	int manager; 
	int choice; 
	int i;
	fp=fopen("password.txt","r");
	for(i=0;i<3;i++)
	{
		fscanf(fp,"%s",password[i]);
	} 
	fclose(fp);
cho: 	printf("            \n\n\t\t1.   ��Ŀ����Ա             \n") ;
		printf("            \n\n\t\t2.   �˶�Ա����Ա            \n") ;
    	printf("            \n\n\t\t3.   �ɼ�����Ա             \n") ;
		printf("\n\t��ѡ��Ҫ����Ĺ���Ա���\n") ;
        scanf("%d",&choice); 
		
            switch(choice)
	        {
			case 1 : strcpy(password[0],"000000"); break; //����ΪĬ������000000 
			case 2 : strcpy(password[1],"000000"); break;  //����ΪĬ������000000 
            case 3 : strcpy(password[2],"000000"); break; //����ΪĬ������000000 
            default : printf("�޷�ʶ�����������룡\n");getch();system("cls");goto cho; break;
            } 
            fp=fopen("password.txt","w");
			for(i=0;i<3;i++)
			{
				fprintf(fp,"%s\n",password[i]);
			}
			fclose(fp);
			printf("\n\t���óɹ���������������˵�\n");
			getch();
			return main();
} 


//�޸Ĺ���Ա���� 
int ChangeAdministratorPassword()
{
	int choose;
	printf("\n\n\t\t��ѡ��Ҫ�޸�����Ĺ���ģ��\n\n");
	printf("            \n\n\t\t1.   ��Ŀ���������޸�             \n") ;
	printf("            \n\n\t\t2.   �˶�Ա���������޸�           \n") ;
   	printf("            \n\n\t\t3.   �ɼ����������޸�             \n") ;
	scanf("%d",&choose);
	ChangeAdPasw(choose);
}

//==========����Ա�������뺯��======// 
ChangeAdPasw(int cho)                
{
	int i;//ѭ�� 
	char c; //��ȡ�������� 
	int choose;
	char previous[20],newp[20] , check[20] ;
	printf("\n\n\t�����뵱ǰ���룺") ;
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
		printf("\n\t�����������룺") ;
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
		printf("\n\t���ٴ����������룺");
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
			printf("\n\t���ĳɹ���\n") ;
			
					
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
			printf("\n\t�������벻һ�£����ٴ������������ȡ�����ģ�") ;
			printf("\t1.�ٴ�����     2.ȡ������") ;
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
		printf("\n\n\t��ǰ���벻��ȷ���볢����ϵ�����û�!");
		getch();
		return(main());
	}

}

//����Աģʽ 
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
	 L: printf("            \n\n\t\t1.��������              		 \n") ;
	    printf("            \n\n\t\t2.���Ĺ���Ա����                 \n") ;
	    printf("            \n\n\t\t3.�˳�����ʼ����                 \n") ;
	    printf("\n\t���������ֽ���ѡ��\n") ;
		scanf("%d",&choice);
	        switch(choice)
	        {
			case 1 : system("cls");Manage(); break; //��¼����Ա�˺� 
            case 2 : system("cls");ChangeAdministratorPassword(); break;
            case 3 : system("cls");main(); break;
            default : printf("�޷�ʶ�����������룡\n"); getch();system("cls");goto L; break;
            }
}


//�״�ʹ��ʱ���ù���Ա����  
int Developer()
{
	 if((fp = fopen("password.txt","r"))==NULL) 
		{
			int i,j;//ѭ�� 
			char c;//��ȡ�������� 
			fp=fopen("password.txt","a+");
			for(i=0;i<3;i++)
			{
				switch(i)
				{
					case 0:printf("\n��������Ŀ����Ա����\n");break; 
					case 1:printf("\n�������˶�Ա����Ա����\n");break;
					case 2:printf("\n������ɼ�����Ա����\n");break;
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


//�������© 
WaysToAd(int i)              //=======��¼����Աģʽ�Ľ����б���=======// 
{
	int k,j=0;			
	char c;			//��ȡ�������� 
	char  PassWord[20] ;
	char  null[20]={0};
	L:	
		strcpy(PassWord,null);
		printf("���������룺") ;
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
			printf("\n��¼�ɹ��������������");
			getch() ;           //======���ù���Ա����======// 
	    }
		else
		{
			printf("\n������󣬰�������ٴ����룡");
			getch();
			system("cls") ;
			goto L;
		}
}

//����Աģʽ������� 
int Manage()
{
	system("cls");
	
	    int choice;  
	L:    
        printf("			\n\n\t����Աģʽ					\n") ;
		printf("            \n\n\t\t1.   ��Ŀ����             \n") ;
		printf("            \n\n\t\t2.   �˶�Ա����            \n") ;
    	printf("            \n\n\t\t3.   �ɼ�����             \n") ;
    	printf("            \n\n\t\t4.   �������˵�             \n") ;
    	printf("\n\t���������ֽ���ѡ��\n") ;
    	scanf("%d",&choice); 
            switch(choice)
	        {
			case 1 : system("cls");  system("cls");Sportop();break; 
            case 2 : system("cls"); system("cls");Playerop() ; break; 
            case 3 : system("cls"); system("cls");Gradeop();break;
            case 4 : return(main());break;
            default : printf("�޷�ʶ�����������룡\n");getch();system("cls");goto L; break;
            } 
 
}

//��Ŀ����˵� 
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
    } //�ܵ���Ŀ����
    fclose(fp) ; //�ر��ļ�
	//-----------------------------------------------//	
	//��������Ŀ����		
	for(tag=0;tag<numofAS;tag++)
	{	
		if(allsport[tag].sex == 1)//���� 
		{
			boysport[numofbS]=allsport[tag];
			numofbS++ ;	
		}
		else  
		{
			girlsport[numofgS]=allsport[tag];
			numofgS++ ;//Ů��
		}

		if(allsport[tag].type == 1)//����
		{
			tracksport[numofRS]=allsport[tag];
			numofRS++ ;
		}
		else 
		{
			fieldsport[numofFS]=allsport[tag];
			numofFS++ ;//���� 
		}
	}
	}
	printf("\n\n-------��Ŀ����-------\n\n\t1.������λ����\n\n\t2.��Ŀ�б�\n\n\t3.�����Ŀ\n\n\t4.ɾ����Ŀ\n\n\t5.�޸���Ŀ��Ϣ\n\n\t6.����\n");
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
		default : printf("�޷�ʶ��������ȷ�ϣ�") ; getch();system("cls") ; goto L ;break;
	}	
}

//������λ���� 
int ParticUnit()
{	
	system("cls");
	int max;//��������λ�� 
	int i;//ѭ��
	char Unit[20][20];
	int choose;
	printf("\n\n\t�����������λ��Ŀ(����20)\n");	
	scanf("%d",&max);
	printf("\n\n\t����һ���������λ���س�ȷ�ϵ�����λ��\n");
	for(i=0;i<max;i++)
	{
	scanf("%s",Unit[i]);	
	}
	printf("\n\n\t������λ�趨�󲻵ø��ģ��Ƿ�ȷ��\n");
	printf("\n\t1.��\t2.��\n");
L:	scanf("%d",&choose);
	if(choose==1)
	{
		fp=fopen("Dorm.txt","a+");
		for(i=0;i<max;i++)
		{
		fprintf(fp,"%s\n",Unit[i]);
		}
		fclose(fp);
		printf("\n\t�ѱ��棬�����������Ŀ����˵�!\n");
		getch();
		return(Sportop());
	}
	else if(choose==2)
	{
		printf("\n\n\t�����������Ŀ����˵�ҳ��\n") ;
		getch();
		return(Sportop());
	}
	else
	{
		printf("\n\n\t��ѡ����Чѡ��\n");
		goto L;
	}
}

//�޸���Ŀ��Ϣ������ÿ�������ÿ�˵���ʱ 
int Change_Sports()
{
	system("cls");
	int Change_Save_typeChoice = 0;			//�޸���ʱ 
	int tag;//ѭ���ж� 
	int choose;			//ѡ�� 
	int confirm;		//ȷ��ִ�в��� 
edit:	printf("\n\n\t�޸ľ���ÿ�������ÿ�˵���ʱ\n");
	for(tag=0;tag<numofAS;tag++)
	{
		printf("\t%d.%s\n",tag+1,allsport[tag].Sports_Name);
	} 
	printf("\n\t��ѡ��Ҫ�޸ĵ���Ŀ���:");
	scanf("%d",&choose);
	if(choose<1||choose>numofAS)
		{
			printf("\n��ѡ����Ч�����\n");getch();system("cls");goto edit;
		}
	system("cls");
conf:	printf("\n\n\tȷ���޸ĸ���Ŀ��?\n");
	printf("\t%s\n",allsport[choose-1].Sports_Name);
	printf("\n\t1.��\t2.��\n");
	scanf("%d",&confirm);
	if(confirm==2)
	{
		printf("\n\t�����������Ŀ����˵�ҳ��\n") ;
		getch();
		return(Sportop());
	}
	if(confirm==1)
	{
		if(allsport[choose-1].type==1)			//���� 
		printf("\n\t���޸�ÿ�����ʱ\n");
		else if(allsport[choose-1].type==2)
		printf("\n\t���޸�ÿ�˵���ʱ\n");
		scanf("%d",&Change_Save_typeChoice);
		allsport[choose-1].eachtime=Change_Save_typeChoice;				//�޸� 
		p_fold_Allsports=fopen("Allsports.txt","w");		//д���ļ� 
		for(tag=0;tag<numofAS;tag++)
		fprintf(p_fold_Allsports,"%s\t\t%d\t%d\t%d\t%d\n",allsport[tag].Sports_Name,allsport[tag].sex,allsport[tag].type,
										allsport[tag].Group_OR_Population,allsport[tag].eachtime);	
		fclose(p_fold_Allsports) ;
		printf("\n\t�޸ĳɹ�,�������������Ŀ����˵�ҳ��");
		getch();
		return(Sportop());
	}
	else
	{
		printf("\n\t��ѡ����Чѡ��\n");
		goto conf;
	}
}

//ɾ����Ŀ 
int Delete_Sports(){
		
	system("cls");
	int tag;//ѭ���ж� 
	int choose;
	int confirm;
del:printf("\n\n");
	for(tag=0;tag<numofAS;tag++)
	{
		printf("\t%d.%s\n",tag+1,allsport[tag].Sports_Name);
	} 
	printf("\n\n\t��ѡ��Ҫɾ������Ŀ���:");
	scanf("%d",&choose);
	if(choose<1||choose>numofAS)
		{
			printf("\n\t��ѡ����Ч�����\n");getch();system("cls");goto del;
		}
	system("cls");
conf:	printf("\n\n\tȷ��ɾ������Ŀ��?\n");
	printf("\t%s\n",allsport[choose-1].Sports_Name);
	printf("\n\t1.��\t2.��");
	scanf("%d",&confirm);
	if(confirm==1)				//ȷ��ɾ�� 
	{
		for(tag=choose-1;tag<numofAS;tag++)
			allsport[tag]=allsport[tag+1];				//ɾ�� 
			
		numofAS--;
		
		p_fold_Allsports=fopen("Allsports.txt","w");		//д���ļ� 
		for(tag=0;tag<numofAS;tag++)
		fprintf(p_fold_Allsports,"%s\t\t%d\t%d\t%d\t%d\n",allsport[tag].Sports_Name,allsport[tag].sex,allsport[tag].type,
										allsport[tag].Group_OR_Population,allsport[tag].eachtime);	
		fclose(p_fold_Allsports) ;
		printf("\n\tɾ���ɹ�,�������������Ŀ����˵�ҳ��");
		getch();
		return(Sportop());
	}
	else if(confirm==2)			//��ɾ�� 
	{
		return(Sportop());
	}
	else
	{
		printf("\t��ѡ����Чѡ��\n");
		goto conf; 
	}		
} 


//��Ŀ�б� 
int Search_Sports(){
	
	system("cls");
	printf("\n\n\t��ѡ���������\n\n\t1.����\n\n\t2.����\n\n\t3.������Ŀ\n\n\t4.Ů����Ŀ\n\n\t5.����\n");
	int choose;
	int tag;
	int num=1; 
	scanf("%d",&choose);
	if(choose<1||choose>5)
	{
		printf("\n\t������ѡ����ȷ���");getch(); return Search_Sports();
	}
	if(choose==5) return Sportop(); 
	for(tag=0;tag<numofAS;tag++)
	{	
		if(allsport[tag].type==choose)						//����������߾��� 
		{
			printf("\t%d.%s\tÿ�������ʱ:%d����\n",num,allsport[tag].Sports_Name,allsport[tag].eachtime);num++;
		}
		if(choose==3)
		{
			if(allsport[tag].sex==1)						//���������Ŀ 
			{
				printf("\t%d.%s\tÿ�������ʱ:%d����\n",num,allsport[tag].Sports_Name,allsport[tag].eachtime);num++;
			}
		}
		if(choose==4)
		{		
			if(allsport[tag].sex==2)						//���Ů����Ŀ 
			{
				printf("\t%d.%s\tÿ�������ʱ:%d����\n",num,allsport[tag].Sports_Name,allsport[tag].eachtime);num++;
			}
		}			
	}
	printf("\n\n\t����������ز�����Ŀ��ҳ��");
	getch();
	return(Search_Sports());

} 


//��Ŀ��� 
int Add_Sports()
{
 	Sports_Single temp;			//��ʱ�洢������Ŀ
	int   firstchoice;	//ѡ���������� 
	char  name[20] ;//��ʱ�洢�������Ŀ����
	int   sex_choice= 0 ; //��ʱ�洢ѡ�����Ŀ���Ա� 
	char sex1[20]={"����"};
	char sex2[20]={"Ů��"};
	char sex[20];
	int tag,Lab; //ѭ����־ 
loop:system("cls");
	numofbS=0;numofgS = 0;numofDorm=0;numofAS=0;numofRS=0;numofFS=0;
	//-------------------------------------------------//
	//���ļ���ȡ��Ŀ
	//===================������Ŀ���ڸ�������================// 
	fp = fopen("Allsports.txt","r");
	while(fscanf(fp,"%s%d%d%d%d",allsport[numofAS].Sports_Name,&allsport[numofAS].sex,
	&allsport[numofAS].type,&allsport[numofAS].Group_OR_Population,&allsport[numofAS].eachtime)!=EOF)
    {
    numofAS++;                                     
    } //�ܵ���Ŀ����
    fclose(fp) ; //�ر��ļ�
	//-----------------------------------------------//
	//��������Ŀ����
	printf("\t��ǰ����Ŀ��Ϊ%d\n",numofAS);
	
	
	for(tag=0;tag<numofAS;tag++)
	{	printf("\t%s\t\t%d\t%d\t%d\t%d\n",allsport[tag].Sports_Name,allsport[tag].sex,allsport[tag].type,allsport[tag].Group_OR_Population,allsport[tag].eachtime);
		if(allsport[tag].sex == 1)//���� 
		{
			boysport[numofbS]=allsport[tag];
			numofbS++ ;	
		}
		else  
		{
			girlsport[numofgS]=allsport[tag];
			numofgS++ ;//Ů��
		}

		if(allsport[tag].type == 1)//����
		{
			tracksport[numofRS]=allsport[tag];
			numofRS++ ;
		}
		else 
		{
			fieldsport[numofFS]=allsport[tag];
			numofFS++ ;//���� 
		}
	} 

	//------------��������Ŀ����Ϣ------------------//
		printf("\n\t��ѡ��������\n") ; 
    	printf("\t1.����\t\t2.����\n");
	re:	scanf("%d",&firstchoice);
		if(firstchoice==1)
		temp.type=1;
		else if(firstchoice==2)
		temp.type=2;
		else
		{
			printf("����������\n");goto re;
		}
		printf("\n\t�����������Ŀ��") ;

//	char  Judge_name_type[10] ; //���������ж��û���������Ŀ��ѡ�����Ŀ����Ƿ�һ�µı��� 
	scanf("%s",name) ;

	//---------------------------// 
	printf("\n\t��ѡ�������Ŀ�Ĳ�����Ա�Ա𣺣��������֣�\n");
	printf("\t1.����");
	printf("\t2.Ů��") ;
	printf("\t3.���Ӻ�Ů��	\n") ;
	scanf("%d",&sex_choice);
	
	if(temp.type==1)			 
	temp.Group_OR_Population=1;			//��������ֻ��һ��
	else if (temp.type==2)
	temp.Group_OR_Population=0;			//��ʾ������������ʼΪ0 
	printf("\t���趨������Ŀ��Ҫ���ѵı���ʱ��\n(������ÿ�飬������ÿ�˼��㣬����������)��");
	scanf("%d",&temp.eachtime); 
	//---------------------------------------//
	//��ȡ�����ַ�����ǰ�����ַ�

	
	p_fold_Allsports=fopen("AllSports.txt","a+") ;
	if(sex_choice==1)//ѡ�������� 
	{
		temp.sex=1;
		strcpy(sex,sex1);
		strcat(sex,name);
		strcpy(temp.Sports_Name,sex) ;//�洢����Ŀ��Ϊ ����+��Ŀ
		for(tag=0;tag<numofAS;tag++)
	{
		if(strcmp(temp.Sports_Name,allsport[tag].Sports_Name)==0)
		{
			printf("\n\t����Ŀ����ӣ������������Ŀ");			
			getch();
			system("cls");
			goto loop;
		}
	}
		//δ��ӵ���Ŀ�ɹ���� 
		fprintf(p_fold_Allsports,"%s\t\t%d\t%d\t%d\t%d\n",temp.Sports_Name,temp.sex,temp.type,temp.Group_OR_Population,temp.eachtime);
		numofbS++;
	}


	if(sex_choice==2) 
	{//ѡ����Ů�� 
		temp.sex=2;
		strcpy(sex,sex2);
		strcat(sex,name);
		strcpy(temp.Sports_Name,sex) ;			//�洢����Ŀ��Ϊ Ů��+��Ŀ
		for(tag=0;tag<numofAS;tag++)
		{
		if(strcmp(temp.Sports_Name,allsport[tag].Sports_Name)==0)
		{
			printf("\n\t����Ŀ����ӣ������������Ŀ");			
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
		//�Ȳ������� 
		temp.sex=1;
		strcpy(sex,sex1);
		strcat(sex,name);
		strcpy(temp.Sports_Name,sex) ;//�洢����Ŀ��Ϊ ����+��Ŀ
		for(tag=0;tag<numofAS;tag++)
		{
			if(strcmp(temp.Sports_Name,allsport[tag].Sports_Name)==0)		//������Ŀ�Ѵ洢 
			{	
				printf("\n\t������Ŀ����ӣ������ظ����\n"); 
				repeat=1;
				break;
			}
		}
		if(repeat==0)
		{
		fprintf(p_fold_Allsports,"%s\t\t%d\t%d\t%d\t%d\n",temp.Sports_Name,temp.sex,temp.type,temp.Group_OR_Population,temp.eachtime);
		printf("\t������Ŀ��ӳɹ�\n");
		}		
						
		temp.sex=2;
		strcpy(sex,sex2);
		strcat(sex,name);
		strcpy(temp.Sports_Name,sex) ;			//�洢����Ŀ��Ϊ Ů��+��Ŀ 
		for(Lab=0;Lab<numofAS;Lab++)
		{
			if(strcmp(temp.Sports_Name,allsport[Lab].Sports_Name)==0)		//Ů����Ŀ�Ѵ洢 ��ֱ�ӷ�����Ŀ���� 
			{
				printf("\n\tŮ����Ŀ����ӣ������ظ����");	
				repeat=2;		
				break;		
			}
		}	
		if(repeat==0||repeat==1)
		{
			printf("\tŮ����Ŀ��ӳɹ�\n");
			fprintf(p_fold_Allsports,"%s\t\t%d\t%d\t%d\t%d\n",temp.Sports_Name,temp.sex,temp.type,temp.Group_OR_Population,temp.eachtime);
		}		
	}


	fclose(p_fold_Allsports);  //�ر�����Ŀ�ļ� 
	int continue_choice;//�����ж��Ƿ���� 
	printf("\n\n\t�������!\n") ;
	con:	printf("\n\t�Ƿ����������Ŀ?\n");
		printf("\t1.��������	2.�˳�\n");
		scanf("%d",&continue_choice) ;
		if(continue_choice==1) {goto loop;}
		else if(continue_choice==2)
		{
			return Sportop();
		}
		else
		{
			printf("\t������ѡ����Чѡ��\n");
			getch();
			system("cls");
			goto loop;
		}	
} 

//�˶�Ա����˵� 
int Playerop()
{
	system("cls");
	{//���ļ������б�����Ϣд��ṹ����
	h=0; 
	if((fp=fopen("PLAYERS.txt","a+"))==NULL) 			
    {
    	printf("�޷��򿪴��ļ�\n");	
		system("pause")	;		 	 	
		exit(0); 
	}
    while(fscanf(fp,"%s%s%s%s%s%s%lf%d",Players[h].g_sStuNum,Players[h].g_sStuName,Players[h].g_cSex,Players[h].g_sPhoneNumber,Players[h].g_sDormitory,Players[h].g_sRegSport,&Players[h].g_dMark,&Players[h].g_iScore)!=EOF) 
   {
//  ��ӡ�˶�Ա��Ϣ 	print(h);
   h++;                                     
   }
   fclose(fp);
   }
	printf("\n\n-------�˶�Ա����-------\n\n\t1.ɾ���˶�Ա\n\n\t2.�޸ı�����Ŀ\n\n\t3.�˶�Ա�б�\n\n\t4.���������\n\n\t5.����\n");
	int choose;
L:	scanf("%d",&choose);
	switch(choose)
	{

		case 1:Delete();break;
		case 2:Edit_Player_infomation();break;
		case 3:search_Player();break;
		case 4:Volume(); break;
		case 5:return(Manage());break;
		default : printf("�޷�ʶ��������ȷ�ϣ�") ; getch();system("cls") ; goto L ;break;
	}
}


//����� 
int Volume()
{
	int startt1,startt2;
	int timelength ;
	Time ctimelist[numofRS];		//����ʱ��� 
	Time ftimelist[numofFS];		//����ʱ��� 
	system("cls");
	check();
	if(judge1==true)
	close();
	int i 	;		//ѭ������Ŀ��� 
	int j	;		//ѭ������Ŀ��� 
	fp=fopen("Beginning.txt","a+");
	fscanf(fp,"%s%d%d%d",university.name,&university.number,&university.month,&university.bday);
	printf("\n\n\t\t\t%s%d���˶��Ὰ�����\n\n",university.name,university.number);
	printf("\tһ.���쵥λ:%s\n",university.name);
	printf("\t��.�а쵥λ:%s������\n",university.name);
	printf("\t��.���ں�:�������࣬�������裬�������䣬��չ��ͼ��\n");
	printf("\n\t��.��������\n\t1.����ʱ��ѧ��������д������Ϣ�������ظ�����ͬ����Ŀ\n\t2.�����Ķ�����������Ϣ�����ñ�����ʱ��εı����������粻��������\n");
	printf("\t3.��������λ�����ñ����������ƣ�������Ŀ�б�����������\n");
	printf("\n\t��.�÷���\n");
	printf("\tѧ�������Լ��ı����ɼ������Ӧ�÷�\n");
	printf("\n\t��.������Ա\n");
	printf("\t�˶��Ṥ����Աȫ��У�ڽ�ְ�����𣬸���Ŀ������������ʦ����\n"); 
    printf("\n\t��.�˶��Ḻ������Ϣ\n");
    printf("\t1.�绰��12345678901\n");
    printf("\t2.���䣺123@stu.edu.cn\n");
    printf("\n\t��.ע������\n");
    printf("\t1.�˶��Ὺʼǰ��������ԱӦ�����鳡�ؼ��豸\n");
	printf("\t2.�˶�ԱӦ����Դ�����������������Σ�����ǰӦ����׼�������ֹ������ʹ�����\n");	
	printf("\t3.�˶�Ա����ǰ���˴�����ʳ���˶����ʵ���ˮ�������ǵȣ����˺�����\n"); 
	printf("\t4.���˶�Ա��ѧ�����ǹ�����Ա��ʦ�����ڱ��������ϴ��У�Ӧ��ָ���ص�ۿ��������˶�Ա����������,\nҲӦѸ�ٳ���������أ���ָ���ص���Ϣ\n"); 
	printf("\n\t����\n");
	printf("\t��Ŀ\t\t\t����\t\tʱ��\n");
	fclose(fp);
	for(i=0;i<numofRS;i++)
	{

		ctimelist[i].num1=1;		//��ʱ�޶�һ�飬���ж���Ӧ��Ϊ�������ж� �������������������������������������������������������� 
	
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
		
	}//for(����) 
	
	
	printf("\n\t����\n");
	printf("\t��Ŀ\t\t\t����\t\tʱ��\n");
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


//�޸��˶�Ա��Ϣ 
int Edit_Player_infomation()
{
	int tag1=0;
	int tag2=0;

	char num[20],sport[20];
	int i,j,tag;				//ѭ�� 
	int firstchoose;			//�ѱ�����Ŀ��� 
	int secondchoose;			//�޸ĵ���Ŀ��� 
Editt:	system("cls");
	printf("\n\n\t1.�޸�\t2.����\n");
	scanf("%d",&firstchoose);
	if(firstchoose==2) return(Playerop());
	if(firstchoose<1||firstchoose>2) {
		printf("\n\n\t�������������ѡ��\n");
		getch();
		goto Editt;
	} 
	printf("\n\t�������޸ĵ��˶�Աѧ�ţ�");
	scanf("%s",&num);				//����ѧ�� 
	
	//ѡ����Ŀ 
	for(j=0;j<numofAS;j++)				
		{
				printf("\t\t%d.%s\n\n",j+1,allsport[j]);
		}
      L:scanf("%d",&firstchoose);				//ѡ����Ŀ 	
       	if(firstchoose>=1&&firstchoose<=numofAS)		
			strcpy(sport,allsport[firstchoose-1].Sports_Name);				
		else	
			{printf("\n\t�޷�ʶ�����ٴ�����")	;getch();goto L ; }
			
			
	//���Ҷ�Ӧ���˶�Ա��Ϣ 
	for(i=0;i<h;i++)
	{
		if(strcmp(Players[i].g_sStuNum,num)==0)
		    {
		    	tag1==1;		//��ʾ�ж�Ӧѧ�ŵ���Ϣ 
		    	
		    	if(strcmp(Players[i].g_sRegSport,sport)==0)
		    	{tag2==1;		//��ʾ�ж�Ӧ��Ŀ����Ϣ 
				printf("%s\t%s\t%s\t%s\t%s\t%s\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
				Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_sPhoneNumber);
				P:
                printf("\n\t�Ƿ��޸ĸ��˶�Ա��Ϣ��\t1.��\t2.��\n");
                scanf("%d",&j);
				switch(j)
				{
					case 1:
					{     //ȷ���޸� 
					printf("\n\n\t�޸ı�����ĿΪ(ѡ�����)\n\n");
					if((strcmp(Players[i].g_cSex,"��"))==0)
					{
						for(tag=0;tag<numofbS;tag++)
						{
							printf("\t%d.%s\n\n",tag+1,boysport[tag].Sports_Name);
						}	
										
			editsport:	scanf("%d",&secondchoose);
						if(secondchoose<1||secondchoose>numofbS)		//�������Ų��Ϸ� 
						{
							printf("\n\n\t������ѡ��\n");
							goto editsport;
						}
					//�ж���Ŀ�Ƿ���Ա 
						if((Adm_maxPlayer(boysport[secondchoose-1].Sports_Name,h))==1)
						{
							printf("\n\n\t!!��ǰ��ѡ��Ŀ�Ѿ���Ա��������󷵻�!!\n");
							getch();
							goto Editt;
						}
						printf("\n\n\t�Ƿ񽫱�����Ϣ�޸�Ϊ\n");
						printf("%s\t%s\t%s\t%s\t%s\t%s\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
						Players[i].g_sDormitory,boysport[secondchoose-1].Sports_Name,Players[i].g_sPhoneNumber);
						printf("\n\t1.��\t2.��\n");
                		scanf("%d",&j);
                		if(j==1)
                		{
                			strcpy(Players[i].g_sRegSport,boysport[secondchoose-1].Sports_Name);
						}
						else if(j==2)
						{
						printf("\n\n\tȡ���޸�\n");
						printf("\n\t������������˶�Ա����˵�\n");
						getch();
						Playerop();
						}
					}//if(��)
					 
					else if((strcmp(Players[i].g_cSex,"Ů"))==0)
					{
						for(tag=0;tag<numofgS;tag++)
						{
							printf("\t%d.%s\n\n",tag+1,girlsport[tag].Sports_Name);
						}					
			choseditsport:	scanf("%d",&secondchoose);
						if(secondchoose<1||secondchoose>numofbS)		//�������Ų��Ϸ� 
						{
							printf("\n\n\t������ѡ��\n");
							goto choseditsport;
						}
						//�ж���Ŀ�Ƿ���Ա 
						if((Adm_maxPlayer(girlsport[secondchoose-1].Sports_Name,h))==1)
						{
							printf("\n\n\t!!��ǰ��ѡ��Ŀ�Ѿ���Ա��������󷵻�!!\n");
							getch();
							goto Editt;
						}
						printf("\n\n\t�Ƿ񽫱�����Ϣ�޸�Ϊ\n");
						printf("%s\t%s\t%s\t%s\t%s\t%s\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
					Players[i].g_sDormitory,girlsport[secondchoose-1].Sports_Name,Players[i].g_sPhoneNumber);
					printf("\n\t1.��\t2.��\n");
                	scanf("%d",&j);
                	if(j==1)
                	{
                		strcpy(Players[i].g_sRegSport,girlsport[secondchoose-1].Sports_Name);
					}
					else if(j==2)
					{
						printf("\n\n\tȡ���޸�\n");
						printf("\n\t������������˶�Ա����˵�\n");
						getch();
						Playerop();
					}
					}//if(Ů)
					
					//����������д���ļ� 
					if((fp=fopen("PLAYERS.txt","w"))==NULL)
						{
							printf("�޷��򿪴��ļ�");
							exit(0);
						}
               		 for(i=0;i<h;i++)
					{
 						fprintf(fp,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
							Players[i].g_sPhoneNumber,Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
					}
					fclose(fp);
					system("cls");
					printf("\n\n\t�ѳɹ��޸ĸñ�����Ŀ��Ϣ\n");					
					printf("\n\t������������˶�Ա����˵�\n");
					getch();
					Playerop();
					break;
					} 
					
					case 2:{     //���޸�
					if((fp=fopen("PLAYERS.txt","w"))==NULL)
						{
							printf("�޷��򿪴��ļ�");
							exit(0);
						}
					for(i=0;i<h;i++)
					{
 					fprintf(fp,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
						Players[i].g_sPhoneNumber,Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
					}
					fclose(fp);
					printf("\n\t������������˶�Ա����˵�\n");
					getch();
					Playerop();
					break;
					}					
					default:printf("\t��ѡ����ȷ�����\n");goto P;break;
				}                
				}
			}
		
	}//for
			
		if(tag1==0)
		{
			if((fp=fopen("PLAYERS.txt","w"))==NULL)
						{
							printf("�޷��򿪴��ļ�");
							exit(0);
						}
			for(i=0;i<h;i++)
				{
 					fprintf(fp,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
						Players[i].g_sPhoneNumber,Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
				}
			fclose(fp);
			printf("\tδ�ҵ�ƥ��ѧ�ŵ�ѧ����Ϣ\n");
			printf("\n\t������������˶�Ա����˵�\n");
			getch();
			Playerop();
		}
		if(tag1==1&&tag2==0)
		{
			if((fp=fopen("PLAYERS.txt","w"))==NULL)
						{
							printf("�޷��򿪴��ļ�");
							exit(0);
						}
			for(i=0;i<h;i++)
				{
 					fprintf(fp,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
						Players[i].g_sPhoneNumber,Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
				}
			fclose(fp);
			printf("\tδ�ҵ�ƥ����Ŀ��ѧ����Ϣ\n");
			printf("\n\t������������˶�Ա����˵�\n");
			getch();
			Playerop();
		}	

} 


//�����˶�Ա 
int search_Player()
{
	 
	int firstchoose;		//ѡ��������
	int secondchoose;        //ѡ����Ŀ 
	int tag; 		//ѭ���ж� 
first:system("cls");
	printf("\n\n\t������������");
	printf("\n\n\t1.����\n\n\t2.����\n\n\t3.����\n");
	scanf("%d",&firstchoose);
	
	system("cls");
	printf("\n\t��ѡ����Ŀ���\n\n");
	
	//ѡ����
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
		printf("\n�������\n");
		getch();
		goto first;
			
	}//if
	
	//ѡ������ 
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
		printf("\n�������\n");
		getch();
		goto first;
		
	}//if
	else if(firstchoose==3)
	{
		return(Playerop());
	}
	else
	{
		printf("\n\n\t������ѡ��");
		goto first;
	}
	


	
}




//�˶�Ա��Ϣɾ�� 
int Delete()
{
	int tag1=0;
	int tag2=0;
	if((fp=fopen("PLAYERS.txt","w"))==NULL)
	{
		printf("�޷��򿪴��ļ�");
		exit(0);
	}
	char num[20],sport[20];
	int i,j,c;
	system("cls");
	printf("\n\t������ɾ�����˶�Աѧ�ţ�");
	scanf("%s",&num);
	//ѡ����Ŀ 
	for(j=0;j<numofAS;j++)				
		{
				printf("\t\t%d.%s\n",j+1,allsport[j]);
		}
      L:scanf("%d",&c);
       	if(c>=1&&c<=numofAS)
			strcpy(sport,allsport[c-1].Sports_Name);				
		else	
			{printf("\n\t�޷�ʶ�����ٴ�����")	;getch();goto L ; }
	//���� 
	for(i=0;i<h;i++)
	{
		if(strcmp(Players[i].g_sStuNum,num)==0)
		    {
		    	tag1==1;		//��ʾ�ж�Ӧѧ�ŵ���Ϣ 
		    	
		    	if(strcmp(Players[i].g_sRegSport,sport)==0)
		    	{tag2==1;		//��ʾ�ж�Ӧ��Ŀ����Ϣ 
				print(i);
				P:
                printf("\n\t�Ƿ�ɾ�����˶�Ա��Ϣ��\t1.��\t2.��\n");
                scanf("%d",&j);
				switch(j)
				{
					case 1:     //ȷ��ɾ�� 
					g_ptoSTUDENT=fopen("DELETE.txt","a+");				//���Ᵽ�汻ɾ������Ϣ 
					fprintf(g_ptoSTUDENT,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
							Players[i].g_sPhoneNumber,Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
					fclose(g_ptoSTUDENT);
						//ɾ�� 
					for(j=i;j<h;j++)		
					Players[j]=Players[j+1];
						//ʣ������д���ļ� 
               		 for(i=0;i<h-1;i++)
					{
 						fprintf(fp,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
							Players[i].g_sPhoneNumber,Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
					}
					fclose(fp);
					system("cls");
					printf("\n\n\t�ѳɹ�ɾ���ñ�����Ϣ\n");					
					printf("\n\t������������˶�Ա����˵�\n");
					getch();
					Playerop();
					break;
					
					case 2:     //��ɾ�� 
					for(i=0;i<h;i++)
					{
 					fprintf(fp,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
						Players[i].g_sPhoneNumber,Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
					}
					fclose(fp);
					printf("\n\t������������˶�Ա����˵�\n");
					getch();
					Playerop();
					break;					
					default:printf("\t��ѡ����ȷ�����\n");goto P;break;
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
			printf("\tδ�ҵ�ƥ��ѧ�ŵ�ѧ����Ϣ\n");
			printf("\n\t������������˶�Ա����˵�\n");
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
			printf("\tδ�ҵ�ƥ����Ŀ��ѧ����Ϣ\n");
			printf("\n\t������������˶�Ա����˵�\n");
			getch();
			Playerop();
		}	

} 

//�ɼ�����˵�
int Gradeop()
{
	{//���ļ������б�����Ϣд��ṹ����
	h=0; 
	if((fp=fopen("PLAYERS.txt","a+"))==NULL) 			
    {
    	printf("�޷��򿪴��ļ�\n");	
		system("pause")	;		 	 	
		exit(0); 
	}
    while(fscanf(fp,"%s%s%s%s%s%s%lf%d",Players[h].g_sStuNum,Players[h].g_sStuName,Players[h].g_cSex,Players[h].g_sPhoneNumber,Players[h].g_sDormitory,Players[h].g_sRegSport,&Players[h].g_dMark,&Players[h].g_iScore)!=EOF) 
   {
//  ��ӡ�˶�Ա��Ϣ 	print(h);
   h++;                                     
   }
   fclose(fp);
   }
	system("cls") ;
	int FirstChioce;
	FirstMenu:
	printf("\n\n\t1.����ɼ�\n");
	printf("\n\n\t2.����÷�\n");
	printf("\n\n\t3.���ĳɼ�\n");
	printf("\n\n\t4.�鿴�ɼ�\n");
	printf("\n\n\t5.ɾ���ɼ�\n"); 
	printf("\n\n\t6.����\n");
	printf("\n\t���������ֽ���ѡ��\n") ;
	scanf("%d",&FirstChioce);
	system("cls") ;                //===����====// 
	switch(FirstChioce)
	{
		case 1 : getMark() ;    break ;                
		case 2 : getScore();	break ; 
		case 3 : changeMark();      break ;               
		case 4 : SearchMenu();      break ;                 
		case 5 : DeleteMark();       break ;   
		case 6 : Manage() ;      break ;                    
		default: printf("\n�޷�ʶ�����ٴ�����\n") ; goto FirstMenu ; break; 
	}
} 


//Ϊ��ѡ�˶�Ա����ɼ� 
int  getMark()
{
	
	if(h==0)
	{
		printf("\n\t��ǰû��ѧ����Ϣ\n");
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
		printf("\n\n\t��������Ŀ��ţ�");
        scanf("%d",&c);      //ѡ����Ҫ����ɼ�����Ŀ 
        if(c<=numofAS&&c>0)
		{
			for(i=0;i<numofAS;i++)
			{
				if(i+1==c)
					strcpy(sport,allsport[i].Sports_Name);
			}
		}
		else 
			{printf("\n\n\t��ѡ����ȷ�����\n");goto L;}
			
		system("cls");
		
		for(i=0;i<h;i++)
		{
			if(strcmp(Players[i].g_sRegSport,sport)==0)     //������ѡ�����Ŀ���˶�Ա����������������������������ɼ���Ҫ�����������÷�ʽ 
			{
				 
				printf("\n");
		        printf("%s\t%s\n",Players[i].g_sStuNum,Players[i].g_sStuName);
		        printf("\n��Ϊ���˶�Ա����ɼ�:");
		        scanf("%lf",&j);
		        Players[i].g_dMark = j;
		        printf("\n");
		        printf("%s\t%s\t%.2f\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_dMark);
		        printf("\n");
			}
		}
		if((fp=fopen("PLAYERS.txt","w"))==NULL)
		{
			printf("�޷��򿪴��ļ�");
			exit(0);
		}
	    for(i=0;i<h;i++)
		{
 			fprintf(fp,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
			Players[i].g_sPhoneNumber,Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
		}
		fclose(fp);
		int choosenext; 
		printf("\n\n\t�������");
		printf("\n\n\t1.ѡ��������Ŀ\n\n\t2.���سɼ�����˵�\n\n\t3.�˳�");
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
			printf("\n\n\t����ѡ��\n");
			goto waycho;
		}
}

//�����˶�Ա����Ŀ�ɼ�����÷�
int getScore()
{
	if((fp=fopen("Gradepublish.txt","r"))==NULL) 
	{
		if((MessageBox(NULL,TEXT("�Ƿ񷢲��ɼ�"), TEXT("��ʾ"),MB_OKCANCEL))==1)
		{	
			fp=fopen("Gradepublish.txt","w");//�ɼ����� 
			fclose(fp);
			MessageBox(NULL,TEXT("�ɼ��ѷ���"), TEXT("��ʾ"),MB_OK); 
		}
		else
		{
			MessageBox(NULL,TEXT("�ɼ�δ����"), TEXT("��ʾ"),MB_OK); 
		}
	}
	else
	{
		printf("\n\t��ǰ�ɼ����÷��ѷ�����ѧ���ɲ�ѯ�ɼ�\n");
	}
	
	int i,j,k,m;
	i=j=k=m=0;
	int choosenext;
    char sport[50];
    fp=fopen("PLAYERS.txt","w");
    fclose(fp);	
    for(i=0;i<numofAS;i++)			//����������Ŀ 
	{
		k=0;
		for(j=0;j<h;j++)				
		{
			if(strcmp(Players[j].g_sRegSport,allsport[i].Sports_Name)==0)		//��ͬһ��Ŀ���˶�Ա��Ϣ����һ����ʱ���� 
				temp[k++] = Players[j];
		}
		if(allsport[i].type==1)				//��ĿΪ���� 
			LowtoHighSort(temp,k);			//����д���ļ� �������ĳɼ�����ʽ���������������������� 
		else if(allsport[i].type==2)		//��ĿΪ���� 
			HightoLowSort(temp,k);			//����д���ļ� �������ĳɼ�����ʽ���������������������� 
			
	}
	//���ļ�����������Ϣд��ṹ����
	i=0; 
	fp=fopen("PLAYERS.txt","r");			
    while(fscanf(fp,"%s%s%s%s%s%s%lf%d",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,Players[i].g_sPhoneNumber,Players[i].g_sDormitory,Players[i].g_sRegSport,&Players[i].g_dMark,&Players[i].g_iScore)!=EOF) 
   	{
   		i++;                                     
   	}
   	fclose(fp); 
 L: k=0;
 	printf("\n\n\t\t��ѡ��Ҫ��ӡ��ѧ���÷ֱ�\n\n ");
	for(i=0;i<numofAS;i++)				//��ӡ��ѡ��Ŀ��Ϣ 
	{
		printf("\t%d.",i+1);			 
		printf("%s\n",allsport[i].Sports_Name);
	}
		scanf("%d",&m);					//������Ŀ��Ϣ 
		if(m<=numofAS&&m>0)
		{
			strcpy(sport,allsport[m-1].Sports_Name);
		}
		else 
		{printf("\n\n\t��ѡ����ȷ�����\n");goto L;}
   	
   	//��ӡ��Ӧ��Ŀ�ɼ��͵÷� 
   	for(i=0;i<numofAS;i++)
	{
		if(strcmp(sport,allsport[i].Sports_Name)==0)			//�ҵ�ѡ�����Ŀ 
		{
			system("cls");
			printf("\n\n\t\t%s",allsport[i].Sports_Name);
			printf("�ɼ����÷�\n\n");
			for(j=0;j<h;j++)
			{
				if(strcmp(Players[j].g_sRegSport,allsport[i].Sports_Name)==0)
					temp[k++] = Players[j];				//������Ŀ���˶�Ա��Ϣд��һ������ 
			}
			if(k<ddlperson)
			{
				printf("����Ŀ�����������㣬�޷�ͳ�Ƴɼ���");
				getch();
				return(Gradeop());				
			}
			else
				printScore(temp,k);						//��ӡ 
		}
	}
	printf("\n\n\t1.ѡ��������Ŀ\n\n\t2.���سɼ�����˵�\n\n\t3.�˳�\n");
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
		printf("\n\n\t����ѡ��\n");
		goto waycho;
	}
} 

//�Գɼ����иߵ�������ĺ�������д���ļ� �������� 
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
		printf("�޷����ļ�");
		exit(0);
	}
	for(i=0;i<y;i++)
	{
		fprintf(g_ptoSTUDENT,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",x[i].g_sStuNum,x[i].g_sStuName,x[i].g_cSex,
			x[i].g_sPhoneNumber,x[i].g_sDormitory,x[i].g_sRegSport,x[i].g_dMark,x[i].g_iScore);
	}
	fclose(g_ptoSTUDENT); 
}

//�͵������� ����д���ļ��������� 
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
		printf("�޷����ļ�");
		exit(0);
	}
	for(i=0;i<y;i++)
	{
		fprintf(g_ptoSTUDENT,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",x[i].g_sStuNum,x[i].g_sStuName,x[i].g_cSex,
			x[i].g_sPhoneNumber,x[i].g_sDormitory,x[i].g_sRegSport,x[i].g_dMark,x[i].g_iScore);
	}
	fclose(g_ptoSTUDENT); 
	
} 

//��ӡ�÷ֱ� 
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

//�޸ĳɼ�
int changeMark()
{
	int choosenext;
	if(h==0)
	{
	printf("��ǰû��ѧ����Ϣ");
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
		printf("��������Ŀ��");
        scanf("%d",&c);      //ѡ����Ҫ����ɼ�����Ŀ 
        if(c<=numofAS&&c>0)
		{
			for(i=0;i<numofAS;i++)
			{
				if(i+1==c)
					strcpy(sport,allsport[i].Sports_Name);
			}
		}
		else 
			{printf("��ѡ����ȷ�����\n");goto editgrade;}
		
		system("cls");
		
	S:	for(i=0;i<h;i++)
		{
				
			if(strcmp(Players[i].g_sRegSport,sport)==0)     //������ѡ�����Ŀ���˶�Ա����ִ��{}����� 
			{
				targe[x++]=Players[i];
				printf("\t%d.",k++);
				printf("%s\t%s\t%.2f\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_dMark);
			}
		} 
		printf("����Ҫ�޸ĳɼ����˶�Ա��ţ�");
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
			{printf("��ѡ����ȷ�����\n");goto S;} 
		
	for(i=0;i<h;i++)
	{
		if(strcmp(Players[i].g_sRegSport,sport)==0)
			if(strcmp(Players[i].g_sStuNum,number)==0)
			{
				printf("\n");
			    printf("%s\t%s\t%.2f\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_dMark);
				printf("\n��Ϊ���˶�Ա�޸ĳɼ��ɼ�:");
			    scanf("%lf",&j);
			    Players[i].g_dMark = j;
			    printf("\n");
			    printf("%s\t%s\t%.2f\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_dMark);
			}
	}
	if((fp=fopen("PLAYERS.txt","w"))==NULL)
	{
		printf("�޷��򿪴��ļ�");
		exit(0);
	}
	for(i=0;i<h;i++)
	{
 		fprintf(fp,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
		Players[i].g_sPhoneNumber,Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
	}
	fclose(fp);
	printf("�޸ĳɹ�");
	printf("\n\n\t1.ѡ��������Ŀ\n\n\t2.���سɼ�����˵�\n\n\t3.�˳�\n");
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
		printf("\n\n\t����ѡ��\n");
		goto waycho;
	}
}

//ɾ���ɼ�
int DeleteMark()
{
	int choosenext;
	if(h==0)
	{
	printf("��ǰû��ѧ����Ϣ");
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
		printf("\n\t\t��������Ŀ��");
        scanf("%d",&c);      //ѡ����Ҫ����ɼ�����Ŀ 
        if(c<=numofAS&&c>0)
		{
			for(i=0;i<numofAS;i++)
			{
				if(i+1==c)
					strcpy(sport,allsport[i].Sports_Name);
			}
		}
		else 
			{printf("\n\t��ѡ����ȷ�����\n");goto deletegrade;}
		
		system("cls");
		
	S:	for(i=0;i<h;i++)
		{
				
			if(strcmp(Players[i].g_sRegSport,sport)==0)     //������ѡ�����Ŀ���˶�Ա����ִ��{}����� 
			{
				targe[x++]=Players[i];
				printf("\t%d.",k++);
				printf("%s\t%s\t%.2f\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_dMark);
			}
		} 
		printf("\n\n\t����Ҫ�޸ĳɼ����˶�Ա��ţ�");
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
			{printf("\n\n\t��ѡ����ȷ�����\n");goto S;} 
		
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
		printf("�޷��򿪴��ļ�");
		exit(0);
	}
	for(i=0;i<h;i++)
	{
 		fprintf(fp,"%s\t%s\t%s\t%s\t%s\t%s\t\t%.2lf\t%d\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
		Players[i].g_sPhoneNumber,Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
	}
	fclose(fp);
	printf("\n\n\t�޸ĳɹ�\n");
	printf("\n\n\t1.ѡ��������Ŀ\n\n\t2.���سɼ�����˵�\n\n\t3.�˳�\n");
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
		printf("\n\n\t����ѡ��\n");
		goto waycho;
	}
}


int numSearch()
//����Ա��ѧ�Ų�ѯ// 
{ 
    system("cls");	//����// 
    int i,j;
	char num[20];	//ѧ�����루�βΣ�// 
	int tag=0;	//��־�Ƿ���ڸ�ѧ�ŵ��˶�Ա// 
	int sum=0;
	printf("\n\n\t************��ѧ�Ų�ѯ************\n");
	printf("\t�����˶�Աѧ�ţ�");
	scanf("%s",num);
	
	//ͨ��ѭ���ҵ��ض�ѧ�ŵĽṹ��// 
	printf("\n\tѧ��\t����\t��Ŀ\t\t�ɼ�\t�÷�\n");
	for(i=0;i<h;i++)
	{
		if(strcmp(Players[i].g_sStuNum,num)==0)
		{
            printf("\t%s\t%s\t%s\t%.2lf\t%d\n\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
			tag=1;	//�ҵ���ѧ�ŵ���Ϣ������־ֵ�ı�// 
			sum=sum+Players[i].g_iScore;
		}
	} 
	if(tag==1)
	{
		printf("%s���ܵ÷���%d",num,sum);
	 } 
		if(tag==0)
	{
		printf("\n\tδ�ҵ����˶�Ա,��������سɼ�����˵�\n");
		getch();
		return(Gradeop());
	} 	
	printf("\n\n\t��������سɼ�����˵�\n");
    getch();
	return(Gradeop());
	//��־ֵδ�ı䣬δ�ҵ���ѧԱ��Ϣ// 
}


//��������Ŀ��ѯ// 
int sportSearch()
{

	int i,c;
	int num;	//ѡ����Ŀ�����//  
	int n=0;	//�ض���Ŀ�Ľṹ��������±�//

	char sport[20]; 
P:	system("cls");	//���� 

	printf("\n\n\t************��������Ŀ��ѯ************\n\n");
		
		for(i=0;i<numofAS;i++)
		{
			printf("\t%d.",i+1);
			printf("%s\n",allsport[i].Sports_Name);
		}
		printf("��������Ҫ��ѯ����Ŀ��");
        scanf("%d",&c);      //ѡ����Ҫ����ɼ�����Ŀ 
        if(c<=numofAS&&c>0)
		{
			for(i=0;i<numofAS;i++)
			{
				if(i+1==c)
					strcpy(sport,allsport[i].Sports_Name);
			}
		}
		else 
			{printf("\n\n\t��ѡ����ȷ�����\n");getch();goto P;}
			
 		system("cls");
		printf("\n\n\t\t%s\n\n",sport);
		printf("\n\tѧ��\t����\t��Ŀ\t\t�ɼ�\t�÷�\n");
		for(i=0;i<h;i++)
		{
			if(strcmp(Players[i].g_sRegSport,sport)==0)     //������ѡ�����Ŀ���˶�Ա����ִ��{}����� 
			{
				 
				printf("\n");
		        printf("\t%s\t%s\t%s\t%.2lf\t%d\n\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
		    }
		}
		printf("\n\n��ѯ��ϣ���������سɼ�����˵�\n");
		getch();
		return(Gradeop());
}
 
 
//������ѧԺ��ѯ 
int DorSearch()
{
	int i,c;
	int num;	//ѡ����Ŀ�����// 
	int tag=0;	//�鿴��������Ƿ����ı�־��// 
	int n=0;	//�ض���Ŀ�Ľṹ��������±�// 
	int sum=0;
	char dorm[20];
	system("cls");	//���� 

	printf("\n\n\t************������ѧԺ��ѯ************\n\n");
L:
		for(i=0;i<numofDorm;i++)
		{
			printf("\t%d.",i+1);
			printf("%s\n",Dorm[i]);
		}
		printf("��������Ŀ��");
        scanf("%d",&c);      //ѡ����Ҫ����ɼ�����Ŀ 
        if(c<=numofDorm&&c>0)
		{
			for(i=0;i<numofDorm;i++)
			{
				if(i+1==c)
					strcpy(dorm,Dorm[i]);
			}
		}
		else 
			{printf("��ѡ����ȷ�����\n");goto L;}
 		
		system("cls");	
 		printf("\n\t\t\t\t%s\n\n",dorm);
 		printf("\n\tѧ��\t����\t��Ŀ\t\t�ɼ�\t�÷�\n");
 		for(i=0;i<h;i++){
			if(strcmp(Players[i].g_sDormitory,dorm)==0)     //������ѡ�����Ŀ���˶�Ա����ִ��{}����� 
			{
				printf("\t%s\t%s\t%s\t%.2lf\t%d\n\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore);
				sum=sum+Players[i].g_iScore;
			}
		}
		printf("\n\t%s���ܵ÷���%d\n",dorm,sum); 
		printf("\n\n��ѯ��ϣ���������سɼ�����˵�\n");
		getch();
		return(Gradeop());
} 

//��ѯ�˵� 
int SearchMenu(){
	int ch1;
        L:
		printf("1.��ѧ�Ų���\n2.��������Ŀ����\n3.������ѧԺ����\n"); 
		printf("\nѡ���ܣ�");
	    scanf("%d",&ch1);
		switch(ch1)                                  //ѡ����ҷ�ʽ 
		{ 
		case 1:numSearch();break;
		case 2:sportSearch();break;
		case 3:DorSearch();break;
		default:printf("��ѡ����Ч��ѡ��\n");goto L;break;
		} 
		system("cls");
}

//��������
int backinAdm()
{
	int j ;
		printf("\n\n\t1.�������˵�\t2.���ع���Ա�˵�\t3.�˳�\n"); 
		         L:	scanf("%d",&j);
			switch(j)
			{	case 1:h=0;return(main());break;
			 	case 2:return(Administrator()); break;
				case 3:exit(0); break;
				default:printf("\n\t��ѡ����Ч��ѡ��\n");goto L;break;
		    }
} 

int back()                          //�������ж��Ƿ񷵻����˵� 
{
	int j ;
		printf("\n\n\t1.�������˵�\t2.�˳�\n"); 
		L:	scanf("%d",&j);
		switch(j)
		{	case 1:h=0;return(main());break;
			case 2:exit(0); break;
			default:printf("\n\t��ѡ����Ч��ѡ��\n");goto L;break;
	    }
} 


 //��������ʱ��ͨ���������´�����ȷ�ϱ����Ƿ�ر� 
int check()
{
	char ch ;
	ch = '1';
	if((g_ptoSTUDENT=fopen("Close.txt","a+"))==NULL)
		{
		printf("�޷��򿪴��ļ�\n");
		exit(0); 
	} 
	ch=fgetc(g_ptoSTUDENT);          //��ȡclose�������� close.txt��ֵ������ch 
	if(ch=='0')           
	{
		judge1 = false;           //��ch='0',�ı�judge��ֵ�����жϱ����ѹر� 
	}
	fclose(g_ptoSTUDENT);
}

//���������ͬʱ�رձ��� 
int close()
{
    int x;   
    char ch;
	printf("\n\n\t\t�Ƿ�رձ���\n\n\t\t1.��\t\t2.��\n");
	scanf("%d",&x);
	if(x==1)  
	{
	ch='0';
	g_ptoSTUDENT=fopen("Close.txt","w+");
	fputc(ch,g_ptoSTUDENT);                    //�ر�ͨ��ʱ����ch��ֵΪ'0'������close.txt 
	fclose(g_ptoSTUDENT); 
	printf("\n\n\t�����ѹر�,�����������\n");
	getch();
	system("cls");
	}	
	else
	{
	system("cls");	
	back();
	}
	
} 

//��ӡ�˶�Ա��Ϣ 
int print(int i)                  
{
	printf("%s\t%s\t%s\t%s\t%s\t%.2f\t%d\t%s\n",Players[i].g_sStuNum,Players[i].g_sStuName,Players[i].g_cSex,
				Players[i].g_sDormitory,Players[i].g_sRegSport,Players[i].g_dMark,Players[i].g_iScore,Players[i].g_sPhoneNumber);
}

//-------------������---------------------------------------------------------------// 
int main()
{   
	system("cls"); //���� 
	Screen();
    getch();
	system("cls");
	int tag;//ѭ���ж� 
	numofDorm=0;
	numofbS=0;numofgS = 0;numofAS=0;numofRS=0;numofFS=0;
	
	
	//��ȡ������λ 
	if((fp = fopen("Dorm.txt","r"))!=NULL)
	{
		while(fscanf(fp,"%s",Dorm[numofDorm])!=EOF)
		{
			numofDorm++;
		}	
	}fclose(fp);
	//--------------��ȡ�ļ���������Ŀ�ļ� 
	if((fp = fopen("Allsports.txt","r"))!=NULL)
	{
	while(fscanf(fp,"%s%d%d%d%d",allsport[numofAS].Sports_Name,&allsport[numofAS].sex,
	&allsport[numofAS].type,&allsport[numofAS].Group_OR_Population,&allsport[numofAS].eachtime)!=EOF)
    {
    	numofAS++;                                     
    } //�ܵ���Ŀ����
    fclose(fp) ; //�ر��ļ�
	//-----------------------------------------------//
	
	//��������Ŀ����
		
	for(tag=0;tag<numofAS;tag++)
	{	
	//	printf("\t%s\t\t%d\t%d\t%d\t%d\n",allsport[tag].Sports_Name,allsport[tag].sex,allsport[tag].type,allsport[tag].Group_OR_Population,allsport[tag].eachtime);
		if(allsport[tag].sex == 1)//���� 
		{
			boysport[numofbS]=allsport[tag];
			numofbS++ ;	
		}
		else if(allsport[tag].sex == 2)
		{
			girlsport[numofgS]=allsport[tag];
			numofgS++ ;//Ů��
		}

		//����
		if(allsport[tag].type == 1)
		{
			tracksport[numofRS]=allsport[tag];
			numofRS++ ;
		}
		else if(allsport[tag].type == 2)
		{
			fieldsport[numofFS]=allsport[tag];
			numofFS++ ;//���� 
		}
	}
	}
	
	{//���ļ������б�����Ϣд��ṹ����
		h=0; 
		if((fp=fopen("PLAYERS.txt","a+"))==NULL) 			
    	{
    		printf("�޷��򿪴��ļ�\n");	
			system("pause")	;		 	 	
			exit(0); 
		}
    	while(fscanf(fp,"%s%s%s%s%s%s%lf%d",Players[h].g_sStuNum,Players[h].g_sStuName,Players[h].g_cSex,
		Players[h].g_sPhoneNumber,Players[h].g_sDormitory,Players[h].g_sRegSport,&Players[h].g_dMark,&Players[h].g_iScore)!=EOF) 
		{
	//  ��ӡ�˶�Ա��Ϣ 	print(h);
			h++;                                     
		}
		fclose(fp);
	}
	setjmp(jb);
    int choice=0; 
     	Developer(); 
   		system("cls");
			printf("\n\n\n\n\t------------�˶������ϵͳ-------------\n\n");
    		printf("\n\n\t\t1.ѡ�ֶ�\n\n\t\t2.����Ա��\n\n\t\t3.����\n\n\t\t4.�˳�\n\n");
           	printf("\n\n\t���������ֽ���ѡ��\n");
		   	scanf("%d",&choice);
            switch(choice)
            {
				case 1 : system("cls");Student(); break; 
                case 2 : system("cls");WaysToAd(choice); Administrator(); break;
                case 3 : system("cls");backup(); break;
                case 4 : exit(0);break;
				case 0 : system("cls");Vip();break;
                default : printf("\t\n�޷�ʶ�����������룡\n"); break;
   	        }
}

