# libthe

library for determining the popularity of words/phrases by comparing the number of hits on certain platforms to the number of hits of the word "the"

**NOTE**:  due to the nature of the unit *the*'s name, every time that the "*the*" unit is being referred to, it is in italics.

## example

```c
#include <stdio.h>
#include <libthe.h>

int main (int argc, char **argv) {
    the_init (&the_source_google);    // initializes the library and sets the source to google

    const char *term = argv [1];
    the_t thes = the_getthe (term);    // fetch the *the* count for `term` and puts it into `thes`

    char buf [512];
    the_thetostr (thes, buf, 512);    // converts `thes` into a string, accounting for units (see units)

    printf ("term %s is %s\n", term, buf);    // print the result

    the_deinit ();

    return 0;
}
```

## what is a "the"?

a *the* is a measurement initially imagined by [trimill](https://trimill.xyz) and implemented by me. the goal of the the is to measure the popularity of a certain word, by getting the total hits on a specified source (see [sources](#sources)) compared to that of the word "the". higher numbers mean that theyre more popular, lower means theyre more obscure. one the is equal to the search hits of a specified term divided by that of the word "the".

as a sanity check, "the" should *always* equal 1 *the*. if it does not, either something is wrong with the source used, or youre using bing (see [sources](#sources)).

## api

the api of libthe is simple-ish. heres a simple breakdown of how a typical libthe program would be structured: (see individual function references for more information)

- before any other calls to libthe are made, [`the_init (struct the_source*)`](#the_init-) is called to initialize the library. the parameter specifies what kind of source to use, if any is desired (see [sources](#sources)). this can be changed later on, if desired (see [`the_setsource`](#the_setsource-)).

- after this, either [`the_getthe (const char *)`](#the_getthe-) or [`the_getcnt (const char *)`](#the_getcnt-) is typically called, to get the *the* count for a term (see [what is a "*the*"?](#what-is-a-the)). see their specific function information for more details on how these work.

- if the program desires, it can pass the information into [`the_thetostr (the_t, char *buf, size_t)`](#the_thetostr-). this converts the *the* value to a string, with a unit attached to it (see [`struct the_unit`](#struct-the_unit-)).

- finally, when it is all done, [`the_deinit (void)`](#the_deinit-) is called to close all connections and finalize the library.

### sources

libthe requires a source for its information. this can be google, wikipedia, bing, stackoverflow, or any other service that has a search function and a way to capture the number of hits. to use a source, either pass the desired source to [`the_init`](#the_init-) or to [`the_setsource`](#the_setsource-). see their respective articles for more information on how to use these.

as of v0.1, there are three predefined sources:

| name                                             | url                                                                                                 | notes                                                                                                                                                 |
| ------------------------------------------------ | --------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------- |
| [`the_source_google`](#the_source_google-)       | `https://www.google.com/search?q="%s"`                                                              | uses html parsing; long wait times; can run into captcha and/or ratelimit                                                                             |
| [`the_source_bing`](#the_source_bing-)           | `https://www.bing.com/search?q="%s"`                                                                | uses html parsing; long wait times; can run into captcha and/or ratelimit; certain queries return 0 thes, due to bing not always reporting hit counts |
| [`the_source_wikipedia`](#the_source_wikipedia-) | `https://en.wikipedia.org/w/api.php?action=query&list=search&format=json&srwhat=text&srsearch="%s"` | reliable, but sometimes not particularly accurate                                                                                                     |

if unsure, [`the_source_google`](#the_source_google-) is usually a good default. if you want to create your own source, see [`struct the_source`](#struct-the_source).

### reference

#### types

##### `the_t`

```c
typedef long double the_t;
```

a scalar type for holding *the* values. values can be very large or very small!

##### `thecnt_t`

```c
typedef unsigned long long thecnt_t;
```

a scalar type for holding hit counts. values can be very large!

##### `struct the_source`

```c
struct the_source {
    const char *fmat;
    const char *url;
};
```

a struct for holding source information.

###### `.fmat`

format string for getting the number of hits for the query, in the format defined by `scanf(3)`. use `%[0-9 ... ]` to specify the value; any non-numeric characters caught by this format specifier will be skipped. see man page `scanf(3)` for more information on how format specifiers work.

###### `.url`

format string for the url which to obtain the hit count from, in the format defined by `printf(3)`. use `%s` to show where the query should be put, url-encoded. it is recommended to put quotations around the query, to reduce searches to those who include the exact phrase. see man page `printf(3)` for more information on how format specifiers work.

##### `struct the_unit`

```c
struct the_unit {
    the_t the;
    const char *name;
};
```

a struct for holding information on a unit for [`the_inunit ()`](#the_inunit-) and [`the_thetostr ()`](#the_thetostr-). this is often only used for internal purposes. units are stored in array [`the_units []`](#the_units-).

###### `.the`

value in *the*s that one of these units is equal to.

###### `.name`

name of the unit, singular.

#### constants

##### `the_units []`

```c
extern const struct the_unit the_units [];
```

a list of unit specifiers, used by [`the_inunit ()`](#the_inunit-) and [`the_thetostr ()`](#the_thetostr-). terminated by `.name = NULL`.

##### `the_source_google`

```c
extern const struct the_source the_source_google;
```

source definition for [https://www.google.com](https://www.google.com)

##### `the_source_bing`

```c
extern const struct the_source the_source_bing;
```

source definition for [https://www.bing.com](https://www.bing.com)

##### `the_source_wikipedia`

```c
extern const struct the_source the_source_wikipedia;
```

source definition for https://en.wikipedia.org

##### `the_currentsource`

```c
extern struct the_source the_currentsource;
```

holds the current source definition. please dont write to this value directly, use [`the_setsource ()`](#the_setsource-) instead.

#### functions

##### `the_init ()`

```c
extern void the_init (struct the_source *source);
```

initializes the library. this must be called before any other function call to libthe. `source` specifies the source definition to use initially, but this can be changed at any time with [`the_setsource ()`](#the_setsource-). if this value is `NULL`, the source will not be set.

##### `the_deinit ()`

```c
extern void the_deinit (void);
```

shuts down the library after use

##### `the_setsource ()`

```c
extern void the_setsource (const struct the_source *source);
```

sets the current source to use

##### `the_getcnt ()`

```c
extern thecnt_t the_getcnt (const char *term);
```

returns the number of hits for `term`

##### `the_getthe ()`

```c
extern the_t the_getthe (const char *term);
```

returns the *the* value for `term`.

##### `the_thetostr ()`

```c
extern char *the_thetostr (the_t the, char *buf, size_t maxlen);
```

creates a string in buffer `buf` representing the *the* count of `the`, using the unit conversion and pluralization. returns `buf` on success, `NULL` on error.

##### `the_getthe_unit ()`

```c
extern struct the_unit the_getthe_unit (the_t thes);
```

returns the optimal unit to use to represent `thes`, chosen from [`the_units []`](#the_units-).

##### `the_inunit ()`

```c
extern the_t the_inunit (the_t the, the_t unit);
```

returns the *the* value of `the`, converted to unit `unit`. `unit` should be set to what to divide `the` by to get the unit. this function is mostly useless, so im probably going to depreciate it soon. 

## faq

here are a list of "frequently" asked questions (ie, once.)

### can i use duckduckgo as a source?

at the moment, no. duckduckgo has no way of obtaining the number of hits a query represents. if in the future it has this option, then likely it will become a good source.

### why do most of the predefined sources use html parsing, rather than an actual api?

simply, most good web search engines dont have an open api. googles api has been deprecated for years (i do not have a time machine), and bings api requires azure (i do not have azure).

### why does this exist?

boredom.

### whats your favorite color?

why are you asking me this here? anyway, its red.
