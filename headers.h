typedef struct command
{
	char* command ;
	char* arg ;
	char *ip_redirect ;
	char* op_redirect ;
	char* data ;
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
void list(command* com , char* sh_mem) ;
void disp(command* com , char* sh_mem) ;
void history(command* com , char* sh_mem , QNode* history_head , QNode* history_end) ;
void sort(command* com , char* sh_mem) ;
int comp_func(const void* s1 , const void* s2) ;
void grep(command* com , char* sh_mem) ;
void cd(command *com,char *sh_mem);
void who(command *com,char *sh_mem);
