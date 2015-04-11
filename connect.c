#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <regex.h>
#include <curl/curl.h>

#include "connect.h"
#include "easy_string.h"

int try_login(char* username, char* password, website* ws) {
	//Vars
	int success;
	char *data, token[100], *newURLpattern;
	struct memoryStruct chunk;
  	CURL *curl_handle;
	
	token[0] = '\0';
	
	//Initialization
	success = 1; //1 = fail
	chunk.memory = malloc(1); 
  	chunk.size = 0;
  	curl_global_init(CURL_GLOBAL_ALL);

  	if (curl_handle = curl_easy_init())
  	{ 

	    if(DEBUG)
	      curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1);
	    else
	      curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 0);
	    
	    curl_easy_setopt(curl_handle, CURLOPT_URL, ws->form);
	    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "Mozilla/5.0");
	    curl_easy_setopt(curl_handle, CURLOPT_AUTOREFERER, 1 );
	    curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1 );
	    curl_easy_setopt(curl_handle, CURLOPT_COOKIEFILE, "cookie");
	    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

	    if(ws->connect_first || ws->regex_token != NULL)
	    	curl_easy_perform(curl_handle);
	    

	    //Token extraction
	    if(ws->regex_token != NULL){
			if(easy_extract(ws->regex_token, chunk.memory, token))
			{	
				printf("\nERROR: Couldn't get token");
				return 1;
			}else if(DEBUG){
				printf("\n [DEBUG] {%s} token = %s\n", ws->name, token);
			}
	    }

	    //Crafting POST/GET data
		data = easy_snprintf(ws->data, username, password, token);

		if(data == NULL)
			printf("\n ERROR : Couldn't make a string for POST or GET data \n");
		if(DEBUG)
			printf("\n [DEBUG] {%s} data = %s\n", ws->name, data); 

		//Changing URL if GET
		if (ws->method == GET)
		{
			newURLpattern = easy_snprintf("%s?%s", ws->action, data, NULL); //BAD !!
			ws->action = easy_snprintf(newURLpattern, username, password, token);
		}





		//Next URL to get is the ACTION one /!\ PAS BIEN
		if (ws->regex_token == NULL)
			curl_easy_setopt(curl_handle, CURLOPT_URL, ws->action);


	    if (ws->method == POST)
   			curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, data);

	    curl_easy_perform(curl_handle);

	    //At this point we have tryed to log in and we have the HTML answer of this attempt in chunk.memory
	    //We just need to check if the HTML contain the login REGEX
	    if (!easy_match(ws->regex_login, chunk.memory))
      		success = 0;

	    if(DEBUG)
	    	buffer_in_file(ws->name, chunk.memory);


  	}else{
  		printf("\nERROR : Couldn't initialize CURL object\n");
  	} 


	return success;
}

/* Callback function on curl_easy_perform()
copying the web page to a memoryStruct structure (http://curl.haxx.se/libcurl/c/getinmemory.html) */
size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct memoryStruct *mem = (struct memoryStruct *)userp;
 
  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if (mem->memory == NULL) {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    exit(EXIT_FAILURE);
  }
 
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

//Copy buffer in file
void buffer_in_file(char* file_name, char* buffer)
{
    FILE* file = NULL;
    file = fopen(file_name, "w");

    if(!fputs(buffer, file))
    	printf("Can't write buffer\n");

    fclose(file);
}