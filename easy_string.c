#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "connect.h"

//Concatenation
char* easy_snprintf(char* pattern, char* username, char* password, char* token) {
	size_t length;
	char* data;


	if(token == NULL) 
	{
		length = snprintf(NULL, 0, pattern, username, password);

		if (length <0)
			return NULL;

		data = malloc(length + 1);
		snprintf(data, length + 1, pattern, username, password);

	}else{
		length = snprintf(NULL, 0, pattern, username, password, token);

		if (length <0)
			return NULL;

		data = malloc(length + 1);
		snprintf(data, length + 1, pattern, username, password, token);
	}
	
	return data;
}

// Testing if buffer match regex return 1 if it does
int easy_match(char *strRegex, char *buffer)
{
  regex_t preg;
  int err;

  
  err = regcomp (&preg, strRegex, REG_EXTENDED);
  if (err == 0)
  {
    int match;

    match = regexec (&preg, buffer, 0, NULL, 0);
    regfree (&preg);
    if (match == 0)
    {
      if (DEBUG)
        printf("match !\n");

      return 1;
    }
    else if (match == REG_NOMATCH)
    {
      if (DEBUG)
        printf("no match !\n");

      return 0;  
    }
  }
  else if (DEBUG)
  {
      char *text;
      size_t size;
      
      printf("REGEX : %s \n ERROR :", strRegex);  
      size = regerror (err, &preg, NULL, 0);
      text = malloc (sizeof (*text) * size);
      if (text)
      {
        regerror (err, &preg, text, size);
        fprintf (stderr, "%s\n", text);
        free (text);
      }
  }  
}

//Extract substring strRegex from buffer and put it on result
int easy_extract(char *strRegex, const char *buffer, char *result){
	regex_t regex;
	regmatch_t matches[2];
	int match_length,
		err,
		success,
		match;

	success = 1; //1 = fail

	err = regcomp(&regex, strRegex, REG_EXTENDED);
	
	

	if(!err){
		match = regexec (&regex, buffer, 2, matches, 0);

		if(!match)
		{
			match_length = (int)matches[1].rm_eo - (int)matches[1].rm_so;

			//SEG FAULT ICI
			strncpy(result, buffer+matches[1].rm_so, match_length);

			result[match_length] = '\0';

			success = 0;
		}
	}else if (DEBUG){
		printf("\n******** ERREUR REGEX **********\n");
		char *text;
      	size_t size;
      
     	printf("REGEX : %s \nERROR :", strRegex);  
      	size = regerror (err, &regex, NULL, 0);
      	text = malloc (sizeof (*text) * size);
      	if (text)
      	{
        	regerror (err, &regex, text, size);
        	printf ("%s\n", text);
        	free (text);
      	}else{
      		printf("fail in fail");
      	}
	}

	return success;
}

