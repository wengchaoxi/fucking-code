/*
* 题目：OS进程状态转换模拟 
* 作者：公羽向阳 
* 时间：2020年6月11日 
* Windows下g++编译通过
*/
#include <iostream>
#include <queue>
#include <cstdlib> 
#include <ctime>
#include <pthread.h>
#include <windows.h>

#define _CPU 2 		// 模拟CPU个数 
#define _TIME 50 	// 时间片 
#define _PL 100 	// 进程数量 
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
		cout << "PID:" << pid << " 时间: " << time << " 类型: " << type << endl; 
	};
};

queue<PCB *> _ready, _block_i, _block_o; 

void *CPU(void *args)
{
	printf("CPU_%d 开始工作\n", *((int*)args));
	PCB *pcb = NULL;
	bool cpu_free = true;
	int _time = _TIME; 
	while(true) 
	{
		// 当前CPU空闲，就绪队列存在进程 
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
				printf("CPU_%d 等待中\n", *((int*)args));
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
						cout << " 执行计算指令XXX" << endl;
					}
					else
					{
						cout << " 释放" << endl;
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
					cout << " 时间片用完" << endl; 
				}
				break;
			case I:
				cout << " 发起INPUT请求" << endl;
				_block_i.push(pcb);
				cpu_free = true;
				break;
			case O:
				cout << " 发起OUTPUT请求" << endl;
				_block_o.push(pcb);
				cpu_free = true;
				break; 
			default : cout << "出错!" << endl; exit(0);
		}
		Sleep(10);
	}
}

void *IODevice(void *args)
{
	if(*((int*)args) == 0)
		cout << "INPUT设备开始工作" << endl; 
	else cout << "OUTPUT设备开始工作" << endl; 
	while(true)
	{
		if(*((int*)args) == 0) 
		{
			if(!_block_i.empty())
			{
				PCB *pcb = _block_i.front();
				_block_i.pop();
				cout << "INPUT ";
				cout << "PID:" << pcb->pid << " 执行输入指令III" << endl;
			}
			else
			{
				_IO_WAIT
				if(_block_i.empty()) 
				{
					cout << "INPUT设备等待中" << endl;
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
				cout << "PID:" << pcb->pid << " 执行输出指令OOO" << endl;
			}
			else
			{
				_IO_WAIT
				if(_block_o.empty()) 
				{
					cout << "OUTPUT设备等待中" << endl;
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
	// 产生随机进程进入就绪队列 
	srand((unsigned)time(NULL)); 
	for(int i=0; i<_PL; i++)
	{
		PCB *p = new PCB();
		p->time = rand()%80 + 10; 
		p->type = TYPE(rand()%3);
		_ready.push(p);
		p->print();
	}
	
	// cpu访问就绪队列的锁
	int r = pthread_mutex_init(&cpu_lock, NULL);
	if(r)
		exit(-1);
		
	// io设备使用2个线程，cpu使用_CPU个线程
	pthread_t io_tid[2], cpus[_CPU];
	int args[_CPU];
	int _io[2];
	_io[0] = 0; _io[1] = 1;
	pthread_create(&io_tid[0], NULL, IODevice, (void *)&_io[0]); // I线程 
	pthread_create(&io_tid[1], NULL, IODevice, (void *)&_io[1]); // O线程 
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
