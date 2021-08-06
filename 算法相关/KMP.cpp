#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
/**
* @brief ���㲿��ƥ��� PMT��һ����ʽ��next���� 
*
* @param[in] pattern ģʽ��
* @param[out] next next����
* @return �� 
*/
void compute_next(char *pattern, int *next)
{
	next[0] = -1; 
	int i = 0;  
	int j = -1; 
	const int n = strlen(pattern);
	while(i < n-1)
	{
		if(j==-1 || pattern[i]==pattern[j])
		{
			++i;
			++j;
			next[i] = j; 
		}
		else j = next[j]; 
	} 
}
/**
* @brief KMP�㷨 
*
* @param[in] text ��ƥ���ı� 
* @param[in] pattern ģʽ�� 
* @return ƥ��ɹ�����ģʽ����һ�γ������ı���λ�ã����򷵻�-1 
*/
int KMP(char *text, char *pattern)
{
	const int m = strlen(text);
	const int n = strlen(pattern); 
	
	int *next = (int*)malloc(sizeof(int) * n);
	compute_next(pattern, next);
	
	int i = 0; // ָ��Դ�� 
	int j = 0; // ָ��ģʽ�� 
	while(i<m && j<n)
	{
		// ��ģʽ��ͷ��ʼ ���� ��ǰָ���ַ�ƥ�� 
		if(j==-1 || text[i]==pattern[j])
		{
			++i;
			++j;
		}
		else j = next[j]; // ��ƥ���� jָ�� ��ǰ�ƶ�������next������ٲ���Ҫ���ظ�ƥ�� 
	}
	free(next);
	// jָ���ߵ������ƥ��ɹ�������ģʽ����ԭ���еĳ��ֵ�λ�� 
	if(j == n)
		return i - j;
	else return -1;
} 

int main()
{
	char *text = "abc123 456789";
	char *pattern = "123";
	int i = KMP(text, pattern);
	printf("index: %d\n", i);
	return 0;
}
