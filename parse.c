// #include "limits.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

command* init(command* comm)
{
	comm -> command = NULL ;
	comm -> arg = NULL ;
	comm -> ip_redirect = NULL ;
	comm -> op_redirect = NULL ;
	comm -> data = NULL ;
}

command* parse(char* com)
{
	purify(com) ;
	int len = strlen(com) ;
	char* com_p = strchr(com , ' ') ;
	command* parsed = (command*) malloc(sizeof(command)) ;
	init(parsed) ;
	int pos = lastIndexOf(com , '>') ;
	if(pos != -1)
	{
		com[pos - 1] = '\0' ;
		parsed -> op_redirect = com + pos + 2 ;
	}
	pos = lastIndexOf(com , '<') ;
	if(pos != -1)
	{
		parsed -> ip_redirect = com + pos + 2 ;
	}
	else
	{
		pos = firstIndexOf(com , ' ') ;
		if(pos != -1)
		{
			parsed -> arg = com + pos + 1 ;
			com[pos] = '\0' ;
		}
	}
	parsed -> command = com ;
	return parsed ;
}

void print_info(command* com)
{
	if(com -> command)
		printf("Command : %s\n", com -> command);
	if(com -> arg)
		printf("Arg : %s\n", com -> arg);
	if(com -> ip_redirect)
		printf("Input Redirect : %s\n", com -> ip_redirect);
	if(com -> op_redirect)
		printf("Output Redirect : %s\n", com -> op_redirect);
}

void print_infos(command** comms , int n)
{
	while(n-- != 0)
	{
		print_info(*comms) ;
		comms++ ;
	}
}

command** parse_total(char* string , int* n)
{
	purify(string) ;
	int oss = countOccurences(string , '|') ;
	command** commands = (command**) malloc((oss + 1) * sizeof(*commands)) ;
	char* p = strtok(string , "|")  ;
	int i = 0 ;
	while(p != NULL) 
	{
		commands[i++] = parse(p) ;
		p = strtok(NULL , "|") ;
	}
	*n = i ;
	return (command**) commands ;
}

void free_commands(command** comms , int n)
{
	int i = 0 ;
	for(; i < n ; i++)
	{
		free(comms[i]) ;
	}
	free(comms) ;
}