#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define POST 1
#define GET 2
#define DEBUG 0


//Structure used to try and connect to a website
typedef struct website{
	char* name;
	char* regex_login;
	char* regex_token;
	char* form;
	int method;
	char* data;
	char* action;
	int connect_first;
} website;

//Used to debug and write HTTP answers in a FILE
struct memoryStruct {
  char *memory;
  size_t size;
};

//trying to login
int try_login(char* username, char* password, website* ws);
size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
void buffer_in_file(char* filename, char* buffer);