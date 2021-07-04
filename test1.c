#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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



// artan numara öğrenciNO- Derscode 
int is_course_available_quota(char* course_code) {
	FILE *fp; 
	fp = fopen("ÖğrenciDersKayıt.txt", "r");
	char* block = (char*) malloc(sizeof(char)*200);
	int lec_students = 0;
	while( fgets(block, 2400, fp) ) { // ders kodu , kayıt durumu enum sayilar
		printf("s: %s", block);
		char* course_code_log = get_unique_element(block, 1);
		char* reg_log = get_unique_element(block,4);
		if(strcmp(course_code_log, course_code) && strcmp(reg_log, "1")) 
			lec_students++;
		free(course_code_log);
		free(reg_log);
	}
	fclose(fp);
	fp = fopen("dersler.txt", "r") ;
	while( fgets(block, 200, fp) != NULL ) {
		char* course_code_lec = get_unique_element(block,0);
		char* quota_lec = get_unique_element(block, 3); 
		if ( !strcmp(course_code_lec, course_code) ) {
			if(atoi(quota_lec) > lec_students) {
				free(block);
				free(quota_lec);
				free(course_code_lec);
				return 1 ;
			} 
		}
	}
	free(block);
	return 0;
}

int main() {
	int a = is_course_available_quota("MAT1010");
	printf ("%d",a);
	return 0;
}
