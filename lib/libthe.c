#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>

#include "urlencode.h"
#include "libthe.h"

CURL *curl;
thecnt_t the_thes;

const struct the_unit the_units [] = {
	{ 0.000000000000000000000000000001, "quectothe" },
	{ 0.000000000000000000000000001, "rontothe" },
	{ 0.000000000000000000000001, "yoctothe" },
	{ 0.000000000000000000001, "zeptothe" },
	{ 0.000000000000000001, "attothe" },
	{ 0.000000000000001, "femtothe" },
	{ 0.000000000001, "picothe" },
	{ 0.000000001, "nanothe" },
	{ 0.000001, "microthe" },
	{ 0.001, "millithe" },
	{ 0.01, "centithe" },
	{ 0.1, "decithe" },
	{ 1.0, "the" },
	{ 10.0, "decathe" },
	{ 100.0, "hectothe" },
	{ 1000.0, "kilothe" },
	{ 1000000.0, "megathe" },
	{ 1000000000.0, "gigathe" },
	{ 1000000000000.0, "terathe" },
	{ 1000000000000000.0, "petathe" },
	{ 1000000000000000000.0, "exathe" },
	{ 1000000000000000000000.0, "zettathe" },
	{ 1000000000000000000000000.0, "yottathe" },
	{ 1000000000000000000000000000.0, "ronnathe" },
	{ 1000000000000000000000000000000.0, "quettathe" },
	{ 0, NULL }
};

const struct the_source the_source_google = {
	"<div id=\"result-stats\">About %m[0-9,] results",
	"https://www.google.com/search?q=\"%s\""
};

const struct the_source the_source_wikipedia = {
	"{\"totalhits\":%m[0-9],",
	"https://en.wikipedia.org/w/api.php?action=query&list=search&format=json&srwhat=text&srsearch=\"%s\""
};

const struct the_source the_source_bing = {
	"<span class=\"sb_count%*[^>]>About %m[0-9,] results",
	"https://www.bing.com/search?q=\"%s\""
};

struct the_source the_currentsource;






thecnt_t atothecnt (const char *str) {
	thecnt_t out = 0;
	for (size_t i = 0; str [i] != '\0'; i ++) {
		if (str [i] >= '0' && str [i] <= '9')
			out = out * 10 + str [i] - '0';
	}
	return out;
}

thecnt_t the_getcnt (const char *term) {
	FILE *f = tmpfile ();
	if (f == NULL)
		return 0;
	curl_easy_setopt (curl, CURLOPT_WRITEDATA, f);
	
	char *uterm = url_encode (term);
	static char tmp [512];
	snprintf (tmp, sizeof (tmp) / sizeof (tmp [0]), the_currentsource.url, uterm);
	curl_easy_setopt (curl, CURLOPT_URL, tmp);
	free (uterm);
	
	if (curl_easy_perform (curl))
		return 0;
	
	rewind (f);
	while (1) {
		int c = fgetc (f);
		if (c == EOF)
			return 0;
		if (c == *the_currentsource.fmat) {
			char *outstr;
			if (fscanf (f, the_currentsource.fmat + 1, &outstr) == 1) {
				fclose (f);
				thecnt_t out = atothecnt (outstr);
				free (outstr);
				return out;
			}
		}
	}
}

the_t the_getthe (const char *term) {
	if (the_thes == 0) {
		return 0;
	}
	return (the_t) the_getcnt (term) / (the_t) the_thes;
}

the_t the_inunit (the_t the, the_t unit) {
	return the / unit;
}

struct the_unit the_getthe_unit (the_t thes) {
	struct the_unit out;
	
	if (thes == 0) {
		out.the = 0;
		out.name = "the";
		return out;
	}
	
	the_t inunit = the_inunit (thes, the_units [0].the);
	if (inunit <= 1) {
		out.the = inunit;
		out.name = the_units [0].name;
		return out;
	}
	
	size_t i = 1;
	while (1) {
		inunit = the_inunit (thes, the_units [i].the);
		if (inunit < 1) {
			out.the = the_inunit (thes, the_units [i - 1].the);
			out.name = the_units [i - 1].name;
			break;
		} else
		if (the_units [i + 1].name == NULL) {
			out.the = inunit;
			out.name = the_units [i].name;
			break;
		} else
			i ++;
	}
	
	return out;
}


char *the_thetostr (the_t the, char *buf, size_t maxlen) {
	struct the_unit unit = the_getthe_unit (the);
	if (snprintf (buf, maxlen, "%Lf %s%c", unit.the, unit.name, (unit.the == 1) ? '\0' : 's') == EOF)
		return NULL;
	else
		return buf;
}

void the_setsource (const struct the_source *source) {
	the_currentsource = *source;
	the_thes = the_getcnt ("the");
}

void the_init (struct the_source *source) {
	curl_global_init (CURL_GLOBAL_ALL);
	curl = curl_easy_init ();
	
	curl_easy_setopt (curl, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Linux x86_64; rv:102.0) Gecko/20100101 Firefox/102.0");
	
	if (source != NULL)
		the_setsource (source);
	
}

void the_deinit (void) {
	curl_easy_cleanup (curl);
	curl_global_cleanup ();
}
