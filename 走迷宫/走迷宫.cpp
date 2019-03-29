#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<Windows.h>
#include<time.h>

#define MaxL 50
#define MaxK 50

typedef enum //ö����
{
	False,True
}NewBool;
//False=0,True=1

////��������Ľṹ��
typedef struct zb
{
	int x;
	int y;
}SITE;

//�ڵ���ʮ������
typedef struct Node NODE;
struct Node
{
	SITE site;
	//�ڵ�ı�ǣ���ʱ0Ϊ��ͨ��·)
	int sign;
	//�Ƿ��Ѿ����ù�·��
	NewBool IsUse;
	NewBool IsUpConnected;//���� �� �Ƿ���ͨ
	NewBool IsDownConnected;//���� �� �Ƿ���ͨ
	NewBool IsLeftConnected;//���� �� �Ƿ���ͨ
	NewBool IsRightConnected;//���� �� �Ƿ���ͨ
	NODE * Up;//�� �� �ڵ�
	NODE * Down;//�� �� �ڵ�
	NODE * Left;//�� �� �ڵ�
	NODE * Right;//�� �� �ڵ�
};

//��ͼ�ڵ�
typedef struct mn
{
	SITE site;
	int sign;
	//�Ƿ��ǽ�ɫ�ڵ�λ��
	NewBool IsCharacter;
	//��ɫ�Ƿ��߹���·
	NewBool IsPassing;
}MapNode;

NODE * CreateMap(int L,int K);
NODE * CreateNode(int x, int y, int sign);
void RandomMap(NODE * head, SITE * start, SITE * end);
void main()
{
	srand(121);
	//��ʼ�����������
	StartGame();
}

//����һ����ͼ������һ��NODE��ָ�룬�����ͼ�ĳ�L��K
NODE * CreateMap(int L,int K)
{
	int count1,count2;
	NODE *head,*Lhead,*p1,*p2,*pUp;//p1 p2Ϊͬһ�п������ӣ�pUp�ǿ����²����������ϲ��
	for(count1 = 0;count1 < K;count1++)//���ƿ�
	{
		Lhead = p1 =p2 =CreateNode(0, count1, 0);
		if(count1 == 0)
		{
			head=Lhead;
		}
		for(count2 = 0;count2 < L; count2++)//���Ƴ�
		{
			if(count2 == 0)
			{
				p1->Left = NULL;
			}
			else
			{
				p2->Right = p1;
				p1->Left =p2;
			}
			if (count1 == 0)
			{
				p1->Up = NULL;
			}
			else
			{
				p1->Up = pUp;
				pUp->Down =p1;
			}
			p2=p1;
			//���´�ѭ���������ʱ�������ڵ�
			if(count2 != L - 1)
			{
				p1 = CreateNode(count2 + 1,count1,0);
			}
			//�����ǵ�һ�е�ʱ��pUp����ֵ�ſ�����һ��
			if(count1 !=0)
			{
				pUp = pUp->Right;
			}
		}
		pUp = Lhead;
		p2->Right =NULL;
	}
	while(pUp !=NULL)
	{
		pUp->Down = NULL;
		pUp = pUp->Right;
	}
	return head;
}

NODE * CreateNode(int x,int y,int sign)
{
	NODE * p;
	p = (NODE *)malloc(sizeof(NODE));
	p->site.x = x;
	p->site.y = y;
	p->sign = sign;
	p->IsUse = False;
	p->IsUpConnected = False;
	p->IsDownConnected = False;
	p->IsLeftConnected = False;
	p->IsRightConnected = False;
	return p;
}

//�������һ���Թ������뿪ʼ�ͽ��������꣬��x,y����ָ��NULL�����һ������
void RandomMap(NODE * head, SITE * start, SITE * end)
{
	int L,K, ran,sign=0;
	NODE * p ;
	SITE Used[MaxL*MaxK];
	int count = 0;
	LKMap(head,&L,&K);
	//���NULL�����һ������
	if(start == NULL)
	{
		start = (SITE *)malloc(sizeof(SITE));
		start->x = 0;
		start->y = Random(K);
	}
	if(end == NULL)
	{
		end = (SITE *)malloc(sizeof(SITE));
		end->x = L;
		end->y = Random(K);
		while (end->y == start->y)
		{
			end->y = Random(K);
		}
	}
	p = MoveMap(head, *start);
	p->IsUse = True;
	Used[count].x = p->site.x;
	Used[count].y = p->site.y;
	count++;
	while (!IsAllUse(head))
	{
		while (IsBesideUse(p))
		{
			sign = 0;
			while (sign == 0)
			{
				ran = Random(4);
				switch (ran)
				{
				case 0:
					if (p->Up != NULL&&p->Up->IsUse == 0)
					{
						p->IsUpConnected = True;
						p = p->Up;
						p->IsUse = True;
						p->IsDownConnected = True;
						sign = 1;
						Used[count].x = p->site.x;
						Used[count].y = p->site.y;
						count++;
						break;
					}
					else
						break;
				case 1:
					if (p->Down != NULL&&p->Down->IsUse == 0)
					{
						p->IsDownConnected = True;
						p = p->Down;
						p->IsUse = True;
						p->IsUpConnected = True;
						sign = 1;
						Used[count].x = p->site.x;
						Used[count].y = p->site.y;
						count++;
						break;
					}
					else
						break;
				case 2:
					if (p->Left != NULL&&p->Left->IsUse == 0)
					{
						p->IsLeftConnected = True;
						p = p->Left;
						p->IsUse = True;
						p->IsRightConnected = True;
						sign = 1;
						Used[count].x = p->site.x;
						Used[count].y = p->site.y;
						count++;
						break;
					}
					else
						break;
				case 3:
					if (p->Right != NULL&&p->Right->IsUse == 0)
					{
						p->IsRightConnected = True;
						p = p->Right;
						p->IsUse = True;
						p->IsLeftConnected = True;
						sign = 1;
						Used[count].x = p->site.x;
						Used[count].y = p->site.y;
						count++;
						break;
					}
					else
						break;
				}
			}
		}
		//����ٴ������λ��Ϊδ����·���ģ��������
		p=MoveMap(head, Used[Random(count)]);
	}
}
