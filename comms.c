#include <stdio.h>
#include <stdlib.h>

//This file contains actual implementations of all the commands .

//add function declarations to the header file also . 

int execute(command* comm , char* sh_mem , QNode* history_head , QNode* history_end)
{
	//check for appropriate command to execute
	if(strcmp(comm -> command , "ls") == 0)
		return list(comm , sh_mem) ;
	else if(strcmp(comm -> command , "disp") == 0)
		return disp(comm , sh_mem) ;
	else if(strcmp(comm -> command , "history") == 0)
		return history(comm , sh_mem , history_head , history_end) ;
	else if(strcmp(comm -> command , "sort") == 0)
		return sort(comm , sh_mem) ;
	else if(strcmp(comm -> command , "grep") == 0)
		return grep(comm , sh_mem) ;
	else if(strcmp(comm -> command, "who") == 0)
		return who(comm , sh_mem);
	else if(strcmp(comm -> command , "wget") == 0)
		return wget(comm , sh_mem) ;
	else if(strcmp(comm->command , "cpy") == 0)
		return cpy(comm, sh_mem) ;	
	else if(strcmp(comm->command , "pwd") == 0)
		return pwd(comm, sh_mem);
	else if(strcmp(comm->command , "mkdir") == 0)
		return makedir(comm, sh_mem);
	else
	{
		strcpy(sh_mem , "Unknkown command .") ;
		return 0 ;
	}
	return 1 ;
}

int list(command* com , char* sh_mem)
{
	char* cc = sh_mem ;
	char* directory ;
	int to_free = 0 ;
	if(com -> arg != NULL) //if the argument is nor null , take the input directory as argument
	{
		directory = com -> arg ;
	}
	else //else take the input directory as current directory
	{
		directory = (char*) malloc(MAX_DIREC_SIZE*sizeof(char)) ;
		to_free = 1 ;
		getcwd(directory , MAX_DIREC_SIZE) ;
	}
	DIR *dir = opendir(directory) ;
	if(dir == NULL) //unable to open directory stream => directory doesnot exist . return .
	{
		sprintf(sh_mem , "%s folder does not exist" , directory) ;
		return 0 ;
	}
	struct dirent *entry = readdir(dir) ;
	char *ptr = sh_mem ;
	while(entry != NULL)//iterate through all elements in the directory stream and print to the shared memory .
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
	*(--ptr) = '\0' ; //add null character to end of shared memory
	closedir(dir) ; //close directory stream .
	if(to_free == 1)
		free(directory) ;
	if(com -> op_redirect != NULL) //if there is an ouput redirect , write contents of shared memory to the output file .
	{
		FILE* fp = fopen(com -> op_redirect , "w") ;
		fwrite(cc , 1 , strlen(cc) , fp) ;
		fclose(fp) ;
		cc[0] = '\0' ; //reset the shared nenory , as output id redirected .
	}
	return 1 ;
}

int disp(command* com , char* sh_mem)
{
	char* cc = sh_mem ;
	if(com -> data != NULL) //if there is data coming from previous command , consider it to be the data to be displayed and ignore the argument
	{
		strcpy(sh_mem , com -> data) ;
		return 1;
	}
	char* file = NULL ;
	if(com -> ip_redirect != NULL)
		file = com -> ip_redirect ;
	else if(com -> arg != NULL)
		file = com -> arg ;
	else
	{
		strcpy(sh_mem , "No file given") ;
		return 0;
	}
	FILE* fp = fopen(file , "rb") ;
	if(fp == NULL)
	{
		sprintf(sh_mem , "%s file does not exist" , file) ;
		return 0 ;
	}
	int read , i = 0 ;
	while(i < MAX_SHARED_MEMORY - 1 && (read = fgetc(fp)) != EOF) //copy data contents into the shared memory .
		sh_mem[i++] = read ;
	if(read != '\n') //remove trailing newlines
		sh_mem[i++] = '\n' ;
	i-- ;
	while(i >= 0 && sh_mem[i] == '\n')
		i-- ;
	sh_mem[i + 1] = '\0' ;
	fclose(fp) ;
	if(com -> op_redirect != NULL)//if there is an ouput redirect , write contents of shared memory to the output file .
	{
		FILE* fp = fopen(com -> op_redirect , "w") ;
		fwrite(cc , 1 , strlen(cc) , fp) ;
		fclose(fp) ;
		cc[0] = '\0' ; //reset the shared nenory , as output id redirected .
	}
	return 1 ;
}

