#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//add function declarations to the header file also . 
//String functions

int lastIndexOf(char *str , char c)
{
	int len = strlen(str) ;
	int i = len - 1 ;
	int pos = -1 ;
	while(i >= 0)
	{
		if(str[i] == c)
		{
			pos = i ;
			break ;
		}
		i-- ;
	}
	return pos ;
}

int firstIndexOf(char* str , char c)
{
	char* p = strchr(str , c) ;
	if(p != NULL)
		return p - str ;
	return -1 ;
}

int countOccurences(char *str , char c)
{
	int re = 0 ;
	while(*str != '\0')
	{
		if(*str++ == c)
			re++ ;
	}
	return re ;
}

void purify(char *string)
{
	int i_pos = 0 , i = 0;
	while(string[i] == ' ')
		i++ ;
	while(string[i] != '\0')
	{
		if(string[i] != ' ')
			string[i_pos++] = string[i++] ;
		else
		{
			while(string[i] == ' ' && string[i+1] == ' ')
				i++ ;
			string[i_pos++] = string[i++] ;
		}
	}
	if(string[i_pos - 1] == ' ')
		string[i_pos - 1] = '\0' ;
	else
		string[i_pos] = '\0' ;
}

void printArr(char** strings , int n) 
{
	int i = 0 ;
	for( ; i < n ; i++)
		printf("%s\n", strings[i]);
}

//Queue implementation

QNode* enqueue(QNode* start , QNode* end , char* string)
{
	QNode* new = (QNode*) malloc(sizeof(QNode)) ;
	new -> string = string ;
	new -> next = NULL ;
	end -> next = new ;
	end = end -> next ;
	return end ;
}

QNode* deque(QNode* head , QNode* end)
{
	if(head -> next != NULL)
	{
		if(head -> next == end)
		{
			free(end) ;
			end = head ;
		}
		else
		{
			QNode* to_del = head -> next ;
			head -> next = head -> next -> next ;
			free(to_del -> string) ;
			free(to_del) ;
		}
	}
	return end ;
}

int size(QNode* head , QNode* end)
{
	if(head != NULL && head -> next == NULL)
		return 0 ;
	else
	{
		QNode* current = head -> next ;
		int count = 0 ;
		while(current != NULL)
		{
			count++ ;
			current = current -> next ;
		}
		return count ;
	}
}

void printQ(QNode* head , QNode* end)
{
	QNode* current = head -> next ;
	while(current != NULL)
	{
		printf("%s\n" , current -> string) ;
		current = current -> next ;
	}
}

QNode* getQ(QNode* head , QNode* end , int n)
{
	int si = size(head , end) ;
	if(si < n)
		return NULL ;
	else
	{
		QNode* current = head ;
		while(n-- != 0)
			current = current -> next ;
		return current ;
	}
}

void deleteQueue(QNode* head , QNode*end)
{
	QNode* current = head ;
	while(head -> next != NULL)
	{
		current = head -> next ;
		head -> next = current -> next ;
		free(current -> string) ;
		free((void*)current) ;
	}
	free((void*)head) ;
}
