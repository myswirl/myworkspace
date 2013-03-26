//*************************************************************
//顺序表的C实现(数据结构+算法)
//CreateDate:20100202
//编译工具:VC++6.0
//File Name:SeqList.c
//线性表的顺序存储表示
//*************************************************************
#include	<stdio.h>  
#include	<stdlib.h>  
#include	<time.h>  

const int MAXLISTSIZE=80;	//预设的存储空间最大容量

typedef   int   ElemType;  

typedef struct { 
	ElemType *elem;			//存储空间基址
	int length;				//当前长度
	int listsize;			//允许的最大存储容量(以sizeof(ElemType)为单位)
} SqList;					//俗称-顺序表

//基本操作接口(函数声明)----------------------------
//构造一个最大存储容量为 maxsize 的空的顺序表 L
//maxsize 由用户设定，其默认值为系统设定值MAXLISTSIZE
void InitList( SqList &L, int maxsize );
//销毁顺序表 L
void DestroyList( SqList &L );
//若顺序表 L 为空表，则返回TRUE，否则返回 FALSE
bool ListEmpty ( SqList L );
//若顺序表 L 为空表，则返回TRUE，否则返回 FALSE
bool ListFull ( SqList L );
//返回顺序表 L 中元素个数
int ListLength ( SqList L );
//若 cur_e 是顺序表 L 的元素，则返回其前驱的位序
//(设第一个元素的前驱的位序为0)，否则返回 -1
int PriorElem ( SqList L, ElemType cur_e );
//若 cur_e 是顺序表 L 的元素，则返回其后继的位序
//(设最后一个元素的后继的位序为L的表长+1)，否则返回 -1。
int NextElem ( SqList L, ElemType cur_e );
bool GetElem ( SqList L, int pos, ElemType &e );
bool ListInsert(SqList &L, int pos, ElemType e);


//基本操作接口(函数实现)-----------------------------
void InitList(SqList &L, int maxsize) 
{
	// 构造一个空的线性表 L
	if ( maxsize <= 0 )
	{
		maxsize = MAXLISTSIZE; 
	}
	L.elem = new ElemType[maxsize];	 	 	 
	if (!L.elem)
	{
		exit(1); //存储分配失败
	}
	L.length = 0;	//顺序表的初始长度为0
	L.listsize = maxsize;	//该顺序表可以存储元素的最大容量
} // InitList

//销毁顺序表 L
void DestroyList( SqList &L )
{
	delete L.elem;
}  
//判断是否为空表
bool ListEmpty ( SqList L )
{  
	if(L.length==0)
	{
		return	1;  
	}else 
	{
		return	0;  
	}
}  

bool ListInsert(SqList &L, int pos, ElemType e)
{
	int j;
	// 若存储空间不满且1≤pos≤Listlength(L)+1，则在顺序表 L 的
	// 第 pos 个元素之前插入新的元素 e 且返回TRUE，否则返回FALSE
	if (pos < 1 || pos > L.length+1) return 0 ; // 插入位置不合法
	if (L.length >= L.listsize) return 0;
	// 当前存储空间已满，无法插入
	for (j=L.length-1; j>=pos-1; --j)
		L.elem[j+1] = L.elem[j]; // 插入位置及之后的元素右移
	L.elem[pos-1] = e; // 插入 e
	++L.length;// 表长增1
	return 1;
} // ListInsert

//求表长，返回表中元素个数  
int ListLength ( SqList L )
{     
	return	L.length;    
}  


bool ListFull ( SqList L )
{  
	if( L.length == L.listsize )  
	{
		return   1;  
	}else
	{
		return   0;  
	}
}  

bool ListDelete(SqList &L, int pos, ElemType &e)
{
	int j;
	// 若1≤pos≤Listlength(L)，则以 e 带回从顺序表 L 中删除的
	// 第 pos 个元素且返回 TRUE，否则返回 FALSE
	if ((pos < 1) || (pos > L.length))
		return 0 ; // 删除位置不合法
	for (j = pos; j<L.length; ++j) 
		L.elem[j-1] = L.elem[j];// 被删除元素之后的元素左移	 	
	--L.length; // 表长减1
	return 1;
} // ListDelete 


void   Printlist(SqList   *L)  
{  
	int   n,i;  
	n   =   ListLength( *L);  
	for(i=0;i<n;++i)  
	{  
		printf("[%d]:%d\n",i,L->elem[i]);  	
	}  
} 



//主程序体
void   main(void)  
{  
	int   i;  
	SqList   L;  
	InitList(L,80);  
	srand((unsigned   int)time(0));  
	for(i=0;i<2;++i)  
	{
		ListInsert(L,i,rand()%50);//插入50个元素  
	}
	printf("表中元素为：\n");  
	Printlist(&L);
	printf("\n");
}