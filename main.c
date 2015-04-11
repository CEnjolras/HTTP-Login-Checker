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

	website facebook, bnet;

	facebook.name = "facebook";
	facebook.method = POST;
	facebook.regex_login = "login_error_box";
	facebook.regex_token = NULL;
	facebook.data = "email=%s&pass=%s";
	facebook.action = "http://www.facebook.com/login.php";
	facebook.form = "http://www.facebook.com/login.php";
	facebook.connect_first = 1;

	bnet.name = "bnet";
	bnet.method = POST;
	bnet.regex_login = "Welcome";
	bnet.regex_token = "<input type=\"hidden\" id=\"csrftoken\" name=\"csrftoken\" value=\"(.+)\" />";
	bnet.data = "accountName=%s&password=%s&persistLogin=on&csrftoken=%s";
	bnet.action = "http://eu.battle.net/login/en-us/index";
	bnet.form = "http://eu.battle.net/login/en-us/index";
	bnet.connect_first = 0;


	

	if(!try_login(user, password, &bnet))
		printf("fail\n");
	else
		printf("works!\n");

return 0;
}
