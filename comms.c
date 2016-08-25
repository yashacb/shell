#include <stdio.h>
#include <stdlib.h>

//This file contains actual implementations of all the commands .

//add function declarations to the header file also . 

int execute(command* comm , char* sh_mem , QNode* history_head , QNode* history_end)
{
	if(strcmp(comm -> command , "ls") == 0)
		list(comm , sh_mem) ;
	else if(strcmp(comm -> command , "disp") == 0)
		disp(comm , sh_mem) ;
	else if(strcmp(comm -> command , "history") == 0)
		history(comm , sh_mem , history_head , history_end) ;
	else if(strcmp(comm -> command , "sort") == 0)
		sort(comm , sh_mem) ;
	else if(strcmp(comm -> command , "grep") == 0)
		grep(comm , sh_mem) ;
	return 1 ;
}

void list(command* com , char* sh_mem)
{
	char* directory ;
	int to_free = 0 ;
	if(com -> ip_redirect != NULL)
	{
		directory = (char*) malloc(MAX_DIREC_SIZE*sizeof(char)) ;
		to_free = 1 ;
		FILE* fp = fopen(com -> ip_redirect , "r") ;
		directory = fgets(directory , MAX_DIREC_SIZE , fp) ;
		fclose(fp) ;
	}
	else if(com -> arg != NULL)
	{
		directory = com -> arg ;
	}
	else
	{
		directory = (char*) malloc(MAX_DIREC_SIZE*sizeof(char)) ;
		to_free = 1 ;
		getcwd(directory , MAX_DIREC_SIZE) ;
	}
	DIR *dir = opendir(directory) ;
	struct dirent *entry = readdir(dir) ;
	char *ptr = sh_mem ;
	while(entry != NULL)
	{
		strcpy(ptr , entry -> d_name) ;
		if(*ptr != '.')
		{
			ptr = ptr + strlen(entry -> d_name) ;
			*ptr = '\n' ;
			ptr++ ;
		}
		entry = readdir(dir) ;
	}
	*(--ptr) = '\0' ;
	closedir(dir) ;
	if(to_free == 1)
		free(directory) ;
}

void disp(command* com , char* sh_mem)
{
	if(com -> data != NULL)
	{
		strcpy(sh_mem , com -> data) ;
		return ;
	}
	char* file = NULL ;
	if(com -> ip_redirect != NULL)
		file = com -> ip_redirect ;
	else if(com -> arg != NULL)
		file = com -> arg ;
	else
	{
		strcpy(sh_mem , "No file given") ;
		return ;
	}
	FILE* fp = fopen(file , "rb") ;
	int read , i = 0 ;
	while((read = fgetc(fp)) != EOF)
		sh_mem[i++] = read ;
	if(read != '\n')
		sh_mem[i++] = '\n' ;
	i-- ;
	while(i >= 0 && sh_mem[i] == '\n')
		i-- ;
	sh_mem[i + 1] = '\0' ;
	fclose(fp) ;
}

void history(command* com , char* sh_mem , QNode* history_head , QNode* history_end)
{
	int i = 1 ;
	QNode* current = history_head -> next ;
	while(current != NULL)
	{
		sprintf(sh_mem , "%d %s\n" , i , current -> string) ;
		i++ ;
		sh_mem = sh_mem + strlen(sh_mem) ;
		current = current -> next ;
	}
	sh_mem[strlen(sh_mem) - 1] = '\0' ;
}

void sort(command* com , char* sh_mem)
{
	char* cc = sh_mem ;
	int i = 0 ;
	char* file = NULL ;
	char contents[MAX_SHARED_MEMORY] ;
	if(com -> data == NULL)
	{
		if(com -> ip_redirect != NULL)
			file = com -> ip_redirect ;
		else if(com -> arg != NULL)
			file = com -> arg ;
		else
		{
			strcpy(sh_mem , "No file given") ;
			return ;
		}
		FILE* fp = fopen(file , "r") ;
		fseek(fp , 0 , SEEK_END) ;
		rewind(fp) ;
		int read  ;
		while((read = fgetc(fp)) != EOF)
			contents[i++] = read ;
		i-- ;
		while(i >= 0 && contents[i] == '\n')
			i-- ;
		contents[i+1] = '\0' ;
		fclose(fp) ;
	}
	else
	{
		strcpy(contents , com -> data) ;
	}
	int len = strlen(contents) ;
	int lines = countOccurences(contents , '\n') + 1 ;
	char* ps[lines] ;
	char* l = strtok(contents , "\n") ;
	i = 0 ;
	while(l != NULL)
	{
		ps[i++] = l ;
		l = strtok(NULL , "\n") ;
	}
	qsort(ps , i , sizeof(char*) , comp_func) ;
	for(i = 0 ; i < lines ; i++)
	{
		if(i != lines - 1)
			sprintf(sh_mem , "%s\n" , ps[i]) ;	
		else
			sprintf(sh_mem , "%s" , ps[i]) ;	
		sh_mem = sh_mem + strlen(sh_mem) ;
	}
}

int comp_func(const void* s1 , const void* s2) // comparison functions for qsort
{
	// printf("%s %s\n", *((char**)s1) , *((char**)s2));
	return strcmp(*((char**)s1) , *((char**)s2)) ;
}

void grep(command* com , char* sh_mem)
{	
	char* cc = sh_mem ;
	int i = 0 ;
	char contents[MAX_SHARED_MEMORY] ;
	char* search ;
	if(com -> data == NULL)
	{
		char* space = strchr(com -> arg , ' ') ;
		if(space == NULL)
		{
			sprintf(sh_mem , "Incorrect usage") ;
			return ;
		}
		*space = '\0' ;
		search = space + 1 ;
		char* file = com -> arg ;
		FILE* fp = fopen(file , "r") ;
		fseek(fp , 0 , SEEK_END) ;
		rewind(fp) ;
		int read = fread((void*)contents , 1 , MAX_SHARED_MEMORY - 2 , fp) ;
		contents[read] = '\0' ;
		fclose(fp) ;
	}
	else
	{
		search = com -> arg ;
		strcpy(contents , com -> data) ;
	}
	int len = strlen(contents) ;
	if(contents[len - 1] != '\n') ;
	{
		contents[len] =  '\n' ;
		contents[len + 1] = '\0' ;
	}
	int lines = countOccurences(contents , '\n') ;
	char* ps[lines] ;
	char* l = strtok(contents , "\n") ;
	while(l != NULL)
	{
		ps[i++] = l ;
		l = strtok(NULL , "\n") ;
	}
	for( i = 0 ; i < lines ; i++)
	{
		if(strstr(ps[i] , search) != NULL)
		{
			if(sh_mem - cc == 0)
				sprintf(sh_mem , "%s" , ps[i]) ;	
			else
				sprintf(sh_mem , "\n%s" , ps[i]) ;	
			sh_mem = sh_mem + strlen(sh_mem) ;
			if(sh_mem - cc > MAX_SHARED_MEMORY)
				break ;
		}
	}
}