int history(command* com , char* sh_mem , QNode* history_head , QNode* history_end)
{
	char* cc = sh_mem ;
	int i = 1 ;
	QNode* current = history_head -> next ;
	while(current != NULL) //iterate over the queue and print the string in the queue node .
	{
		sprintf(sh_mem , "%d %s\n" , i , current -> string) ;
		i++ ;
		sh_mem = sh_mem + strlen(sh_mem) ;
		current = current -> next ;
	}
	sh_mem[strlen(sh_mem) - 1] = '\0' ;
	if(com -> op_redirect != NULL)//if there is an ouput redirect , write contents of shared memory to the output file .
	{
		FILE* fp = fopen(com -> op_redirect , "w") ;
		fwrite(cc , 1 , strlen(cc) , fp) ;
		fclose(fp) ;
		cc[0] = '\0' ;
	}
	return 1 ;
}

int sort(command* com , char* sh_mem)
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
			return 0;
		}
		FILE* fp = fopen(file , "r") ;
		if(fp == NULL)
		{
			sprintf(sh_mem , "%s does not exist" , file) ;
			return 0 ;
		}
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
	else//if there is data coming from previous command , consider it to be the data that is to be sorted and ignore the argument
	{
		strcpy(contents , com -> data) ;
	}
	purify(contents) ;
	int len = strlen(contents) ;
	int lines = countOccurences(contents , '\n') + 1 ;
	char* ps[lines] ;
	char* l = strtok(contents , "\n") ;
	i = 0 ;
	while(l != NULL) //retrieve lines in the data
	{
		ps[i++] = l ;
		l = strtok(NULL , "\n") ;
	}
	qsort(ps , i , sizeof(char*) , comp_func) ; //sort the lines .
	for(i = 0 ; i < lines ; i++)
	{
		if(i != lines - 1)
			sprintf(sh_mem , "%s\n" , ps[i]) ;	
		else
			sprintf(sh_mem , "%s" , ps[i]) ;	
		sh_mem = sh_mem + strlen(sh_mem) ;
		if(sh_mem - cc > MAX_SHARED_MEMORY)//do not write out of shared memory !
				break ;
	}
	if(com -> op_redirect != NULL)//if there is an ouput redirect , write contents of shared memory to the output file .
	{
		FILE* fp = fopen(com -> op_redirect , "w") ;
		fwrite(cc , 1 , strlen(cc) , fp) ;
		fclose(fp) ;
		cc[0] = '\0' ;
	}
	return 1 ;
}

int comp_func(const void* s1 , const void* s2) // comparison functions for qsort
{
	// printf("%s %s\n", *((char**)s1) , *((char**)s2));
	return strcmp(*((char**)s1) , *((char**)s2)) ;
}

int grep(command* com , char* sh_mem)
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
			return 0;
		}
		*space = '\0' ;
		search = space + 1 ;
		char* file = com -> arg ;
		FILE* fp = fopen(file , "r") ;
		if(fp == NULL)
		{
			sprintf(sh_mem , "%s does not exist" , file) ;
			return 0 ;
		}
		fseek(fp , 0 , SEEK_END) ;
		rewind(fp) ;
		int read = fread((void*)contents , 1 , MAX_SHARED_MEMORY - 2 , fp) ;
		contents[read] = '\0' ;
		fclose(fp) ;
	}
	else//if there is data coming from previous command , consider it to be the data that is to be searched and take entire argument as search string
	{
		search = com -> arg ;
		strcpy(contents , com -> data) ;
	}
	int len = strlen(contents) ;
	purify(contents) ;
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
		if(strstr(ps[i] , search) != NULL) //search for the search string in each line
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
	if(com -> op_redirect != NULL)//if there is an ouput redirect , write contents of shared memory to the output file .
	{
		FILE* fp = fopen(com -> op_redirect , "w") ;
		fwrite(cc , 1 , strlen(cc) , fp) ;
		fclose(fp) ;
		cc[0] = '\0' ;
	}
	return 1 ;
}

