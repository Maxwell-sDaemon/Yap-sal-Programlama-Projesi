#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

char* get_unique_element(char* string, int tabular_value) {
	// bu işlem string yapısını bozmakta bu yüzden kopyası alınacak.
	char* tokenized = strdup(string);
	char* token = strtok(tokenized, ",");
	for(int i = 0 ; i < tabular_value ; i++) {
		token = strtok(NULL, ",");
	}
	char* result = strdup(token);
	free(tokenized);
	return result;
}

int invert_log_from_id(char* log_id) {
	FILE *fp;
	char* block = (char*) malloc(200 * sizeof(char)); 
	fp = fopen("deneme.txt", "r+");
	int line_start_point = 0;
	
	while(fgets(block, 200, fp)) {
		char* id = get_unique_element(block, 0);
		if( !strcmp(id, log_id) ) {
			char* state = get_unique_element(block, 3);
			char fstate[3] ; sprintf(fstate,",%s,",state) ;
			
			char *stat_pos_ptr = strstr (block, fstate); 
			
			int pos_to_invert = ( stat_pos_ptr - block ) + 1;
			
			fseek(fp, - (ftell(fp)- line_start_point) + pos_to_invert, SEEK_CUR);
			
			printf("%d\t",(int)(fstate[1]));
			putc(!(fstate[1]-48) + 48 , fp); // ascii ile tersini yazdım.
			
			return 1;
		}
		line_start_point = ftell(fp);
	}
	fclose(fp);
	printf("id is not found");
	return 0;
}

int main() {
	int stat = invert_log_from_id("3");
	
	time_t mytime ;
	mytime = time(NULL);
	
	char* timex = (char*) malloc(200);
	timex = asctime(localtime(&mytime));
	printf("%s", timex);
	printf("%d", stat);
}
