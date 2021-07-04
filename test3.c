#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define SEP ","
#define KAYITLI "1"
#define LOGFILE "ÖğrenciDersKayıt.txt"
#define LECFILE "dersler.txt"
#define LINESIZE 500

char* integer_to_string(int integer, int length);

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

/*abc,klm,ade*/
char* set_unique_element(char* string, char* value, int tabular_value) {
	int start_ptr = 0, end_ptr = 0, group = 0, change = 0;
	// start harfi end , ü gösterir.
	for(  ; group < tabular_value + 1 ; end_ptr++) {
		if( change == 1 ) {
			start_ptr = end_ptr;
			change = 0;
		}
		if ( string[end_ptr] == ',' ) {
			group++;
			change = 1;
		}
	}
	
	char* block = (char*) malloc(LINESIZE * sizeof(char));
	
	// 3 tane char bloğunu birleştireceğim 
	string[start_ptr] = '\0';
	int first_partial_len = strlen(string);
	memcpy(block, string, first_partial_len);
	int second_partial_len = strlen(value);
	memcpy(block + first_partial_len, value, second_partial_len);
	char* third_partial = string + first_partial_len + second_partial_len;
	
	int third_partial_len = strlen( third_partial ) ;
	memcpy(block + first_partial_len + second_partial_len ,third_partial, third_partial_len);

	return block;

}

// artan numara öğrenciNO- Derscode 
int is_course_available_quota(char* course_code) {
	FILE *fp; 
	fp = fopen("ÖğrenciDersKayıt.txt", "r");
	char* block = (char*) malloc(sizeof(char)*LINESIZE);
	int lec_students = 0;
	while( fgets(block, LINESIZE, fp) ) { // ders kodu , kayıt durumu enum sayilar
		printf("s: %s", block);
		char* course_code_log = get_unique_element(block, 1);
		char* reg_log = get_unique_element(block,4);
		if(strcmp(course_code_log, course_code) && strcmp(reg_log, "1")) 
			lec_students++;
		free(course_code_log); free(reg_log);
	}
	fclose(fp);
	fp = fopen("dersler.txt", "r") ;
	while( fgets(block, LINESIZE, fp) != NULL ) {
		char* course_code_lec = get_unique_element(block,0);
		char* quota_lec = get_unique_element(block, 3); 
		if ( !strcmp(course_code_lec, course_code) ) {
			if(atoi(quota_lec) > lec_students) {
				free(block); free(quota_lec); free(course_code_lec);
				return 1 ;
			} 
		}
	}
	free(block);
	return 0;
}

int get_lec_credit(char* course_code) {
	FILE *fp;
	fp = fopen(LECFILE, "r");
	char* block = (char*)malloc(LINESIZE*sizeof(char));
	while( fgets(block, LINESIZE, fp) ) {
		char *course_code_lec = get_unique_element(block,0);
		if( !strcmp(course_code_lec, course_code) ) {
			char* course_credit_lec = get_unique_element(block, 2);
			int course_credit = atoi(course_credit_lec);
			free(block);  free(course_credit_lec); fclose(fp);
			return course_credit;
		}
		free(course_code_lec);
	}
	free(block); fclose(fp);
	return -1;
}

int invert_log_from_id(char* log_id) {
	FILE *fp;
	char* block = (char*) malloc(LINESIZE * sizeof(char)); 
	fp = fopen(LOGFILE, "r+");
	int line_start_point = 0;
	
	while(fgets(block, LINESIZE, fp)) {
		char* id = get_unique_element(block, 0);
		if( !strcmp(id, log_id) ) {
			
			char *stat_pos_ptr = strstr (block, ",1,") ;
			
			int pos_to_invert = ( stat_pos_ptr - block ) + 1;
			
			fseek(fp, -(ftell(fp)- line_start_point) + pos_to_invert , SEEK_CUR);
			putc('0', fp);
			fclose(fp); free(block);
			return 1;
		}
		line_start_point = ftell(fp);
	}
	fclose(fp);free(block);
	printf("id is not found");
	return 0;
}

