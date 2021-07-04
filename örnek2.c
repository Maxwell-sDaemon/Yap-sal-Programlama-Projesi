#include <stdio.h>
#include <stddef.h>
#define FAIL 0
#define SUCCESS 1
#define BLOCKSIZE 512
typedef char DATA;
//continued in the next slide

int copyfile(char * infile, char * outfile){
FILE *fp1, *fp2; int num_read; char block[BLOCKSIZE];
if ((fp1 = fopen( infile, "rb" )) == NULL){
printf( "Error opening file %s for input.\n", infile ); return FAIL;
}
if ((fp2 = fopen( outfile, "wb" )) == NULL){
printf( "Error opening file %s for output.\n", outfile );
fclose(fp1); return FAIL;
}
while ((num_read = fread( block, sizeof(DATA), BLOCKSIZE, fp1 )) == BLOCKSIZE)
fwrite( block, sizeof(DATA), num_read, fp2 );
fwrite( block, sizeof(DATA), num_read, fp2 ); //notice this line!
fclose(fp1); fclose(fp2);
if (ferror(fp1)) { printf( "Error reading file %s\n", infile ); return FAIL; }
return SUCCESS;
} //the main method will be the same as the previous example

int main(){
	char infl[BLOCKSIZE], outfl[BLOCKSIZE];
	int result;
	printf("enter name of the input file\n");   scanf("%s", infl);
	printf("enter name of the output file\n");  scanf("%s", outfl);
	
	result = copyfile(infl,outfl);
	if(result == SUCCESS) {
		printf("input file is copied to output file");
	} else {
		printf("input file could not be copied to output file");
	}
	return 0;
}
