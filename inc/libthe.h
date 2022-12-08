#ifndef LIBTHE_H
	
	#define LIBTHE_H
	
	#include <stddef.h>
	
	typedef unsigned long long thecnt_t;
	typedef long double the_t;
	
	struct the_unit {
		the_t the;
		const char *name;
	};
	
	struct the_source {
		const char *fmat;
		const char *url;
	};
	
	extern const struct the_unit	the_units [];
	
	extern const struct the_source	the_source_google;
	extern const struct the_source	the_source_bing;
	extern const struct the_source	the_source_wikipedia;
	
	extern struct the_source	the_currentsource;
	
	extern thecnt_t	the_getcnt	(const char *term);
	extern the_t	the_getthe	(const char *term);
	
	extern char	*the_thetostr	(the_t the, char *buf, size_t maxlen);
		
	extern struct the_unit	the_getthe_unit	(the_t thes);
	extern the_t	the_inunit	(the_t the, the_t unit);
	
	extern void the_setsource	(const struct the_source *source);
	extern void	the_init	(struct the_source *source);
	extern void	the_deinit	(void);

#endif
