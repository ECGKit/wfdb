/* file: getpagenos.c	G. Moody	29 October 2002

Extract page numbers for the table of contents for the WFDB Applications Guide
*/

#include <stdio.h>

main()
{
    static char buf[200], pagename[200], *p, *q;
    int lineno = 0;
    int pageno = 0;
    int n;

    while (fgets(buf, sizeof(buf), stdin)) {
	lineno++;
	if (strncmp(buf, "%%Page: ", 8) == 0) {	/* found a page number */
	    if (sscanf(buf+8, "%d", &n) < 1)
		fprintf(stderr, "%d: missing page number\n", lineno);
	    else {
		if (n != pageno+1)
		    fprintf(stderr, "%d: page %d expected, page %d found\n",
			    lineno, pageno+1, n);
		pageno = n;
		/* Look for page name */
		fgets(buf, sizeof(buf), stdin);	lineno++;
		if (strcmp(buf, "%%BeginPageSetup\n")) {
		    fprintf(stderr, "%d: BeginPageSetup missing\n", lineno);
		    continue;
		}
		fgets(buf, sizeof(buf), stdin);	lineno++;
		if (strcmp(buf, "BP\n")) {
		    fprintf(stderr, "%d: BP\n", lineno);
		    continue;
		}
		fgets(buf, sizeof(buf), stdin);	lineno++;
		if (strcmp(buf, "%%EndPageSetup\n")) {
		    fprintf(stderr, "%d: EndPageSetup missing\n", lineno);
		    continue;
		}
		fgets(buf, sizeof(buf), stdin);	lineno++;
		for (p = buf; p-buf<sizeof(buf)-1 && *p!='(' && *p!='\n'; p++)
		    ;
		if (*p != '(') {
		    fprintf(stderr, "%d: Page name missing\n", lineno);
		    continue;
		}
		for (q = ++p; q-buf<sizeof(buf)-1 && *q!='\\' && *q!='\n'; q++)
		    ;
		*q = '\0';
		if (strcmp(pagename, p)) {
		    strcpy(pagename, p);
		    printf("%d\n", pageno);
		}
	    }
	}
    }
}
