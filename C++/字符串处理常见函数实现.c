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

// �������� O(m*n) 
char* strstr(const char *haystack, const char *needle)
{
	// Ŀ�괮Ϊ�գ��򷵻�Դ�� 
	if(!*needle) return (char*)haystack;
	const char *p1, *p2;
	const char *p1_advance = haystack;
	
	// ��p1_advance����ƶ�Ŀ�괮�ĳ��ȸ���λ�����ô˼���ƥ����� 
	for(p2=&needle[1]; *p2; ++p2)
	{
		p1_advance++;
	}
	
	for(p1=haystack; *p1_advance; p1_advance++)
	{
		// ƥ����ʼ��ַ 
		char *p1_old = (char*)p1;
		p2 = needle;
		
		// ����ƥ�� 
		while(*p1 && *p2 && *p1==*p2)
		{
			p1++;
			p2++; 
		}
		
		// p2Ϊ�գ�֤����ȫƥ���� 
		if(!*p2) return p1_old; 
		
		// ��ʼ��ַ�� 1 
		p1 = p1_old + 1; 
	}
	
	// û�ҵ� 
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
		// �ۼ���� 
		num = num*10 + str[i]-'0';
	}
	return num * sign;
} 

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
	
	int k=0;
	for(k; k<n; k++)
		printf("%d", next[k]);
	
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


void main(void)
{
	char *data = "-123456";
	char *to, *from = "789456";
	
	// �ַ������� 
	printf("%d\n", strlen(data));
	
	// �ַ������� 
//	to = strcpy(to, from);
//	printf("%c", to);
	
	// �ִ����� 
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
