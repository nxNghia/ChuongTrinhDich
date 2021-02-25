#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


struct array
{
	int value;
	struct array* next;
};

struct word
{
	char* name;
	struct array* e;
	int count;
	struct word* next;
};

void push(struct array** root, int new_value)
{
	if(*root == NULL)
	{
		*root = (struct array*)calloc(1, sizeof(struct array));
		(*root) -> next = NULL;
		(*root) -> value = new_value;
	}else{
		if((*root) -> value != new_value)
		{
			struct array* new = (struct array*)calloc(1, sizeof(struct array));
			new -> value = new_value;
			new -> next = *root;
			*root = new;
		}
	}
}

int cmp(char* s1, char* s2)
{
	int length = strlen(s1) > strlen(s2) ? strlen(s2) : strlen(s1);
	for (int i = 0; i < length; ++i)
	{
		if(s1[i] > s2[i])
		{
			return 1;
		}else{
			if(s1[i] < s2[i])
			{
				return -1;
			}
		}
	}

	if(strlen(s1) == strlen(s2))
		return 0;
	else
		if(strlen(s1) > strlen(s2))
			return 1;
		else
			return -1;
}

void Insert(struct word** lw, char* _a, int _e)
{
	if(*lw == NULL)
	{
		*lw = (struct word*)malloc(sizeof(struct word));
		(*lw) -> name = (char*)malloc(sizeof(_a));
		strcpy((*lw) -> name, _a);
		(*lw) -> count = 1;
		(*lw) -> e = NULL;
		push(&((*lw) -> e), _e);
		(*lw) -> next = NULL;
	}else{
		struct word* cur = *lw;
		struct word* pre = NULL;
		struct word* new = (struct word*)malloc(sizeof(struct word));
		new -> name = (char*)malloc(sizeof(_a));
		strcpy(new -> name, _a);
		new -> count = 1;
		new -> e = NULL;
		push(&(new -> e), _e);
		new -> next = NULL;

		while(cur != NULL)
		{
			if(cmp(cur -> name, _a) == 1)
			{
				if(pre == NULL)
				{
					new -> next = *lw;
					*lw = new;
				}else{
					pre -> next = new;
					new -> next = cur;
				}
				break;
			}else{
				if(cmp(_a, cur -> name) == 0)
				{
					++(cur -> count);
					push(&(cur -> e), _e);
					break;
				}
			}
			pre = cur;
			cur = cur -> next;

			if(cur == NULL)
			{
				pre -> next = new;
			}
		}
	}
}

void Print2(struct array* a)
{
	while(a != NULL)
	{
		printf("%-5d", a -> value);
		a = a -> next;
	}
	printf("\n");
}

void Print(struct word* l)
{
	while(l != NULL)
	{
		printf("%s: %d ", l -> name, l -> count);
		Print2(l -> e);	
		l = l -> next;
	}
	printf("\n");
}

int ExistIn(char* s, struct word* list_word)
{
	while(list_word != NULL)
	{
		if(strcmp(s, list_word -> name) == 0)
		{
			return 1;
		}
		list_word = list_word -> next;
	}
	return 0;
}

int main(int argc, char const *argv[])
{
	FILE* f = fopen(argv[1], "r");
	FILE* f2 = fopen(argv[2], "r");

	char* s1 = calloc(100, sizeof(char));
	char c = fgetc(f);
	int i = 0;
	int stop = 0;
	int space = 0;
	int row = 1;
	int breakLine = 0;

	struct word* stopWord = NULL;
	struct word* normalWord = NULL;
	s1[0] = '\0';
	while(c != EOF)
	{
		c = tolower(c);
		if(c >= 'a' && c <= 'z')
		{
			stop = 1;
			s1[i] = c;
			++i;
		}else{
			if(stop == 1)
			{
				s1[i] = '\0';
				Insert(&stopWord, s1, row);
				stop = 0;
				i = 0;
			}
			if(c == '\n')
				++row;
		}
		c = fgetc(f);
	}
	fclose(f);

	row = 1;
	space = 0;
	stop = 0;
	i = 0;
	breakLine = 0;
	c = fgetc(f2);
	s1[0] = '\0';
	while(c != EOF)
	{
		c = tolower(c);
		if(c >= 'a' && c <= 'z' || c == 39)
		{
			stop = 1;
			s1[i] = c;
			++i;
		}else{
			if(stop == 1)
			{
				s1[i] = '\0';
				if(ExistIn(s1, stopWord) == 0)
					Insert(&normalWord, s1, row);
				stop = 0;
				i = 0;
			}
			if(c == '\n')
				++row;
		}
		c = fgetc(f2);
	}
	fclose(f2);

	// printf("done\n");

	// printf("%s\n", normalWord -> next -> name);

	Print(normalWord);
}