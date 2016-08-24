typedef struct command
{
	char* command ;
	char* arg ;
	char *ip_redirect ;
	char* op_redirect ;
} command ;

int lastIndexOf(char *str , char c) ;
int firstIndexOf(char* str , char c) ;
int countOccurences(char *str , char c) ;
void purify(char *string) ;
void printArr(char** strings , int n)  ;
char** split(char* string , char* delim) ;

int execute(command* comm , char* sh_mem) ;
void list(command* com , char* sh_mem) ;
void disp(command* com , char* sh_mem) ;
