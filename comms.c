#include <stdio.h>
#include <stdlib.h>

//add function declarations to the header file also . 

int execute(command* comm , char* sh_mem)
{
	if(strcmp(comm -> command , "ls") == 0)
		list(comm , sh_mem) ;
	else if(strcmp(comm -> command , "disp") == 0)
		disp(comm , sh_mem) ;
	return 1 ;
}

void list(command* com , char* sh_mem)
{
	char* directory ;
	if(com -> ip_redirect != NULL)
	{
		directory = (char*) malloc(MAX_DIREC_SIZE*sizeof(char)) ;
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
		getcwd(directory , MAX_DIREC_SIZE) ;
	}
	char sep = '\t' ;
	printf("%s\n", directory);
	DIR *dir = opendir(directory) ;
	struct dirent *entry = readdir(dir) ;
	char *ptr = sh_mem ;
	while(entry != NULL)
	{
		strcpy(ptr , entry -> d_name) ;
		if(*ptr != '.')
		{
			ptr = ptr + strlen(entry -> d_name) ;
			*ptr = sep ;
			ptr++ ;
			if(sep == '\t')
				sep = '\n' ;
			else
				sep = '\t' ;
		}
		entry = readdir(dir) ;
	}
	*(--ptr) = '\0' ; //Remove '\n' from last line .
	closedir(dir) ;
}

void disp(command* com , char* sh_mem)
{
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
	fseek(fp , 0 , SEEK_END) ;
	rewind(fp) ;
	int read = fread((void*)sh_mem , 1 , MAX_SHARED_MEMORY - 1 , fp) ;
	sh_mem[read] = '\0' ;
	fclose(fp) ;
}

