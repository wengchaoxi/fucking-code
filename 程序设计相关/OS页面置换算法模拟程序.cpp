/*
* ��Ŀ��OSҳ���û��㷨ģ�����
* ���ߣ��������� 
* ʱ�䣺2020��6��3�� 
* CentOS��Windows��g++����ͨ��
*/
#include <cstdio>
#include <ctime>
#include <cstdlib> 
#define VP 32
#define T 320
#define INVALID -1
#define MAX 32767

struct page_struct // ҳ��ṹ 
{
	int data; 
	
	int pnum; // ҳ��� 
	int pfacen; // ָ��ҳ��� 
	int time; // ʹ�ô��� 
}; 

struct page_control // ҳ����ƽṹ 
{
	int data;
	
	int pnum;
	int pfacen;
	page_control *next;
}; 

page_struct  vpage[VP]; // �����ڴ�ҳ 
page_control pfc[VP]; // ҳ����� (4 - VP)
page_control *freepf_head, *busypf_head, *busypf_tail; 

int dp[T]; // dp �������У�ȡֵ��0-T
int page[T]; //����ҳ��ţ��ɵ������г�ʼ����ȡֵ��0 - VP-1 
int diseffect; // ȱҳ���� 

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
		vpage[i].pnum = i; // ��ʼ��ҳ��� 
		vpage[i].pfacen = INVALID; // ��Ϊ��Ч 
		vpage[i].time = -1; // ���ù���ҳ����Ƶķ��ʴ��� 
	}
	
	// ������������(���ȣ�4-VP)
	for(int i=1; i<pp; i++)
	{
		pfc[i-1].next = &pfc[i]; // ǰָ���
		pfc[i-1].pfacen = i - 1;  
	} 
	pfc[pp-1].next = NULL;
	pfc[pp-1].pfacen = pp - 1;
	freepf_head = &pfc[0]; // ����ָ��ָ���һҳ 
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
		if(vpage[page[i]].pfacen == INVALID) // ҳ��δ��ʹ�� 
		{
			diseffect ++; // ҳ���жϴ���
			if(freepf_head == NULL) // ����ҳ���ˣ��ڴ�ռ���� 
			{
				// �����ӣ�FIFO
				p = busypf_head->next;
				vpage[busypf_head->pnum].pfacen = INVALID; 
				freepf_head = busypf_head; // ������ָ��ָ�����Ԫ�������ڴ� 
				freepf_head->next = NULL;
				busypf_head = p; 
			} 
			
			// ��һ�����ҳ���ڴ� 
			p = freepf_head->next;
			
			freepf_head->pnum = page[i];
			freepf_head->next = NULL;
			// ͨ�� pfacen ������ϵ (0 - pp-1)
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
	int present_time = 0; // ��ǰʱ��
	init(pp);
	for(int i=0; i<T; i++)
	{
		if(vpage[page[i]].pfacen == INVALID) // ҳ��δ��ʹ�� 
		{
			diseffect ++; 
			if(freepf_head == NULL) // ����ҳ�����ڴ����� 
			{
				// �����δʹ��
				int min = MAX; // ��ʼ minΪ���ֵ 
				int min_index = 0;
				for(int j=0; j<VP; j++)
				{
					if(min>vpage[j].time && ~vpage[j].pfacen) // ��ʹ�ô������� 
					{
						min = vpage[j].time;
						min_index = j;
					}
				} 
				
				//������ָ��ָ��ʹ�ô�������ҳ�����ʹ��״̬ 
				freepf_head = &pfc[vpage[min_index].pfacen]; 
				freepf_head->next = NULL; 
				vpage[min_index].pfacen = INVALID;
				vpage[min_index].time = -1;
			}
			
			// ������ϵ 
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
	int vdistance[VP]; // �洢��Ӧҳ������ 
	init(pp);
	for(int i=0; i<T; i++)
	{
		if(vpage[page[i]].pfacen == INVALID) // ҳ��δ��ʹ�� 
		{
			diseffect ++;
			if(freepf_head == NULL) // ����ҳ�����ڴ�����
			{
				// ��ʼ��� 
				for(int j=0; j<VP; j++)
				{
					if(~vpage[j].pfacen) // ��ǰҳû�б�ʹ�� 
						vdistance[j] = MAX;
					else vdistance[j] = 0;
				}
				int distance = 1; // Խ��Խ���ᱻ��Ӧ 
				
				// ���¼�� 
				for(int j=i+1; j<T; j++)
				{
					if((vdistance[page[j]]==MAX) && (~vpage[page[j]].pfacen))
						vdistance[page[j]] = distance;
					distance ++;
				}
				
				// ����Զ 
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
				
				// ������ϵ 
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
		r = _rand(); // �ɸĽ��� 
	}
	for(int i=0;i<T;i++) //��dp��ָ�����б任��ҳ��ַ��
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

