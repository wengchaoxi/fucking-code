#include <stdio.h>
#include <stdlib.h>

/**
* @brief 链栈模板类 
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
		p->next = _top; // 入栈 
		_top = p; // 更新栈顶 
		++_size; 
	}
	T pop()
	{
		T e;
		if(_size == 0)
			return e;
		Node *p = _top;
		e = _top->data;
		_top = _top->next; // 出栈 
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
* @brief 利用 链栈 将10进制转换为16及以下进制
* 
* @param[in] n 十进制数
* @param[in] d 目标进制 
* @return 无 
*/
void convert_base(int n, const int d)
{
	Stack<int> s;
	int e;
	do{
		e = n % d; // 获取当前位置的数值（从最低位开始） 
		s.push(e); // 将该数值入栈 
		n /= d; // 向高位移动一位 
	} while(n != 0);
	
	while(!s.empty())
	{
		e = s.pop();
		printf("%X", e);
	}
} 

/**
* @brief 利用 数组 将10进制转换为16及以下进制
* 
* 如果可以预估栈的最大空间，则用数组来模拟栈，这样更加节省资源。 
* 栈的最大长度是多少？假设CPU是64位，最大整数则是2^64。
* 由于数制最小为2，在这个进制下所需栈的空间最大，所以最长为64。 
* 
* @param[in] n 十进制数
* @param[in] d 目标进制 
* @return 无 
*/
const int MAX_LEN = 64;
int stack[MAX_LEN];
int top = -1;
void convert_base_array(int n, const int d)
{
	int e;
	do{
		e = n % d; // 获取当前位数值
		stack[++top] = e; // 入栈 
		n /= d; // 向前移动 
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
