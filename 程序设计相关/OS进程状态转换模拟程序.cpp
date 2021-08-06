/*
* ��Ŀ��OS����״̬ת��ģ�� 
* ���ߣ��������� 
* ʱ�䣺2020��6��11�� 
* Windows��g++����ͨ��
*/
#include <iostream>
#include <queue>
#include <cstdlib> 
#include <ctime>
#include <pthread.h>
#include <windows.h>

#define _CPU 2 		// ģ��CPU���� 
#define _TIME 50 	// ʱ��Ƭ 
#define _PL 100 	// �������� 
#define _IO_WAIT Sleep(3000);
#define _CPU_WAIT Sleep(3000);
using namespace std;

pthread_mutex_t cpu_lock;
unsigned int _pid = 0;
enum TYPE {C, I, O};
struct PCB
{
	unsigned int pid;
	unsigned int time;
	TYPE type;
	PCB():pid(++_pid), time(0), type(C) {};
	bool operator==(const PCB pcb) const {
		return this->pid == pcb.pid;
	};
	void print() {
		cout << "PID:" << pid << " ʱ��: " << time << " ����: " << type << endl; 
	};
};

queue<PCB *> _ready, _block_i, _block_o; 

void *CPU(void *args)
{
	printf("CPU_%d ��ʼ����\n", *((int*)args));
	PCB *pcb = NULL;
	bool cpu_free = true;
	int _time = _TIME; 
	while(true) 
	{
		// ��ǰCPU���У��������д��ڽ��� 
		if(cpu_free && !_ready.empty())
		{
			cpu_free = false;
			pthread_mutex_lock(&cpu_lock);
			pcb = _ready.front();
			_ready.pop();
			pthread_mutex_unlock(&cpu_lock);
		}
		else if(cpu_free)
		{
			_CPU_WAIT
			if(_ready.empty())
			{
				printf("CPU_%d �ȴ���\n", *((int*)args));
				// pthread_exit(NULL);
				continue; 
			}
		}
		
		printf("CPU_%d ", *((int*)args));
		cout << "PID:" << pcb->pid;
		switch(pcb->type)
		{
			case C: 
				if(_time-- > 0)
				{
					if(pcb->time-- > 0)
					{
						cout << " ִ�м���ָ��XXX" << endl;
					}
					else
					{
						cout << " �ͷ�" << endl;
						delete pcb; pcb = NULL;
						cpu_free = true;
						_time = _TIME;
					}
				}
				else
				{
					_ready.push(pcb);
					cpu_free = true;
					_time = _TIME;
					cout << " ʱ��Ƭ����" << endl; 
				}
				break;
			case I:
				cout << " ����INPUT����" << endl;
				_block_i.push(pcb);
				cpu_free = true;
				break;
			case O:
				cout << " ����OUTPUT����" << endl;
				_block_o.push(pcb);
				cpu_free = true;
				break; 
			default : cout << "����!" << endl; exit(0);
		}
		Sleep(10);
	}
}

void *IODevice(void *args)
{
	if(*((int*)args) == 0)
		cout << "INPUT�豸��ʼ����" << endl; 
	else cout << "OUTPUT�豸��ʼ����" << endl; 
	while(true)
	{
		if(*((int*)args) == 0) 
		{
			if(!_block_i.empty())
			{
				PCB *pcb = _block_i.front();
				_block_i.pop();
				cout << "INPUT ";
				cout << "PID:" << pcb->pid << " ִ������ָ��III" << endl;
			}
			else
			{
				_IO_WAIT
				if(_block_i.empty()) 
				{
					cout << "INPUT�豸�ȴ���" << endl;
					// pthread_exit(NULL); 
					continue;
				}	
			} 
		}
		else if(*((int*)args) == 1) 
		{
			if(!_block_o.empty())
			{
				PCB *pcb = _block_o.front();
				_block_o.pop();
				cout << "OUTPUT ";
				cout << "PID:" << pcb->pid << " ִ�����ָ��OOO" << endl;
			}
			else
			{
				_IO_WAIT
				if(_block_o.empty()) 
				{
					cout << "OUTPUT�豸�ȴ���" << endl;
					// pthread_exit(NULL); 
					continue;
				}	
			} 
		}
		Sleep(10);
	}
}

void init()
{
	// ����������̽���������� 
	srand((unsigned)time(NULL)); 
	for(int i=0; i<_PL; i++)
	{
		PCB *p = new PCB();
		p->time = rand()%80 + 10; 
		p->type = TYPE(rand()%3);
		_ready.push(p);
		p->print();
	}
	
	// cpu���ʾ������е���
	int r = pthread_mutex_init(&cpu_lock, NULL);
	if(r)
		exit(-1);
		
	// io�豸ʹ��2���̣߳�cpuʹ��_CPU���߳�
	pthread_t io_tid[2], cpus[_CPU];
	int args[_CPU];
	int _io[2];
	_io[0] = 0; _io[1] = 1;
	pthread_create(&io_tid[0], NULL, IODevice, (void *)&_io[0]); // I�߳� 
	pthread_create(&io_tid[1], NULL, IODevice, (void *)&_io[1]); // O�߳� 
	for(int i=0; i<_CPU; i++) // CPU 
	{
		args[i] = i;
		int r = pthread_create(&cpus[i], NULL, CPU, (void *)&args[i]);
		if(r)
			exit(-1);
	}
	pthread_join(io_tid[0], NULL);
	pthread_join(io_tid[1], NULL);
	for(int i=0; i<_CPU; i++)
		pthread_join(cpus[i], NULL);

	pthread_mutex_destroy(&cpu_lock);
} 

int main()
{
	init();
	return 0;
}
