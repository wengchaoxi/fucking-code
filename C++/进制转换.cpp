#include <stdio.h>
#include <stdlib.h>

/**
* @brief ��ջģ���� 
*/
template <class T>
class Stack
{
	struct Node
	{
		T data;
		Node *next;
		Node():next(NULL){}
	};
	Node *_top;
	unsigned int _size;
public:
	Stack()
	{
		_top = NULL;
		_size = 0;
	} 
	void push(const T &e)
	{
		Node *p = new Node;
		if(p == NULL)
			exit(-1);
		p->data = e;
		p->next = _top; // ��ջ 
		_top = p; // ����ջ�� 
		++_size; 
	}
	T pop()
	{
		T e;
		if(_size == 0)
			return e;
		Node *p = _top;
		e = _top->data;
		_top = _top->next; // ��ջ 
		delete p;
		--_size;
		return e; 
	}
	T top()
	{
		if(_size == 0)
			return T();
		return _top->data;
	}
	unsigned int size()
	{
		return _size;
	}
	bool empty()
	{
		return (_size==0);
	}
	~Stack()
	{
		while(_size--)
		{
			Node *p = _top;
			_top = _top->next;
			delete p;
		}
	}
};

/**
* @brief ���� ��ջ ��10����ת��Ϊ16�����½���
* 
* @param[in] n ʮ������
* @param[in] d Ŀ����� 
* @return �� 
*/
void convert_base(int n, const int d)
{
	Stack<int> s;
	int e;
	do{
		e = n % d; // ��ȡ��ǰλ�õ���ֵ�������λ��ʼ�� 
		s.push(e); // ������ֵ��ջ 
		n /= d; // ���λ�ƶ�һλ 
	} while(n != 0);
	
	while(!s.empty())
	{
		e = s.pop();
		printf("%X", e);
	}
} 

/**
* @brief ���� ���� ��10����ת��Ϊ16�����½���
* 
* �������Ԥ��ջ�����ռ䣬����������ģ��ջ���������ӽ�ʡ��Դ�� 
* ջ����󳤶��Ƕ��٣�����CPU��64λ�������������2^64��
* ����������СΪ2�����������������ջ�Ŀռ���������Ϊ64�� 
* 
* @param[in] n ʮ������
* @param[in] d Ŀ����� 
* @return �� 
*/
const int MAX_LEN = 64;
int stack[MAX_LEN];
int top = -1;
void convert_base_array(int n, const int d)
{
	int e;
	do{
		e = n % d; // ��ȡ��ǰλ��ֵ
		stack[++top] = e; // ��ջ 
		n /= d; // ��ǰ�ƶ� 
	}while(n != 0);
	while(top != -1)
	{
		e = stack[top--];
		printf("%X", e);
	}
} 

int main()
{
	convert_base(10, 16);
	convert_base_array(10, 2);
	return 0;
}
