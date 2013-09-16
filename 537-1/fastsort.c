#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "sort.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int cmpfunc (const void *a, const void *b)
{
	//printf("%d\n", (*(rec_t *)a).key);
	return ((*(rec_t *)a).key - (*(rec_t *)b).key);
}

int
main (int argc, char *argv[])
{
	//arguments
	char *inFile = "/no/such/file";
	char *outFile = "/no/such/file";

	// input params
    int c;
    int checki = 0;
    int checko = 0;
    opterr = 0;
    while ((c = getopt(argc, argv, "i:o:")) != -1) {
	switch (c) {
	case 'i':
	    inFile = strdup(optarg);
	    checki = 1;
	    break;
	case 'o':
		outFile = strdup(optarg);
		checko = 1;
		break;
	default:
	    fprintf(stderr, "Usage: fastsort -i inputfile -o outputfile\n");
	    exit(1);
	}
    }

    //check if both input flags exists
    if(checki == 0 || checko == 0)
    {
    	fprintf(stderr, "Usage: fastsort -i inputfile -o outputfile\n");
	    exit(1);
    }

    //open and create input and output file
    int inFileCheck = open(inFile, O_RDONLY);
    int outFileCheck = open(outFile, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
    if(inFileCheck < 0)
    {
    	fprintf(stderr, "Error: Cannot open file %s\n", inFile);
    }
    if(outFileCheck < 0)
    {
    	fprintf(stderr, "Error: Cannot open file %s\n", outFile);   		
    }
    if(inFileCheck < 0 || outFileCheck < 0)
    {
    	exit(1);
    }

    //read input file
    struct stat buffer;
    int status = fstat(inFileCheck, &buffer);
    if(status != 0)
    {
    	fprintf(stderr, "Error: Cannot read file %s\n", inFile);
    	exit(1);
    }
    int fileSize = (buffer.st_size)/100;
    //printf("%d\n", fileSize);
    rec_t records [fileSize];//array to store records to be sorted
    rec_t r;
    int counter = 0;
    while(1){
    	int rc;
    	rc = read(inFileCheck, &r, sizeof(rec_t));
    	if(rc == 0)
    		break;//EOF
    	if(rc < 0){
    		fprintf(stderr, "Error: Cannot read file %s\n", inFile);
    	}
    	//fprintf(stdout, "key: %u\n", r.key);
    	records[counter] = r;
    	//printf("%d\n", records[counter].key);
    }

    //printf("test\n");
    qsort(records, fileSize, sizeof(rec_t), cmpfunc);
    //printf("test\n");

    int i;
    for(i = 0; i < fileSize; i++)
    {
        int rc = write(outFileCheck, &records[i], sizeof(rec_t));
        if(rc != sizeof(rec_t))
        {
            fprintf(stderr, "Error: Cannot write to file %s\n", outFile);
            exit(1);
        }
    	//printf("%d\n", records[i].key);
    }

	return 0;
}

