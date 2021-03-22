#include <stdio.h>
#include <stdlib.h> 

/**
* @brief ���� 
*/
template<class T>
class Queue
{
	struct Node
	{
		T data;
		Node *next;
		Node():next(NULL){}
	};
	Node *_head, *_tail; 
	unsigned int _size;
public:
	Queue()
	{
		_head = _tail = NULL;
		_size = 0;
	}
	void push(const T &e) // ��� 
	{
		Node *p = new Node;
		if(p == NULL)
			exit(-1);
		
		p->data = e;
		
		if(_head == NULL)
			_head = _tail = p;
		else
			_tail = _tail->next = p;
		++_size;
	}
	T pop()
	{
		T e;
		Node *p = _head;
		if(_head == NULL)
			return e;
		e = p->data;
		_head = _head->next;
		delete p;
		--_size;
		return e;
	}
	unsigned int size()
	{
		return _size;
	}
	bool empty()
	{
		return (_size==0);
	}
	~Queue()
	{
		Node *p;
		while(_size--)
		{
			p = _head;
			_head = _head->next;
			delete p;
		}
	}
}; 

/**
* @brief ��ӡ�������ϵ�� 
* ���д�ӡ����ʽ (a+b)^n ��չ��ʽ��ϵ�� 
* 
* @param[in] n (a+b)^n 
* @return �� 
***
*/
void yanghui(const int n)
{
	Queue<int> q;
	q.push(1); // �����һ�е� 1 
	
	for(int i=0; i<=n; i++) // �� 
	{
		int s = 0;
		q.push(s);
		
		for(int j=0; j<i+2; j++)
		{
			int t;
			int tmp;
			t = q.pop();
			tmp = s + t;
			q.push(tmp);
			s = t;
			if(j != i+1)
			{
				printf("%d ", s);
			}
		}
		printf("\n");
	}
}

int main()
{
	yanghui(10);
	return 0; 
} 
