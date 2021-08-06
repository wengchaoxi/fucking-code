#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

/**
* @brief 链表的节点,存储一个单词 
*/
struct node
{
	char data[46]; // 据查单词最长有45个字母 
	unsigned int location;
	struct node* next;
};
typedef struct node _node_word;

/**
* @brief 一个检索答案的结构体 
*/
struct result
{
	_node_word word;
	unsigned int count;
	unsigned int total;
	char array_index_str[0x80000]; // 2^19 
};
typedef struct result _result;

_node_word *_hash_table[26];
unsigned int _total = 0;

/**
* @brief 判断字符是否为字母 
* 
* @param[in] ch 一个字符
* @return 1:是, 0:否 
*/
unsigned char is_alphabet(const char ch)
{
	if(ch>='A' && ch<='Z' || ch>='a' && ch<='z')
		return 1;
	else return 0;
}

/**
* @brief 判断是否为单词 
*/ 
int is_word(const char *str)
{
	int i = 0;
	int len = strlen(str);
	for(i; i<len; ++i)
	{
		if(!is_alphabet(str[i]))
			return 0;
	}
	return 1;
}

/**
* @brief 单词hash值 
* 
* @param[in] 单词字符串 
* @return 0-25
*/
int hash(const char *word)
{
	char ch = word[0];
	if(ch>='a' && ch<='z')
		ch -= 0x20;  
	return ch-0x41; 
}

/**
* @brief 初始化哈希表 
*/
void init_hash_table()
{
	int i = 0;
	for(i; i<26; i++)
	{
		_hash_table[i] = malloc(sizeof(_node_word));
		_hash_table[i]->next = NULL;
	}
}

/**
* @brief 从文件获取单词数据来填充哈希表  
* 
* @param[in] file_path 文件路径 
*/
void fill_hash_table_from_file(const char *file_path)
{
	_node_word *new_word, *tmp;
	char ch, tmp_word[46] = {'\0'};
	int index = 0, location = 0;
	FILE *fp = fopen(file_path, "r");
	if(!fp)
		return;
	init_hash_table();
	while(1) 
	{
		ch = fgetc(fp);
		if(is_alphabet(ch)) // 是字母 
		{
			tmp_word[index++] = ch;
		}
		else // 不是字母 
		{
			if(ch == EOF) // 读文件直到遇到文件结束符EOF 
				break;
			if(tmp_word[0] == '\0') // 过滤 
				continue;
				
			// 统计
			++_total; 
				
			 // 新建节点 
			new_word = malloc(sizeof(_node_word));
			if(!new_word)
				return;
			strcpy(new_word->data, tmp_word);
			new_word->location = ++location;
			new_word->next = NULL;

			// 加入hash表 
			tmp = _hash_table[hash(tmp_word)];
			while(tmp)
			{
				if(tmp->next == NULL)
					break;
				tmp = tmp->next;
			}
			tmp->next = new_word; // 将节点加入  

			// 初始化tmp_word 
			while(index >= 0)
				tmp_word[index--] = '\0';
			index = 0;
		}
	}
	fclose(fp);
}

/**
* @brief 从hash表搜索单词 
* 
* @param[in] word 检索关键字 
* @return 返回检索结果_result 
*/
_result search(const char *word)
{
	_result result; 
	_node_word *p = _hash_table[hash(word)];
	char tmp[10]; 
	
	result.count = 0;
	strcpy(result.array_index_str, "");
	if(!is_word(word))
		return result;
	while(p)
	{
		if(!strcmp(word, p->data))
		{
			sprintf(tmp, "%u ", p->location);
			strcat(result.array_index_str, tmp);
			++result.count; 
		}
		p = p->next;
	}
	result.total = _total;
	if(result.count > 0)
	{
		strcpy(result.word.data, word);
	}
	return result;
}

void print_result(_result data) 
{
	if(data.count == 0)
		printf("无\n\n");
	else printf("%s出现的位置有：%s\n%s出现次数：%u\n全文单词总数：%u\n\n", \
		data.word, data.array_index_str, data.word, data.count, data.total); 
}

void main()
{
	const char file_path[] = "data.txt";
	char key_word[46];

	fill_hash_table_from_file(file_path); // 填充数据 
	
	printf("> ");
	while(scanf("%s", key_word) != EOF)
	{
		print_result(search(key_word));
		key_word[0] = '\0';
		printf("> ");
	} 
}