/*logda tutulan kayıtlı degeri sona kayıtlı durumu 1 olarak taşır.*/
int move_log_to_end(char* log_id, char* log_block) {
	FILE *fp; 
	invert_log_from_id(log_id);
	fp = fopen(LOGFILE, "a");
	int id = get_last_id_log();
	
	char* value = integer_to_string(id,LINESIZE);
	char* block = set_unique_element(block, value, 0);
	
	if(fputs(block, fp) != NULL) {
		free(block);
		return 1;
	}
	free(block);
	
	return 0;
}



/*öğrencinin mevcut ders sayısını ve derslerinin kredi durumunu sayan ve 
 * fonksiyonda verilen dersin 
 * alınabilirliğini belirleyen fonksiyon.
 *  */
int is_student_add_lec_available(char* student_no, char* given_course, int max_credit, int max_lec ) {
	FILE *fp;
	fp = fopen(LOGFILE, "r");
	char* block = (char*)malloc(LINESIZE*sizeof(char));
	int std_credit = 0, std_lec_number = 0;
	while( fgets(block, LINESIZE, fp) ) {
		char* std_no_log = get_unique_element(block, 2);
		char* reg_stat_log = get_unique_element(block, 3);
		char* course_name_log = get_unique_element(block, 4);
		
		//zaten kayıtlıysa kaydı bıraktıya almalı ki kursu tekrar eklesin yani aldığı dersi silsin. 
		if ( !strcmp(std_no_log, student_no) && !strcmp(course_name_log, given_course) && !strcmp(reg_stat_log, KAYITLI)) {
			char* id_log = get_unique_element(block, 1);
			move_log_to_end(id_log, block); // bu fonksiyon kayıtlı olunan dersi sona taşıyacak.(bu öğrenci tekrar kaydolmaya çalışmış bilgisi)
			free(std_no_log);free(reg_stat_log);free(course_name_log);free(id_log);free(block);fclose(fp);
			return 0;
		}
		
		if( !strcmp(std_no_log, student_no) && !strcmp(reg_stat_log, KAYITLI) ) {
			char* lec_name_log = get_unique_element(block, 1);
			int credit = get_lec_credit(lec_name_log);
			std_credit += credit;
			std_lec_number++;
		}
		free(std_no_log); free(reg_stat_log); free(course_name_log);
	}
	
	free(block); 
	fclose(fp);
	
	int given_credit = get_lec_credit(given_course);
	
	if(std_lec_number <= max_lec && std_credit + given_credit <= max_credit ) {
		return 1;// ders alınabilir.
	}
	
	return 0;
}

int get_last_id_log() {
	FILE * fp;
	fp = fopen(LOGFILE, "r");
	int id=0;
	char* block = (char*) malloc(sizeof(char)*LINESIZE);
	if ( fp ) {
		while( fgets(block, LINESIZE, fp) != NULL ) {
		// Just search for the latest line, do nothing in the loop
		} 
		sscanf(block,"%d",&id);
		id++;
	}
	free(block); fclose(fp);
	return id;
}


time_t get_system_date() {
	time_t mytime ;
	time(&mytime);
	return mytime;
}

char* integer_to_string(int integer, int length) {
	char *block = (char*) malloc(sizeof(char) * length);
	sprintf(block, "%d", integer);	
	return block;
}

int add_course_register(char* student_no, char* course_code, int max_credit, int max_lec) {
	FILE* fp;
	
	int id = get_last_id_log();
	
	if( is_course_available_quota(course_code) && is_student_add_lec_available(student_no, course_code, max_credit, max_lec)) {
		time_t date = get_system_date();
		fp = fopen(LOGFILE, "a");
		
		fprintf(fp, "%d%s%s%s%s%s%s%s%s",id, SEP,
							  course_code, SEP,
							  student_no, SEP,
							  KAYITLI, SEP,
							  ctime(&date));
							  
	}
	
	return 0;
}


int main() {
	add_course_register(18011078, "BLM1010", 9, 20);
	// int id = get_last_id_log(); // work
	// int id = get_lec_credit("MAT1010"); // tested work
	
	// int available = is_student_add_lec_available("18011078","bbg", 5, 20); // tested work

	//int available = add_course_register("18011078", "bbg", 10, 20);

	//printf("last id in the log is %d", available);
	
	//move_log_to_end("3","3,bbg,18011078,1,Fri Jan  1 17:00:16 2021");
	
	return 0;
}
