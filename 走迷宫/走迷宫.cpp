#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<Windows.h>
#include<time.h>

#define MaxL 50
#define MaxK 50

typedef enum //枚举型
{
	False,True
}NewBool;
//False=0,True=1

////储存坐标的结构体
typedef struct zb
{
	int x;
	int y;
}SITE;

//节点用十字链表
typedef struct Node NODE;
struct Node
{
	SITE site;
	//节点的标记（暂时0为普通道路)
	int sign;
	//是否已经布置过路径
	NewBool IsUse;
	NewBool IsUpConnected;//方向 上 是否连通
	NewBool IsDownConnected;//方向 下 是否连通
	NewBool IsLeftConnected;//方向 左 是否连通
	NewBool IsRightConnected;//方向 右 是否连通
	NODE * Up;//连 上 节点
	NODE * Down;//连 下 节点
	NODE * Left;//连 左 节点
	NODE * Right;//连 右 节点
};

//地图节点
typedef struct mn
{
	SITE site;
	int sign;
	//是否是角色在的位置
	NewBool IsCharacter;
	//角色是否走过该路
	NewBool IsPassing;
}MapNode;

NODE * CreateMap(int L,int K);
NODE * CreateNode(int x, int y, int sign);
void RandomMap(NODE * head, SITE * start, SITE * end);
void main()
{
	srand(121);
	//初始化随机种子数
	StartGame();
}

//创建一个地图，返回一个NODE的指针，输入地图的长L宽K
NODE * CreateMap(int L,int K)
{
	int count1,count2;
	NODE *head,*Lhead,*p1,*p2,*pUp;//p1 p2为同一行控制连接，pUp是控制下层链表连接上层的
	for(count1 = 0;count1 < K;count1++)//控制宽
	{
		Lhead = p1 =p2 =CreateNode(0, count1, 0);
		if(count1 == 0)
		{
			head=Lhead;
		}
		for(count2 = 0;count2 < L; count2++)//控制长
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
			//当下次循环不会继续时不创建节点
			if(count2 != L - 1)
			{
				p1 = CreateNode(count2 + 1,count1,0);
			}
			//当不是第一行的时候pUp才有值才可以下一步
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

//随机创建一个迷宫，输入开始和结束的坐标，若x,y坐标指向NULL则随机一个坐标
void RandomMap(NODE * head, SITE * start, SITE * end)
{
	int L,K, ran,sign=0;
	NODE * p ;
	SITE Used[MaxL*MaxK];
	int count = 0;
	LKMap(head,&L,&K);
	//如果NULL则随机一个坐标
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
		//如果再次随机的位置为未设置路径的，重新随机
		p=MoveMap(head, Used[Random(count)]);
	}
}
