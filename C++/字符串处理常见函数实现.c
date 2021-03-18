#include <assert.h>
#include <stdlib.h>

size_t strlen(const char *str) 
{
	const char *s;
	for(s=str; *s; ++s);
	return s-str;
} 

char* strcpy(char *to, const char *from)
{
	assert(to!=NULL && from!=NULL);
	char *p = to;
	while((*p++=*from++) != '\0'); 
	return to;
}

// 暴力查找 O(m*n) 
char* strstr(const char *haystack, const char *needle)
{
	// 目标串为空，则返回源串 
	if(!*needle) return (char*)haystack;
	const char *p1, *p2;
	const char *p1_advance = haystack;
	
	// 将p1_advance向后移动目标串的长度个单位，可用此减少匹配次数 
	for(p2=&needle[1]; *p2; ++p2)
	{
		p1_advance++;
	}
	
	for(p1=haystack; *p1_advance; p1_advance++)
	{
		// 匹配起始地址 
		char *p1_old = (char*)p1;
		p2 = needle;
		
		// 遍历匹配 
		while(*p1 && *p2 && *p1==*p2)
		{
			p1++;
			p2++; 
		}
		
		// p2为空，证明完全匹配了 
		if(!*p2) return p1_old; 
		
		// 起始地址加 1 
		p1 = p1_old + 1; 
	}
	
	// 没找到 
	return NULL; 
}

int atoi_(const char *str)
{
	int num = 0;
	int sign = 1;
	const int len = strlen(str);
	int i = 0;
	while(str[i]==' ' && i<len)
		i++;
	if(str[i] == '+')
		i++;
	if(str[i] == '-')
	{
		sign = -1;
		i++;
	}
	for(; i<len; i++)
	{
		if(str[i]<'0' || str[i]>'9')
			break;
		if(num>INT_MAX/10 ||
			(num==INT_MAX/10 && 
				(str[i]-'0') > INT_MAX%10))
		{
			return sign==-1 ? INT_MIN : INT_MAX;
		}
		// 累计求和 
		num = num*10 + str[i]-'0';
	}
	return num * sign;
} 

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
	
	int k=0;
	for(k; k<n; k++)
		printf("%d", next[k]);
	
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


void main(void)
{
	char *data = "-123456";
	char *to, *from = "789456";
	
	// 字符串长度 
	printf("%d\n", strlen(data));
	
	// 字符串复制 
//	to = strcpy(to, from);
//	printf("%c", to);
	
	// 字串查找 
	char *str = strstr(data, "3"); 
	printf("%s\n", str);
	
	// atoi
	int i = atoi_(data); 
	printf("%d\n", i);
	
	// kmp
	char *t = "0111a babab ca1";
	char *p = "1212123";
	i = KMP(t, p);
	printf("index: %d\n", i);
} 