int who(command *com, char *sh_mem)
{
	struct utmp *user;
	char *a;
	int i;
	setutent();
	user = getutent();
	while(user)
	{
		if(user -> ut_type == 7)
		{
			sprintf(sh_mem, "%s", user -> ut_user);
		}
		user = getutent();
	}
	return 1;
}

int pwd(command *com , char* sh_mem) 
{
	getcwd(sh_mem , MAX_SHARED_MEMORY) ;
	return 1 ;
}

int wget(command* com , char* sh_mem)//did it directly using curl api
{
	if(com -> arg == NULL)
	{
		strcpy(sh_mem , "Incorrect usage") ;
		return 0 ;	
	}
	char* p = strchr(com -> arg , ' ') ;
	if(p == NULL)
	{
		strcpy(sh_mem , "Incorrect usage") ;
		return 0 ;	
	}
	*p = '\0' ;
	p++ ;
	CURL *curl;
    FILE *fp;
    CURLcode res;
    char *url = com -> arg;
    char* outfilename = p;
    curl = curl_easy_init();
    if (curl) 
    {
        fp = fopen(outfilename,"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl,  CURLOPT_ERRORBUFFER, sh_mem); 
        //only http proxy are available for now
        curl_easy_setopt(curl, CURLOPT_PROXY, getenv("http_proxy"));
        res = curl_easy_perform(curl);
        if(res == CURLE_OK)
        	sprintf(sh_mem , "Download complete . Saved to %s" , outfilename) ;
        else
        	strcpy(sh_mem , "Error encountered . Check proxy settings "
        		"and try again .") ;
        curl_easy_cleanup(curl);
        fclose(fp);
    }
    return 1 ;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) 
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int cpy(command *comm, char *sh_mem)
{
	int temp = countOccurences(comm -> arg , ' ');
	if(temp < 1)
	{
		sprintf(sh_mem , "Usage : %s <source> <destination>", comm -> command);
		return 0 ;
	}
	int pos = firstIndexOf(comm -> arg, ' ');
	comm -> arg[pos]='\0';
	char *src = comm -> arg;
	char *dest = comm -> arg + pos + 1;
	if( src[0] != '/' && dest[0] != '/')
		copyFiles(src, dest);
	else if(src[0] !='/' && dest[0] == '/' )
	{
		int i;
		for(i = 1 ; i <= strlen(dest) ; i++)
			dest[i-1] = dest[i];
		strcat(dest , "/");
		strcat(dest , src );
		copyFiles(src, dest);
	}
	else if(src[0] == '/' && dest[0] == '/' )
	{
		int i;
		for(i=1 ; i <= strlen(dest) ; i++)
			dest[i-1] = dest[i] ;
		for(i=1 ; i<= strlen(src) ; i++)
			src[i-1]=src[i];
		copyDir(src , dest );
	}
	else
	{
		sprintf(sh_mem, " Usage: cpy <source> <destination> \n");
		return 0 ;
	}sh_mem[0] = '\0' ;
	return 1;
}

int makedir(command* com , char* sh_mem)
{
	if(com -> arg == NULL)
	{
		sprintf(sh_mem , "No directory given") ;
		return 0 ;
	}
	int res = mkdir(com -> arg , 0700) ;
	if(res == -1)
	{
		sprintf(sh_mem , "Unable to create directory") ;
		return 0 ;
	}
	return 1 ;
}