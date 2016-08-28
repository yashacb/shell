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

static char** my_completion(const char*, int ,int);

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
int who(command *com,char *sh_mem) ;
int wget(command* com , char* sh_mem) ;
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)  ;
int cpy(command *com, char *sh_mem) ;
void printerror(char *, char *) ;
int copyDir(char *src, char *dest) ;
int copyFiles(char *src, char *dest) ;
int dostat(char *filename) ;
int cwd(command *com , char* sh_mem) ;
int makedir(command* com , char* sh_mem) ;
