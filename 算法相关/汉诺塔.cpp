#include <stdio.h>

/*
* @brief 汉诺塔问题 
*
* @param[in] n 圆盘个数
* @param[in] a 源塔座
* @param[in] b 辅助塔座 
* @param[in] c 目标塔座
* @return 无 
*/
void hanoi(int n, char a, char b, char c)
{
	if(n == 1)
	{
		printf("1: %c->%c\n", a, c); // 将最后一块圆盘，即 1 号圆盘移动到 C 塔座 
		return ;
	}
	else
	{
		hanoi(n-1, a, c, b); // 借助 C 塔座，将 1~n-1号圆盘 移动到 B 塔座。 
		printf("%d: %c->%c\n", n, a, c); // 将 A 塔座的 n 号圆盘移动到 C 塔座 
		hanoi(n-1, b, a, c); // 借助 A 塔座，将 1~n-1号圆盘 移动到 C 塔座。 
	}
} 

int main()
{
	int n;
	scanf("%d", &n);
	int k = (1<<n) - 1; // 最少步数为 2^n - 1 
	printf("%d\n", k);
	hanoi(n, 'A', 'B', 'C');
	return 0; 
}
