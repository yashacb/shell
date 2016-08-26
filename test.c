#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

int main()
{
	printf("%s\n", getenv("http_proxy"));
}