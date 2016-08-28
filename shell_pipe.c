#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/stat.h>

#include <time.h>
#include <sys/utsname.h>
#include <utmp.h>
#include <sys/wait.h>


#include <dirent.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "headers.h"

#include "utils.c"
#include "limits.h"
#include "parse.c"
#include "comms.c"
char* my_generator(const char*,int);
char * dupstr (char*);
void *xmalloc (int);

char* cmd [] ={ "ls", "exit", "disp" ,"history", "sort", "grep", 
				"wget" , "pwd" , "cpy" , "cwd" , "mkdir"};


int main()
{
	int n = MAX_SHARED_MEMORY ;
	if(n < 1024)
	{
		printf("Minimum size of shared memory should be 1024\n");
		exit(0) ;
	}
	int i ;
	char *com ;
	rl_attempted_completion_function = my_completion;

	QNode* history_head = (QNode*) malloc(sizeof(QNode)) ;
	history_head -> next = NULL ;
	history_head -> string = "NULL" ;
	QNode* history_end = history_head ; //dummy head
	int shm_id = shmget(IPC_PRIVATE , MAX_SHARED_MEMORY * sizeof(char) , IPC_CREAT | 0666) ;
	char* sh_mem = (char *) shmat(shm_id , NULL , 0) ;
	sh_mem[0] = '\0' ;

	while(1)
	{
		if((com = readline("bash> ")) != NULL)
		{
				rl_bind_key('\t',rl_complete);
				if (com[0] != 0)
            		add_history(com);
		}

		purify(com) ;
		if(strncmp(com , "cwd " , 4) == 0)
		{
			//this should be done in the current process only
			chdir(com + 4) ;
			continue ;
		}
		int l = strlen(com) ;
		char* ref = (char*) malloc((l + 1)*sizeof(char)) ;
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
				if(n <= size(history_head , history_end) && n > 0)
				{
					QNode* req = getQ(history_head , history_end , n) ;
					strcpy(com , req -> string) ;
				}
				else
				{
					printf("Unknown command .\n");
					continue ;
				}
			}
		}
		//check for exit
		if(strcmp(com , "exit") == 0)
		{
			free(ref) ;
			break ;
		}
		//recording history
		strcpy(ref , com) ;
		if(size(history_head , history_end) < MAX_HISTORY_SIZE)
			history_end = enqueue(history_head , history_end , ref) ;
		else
		{
			history_end = deque(history_head , history_end) ;
			history_end = enqueue(history_head , history_end , ref) ;
		}
		//create a new child process
		int pid = fork() ;
		{
			if(pid == 0)
			{
				//things to do int child process
				sh_mem[0] = '\0' ; //reset shared memory
				int n = 0 ;
				char hold[MAX_SHARED_MEMORY] ;
				command** commands = parse_total(com , &n) ; //parse the command
				for(i = 0 ; i < n ; i++) //execute the chain of commands
				{
					if(sh_mem[0] != '\0')
					{
						strcpy(hold , sh_mem) ;
						commands[i] -> data = hold ;						
					}
					int ret = execute(commands[i] , sh_mem , history_head , history_end) ;
					if(!ret)
						break ; // stop if atleast one is unsuccessful
				}
				deleteQueue(history_head , history_end) ; //release the queue memory of the child process . Detected this memory leak with valgrind
				free_commands(commands , n) ; // free memory allocated to commands
				return 0 ;
			}
			else
			{
				int status ;
				wait(&status) ; //wait for child process to end
				if(sh_mem[0] != '\0')
					printf("%s\n", sh_mem); //print the output of the command if any
			}
		}
	}
	deleteQueue(history_head , history_end) ; //release memory at the end .
	shmdt((void*)sh_mem) ; //detach the shared memory
	return 0;
}


static char** my_completion( const char * text , int start,  int end)
{
    char **matches;

    matches = (char **)NULL;

    if (start == 0)
        matches = rl_completion_matches ((char*)text, &my_generator);
    else
        rl_bind_key('\t',rl_abort);

    return (matches);

}

char* my_generator(const char* text, int state)
{
    static int list_index, len;
    char *name;

    if (!state) {
        list_index = 0;
        len = strlen (text);
    }

    while (name = cmd[list_index]) {
        list_index++;

        if (strncmp (name, text, len) == 0)
            return (dupstr(name));
    }

    /* If no names matched, then return NULL. */
    return ((char *)NULL);

}

char * dupstr (char* s) {
  char *r;

  r = (char*) xmalloc ((strlen (s) + 1));
  strcpy (r, s);
  return (r);
}

void * xmalloc (int size)
{
    void *buf;

    buf = malloc (size);
    if (!buf) {
        fprintf (stderr, "Error: Out of memory. Exiting.'n");
        exit (1);
    }

    return buf;
}
