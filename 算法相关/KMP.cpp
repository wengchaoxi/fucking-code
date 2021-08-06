#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
/**
* @brief 计算部分匹配表 PMT的一种形式，next数组 
*
* @param[in] pattern 模式串
* @param[out] next next数组
* @return 无 
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
* @brief KMP算法 
*
* @param[in] text 待匹配文本 
* @param[in] pattern 模式串 
* @return 匹配成功返回模式串第一次出现在文本的位置，否则返回-1 
*/
int KMP(char *text, char *pattern)
{
	const int m = strlen(text);
	const int n = strlen(pattern); 
	
	int *next = (int*)malloc(sizeof(int) * n);
	compute_next(pattern, next);
	
	int i = 0; // 指向源串 
	int j = 0; // 指向模式串 
	while(i<m && j<n)
	{
		// 从模式串头开始 或者 当前指向字符匹配 
		if(j==-1 || text[i]==pattern[j])
		{
			++i;
			++j;
		}
		else j = next[j]; // 不匹配则 j指针 向前移动，利用next数组减少不必要的重复匹配 
	}
	free(next);
	// j指针走到最后，则匹配成功，返回模式串在原串中的出现的位置 
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
