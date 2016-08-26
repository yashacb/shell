typedef struct command
{
	char* command ; // command name
	char* arg ; // argument for the command , mostly a file or directory
	char *ip_redirect ; // input redirect for the commnd . Technically useless :p
	char* op_redirect ; // output is redirected to this file
	char* data ; // the actual data on which the command works
} command ;


typedef struct qnode
{
	char* string ;
	struct qnode *next ;
} QNode ;

int lastIndexOf(char *str , char c) ;
int firstIndexOf(char* str , char c) ;
int countOccurences(char *str , char c) ;
void purify(char *string) ;
void printArr(char** strings , int n)  ;
char** split(char* string , char* delim) ;

int execute(command* comm , char* sh_mem , QNode* history_head , QNode* history_end) ;
int list(command* com , char* sh_mem) ;
int disp(command* com , char* sh_mem) ;
int history(command* com , char* sh_mem , QNode* history_head , QNode* history_end) ;
int sort(command* com , char* sh_mem) ;
int comp_func(const void* s1 , const void* s2) ;
int grep(command* com , char* sh_mem) ;
