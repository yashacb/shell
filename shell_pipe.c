#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>	
#include<time.h>
#include<sys/utsname.h>
#include<utmp.h>
#include<sys/wait.h>

#include "headers.h"

#include "utils.c"
#include "limits.h"
#include "parse.c"
#include "comms.c"

int main()
{
	int i ;
	char com[MAX_COMMAND_SIZE] ;
	QNode* history_head = (QNode*) malloc(sizeof(QNode)) ;
	QNode* history_end = history_head ; //dummy head
	int shm_id = shmget(IPC_PRIVATE , MAX_SHARED_MEMORY * sizeof(char) , IPC_CREAT | 0666) ;
	char* sh_mem = (char *) shmat(shm_id , NULL , 0) ;
	sh_mem[0] = '\0' ;
	while(1)
	{
		printf("bash> ");
		scanf("%[^\n]" , com) ;
		getchar() ;
		int l = strlen(com) ;
		char* ref = (char*) malloc(l*sizeof(char)) ;
		//retrieve command from history if needed .
		if(com[0] == '!')
		{
			if(com[1] == '\0')
				continue ;
			else if(com[1] == '!')
			{
				strcpy(com , history_end -> string) ;
			}
			else
			{
				int n = atoi(com + 1) ;
				if(n <= MAX_HISTORY_SIZE)
				{
					QNode* req = getQ(history_head , history_end , n) ;
					strcpy(com , req -> string) ;
				}
				else
					continue ;
			}
		}
		//check for exit
		if(strcmp(com , "exit") == 0)
			break ;
		//recording history
		strcpy(ref , com) ;
		if(size(history_head , history_end) < MAX_HISTORY_SIZE)
			history_end = enqueue(history_head , history_end , ref) ;
		else
		{
			history_end = deque(history_head , history_end) ;
			history_end = enqueue(history_head , history_end , ref) ;
		}
		int p;
		int pid = fork() ;
		{
			if(pid == 0)
			{
				sh_mem[0] = '\0' ;
				int n = 0 ;
				char hold[MAX_SHARED_MEMORY] ;
				command** commands = parse_total(com , &n) ;
				for(i = 0 ; i < n ; i++)
				{
					if(sh_mem[0] != '\0')
					{
						strcpy(hold , sh_mem) ;
						commands[i] -> data = hold ;
					}
					execute(commands[i] , sh_mem , history_head , history_end) ;
				}
				free_commands(commands , n) ;
				return 0 ;
			}
			else
			{
				waitpid(pid,&p,0) ;
				if(sh_mem[0] != '\0')
					printf("%s\n", sh_mem);
			}
		}
	}
	deleteQueue(history_head , history_end) ;
	shmdt((void*)sh_mem) ;
	return 0;
}
