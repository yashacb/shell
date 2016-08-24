#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char str[40] = "This is a sentence in here " ;
    char* p = strtok(str , " ") ;
    while(p != NULL)
    {
    	printf("%sa\n", p);
    	p = strtok(NULL , " ") ;
    }
}