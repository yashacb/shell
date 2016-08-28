#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//add function declarations to the header file also . 
//String functions

#define BUFFERSIZE 1024
#define COPYMORE 0644

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
	if(str == NULL)
		return 0 ;
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
		if(strchr("\n \t" , string[i]) == NULL)
			string[i_pos++] = string[i++] ;
		else
		{
			if(strchr("\n \t" , string[i]) != NULL && 
				strchr("\n \t" , string[i + 1]) != NULL)
				i++ ;
			else
				string[i_pos++] = string[i++] ;
		}
	}
	if(strchr("\n \t" , string[i_pos - 1]) != NULL)
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

//Copy command utilities

int copyDir( char *source, char *destination)
{
	DIR *dir=NULL;
	struct dirent *direntptr;
	char *dupdest = (char *)malloc( (strlen(destination)+1)*sizeof(char));
	char *dupsrc = (char *)malloc( (strlen(source)+1)*sizeof(char));
	strcat(destination, "/");
	strcat(source, "/");
	strcpy(dupdest, destination);
	strcpy(dupsrc, source);
	struct stat fileinfo;
	if((dir = opendir(source)) == NULL)
	{
		fprintf(stderr, "Cannot open %s for copying\n", source);
		return 0;
	}
	else
	{
		while((direntptr = readdir(dir)))
		{
			if(dostat(direntptr->d_name))
			{
				strcat(dupdest, direntptr->d_name);
				strcat(dupsrc, direntptr->d_name);
				copyFiles(dupsrc, dupdest);
				strcpy(dupdest, destination);
				strcpy(dupsrc, source);
			}
		}
		closedir(dir);
		return 1;
	}
}

int dostat(char *filename)
{
	struct stat fileinfo;
	if(stat(filename, &fileinfo) >= 0)
		if(S_ISREG(fileinfo.st_mode))
			return 1;
	else return 0;
}

int copyFiles(char *source, char *destination)
{
	int in,out,n;
	char *buf = (char *)malloc(BUFFERSIZE*sizeof(char));
	if( (in = open(source, O_RDONLY)) == -1)
		printerror("cannot open ", source);
	if( (out = creat(destination, COPYMORE)) == -1)
		printerror("cannot create ", destination);
	while( (n = read(in,buf, BUFFERSIZE)) > 0)
	{
		if(write(out, buf, n) != n)
			printerror("Write error to ",destination);
		if(n == -1) 
			printerror("Read error from ", source);
	}
	if( close(in) == -1 || close(out) ==-1 )
		printerror("Error closing files ", "");
	return 1;
}

void printerror(char *a, char *b)
{
	fprintf(stderr, "Error :%s ", a);
	perror(b);
	exit(1);
}
