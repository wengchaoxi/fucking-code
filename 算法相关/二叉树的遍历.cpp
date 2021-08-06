#include <iostream>
#include <stack>
#include <queue>

#define T char

/**
* @struct
* @brief 二叉树结点
*/
struct BTNode
{
	T data;
	BTNode *left;
	BTNode *right;
	BTNode():left(NULL), right(NULL){}
}; 

/**
* @brief 构建二叉树，递归 
* param[in] 根结点的引用
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
* @brief 打印元素数据 
*/ 
void visit(const BTNode *e)
{
	printf("%c", e->data);
}

/**
* @brief 先序遍历，递归 
* @param[in] root 根结点
* @param[out] visit 访问数据的函数指针
* 
* @return 无 
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
* @brief 中序遍历，递归 
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
* @brief 后序遍历，递归 
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
* @brief 先序遍历，非递归，DFS 
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
		if(p->right != NULL) // 先右孩子进栈，这样左孩子会先出栈以满足先序遍历 
			s.push(p->right);
		if(p->left != NULL)
			s.push(p->left);
	}
} 

/**
* @brief 中序遍历，非递归 
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
* @brief 后序遍历，非递归 
*/ 
void post_order(const BTNode *root, void (*visit)(const BTNode *))
{
	const BTNode *p = root, *q;
	std::stack<const BTNode *> s;
	
	do {
		while(p != NULL) // 走到最左下 
		{
			s.push(p);
			p = p->left;
		}
		q = NULL;
		while(!s.empty())
		{
			p = s.top(); s.pop();
			if(p->right == q) // 右孩子不存在或已被访问 
			{
				visit(p);
				q = p; // 保存刚访问过的结点 
			} 
			else
			{
				s.push(p); // 当前结点不能访问，需要第二次进栈
				p = p->right; // 先处理右子树
				break; 
			} 
		}
	} while(!s.empty());
} 

/**
* @brief 层次遍历，BFS
* 
* 与先序遍历相同，但实现由栈换成了队列 
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
