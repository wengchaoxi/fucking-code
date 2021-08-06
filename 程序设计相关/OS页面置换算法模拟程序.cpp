/*
* 题目：OS页面置换算法模拟程序
* 作者：公羽向阳 
* 时间：2020年6月3日 
* CentOS、Windows下g++编译通过
*/
#include <cstdio>
#include <ctime>
#include <cstdlib> 
#define VP 32
#define T 320
#define INVALID -1
#define MAX 32767

struct page_struct // 页面结构 
{
	int data; 
	
	int pnum; // 页面号 
	int pfacen; // 指向页面号 
	int time; // 使用次数 
}; 

struct page_control // 页面控制结构 
{
	int data;
	
	int pnum;
	int pfacen;
	page_control *next;
}; 

page_struct  vpage[VP]; // 虚拟内存页 
page_control pfc[VP]; // 页面控制 (4 - VP)
page_control *freepf_head, *busypf_head, *busypf_tail; 

int dp[T]; // dp 调度序列，取值：0-T
int page[T]; //进程页面号，由调度序列初始化，取值：0 - VP-1 
int diseffect; // 缺页计数 

int _rand()
{
	int rand_num = rand()%MAX;
	return (int)(float)(T-1)*rand_num/MAX+1;
}

void init(int pp)
{
	diseffect = 0;
	for(int i=0; i<VP; i++)
	{
		vpage[i].pnum = i; // 初始化页面号 
		vpage[i].pfacen = INVALID; // 设为无效 
		vpage[i].time = -1; // 设置关于页面控制的访问次数 
	}
	
	// 建立控制链表(长度：4-VP)
	for(int i=1; i<pp; i++)
	{
		pfc[i-1].next = &pfc[i]; // 前指向后
		pfc[i-1].pfacen = i - 1;  
	} 
	pfc[pp-1].next = NULL;
	pfc[pp-1].pfacen = pp - 1;
	freepf_head = &pfc[0]; // 空闲指针指向第一页 
}

float ans(int diseffect)
{
	return (1-(float)diseffect/T);
}

void fifo(int pp)
{
	page_control *p;
	init(pp);
	busypf_head = busypf_tail = NULL; // 0
	for(int i=0; i<T; i++)
	{
		if(vpage[page[i]].pfacen == INVALID) // 页面未被使用 
		{
			diseffect ++; // 页面中断次数
			if(freepf_head == NULL) // 控制页满了（内存占满） 
			{
				// 出链队，FIFO
				p = busypf_head->next;
				vpage[busypf_head->pnum].pfacen = INVALID; 
				freepf_head = busypf_head; // 将空闲指针指向出队元素所在内存 
				freepf_head->next = NULL;
				busypf_head = p; 
			} 
			
			// 调一个虚存页进内存 
			p = freepf_head->next;
			
			freepf_head->pnum = page[i];
			freepf_head->next = NULL;
			// 通过 pfacen 建立联系 (0 - pp-1)
			vpage[page[i]].pfacen = freepf_head->pfacen;  
			
			if(busypf_tail == NULL)
				busypf_head = busypf_tail = freepf_head;
			else
			{
				busypf_tail->next = freepf_head;
				busypf_tail = freepf_head; 
			}
			
			freepf_head = p; 
		} 
	}
	printf("FIFO: %.5f\t", ans(diseffect));
}

void lru(int pp)
{
	int present_time = 0; // 当前时间
	init(pp);
	for(int i=0; i<T; i++)
	{
		if(vpage[page[i]].pfacen == INVALID) // 页面未被使用 
		{
			diseffect ++; 
			if(freepf_head == NULL) // 控制页满（内存满） 
			{
				// 找最久未使用
				int min = MAX; // 初始 min为最大值 
				int min_index = 0;
				for(int j=0; j<VP; j++)
				{
					if(min>vpage[j].time && ~vpage[j].pfacen) // 找使用次数最少 
					{
						min = vpage[j].time;
						min_index = j;
					}
				} 
				
				//将空闲指针指向使用次数最少页，清除使用状态 
				freepf_head = &pfc[vpage[min_index].pfacen]; 
				freepf_head->next = NULL; 
				vpage[min_index].pfacen = INVALID;
				vpage[min_index].time = -1;
			}
			
			// 建立联系 
			vpage[page[i]].pfacen = freepf_head->pfacen;
			freepf_head = freepf_head->next; 
		}
		vpage[page[i]].time = present_time; 
		present_time ++;
	}
	printf("LRU: %.5f\t", ans(diseffect));
}

void opt(int pp)
{
	int vdistance[VP]; // 存储对应页所需间隔 
	init(pp);
	for(int i=0; i<T; i++)
	{
		if(vpage[page[i]].pfacen == INVALID) // 页面未被使用 
		{
			diseffect ++;
			if(freepf_head == NULL) // 控制页满（内存满）
			{
				// 初始间隔 
				for(int j=0; j<VP; j++)
				{
					if(~vpage[j].pfacen) // 当前页没有被使用 
						vdistance[j] = MAX;
					else vdistance[j] = 0;
				}
				int distance = 1; // 越大越不会被响应 
				
				// 更新间隔 
				for(int j=i+1; j<T; j++)
				{
					if((vdistance[page[j]]==MAX) && (~vpage[page[j]].pfacen))
						vdistance[page[j]] = distance;
					distance ++;
				}
				
				// 找最远 
				int max = -1;
				int max_index = 0;
				for(int j=0; j<VP; j++)
				{
					if(vdistance[j] > max)
					{
						max = vdistance[j];
						max_index = j;
					}
				}
				
				// 建立联系 
				freepf_head = &pfc[vpage[max_index].pfacen];
				freepf_head->next = NULL;
				vpage[max_index].pfacen = INVALID;
			}
			vpage[page[i]].pfacen = freepf_head->pfacen;
			freepf_head = freepf_head->next;
		}
	}
	printf("OPT: %.5f\t", ans(diseffect));
}

int main()
{
	srand((unsigned)time(NULL)); 
	int r = _rand();
	for(int i=0; i<T; i+=3)
	{
		dp[i] = r;
		dp[i+1] = dp[i]+1;

		dp[i+2] = _rand();
		dp[i+3] = dp[i+2]+1;
		r = _rand(); // 可改进处 
	}
	for(int i=0;i<T;i++) //将dp的指令序列变换成页地址流
	{
		page[i] = (dp[i]/10)%VP;
	}
	
	// test
	for(int i=4; i<VP+1; i++)
	{
		fifo(i);
		lru(i);
		opt(i); 
		printf("\n");
	}
	return 0;
}

