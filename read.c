#include <stdio.h>

void main(){
    FILE *ifp;
    int nchar, ntax;
    char line[80];
    
    ifp = fopen("input", "r");
    if (ifp == NULL) {
        fprintf(stderr, "Can't open input file\n");
    }
    
    while (fgets(line, 80, ifp) != NULL){
	    /* get a line, up to 80 chars from fr.  done if NULL */
	    printf ("%s", line);
    }
    
    
    fclose(ifp);
}
