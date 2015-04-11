#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <regex.h>
#include <curl/curl.h>

#include "connect.h"


int main(void)
{
	char* user="nope@nope.no";
	char* password="nope";

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

return 0;
}
