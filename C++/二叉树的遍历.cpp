#include <iostream>
#include <stack>
#include <queue>

#define T char

struct BTNode
{
	T data;
	BTNode *left;
	BTNode *right;
	BTNode():left(NULL), right(NULL){}
}; 

/**
* @brief �������������ݹ� 
* param[in] ����������
*/
void build_btree(BTNode* &root)
{
	char ch;
	scanf("%c", &ch);
	if(ch != '#')
	{
		root = new BTNode;
		root->data = ch;
		build_btree(root->left);
		build_btree(root->right);
	} 
	else root = NULL;
}

/**
* @brief ��ӡԪ������ 
*/ 
void visit(const BTNode *e)
{
	printf("%c", e->data);
}

/**
* @brief ����������ݹ� 
* @param[in] root ���ڵ�
* @param[out] visit �������ݵĺ���ָ��
* 
* @return �� 
*/
void pre_order_r(const BTNode *root, void (*visit)(const BTNode *))
{
	if(root == NULL)
		return ;
	
	visit(root);
	pre_order_r(root->left, visit);
	pre_order_r(root->right, visit);
} 

/**
* @brief ����������ݹ� 
*/ 
void in_order_r(const BTNode *root, void (*visit)(const BTNode *)) 
{
	if(root == NULL)
		return ;
	in_order_r(root->left, visit);
	visit(root);
	in_order_r(root->right, visit); 
}

/**
* @brief ����������ݹ� 
*/
void post_order_r(const BTNode *root, void (*visit)(const BTNode *))
{
	if(root == NULL)
		return ;
	post_order_r(root->left, visit);
	post_order_r(root->right, visit);
	visit(root);
}  

/**
* @brief ����������ǵݹ� 
*/
void pre_order(const BTNode *root, void (*visit)(const BTNode *))
{
	const BTNode *p = root;
	std::stack<const BTNode *> s;
	if(p != NULL)
		s.push(p);
	while(!s.empty())
	{
		p = s.top(); s.pop();
		visit(p);
		if(p->right != NULL) // ���Һ��ӽ�ջ���������ӻ��ȳ�ջ������������� 
			s.push(p->right);
		if(p->left != NULL)
			s.push(p->left);
	}
} 

/**
* @brief ����������ǵݹ� 
*/
void in_order(const BTNode *root, void (*visit)(const BTNode *))
{
	const BTNode *p = root;
	std::stack<const BTNode *> s;
	
	while(!s.empty() || p!=NULL)
	{
		if(p != NULL)
		{
			s.push(p);
			p = p->left;
		}
		else
		{
			p = s.top(); s.pop();
			visit(p);
			p = p->right;
		}
	}
} 

/**
* @brief ����������ǵݹ� 
*/ 
void post_order(const BTNode *root, void (*visit)(const BTNode *))
{
	const BTNode *p = root, *q;
	std::stack<const BTNode *> s;
	
	do {
		while(p != NULL) // �ߵ������� 
		{
			s.push(p);
			p = p->left;
		}
		q = NULL;
		while(!s.empty())
		{
			p = s.top(); s.pop();
			if(p->right == q) // �Һ��Ӳ����ڻ��ѱ����� 
			{
				visit(p);
				q = p; // ����շ��ʹ��Ľ�� 
			} 
			else
			{
				s.push(p); // ��ǰ��㲻�ܷ��ʣ���Ҫ�ڶ��ν�ջ
				p = p->right; // �ȴ���������
				break; 
			} 
		}
	} while(!s.empty());
} 

/**
* @brief ��α�����BFS
* 
* �����������ͬ����ʵ����ջ�����˶��� 
*/
void level_order(const BTNode *root, void (*visit)(const BTNode *))
{
	const BTNode *p = root;
	std::queue<const BTNode *> q;
	if(p != NULL)
		q.push(p);
	while(!q.empty())
	{
		p = q.front(); q.pop();
		visit(p);
		
		// ��ʱ��������Կ� 
		if(p->left != NULL)
			q.push(p->left);
		if(p->right != NULL)
			q.push(p->right);
	}
} 

int main()
{
	BTNode *root;
	build_btree(root);
	pre_order_r(root, visit);
	in_order_r(root, visit);
	post_order_r(root, visit); 
	return 0;
} 
