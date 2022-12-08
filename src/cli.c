#include <stdio.h>
#include <libthe.h>
void print_thes (const char *name, const char *term) {
	char buf [512];
	the_t the = the_getthe (term);
	the_thetostr (the, buf, 512);
	printf ("%s:\t%s\t(%.20Lf thes)\n", name, buf, the);
}

void main (int argc, char **argv) {
	the_init ();
	
	printf ("thes found for term \"%s\"\n", argv [1]);
	
	the_setsource (&the_source_bing);
	print_thes ("bing     ", argv [1]);
	
	the_setsource (&the_source_google);
	print_thes ("google   ", argv [1]);
	
	the_setsource (&the_source_wikipedia);
	print_thes ("wikipedia", argv [1]);
}
