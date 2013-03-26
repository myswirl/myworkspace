//*************************************************************
//˳����Cʵ��(���ݽṹ+�㷨)
//CreateDate:20100202
//���빤��:VC++6.0
//File Name:SeqList.c
//���Ա��˳��洢��ʾ
//*************************************************************
#include	<stdio.h>  
#include	<stdlib.h>  
#include	<time.h>  

const int MAXLISTSIZE=80;	//Ԥ��Ĵ洢�ռ��������

typedef   int   ElemType;  

typedef struct { 
	ElemType *elem;			//�洢�ռ��ַ
	int length;				//��ǰ����
	int listsize;			//��������洢����(��sizeof(ElemType)Ϊ��λ)
} SqList;					//�׳�-˳���

//���������ӿ�(��������)----------------------------
//����һ�����洢����Ϊ maxsize �Ŀյ�˳��� L
//maxsize ���û��趨����Ĭ��ֵΪϵͳ�趨ֵMAXLISTSIZE
void InitList( SqList &L, int maxsize );
//����˳��� L
void DestroyList( SqList &L );
//��˳��� L Ϊ�ձ��򷵻�TRUE�����򷵻� FALSE
bool ListEmpty ( SqList L );
//��˳��� L Ϊ�ձ��򷵻�TRUE�����򷵻� FALSE
bool ListFull ( SqList L );
//����˳��� L ��Ԫ�ظ���
int ListLength ( SqList L );
//�� cur_e ��˳��� L ��Ԫ�أ��򷵻���ǰ����λ��
//(���һ��Ԫ�ص�ǰ����λ��Ϊ0)�����򷵻� -1
int PriorElem ( SqList L, ElemType cur_e );
//�� cur_e ��˳��� L ��Ԫ�أ��򷵻����̵�λ��
//(�����һ��Ԫ�صĺ�̵�λ��ΪL�ı�+1)�����򷵻� -1��
int NextElem ( SqList L, ElemType cur_e );
bool GetElem ( SqList L, int pos, ElemType &e );
bool ListInsert(SqList &L, int pos, ElemType e);


//���������ӿ�(����ʵ��)-----------------------------
void InitList(SqList &L, int maxsize) 
{
	// ����һ���յ����Ա� L
	if ( maxsize <= 0 )
	{
		maxsize = MAXLISTSIZE; 
	}
	L.elem = new ElemType[maxsize];	 	 	 
	if (!L.elem)
	{
		exit(1); //�洢����ʧ��
	}
	L.length = 0;	//˳���ĳ�ʼ����Ϊ0
	L.listsize = maxsize;	//��˳�����Դ洢Ԫ�ص��������
} // InitList

//����˳��� L
void DestroyList( SqList &L )
{
	delete L.elem;
}  
//�ж��Ƿ�Ϊ�ձ�
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
	// ���洢�ռ䲻����1��pos��Listlength(L)+1������˳��� L ��
	// �� pos ��Ԫ��֮ǰ�����µ�Ԫ�� e �ҷ���TRUE�����򷵻�FALSE
	if (pos < 1 || pos > L.length+1) return 0 ; // ����λ�ò��Ϸ�
	if (L.length >= L.listsize) return 0;
	// ��ǰ�洢�ռ��������޷�����
	for (j=L.length-1; j>=pos-1; --j)
		L.elem[j+1] = L.elem[j]; // ����λ�ü�֮���Ԫ������
	L.elem[pos-1] = e; // ���� e
	++L.length;// ����1
	return 1;
} // ListInsert

//��������ر���Ԫ�ظ���  
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
	// ��1��pos��Listlength(L)������ e ���ش�˳��� L ��ɾ����
	// �� pos ��Ԫ���ҷ��� TRUE�����򷵻� FALSE
	if ((pos < 1) || (pos > L.length))
		return 0 ; // ɾ��λ�ò��Ϸ�
	for (j = pos; j<L.length; ++j) 
		L.elem[j-1] = L.elem[j];// ��ɾ��Ԫ��֮���Ԫ������	 	
	--L.length; // ����1
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



//��������
void   main(void)  
{  
	int   i;  
	SqList   L;  
	InitList(L,80);  
	srand((unsigned   int)time(0));  
	for(i=0;i<2;++i)  
	{
		ListInsert(L,i,rand()%50);//����50��Ԫ��  
	}
	printf("����Ԫ��Ϊ��\n");  
	Printlist(&L);
	printf("\n");
}