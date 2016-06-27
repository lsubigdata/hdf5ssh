/*
**  $Revision: 1.2 $
**  $Date: 2004/02/13 18:06:15 $
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


struct _Info {
    struct _Info *      next;
    char *              package;
    char *              needs[256];
    struct _Info *      relation[256];
    int                 used;
    char *		dirname;
};

typedef struct _Info  Info;

void writedep(struct _Info * info );
void parse( char * filename );
void usage( void );
char * strcreate( char * s );


#define LINE_SIZE	4096
#define BUFFER_SIZE	8192

static char *	ftVarPackage;
static char *	ftVarNeeds;
static Info *	ftInfoList;


int main( int argc, char ** argv )
{
    int		i;
    Info *	info1;
    Info *	info2;

    if (argc < 4)
	usage();

    ftVarPackage = argv[1];
    ftVarNeeds   = argv[2];

    for (i = 3; i < argc; i++)
	parse(argv[i]);

    /*
    for (info1 = ftInfoList; info1; info1 = info1->next) {
	for (i = 0; info1->needs[i]; i++) {
	    printf("package `%s' needs `%s'\n",
		info1->package, info1->needs[i]);
	}
    }
    */

    for (info1 = ftInfoList; info1; info1 = info1->next) {
	for (i = 0; info1->needs[i]; i++) {
	    for (info2 = ftInfoList; info2; info2 = info2->next) {
		if (strcmp(info1->needs[i], info2->package) == 0) {
		    info1->relation[i] = info2;
		    break;
		}
	    }
	}
    }

    for (info1 = ftInfoList; info1; info1 = info1->next) {
	if (info1) {
    	    writedep(info1);
	}
    }

    return 0;
}

void writedep( Info * info )
{
    static int	depth = 0;

    int		i;

    depth++;

    if (info->used)
	return;

    info->used = 1;

    if (depth >= 1000) {
	fprintf(stderr, "ERROR: recursive dependicies\n");
	exit(1);
    }

    for (i = 0; info->needs[i]; i++) {
	if (info->relation[i]) {
	    writedep(info->relation[i]);
	}
    }

    printf("%s ", info->dirname);

    depth--;
}

void parse( char * filename )
{
    FILE *	file;
    Info	info;
    char	line[LINE_SIZE];
    char	buffer[BUFFER_SIZE];
    char *	b;
    char *	token;
    int		len;
    int		n;
    char *	t;

    if (!(file = fopen(filename, "r"))) {
/*	fprintf(stderr, "ERROR: can't open `%s' (%s)\n",
	    filename, strerror(errno)); */
	exit(0);
    }

    info.next     = ftInfoList;
    info.package  = NULL;
    info.needs[0] = NULL;
    info.used     = 0;
    info.dirname  = strcreate(filename);

    if ((b = strrchr(info.dirname, '/')))
	*b = '\0';

    b = buffer;
    while ((fgets(line, LINE_SIZE, file))) {
	len = strlen(line);
	if (len && line[len-1] == '\n') {
	    len--;
	    line[len] = '\0';
	}
	strcpy(b, line);
	if (len && line[len-1] == '\\') {
	    len--;
	    line[len] = '\0';
	    b += len;
	    continue;
	}
	b = buffer;

	token = strtok(buffer, "\t +=");
	if (token && strcmp(token, ftVarPackage) == 0) {
	    if (!(token = strtok(NULL, "\t +=")))
		continue;
	    if (token[0] == '#')
		continue;
	    info.package = strcreate(token);
	    continue;
	}
	if (token && strcmp(token, ftVarNeeds) == 0) {
	    for (n = 0; n < 255; n++)
		if (info.needs[n] == NULL)
		    break;
	    for (; n < 255 && (token = strtok(NULL, "\t +=")); n++) {
		if (token[0] == '#')
	    	    break;
		if (strncmp(token, "-l", 2) == 0)
		    token += 2;
		if (strncmp(token, "$(", 2) == 0) {
		    token += 2;
		    if ((t = strchr(token, ')')))
			*t = '\0';
		    if ((t = strchr(token, '_')))
			*t = '\0';
		}
		info.needs[n] = strcreate(token);
		info.relation[n] = NULL;
	    }
	    info.needs[n] = NULL;
	    continue;
	}
    }

    if (info.package) {
	if (!(ftInfoList = (Info *)malloc(sizeof(Info)))) {
	    fprintf(stderr, "ERROR: can't alloc memory (%s)\n",
		strerror(errno));
	    exit(1);
	}
	memcpy(ftInfoList, &info, sizeof(Info));
    }

    fclose(file);
}

char * strcreate( char * s )
{
    char * t;

    if (!(t = (char *)malloc(strlen(s)+1))) {
	fprintf(stderr, "ERROR: can't alloc memory (%s)\n", strerror(errno));
	exit(1);
    }

    strcpy(t, s);

    return t;
}

void usage( void )
{
    fprintf(stderr,
	"USAGE: findtargets "
	"<varname1> "
	"<varname2> "
	"<Makefile1> [<Makefile2> ...]\n");

    exit(1);
}

