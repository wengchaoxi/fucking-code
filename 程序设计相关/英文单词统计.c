#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

/**
* @brief ����Ľڵ�,�洢һ������ 
*/
struct node
{
	char data[46]; // �ݲ鵥�����45����ĸ 
	unsigned int location;
	struct node* next;
};
typedef struct node _node_word;

/**
* @brief һ�������𰸵Ľṹ�� 
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
* @brief �ж��ַ��Ƿ�Ϊ��ĸ 
* 
* @param[in] ch һ���ַ�
* @return 1:��, 0:�� 
*/
unsigned char is_alphabet(const char ch)
{
	if(ch>='A' && ch<='Z' || ch>='a' && ch<='z')
		return 1;
	else return 0;
}

/**
* @brief �ж��Ƿ�Ϊ���� 
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
* @brief ����hashֵ 
* 
* @param[in] �����ַ��� 
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
* @brief ��ʼ����ϣ�� 
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
* @brief ���ļ���ȡ��������������ϣ��  
* 
* @param[in] file_path �ļ�·�� 
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
		if(is_alphabet(ch)) // ����ĸ 
		{
			tmp_word[index++] = ch;
		}
		else // ������ĸ 
		{
			if(ch == EOF) // ���ļ�ֱ�������ļ�������EOF 
				break;
			if(tmp_word[0] == '\0') // ���� 
				continue;
				
			// ͳ��
			++_total; 
				
			 // �½��ڵ� 
			new_word = malloc(sizeof(_node_word));
			if(!new_word)
				return;
			strcpy(new_word->data, tmp_word);
			new_word->location = ++location;
			new_word->next = NULL;

			// ����hash�� 
			tmp = _hash_table[hash(tmp_word)];
			while(tmp)
			{
				if(tmp->next == NULL)
					break;
				tmp = tmp->next;
			}
			tmp->next = new_word; // ���ڵ����  

			// ��ʼ��tmp_word 
			while(index >= 0)
				tmp_word[index--] = '\0';
			index = 0;
		}
	}
	fclose(fp);
}

/**
* @brief ��hash���������� 
* 
* @param[in] word �����ؼ��� 
* @return ���ؼ������_result 
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
		printf("��\n\n");
	else printf("%s���ֵ�λ���У�%s\n%s���ִ�����%u\nȫ�ĵ���������%u\n\n", \
		data.word, data.array_index_str, data.word, data.count, data.total); 
}

void main()
{
	const char file_path[] = "data.txt";
	char key_word[46];

	fill_hash_table_from_file(file_path); // ������� 
	
	printf("> ");
	while(scanf("%s", key_word) != EOF)
	{
		print_result(search(key_word));
		key_word[0] = '\0';
		printf("> ");
	} 
}
