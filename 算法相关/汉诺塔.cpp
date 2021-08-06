#include <stdio.h>

/*
* @brief ��ŵ������ 
*
* @param[in] n Բ�̸���
* @param[in] a Դ����
* @param[in] b �������� 
* @param[in] c Ŀ������
* @return �� 
*/
void hanoi(int n, char a, char b, char c)
{
	if(n == 1)
	{
		printf("1: %c->%c\n", a, c); // �����һ��Բ�̣��� 1 ��Բ���ƶ��� C ���� 
		return ;
	}
	else
	{
		hanoi(n-1, a, c, b); // ���� C �������� 1~n-1��Բ�� �ƶ��� B ������ 
		printf("%d: %c->%c\n", n, a, c); // �� A ������ n ��Բ���ƶ��� C ���� 
		hanoi(n-1, b, a, c); // ���� A �������� 1~n-1��Բ�� �ƶ��� C ������ 
	}
} 

int main()
{
	int n;
	scanf("%d", &n);
	int k = (1<<n) - 1; // ���ٲ���Ϊ 2^n - 1 
	printf("%d\n", k);
	hanoi(n, 'A', 'B', 'C');
	return 0; 
}
