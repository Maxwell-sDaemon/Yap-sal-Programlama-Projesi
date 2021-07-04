#include <stdio.h>
#include <stddef.h>
#define FAIL 0
#define SUCCESS 1
#define LINESIZE 100

int copyfile(char * infile, char * outfile) {
	FILE *fp1, *fp2; char line[LINESIZE];int size;
	if((fp1 = fopen(infile, "rb")) == NULL ) {
		printf("Cannot Open the file."); return FAIL;
	}
	if ((fp2 = fopen(outfile, "wb")) == NULL ) {
		printf("Couldn't open the output file"); fclose(fp1); return FAIL;
	}

	while((size = fread(line, sizeof(char), LINESIZE, fp1)) == LINESIZE )
		fwrite(line, sizeof(char), LINESIZE, fp1);
	fclose(fp1);fclose(fp2);return SUCCESS;
	
} 

int main(){
	char infl[LINESIZE], outfl[LINESIZE];
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
