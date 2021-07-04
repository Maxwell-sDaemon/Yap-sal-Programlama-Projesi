#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include "macros.h"
#include "structures.h"

int invert_log_by_log_id(char* log_id);
char* set_unique_element(char* string, char* value, int tabular_value);
char* get_unique_element(char* string, int tabular_value);
FILE* open(char* filename, char* mode);
void recreation_of_lecture_file(FILE *fp, char* course_code, char* n_block) ;
char* choosing_degree();
int get_lec_credit(char* course_code) ;
char* get_system_date();
int is_student_add_lec_available(char* student_no, char* given_course, int max_credit, int max_lec );
int is_course_available_quota(char* c_code);
int get_last_id_log();
int is_teacher_available(char* t_id);

STUDENT* create_student() {
	STUDENT* student = (STUDENT*)malloc(sizeof(STUDENT));
	
	printf("Öğrenci numarası giriniz : ");
	scanf("%d", &student->stdno);
	printf("Öğrenci adı : ");
	
	scanf("%[^\n]%*c", student->name);
	printf("Öğrenci Soyadı : ");
	
	scanf("%s", student->surname);
	
	student->numb_of_lect = 0;
	student->numb_of_credit = 0;
	return student;
}

FILE* open(char* filename, char* mode) {
	FILE *fp;
	if((fp = fopen(filename, mode)) == NULL) {
		printf("Couldn't open the output file"); fclose(fp); return NULL;
	}	
	return fp; 
}



/*abc,klm,ade
 * dönen değer yerleştirimiş değerdir.
 * */
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
	// \0 ekleme yolu ile stringin baş ve son parçasını kullandım.
	
	char* block_to_get_len = get_unique_element(string, tabular_value);
	int len_of_block = strlen(block_to_get_len);
	free(block_to_get_len);
	
	// 3 tane char bloğunu birleştireceğim 
	string[start_ptr] = '\0';
	int first_partial_len = strlen(string);
	memcpy(block, string, first_partial_len);
	
	int second_partial_len = strlen(value);
	memcpy(block + first_partial_len, value, second_partial_len);
	char* third_partial = string + first_partial_len + len_of_block;

	int third_partial_len = strlen( third_partial ) ;
	memcpy(block + first_partial_len + second_partial_len ,third_partial, third_partial_len);

	return block;

}

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

//// alternatif 2 bu string değiştirir.
//char* get_unique_element(char* string, int tabular_value) {
	//int start_ptr = 0, end_ptr = 0, group = 0, change = 0;
	//// start harfi end , ü gösterir.
	//for(  ; group < tabular_value + 1 ; end_ptr++) {
		//if( change == 1 ) {
			//start_ptr = end_ptr;
			//change = 0;
		//}
		//if ( string[end_ptr] == ',' ) {
			//group++;
			//change = 1;
		//}
	//}
	////string[start_ptr] = '\0';
	//string[end_ptr] = '\0';
	//return  string + start_ptr + 1;
//}

/*
 * bu fonksiyon yan yana soru işareti oluşturur 
 * ?,?,?,?,? gibi onu char olarak döner.
 * */
char* create_unknown(int size_component) {
	char *block = (char*)malloc(LINESIZE * sizeof(char));
	int i = 0;
	for(int j=0 ; j < size_component ; i+=2, j++) {
		block[i] = UNKNOWN;
		block[i+1] = SEP_U;
	}
	block[2 * size_component -1 ] = '\0';
	return block;
}

/*bu fonksiyon bozuk çalışmakta ikinci ve daha üst lineı almak için bir 
 * seek yapılmalı ya da fgetc()!!
 * */
void set_cursor_previous_line_begin(FILE* fp) {
	fseek(fp,-2,SEEK_CUR);
	while(fgetc(fp) != EOL && ftell(fp) > 1) {
		fseek(fp, -2, SEEK_CUR);
	}
	if(ftell(fp) == 1 ) {
		fseek(fp,-1,SEEK_CUR);
	}
}

char* integer_to_string(int integer, int length) {
	char *block = (char*) malloc(sizeof(char) * length);
	sprintf(block, "%d", integer);	
	return block;
}

void write_space_line(FILE *fp) {
	while(fgetc(fp) != EOL) {
		fseek(fp, -1, SEEK_CUR);
		fputc(' ', fp);
	}
}

int remove_student_from_student_file_by_no(int std_no) {
	FILE *fp;
	fp = open(OGRENCIFILE,"r+");
	char* (*cast)() = integer_to_string;
	
	char* block = (char*)malloc(LINESIZE * sizeof(char));
	while(fgets(block,LINESIZE, fp) != NULL ) {
		char* std_no_from_file = get_unique_element(block, student_std_no);
		if( !strcmp(std_no_from_file, cast(std_no)) ) {
			set_cursor_previous_line_begin(fp);
			write_space_line(fp);
			set_cursor_previous_line_begin(fp);
			char* unknown = create_unknown(UNKNOWN_STUDENT_COMPONENT);
			fputs(unknown,fp);
			free(unknown);free(block);fclose(fp);
			return SUCCESS;
		}
	}
	free(block);fclose(fp);
	return FAIL;
}

/*her bir kursa bakıp */
char** update_log_file_by_removed_student_no(int std_no){
	FILE *fp;
	char* (*cast)() = integer_to_string;
	int size = 0;
	fp = open(LOGFILE, "r");
	
	char *st_no = cast(std_no,100);
	
	char * block = (char*) malloc( sizeof(char) * LINESIZE);
	
	while ( fgets(block, LINESIZE, fp) ) {
		char *block_std_no = get_unique_element(block, log_student_no);
		char *block_rs = get_unique_element(block, log_registration_state);
		if( !strcmp(block_std_no, st_no ) && !strcmp(block_rs, KAYITLI ) ) {
			size++;
		}
		free(block_std_no); free(block_rs); 
	}

	char** erased_block = (char**) malloc( size * sizeof(char*) );

	erased_block[0] = cast(size,100);  
	
	size = 1;
	rewind(fp);
	while( fgets(block, LINESIZE , fp ) != NULL){
		char *block_std_no = get_unique_element(block, log_student_no);
		char *block_rs = get_unique_element(block, log_registration_state);
		if( !strcmp(block_std_no, st_no ) 
				&& !strcmp(block_rs, KAYITLI )) { 
			erased_block[size++] = get_unique_element(block, log_course_code);
			char *block_log_id = get_unique_element(block,log_id);
			invert_log_by_log_id(block_log_id);
			free(block_log_id);
		}
		free(block_std_no); free(block_rs); 
	}
	
	free(block); fclose(fp);free(st_no);
	return erased_block;
}

int invert_log_by_log_id(char* l_id) {
	FILE *fp;
	char* block = (char*) malloc(LINESIZE * sizeof(char)); 
	fp = fopen(LOGFILE, "r+");
	int line_start_point = 0;
	
	while(fgets(block, LINESIZE, fp)) {
		char* id = get_unique_element(block, log_id);
		if( !strcmp(id, l_id) ) {
			char* state = get_unique_element(block, log_registration_state);
			char fstate[3] ; sprintf(fstate,",%s,",state) ;
			
			char *stat_pos_ptr = strstr (block, fstate); 
			
			int pos_to_invert = ( stat_pos_ptr - block ) + 1;
			
			fseek(fp, - (ftell(fp)- line_start_point) + pos_to_invert, SEEK_CUR);
			
			//printf("%d\t",(int)(fstate[1]));
			putc(!(fstate[1]-48) + 48 , fp); // ascii ile tersini yazdım.
			
			printf("id is found");
			free(state); free(block); fclose(fp);
			return SUCCESS;
		}
		line_start_point = ftell(fp);
	}
	fclose(fp);
	printf("id is not found");
	return FAIL;
}

int add_student_to_student_file(STUDENT* student) {
	FILE *fp;
	fp = open(OGRENCIFILE, "a");
	if( fp != NULL ) {
		fprintf(fp,"%d%s%s%s%s%s%d%s%d\n",  student->stdno, SEP,
										student->name, SEP,
										student->surname, SEP,
										student->numb_of_lect, SEP,
										student->numb_of_credit);
	} 
	//null exception handling yapmadım.
	fclose(fp);
	return SUCCESS;
}

char* get_name_surname_from_teacher_id(char* id) {
	FILE *fp;
	fp = open(TEACHERFILE, "r");
	
	char* block = (char*)malloc(LINESIZE*sizeof(char));
	while(fgets(block,LINESIZE,fp)) {
		char* block_tec_id = get_unique_element(block, teacher_id);
		if ( !strcmp(block_tec_id, id) ) {
			char* name = get_unique_element(block,teacher_name);
			char* surname = get_unique_element(block, teacher_surname);
			int len_name = strlen(name);
			int len_surname = strlen(surname);
			memcpy(block,name,len_name);
			block[len_name] = ' ';
			memcpy(block+len_name+1,surname,len_surname);
			block[len_name+len_surname +1] = '\0';
			fclose(fp);
			return block;
		}
	}
	free(block);
	return NULL;
}

char* get_name_surname_from_student_id(char* no) {
	FILE *fp;
	fp = open(OGRENCIFILE, "r");
	
	char* block = (char*)malloc(LINESIZE*sizeof(char));
	while(fgets(block,LINESIZE,fp)) {
		char* block_st_no = get_unique_element(block, student_std_no);
		if ( !strcmp(block_st_no, no) ) {
			char* name = get_unique_element(block,student_std_name);
			char* surname = get_unique_element(block, student_std_surname);
			int len_name = strlen(name);
			int len_surname = strlen(surname);
			memcpy(block,name,len_name);
			block[len_name] = ' ';
			memcpy(block+len_name+1,surname,len_surname);
			block[len_name+len_surname +1] = '\0';
			fclose(fp);
			return block;
		}
	}
	free(block);
	return NULL;
}

void warning_teacher_of_course( char** course_code){
	int len ;char *ptr;
	len = strtol( course_code[0], &ptr, 10);
	for( int i = 0 ; i < len ; i++ ) {
		char *block = (char*)malloc(LINESIZE);
		sprintf(block,"%s", course_code[i]);
		printf("%s \t",block);
		free(block);
	}
	printf("derslerini veren hocalar sınıflarını güncellemelidir.");
}

char* get_student_block_by_student_no(int no_){
	FILE *fp;
	
	fp = open(OGRENCIFILE,"r");
	char* no = integer_to_string(no_,PARTSIZE);
	
	char* block = (char*) malloc(LINESIZE);
	while(fgets(block,LINESIZE,fp)){
		char* block_no = get_unique_element(block,student_std_no);
		if(!strcmp(block_no,no)) {
			free(block_no);
			return block;
		}
	}
	free(block);
	return NULL;
}

void recreation_of_student_file(FILE *fp,int std_no,char* n_block){
	rewind(fp);
	FILE *fw;
	fw = open(TEMPFILE, "w+");
	char* s_no_ = integer_to_string(std_no,PARTSIZE);
	char* block = (char*)malloc(LINESIZE);
	while(fgets(block, LINESIZE, fp)) {
		char * s_no = get_unique_element(block, student_std_no);
		if( !strcmp(s_no_, s_no) ) {
			fputs(n_block, fw);
		}
		else {
			fputs(block, fw);
		}
		free(s_no);
	}
	fclose(fp);fclose(fw);free(block);
	remove(OGRENCIFILE);
	rename(TEMPFILE,OGRENCIFILE);
}

int set_student_newname_by_no(char* newname, int no_){
	FILE *fp;
	
	fp = open(OGRENCIFILE, "r+");
	char *no = integer_to_string(no_, PARTSIZE);
	
	char* block = (char*) malloc(LINESIZE);
	while(fgets(block, LINESIZE, fp)) {
		char *block_no = get_unique_element(block,student_std_no);
		char *stdname = get_unique_element(block,student_std_name); 
		if( !strcmp(block_no,no)) {
			int len_b = strlen(block);
			int len = strlen(newname) - strlen(stdname) ;
			char* student = set_unique_element(block,newname,student_std_name);
			printf("%s",student );
			student[len_b-len-2] = '\0' ;
			//student[len_b-len] = '\0' ;
			//if(len > 0) {
				recreation_of_student_file(fp, no_, student);
			//}
			//printf("%s", student);
			//set_cursor_previous_line_begin(fp);
			//fflush(fp);
			//fputs(student,fp);
			//free(block);fclose(fp);
			return SUCCESS;
		}
		free(block_no);
	}
	free(block);fclose(fp);
	return FAIL;
}

int set_student_newsurname_by_no(char* newsurname, int no_){
	FILE *fp;
	
	fp = open(OGRENCIFILE, "r+");
	char *no = integer_to_string(no_, PARTSIZE);
	
	char* block = (char*) malloc(LINESIZE);
	while(fgets(block, LINESIZE, fp)) {
		char *block_no = get_unique_element(block,student_std_no);
		if( !strcmp(block_no,no)) {
			char* student = set_unique_element(block,newsurname,student_std_surname);
			set_cursor_previous_line_begin(fp);
			fputs(student,fp);
			free(block);
			return SUCCESS;
		}
		free(block_no);
	}
	free(block);
	return FAIL;
}

COURSE* create_course() {
	COURSE* course = (COURSE*) malloc(sizeof(COURSE));

	printf("Eklemek istediğiniz kursun adını giriniz : ");
	scanf("%[^\n]%*c",course->name);
	
	printf("Eklemek istediğiniz kursun kodunu giriniz. : ");
	scanf("%s",course->code);

	printf("kursun maksimum kontenjanını giriniz : ");
	scanf("%d", &course->quota);
	printf("kursun kredisini giriniz : ");
	scanf("%d", &course->credit);
	printf("dersi veren öğretim görevlisinin idsi nedir ? : ");
	scanf("%d", &course->t_id);
	
	return course;
}


int add_course_to_course_file(COURSE *course) {
	FILE *fp ;
	char *block = (char*) malloc(sizeof(char) * TEACHERIDSIZE) ;
	
	
	fp = open(LECFILE, "a");
	
	if( fp ) {
		fprintf(fp,"%s%s%s%s%d%s%d%s%d \n",   course->code, SEP,
										course->name, SEP,
										course->credit, SEP,
										course->quota, SEP,
										course->t_id);
	}
	char * b_t_id = integer_to_string(course->t_id, PARTSIZE);
	int available = is_teacher_available(b_t_id);
	if(!available) {
		printf("Akademisyen Bulunmamaktadır.");
		free(b_t_id);
		return FAIL;
	}
	fclose(fp);
	free(block);
	free(b_t_id);
	return SUCCESS;
}

/*
 * burada krediyi istenen , ders sayısını 1 azaltıyoruz.
 * 	sadece azalma var blok boyutuna dikkat edilmeli 
 * tek atama için güvenli 
 * açılan tek dosya için çoklu atmalarda kullanılması için güncellenecek.
 * 
 * */
int update_student_lect_rcred_by_std_no(char* stud_no, int stud_credit) {
	FILE *fp;
	fp = open(OGRENCIFILE, "r+");
	char* block = (char*) malloc (LINESIZE);
	while(fgets(block, LINESIZE, fp)) {
		char* st_no = get_unique_element(block, student_std_no);
		if( !strcmp(st_no, stud_no) ) {
			//printf("%d", student_std_course_credit);
			//printf("%s", block);
			char* c_credit = get_unique_element(block, student_std_course_credit);
			c_credit[strlen(c_credit)-1] = '\0';
			//printf("%s ", c_credit);
			int new_credit = atoi(c_credit);
			new_credit -= stud_credit;
			char* n_lect = get_unique_element(block, student_std_course_number);
			int new_course_numb = atoi(n_lect);
			new_course_numb--;
			sprintf(c_credit, "%d", new_credit);
			sprintf(n_lect, "%d", new_course_numb);

			//printf("%s-%s ", c_credit,n_lect);
			

			char* nblock = set_unique_element(block, c_credit, student_std_course_credit);
			char* n2block = set_unique_element(nblock, n_lect, student_std_course_number);
			
			//printf("%s",n2block);
			set_cursor_previous_line_begin(fp);
			write_space_line(fp);
			
			set_cursor_previous_line_begin(fp);
			fputs(n2block,fp);
			
			free(c_credit);free(n_lect);free(nblock);free(n2block);

			return SUCCESS;
		}
		free(st_no);
	}
	free(block);
	return FAIL;
}

int update_student_lect_icred_by_std_no(char* stud_no, int stud_credit) {
	FILE *fp;
	fp = open(OGRENCIFILE, "r+");
	char* block = (char*) malloc (LINESIZE);
	while(fgets(block, LINESIZE, fp)) {
		char* st_no = get_unique_element(block, student_std_no);
		if( !strcmp(st_no, stud_no) ) {
			//printf("%d", student_std_course_credit);
			//printf("%s", block);
			char* c_credit = get_unique_element(block, student_std_course_credit);
			c_credit[strlen(c_credit)-1] = '\0';
			//printf("%s ", c_credit);
			int new_credit = atoi(c_credit);
			new_credit += stud_credit;
			char* n_lect = get_unique_element(block, student_std_course_number);
			int new_course_numb = atoi(n_lect);
			new_course_numb++;
			sprintf(c_credit, "%d", new_credit);
			sprintf(n_lect, "%d", new_course_numb);

			//printf("%s-%s ", c_credit,n_lect);
			

			char* nblock = set_unique_element(block, c_credit, student_std_course_credit);
			char* n2block = set_unique_element(nblock, n_lect, student_std_course_number);
			
			//printf("%s",n2block);
			set_cursor_previous_line_begin(fp);
			write_space_line(fp);
			
			set_cursor_previous_line_begin(fp);
			fputs(n2block,fp);
			
			free(c_credit);free(n_lect);free(nblock);free(n2block);

			return SUCCESS;
		}
		free(st_no);
	}
	free(block);
	return FAIL;
}

char* get_course_block_by_course_code(char *course_code){
	FILE *fp;
	fp = fopen(LECFILE, "r");
	char* block = (char*)malloc(LINESIZE);
	while(fgets(block, LINESIZE, fp)) {
		char* c_code = get_unique_element(block, lecture_course_code);
		if(! strcmp(c_code ,course_code)) {
			printf("ilgili ders : %s ", block);
			char* t_id = get_unique_element(block,lecture_teacher_id);
			t_id[strlen(t_id)-1] = '\0';
			char* t_name = get_name_surname_from_teacher_id(t_id);
			printf("Dersin hocası %s ", t_name);
			free(c_code);
			return block;
		}
		free(c_code);
	}
	fclose(fp);
	printf("ilgili ders bulunamadı.(NULL)");
	return block;
}

char* get_only_course_block_by_course_code(char *course_code){
	FILE *fp;
	fp = fopen(LECFILE, "r");
	char* block = (char*)malloc(LINESIZE);
	while(fgets(block, LINESIZE, fp)) {
		char* c_code = get_unique_element(block, lecture_course_code);
		if(! strcmp(c_code ,course_code)) {
			free(c_code);
			return block;
		}
		free(c_code);
	}
	fclose(fp);
	return block;
}

int set_coursename_by_course_code(char* course_name, char* course_code){
	FILE *fp;
	fp = open(LECFILE,"r+");
	char* block = (char*) malloc(LINESIZE);
	
	while(fgets(block, LINESIZE, fp)) {
		//printf("%s", block);
		char* c_name = get_unique_element(block,lecture_course_name);
		int line_off =  strlen(c_name) - strlen(course_name);
		char* c_code = get_unique_element(block, lecture_course_code);
		printf("\n%d", line_off);
		//c_code[line_off+strlen(block)] = '\0';
		if( !strcmp(c_code, course_code) ) {
			char *n_block = set_unique_element(block, course_name, lecture_course_name);
			if(line_off > 0) {
				set_cursor_previous_line_begin(fp);
				write_space_line(fp);
				set_cursor_previous_line_begin(fp);
				fputs(n_block, fp); free(block); free(c_code) ;fclose(fp);
			} else {
				recreation_of_lecture_file(fp, course_code, n_block);
			}
			free(n_block);
			return SUCCESS;
		}
		free(c_code);
	}
	printf("kurs bulunamadı");
	free(block); fclose(fp);
	return FAIL;
}

void write_space_line_off(FILE *fp, int off) {
	int a;
	while((a = fgetc(fp)) != EOL) {
		//printf("%c",a);
		fseek(fp, -1, SEEK_CUR);
		fputc(' ', fp);
	}
	
	fseek(fp, -5, SEEK_CUR);
	
	for(int i = 0 ; i < 6 ; i++) {
		fputc(' ', fp);
	}

}

void recreation_of_lecture_file(FILE *fp, char* course_code, char* n_block) {
	rewind(fp);
	FILE *fw;
	fw = open(TEMPFILE, "w+");
	char* block = (char*)malloc(LINESIZE);
	while(fgets(block, LINESIZE, fp)) {
		char *c_code = get_unique_element(block, lecture_course_code);
		if( !strcmp(c_code, course_code) ) {
			fputs(n_block, fw);
		}
		else {
			fputs(block, fw);
		}
		free(c_code);
	}
	fclose(fp);fclose(fw);free(block);
	remove(LECFILE);
	rename(TEMPFILE,LECFILE);
}

int set_courseoffquota_by_course_code(int offset_quota,char* course_code) {
	FILE *fp;
	fp = open(LECFILE,"r+");
	char* block = (char*) malloc(LINESIZE);
	
	while(fgets(block, LINESIZE, fp)) {
		char* c_code = get_unique_element(block, lecture_course_code);
		if( !strcmp(c_code, course_code) ) {
			char *quota = get_unique_element(block, lecture_quota);
			int n_quota = atoi(quota);
			
			n_quota += offset_quota;
			
			
			sprintf(c_code, "%d", n_quota);
			char* n_block = set_unique_element(block, c_code, lecture_quota);
			
			int space_need = strlen(c_code) - strlen(quota);
			if (space_need < 0) {
				set_cursor_previous_line_begin(fp);
				
				write_space_line_off(fp, space_need);
				
				set_cursor_previous_line_begin(fp);
				fputs(n_block, fp); 
				fclose(fp);
			}
			else {
				//fp içinde kapatıldı
				recreation_of_lecture_file( fp, course_code, n_block );
			}
			
			free(block); free(quota);free(n_block) ; free(c_code);
			return SUCCESS;
		}
		free(c_code);
	}
	printf("kurs bulunamadı");
	free(block); fclose(fp);
	return FAIL;
}

int implement_teacher_id() {
	FILE *fp;
	fp = fopen(TEACHERFILE, "r");
	if(fp == NULL) {
		return 1;
	} 
	fseek(fp, 0, SEEK_END);
	
	set_cursor_previous_line_begin(fp);
	char* block = (char*) malloc(sizeof(LINESIZE));
	fgets(block, LINESIZE, fp);
	char* id = get_unique_element(block, teacher_id);
	int result = atoi(id);
	
	free(id);free(block);
	return result + 1;
}

/*bu fonksiyon kullanıcıdan akademisyen değerlerini alır .(versiyon otomatik artışlı id.) */
TEACHER* create_teacher() {
	TEACHER *teacher = (TEACHER *) malloc(sizeof(TEACHER));
	int i;
	
	teacher->id = implement_teacher_id();
	printf("akademisyenin adı : ");
	scanf("%[^\n]%*c", teacher->name);
	printf("akademisyenin soyadı : ");
	scanf("%[^\n]%*c", teacher->surname);
	
	char* degree = choosing_degree();
	
	for( i = 0 ; degree[i] != '\0' ; i++) {
		teacher->degree[i] = degree[i];
	}
	teacher->degree[i] = '\0';
	return teacher;
}

int add_teacher_to_teacher_file(TEACHER* teacher) {
	FILE *fp;
	
	fp = open(TEACHERFILE, "a");
	
	if( fp != NULL ) {
		fprintf(fp,"%d%s%s%s%s%s%s\n",  teacher->id, SEP,
										teacher->name, SEP,
										teacher->surname, SEP,
										teacher->degree);
	}
	fclose(fp);

	return SUCCESS;
}    

int remove_course(char* course_code) {
	FILE *fp;
	fp = open(LECFILE, "r+");
	int credit = 0;
	char* block = (char*) malloc(LINESIZE);
	while(fgets(block, LINESIZE, fp)) {
		char* c_code = get_unique_element(block, lecture_course_code);
		if( !(strcmp(c_code, course_code) ) ) {
			char* t_id = get_unique_element(block, lecture_teacher_id);
			printf("akademisyen id : %s ", t_id);
			t_id[strlen(t_id)-1] = '\0'; // sonunda new line karakteri var.
			char* name = get_name_surname_from_teacher_id(t_id);
			printf("adı %s olan bilgierini güncellemelidir.", name);
			char* credit_b = get_unique_element(block,lecture_course_credit);
			credit = atoi(credit_b);
			set_cursor_previous_line_begin(fp);
			write_space_line(fp);
			char* unknown = create_unknown(UNKNOWN_COURSE_COMPONENT);
			set_cursor_previous_line_begin(fp);
			fputs(unknown,fp);
			free(t_id);free(name);free(unknown);free(credit_b);
		}
	}
	fclose(fp);
	
	fp = open(LOGFILE, "r+");
	
	while(fgets(block, LINESIZE, fp)) {
		char* c_code = get_unique_element(block, log_course_code);
		if( !strcmp(c_code, course_code) ) {
			char* s_no = get_unique_element(block, log_student_no); 
			char* n_block = set_unique_element(block,BIRAKTI, log_registration_state);
			set_cursor_previous_line_begin(fp);
			fputs(n_block,fp);
			update_student_lect_rcred_by_std_no(s_no, credit);
			free(n_block);free(s_no);
		}
		free(c_code);
	}
	fclose(fp);
	return 0;
}

int remove_teacher_by_id(char* t_id) {
	FILE *fp, *fp2;
	int is_found = 0;
	fp = open(TEACHERFILE, "r+");
	
	char* block = (char*) malloc(LINESIZE);
	
	while(fgets(block, LINESIZE, fp)) {
		char* t_id_= get_unique_element(block, teacher_id);
		if( !strcmp( t_id_, t_id)) {
			is_found = 1;
			
			set_cursor_previous_line_begin(fp);
			write_space_line(fp);
			
			char* unknown = create_unknown(UNKNOWN_TEACHER_COMPONENT);
			
			set_cursor_previous_line_begin(fp);
			fputs(unknown, fp);
			
			fclose(fp);
		}
	}
	
	if( is_found == 0 ) { fclose(fp); free(block); return FAIL; } 
	
	fp = open(LECFILE, "r+");
		////birden fazla okuma
		while(fgets(block , LINESIZE, fp)) {
			char* t_id_ = get_unique_element(block, lecture_teacher_id);
			t_id_[strlen(t_id_)-1] = '\0';
			
			if( !strcmp(t_id, t_id_)) {
				char* c_code = get_unique_element(block, lecture_course_code);
				char* c_credit = get_unique_element(block, lecture_course_credit);
				//ders müfredattan kaldırılmalı
				char* unknown_lec = create_unknown(UNKNOWN_COURSE_COMPONENT);
				
				set_cursor_previous_line_begin(fp);
				write_space_line(fp);
				int end_line = ftell(fp);
				set_cursor_previous_line_begin(fp);
				int line_begin = ftell(fp);
				
				fputs( unknown_lec, fp );
				
				fseek(fp,end_line - (line_begin + strlen(unknown_lec)), SEEK_CUR);
				
				//dersi alan tüm kişilerin kredileri güncellenmeli
				fp2 = open(LOGFILE, "r+");
				while(fgets(block, LINESIZE, fp2)) {
					char* l_c_code = get_unique_element(block, log_course_code);
					char* reg_stat = get_unique_element(block, log_registration_state);
 					if( !strcmp(l_c_code, c_code) && !strcmp(reg_stat, KAYITLI)) {
						char *s_no = get_unique_element(block, log_student_no);
						// bıraktı yapılabilinir ya da log diye unknown yapmadım 
						char *new_value = set_unique_element(block, BIRAKTI, log_registration_state);
						int i;
						for(i = 0 ; new_value[i] != EOL ; i++) {}
						new_value[i+1] = '\0'; // değer 1 taşırıyordu bunu kullandım.

						set_cursor_previous_line_begin(fp2);

						fputs(new_value, fp2);
						
						update_student_lect_rcred_by_std_no(s_no, atoi(c_credit));
				
						free(s_no); free(new_value);
						
					}
					free(l_c_code);
				}
				free(c_code); free(c_credit); free(unknown_lec);
			}
			free(t_id_);
		}
	//printf("başarısız");
	return 0;
}


char* get_teacher_block_by_id(char* t_id) {
	FILE *fp;
	fp = open(TEACHERFILE, "r");
	char* block = (char*) malloc(LINESIZE);
	while( fgets(block, LINESIZE, fp ) ) {
		char* t_id_ = get_unique_element(block, teacher_id);
		if( !strcmp(t_id, t_id_) ) {
			printf("%s",block);
			fclose(fp);free(t_id);
			return block;
		}
	}
	printf("aradığınız akademisyen bulunmamaktadır.n");

	fclose(fp);
	return block; // null
}

int is_teacher_available(char* t_id) {
	FILE *fp;
	fp = open(TEACHERFILE, "r");
	char* block = (char*) malloc(LINESIZE);
	while( fgets(block, LINESIZE, fp ) ) {
		char* t_id_ = get_unique_element(block, teacher_id);
		if( !strcmp(t_id, t_id_) ) {
			fclose(fp);
			return SUCCESS;
		}
	}
	printf("aradığınız akademisyen bulunmamaktadır.n");

	fclose(fp);
	return FAIL;
}


void set_teacher_newname_by_id(char* t_name, char* t_id) {
	FILE *fr,*fw;
	fr = open(TEACHERFILE, "r");
	fw = open(TEMPFILE, "w+");
	char* block = (char*) malloc(LINESIZE);
	while( fgets(block, LINESIZE, fr) ) {
		
		char* t_id_ = get_unique_element(block, teacher_id);
		
		if(!strcmp(t_id_, t_id) ) {
			
			char* n_block = set_unique_element(block, t_name, teacher_name);
			
			fputs(n_block,fw);
			free(n_block);
			
		} else {
			fputs(block, fw) ;
		}
	}
	free(block);
	
	remove(TEACHERFILE);
	rename(TEMPFILE, TEACHERFILE);
	printf("başarıyla tamamlandı\n");
}

char* choosing_degree() {
	char* unvanlar[] = {"Uzman", "Öğretim Planlamacısı", "Çevirmen", "Okutman",
		"Öğretim Görevlisi", "Öğretim Üyesi", "Araştırma Görevlisi", 
		"Araştırma Görevlisi Doktor", "Yardımcı Doçent", "Yardımcı Doçent Doktor", 
		"Doçent", "Doçent Doktor", "Profesör", "Profesör Doktor", "Ordinaryüs"
	};
	int option = -1;
	
	while( option > 15 || option < 0 ) {
		
		printf("Akademik Unvan\n");
		printf("1.Uzman-2.Öğretim Planlamacısı-3.Çevirmen-4.Okutman-5.Öğretim Görevlisi-6.Öğretim Üyesi 7.Araştırma Görevlisi" 
			"\n 8.Araştırma Görevlisi Doktor" "9.Yardımcı Doçent-" "10.Yardımcı Doçent Doktor-" 
			"\n11.Doçent-" "12.Doçent Doktor-" "13.Profesör-" "14.Profesör Doktor-" "15.Ordinaryüs");
		printf("\nbirini seçiniz : ");
		scanf("%d",&option);
		option--;
	}
	return unvanlar[option]; 	
}

void update_degree_by_id(char* t_id) {
	FILE *fr,*fw;
	fr = open(TEACHERFILE, "r");
	fw = open(TEMPFILE, "w+");
	char* block = (char*) malloc(LINESIZE);
	
	while( fgets(block, LINESIZE, fr) ) {
		
		char* t_id_ = get_unique_element(block, teacher_id);

		if(!strcmp(t_id_, t_id) ) {
			printf("Değiştirilmek istenilen ");
			char* degree = choosing_degree();
			char* n_block = set_unique_element(block, degree, teacher_degree );
			fputs(n_block, fw);
			free(n_block);

		} else {
			fputs(block, fw) ;
		}
	}
	
	free(block);
	
	remove(TEACHERFILE);
	rename(TEMPFILE, TEACHERFILE);
	printf("başarıyla tamamlandı\n");
}

/*
 * sonradan 2 boyutlu dizi döndürebilir.
 * */
void get_courses_of_student( int std_no ) {
	FILE *fp;
	fp = open(LOGFILE, "r");
	char* st_no = integer_to_string(std_no, PARTSIZE);
	char* block = (char*)malloc(LINESIZE);
	printf( "Şu kurs ları almaktasınız \n" );
	while( fgets( block, LINESIZE, fp ) ) {
		char* st_no_ = get_unique_element(block, log_student_no);
		char* r_s = get_unique_element(block, log_registration_state);
		if( !strcmp(st_no_, st_no) && !strcmp(r_s, KAYITLI) ) {
			char* c_code = get_unique_element(block, log_course_code);
			char* c_block = get_course_block_by_course_code( c_code );
			printf("%s \n", c_block);
			free(c_code);free(c_block);
		}
		free(st_no_);free(r_s);
	}
	fclose(fp);free(block);
}
/*
 * Öğrenci numarası verilen öğrenciyi kontrollerle Log dosyasına yazar.
 * */
int add_student_to_lecture_byno(char* st_no, char* c_code, int max_credit, int max_lec) {
	FILE* fp;
	
	int id = get_last_id_log();
	
	char* block = get_student_block_by_student_no(atoi(st_no));
	
	if (block == NULL) {
		printf("Öğrenci bulunmamaktadır.");
		return FAIL;
	}
	free(block);

	block = get_only_course_block_by_course_code(c_code);
	if( block == NULL ) {
		printf("Kurs bulunmamaktadır.");
		return FAIL;
	}
	
	free(block);
	
	if(  is_course_available_quota(c_code) && is_student_add_lec_available(st_no, c_code, max_credit, max_lec) ) {
		fp = fopen(LOGFILE, "a");
		int credit = get_lec_credit(c_code);
		
		update_student_lect_icred_by_std_no(st_no, credit);
		
		char* date = get_system_date();
		fprintf(fp, "%d%s%s%s%s%s%s%s%s",id+1, SEP,
							  c_code, SEP,
							  st_no, SEP,
							  KAYITLI, SEP,
							  date);
		printf("%s : bir ders yazıldı.\n",c_code);
		fclose(fp); 
		return SUCCESS;
	}
	
	
	return FAIL;
}

char* get_system_date() {
	time_t mytime ;
	time(&mytime);
	return asctime(localtime(&mytime));
}

/*logda tutulan kayıtlı degeri sona kayıtlı durumu 1 olarak taşır.*/
int move_log_to_end(char* l_id, char* l_block) {
	FILE *fp; 
	invert_log_by_log_id(l_id);
	fp = fopen(LOGFILE, "a");
	int id = get_last_id_log();
	
	char* value = integer_to_string(id,LINESIZE);
	char* block = set_unique_element(l_block, value, log_id);
	//printf("%s", block);
	
	char* time = get_system_date();
	sprintf(value,"%s",time);
	
	printf("-- tarih %s\n",time);
	char* n_block = set_unique_element(block, value, log_date);
	if(fputs(n_block, fp) != EOF) {
		//printf("\n%s", n_block);
		free(block);free(n_block);fclose(fp);free(value);
		return SUCCESS;
	}
	//null kontrolü yapılmalı.
	return FAIL;
}



/*öğrencinin mevcut ders sayısını ve derslerinin kredi durumunu sayan ve 
 * fonksiyonda verilen dersin 
 * alınabilirliğini belirleyen fonksiyon.
 * öğrenci ekleme uygun mu ekli ise eklemeyip tarih güncelleyecek 
 * eski kaydı BIRAKTI yapıp yeni 
 * kayıt oluşturup onu kaydedecek + kredi ders sayısı uygunluğu kontrol edilecek.
 *  */
int is_student_add_lec_available(char* st_no, char* given_course, int max_credit, int max_lec ) {
	FILE *fp;
	fp = fopen(LOGFILE, "r");
	char* block = (char*)malloc(LINESIZE*sizeof(char));
	int std_credit = 0, std_lec_number = 0;
	while( fgets(block, LINESIZE, fp) != NULL) {
		char* std_no_log = get_unique_element(block, log_student_no);
		char* reg_stat_log = get_unique_element(block, log_registration_state);
		char* course_name_log = get_unique_element(block, log_course_code);

		//zaten kayıtlıysa kaydı bıraktıya almalı ki kursu tekrar eklesin yani aldığı dersi silsin. 
		if ( !strcmp(std_no_log, st_no) && !strcmp(course_name_log, given_course) && !strcmp(reg_stat_log, KAYITLI)) {
			char* id_log = get_unique_element(block, log_id);
			//printf("%s",id_log);
			move_log_to_end(id_log, block); // bu fonksiyon kayıtlı olunan dersi sona taşıyacak.(bu öğrenci tekrar kaydolmaya çalışmış bilgisi)
			free(std_no_log);free(reg_stat_log);free(course_name_log);free(id_log);free(block);fclose(fp);
			return FAIL;
		}
		
		if( !strcmp(std_no_log, st_no) && !strcmp(reg_stat_log, KAYITLI) ) {
			char* lec_name_log = get_unique_element(block, log_course_code);
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
		return SUCCESS;// ders alınabilir.
	}
	
	return FAIL;
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

/*
 * Kontenjan uygunluğunu kontrol eden fonksiyon.
 * artan numara öğrenciNO- Derscode
 * */ 
int is_course_available_quota(char* c_code) {
	FILE *fp,*fp2; 
	fp = open(LOGFILE, "r");
	char* block = (char*) malloc(LINESIZE);
	int lec_students = 0;
	while( fgets(block, LINESIZE, fp) ) { // ders kodu , kayıt durumu enum sayilar
		//printf("s: %s", block);
		//printf("%d", strlen(block));
		char* reg_log = get_unique_element(block, 1);
		//printf("   %d", strlen(block));
		char* course_code_log = get_unique_element(block, log_course_code);
		//printf("s: %s", course_code_log);
		//printf("\n,,,,%d,,,,",reg_log);	
		if(!strcmp(course_code_log, c_code) && !strcmp(reg_log, KAYITLI)) 
			lec_students++;
		free(course_code_log); free(reg_log);
	}
	//printf("lec stud %d \n", lec_students);
	//printf("%s",c_code);
	
	fp2 = fopen(LECFILE, "r") ;
	while( fgets(block, LINESIZE, fp2) != NULL ) {
		//printf("\n%s",block);
		char* course_code_lec = get_unique_element(block,lecture_course_code);
		char* quota_lec = get_unique_element(block, lecture_quota); 
		if ( !strcmp(course_code_lec, c_code) ) {
			if(atoi(quota_lec) > lec_students) {
				free(block); free(quota_lec); free(course_code_lec);fclose(fp);
				return SUCCESS ;
			} 
		}
		free(quota_lec); free(course_code_lec);
	}
	free(block);
	return 0;
}

int get_last_id_log() {
	FILE * fp;
	fp = fopen(LOGFILE, "r");
	
	char* block = (char*) malloc(sizeof(char)*LINESIZE);
	if ( fp ) {
		fseek(fp,0,SEEK_END);
		set_cursor_previous_line_begin(fp);
		fgets(block, LINESIZE, fp);
		char* l_id = get_unique_element(block, log_id);
		int idx = atoi(l_id);
		free(block); free(l_id); fclose(fp);
		return  idx ;
	}
	free(block); fclose(fp);
	return -1;
}

int remove_student_from_lecture_byno( char* st_no, char* c_code ) {
	FILE* fp;
	fp = open(LOGFILE, "r+");
	
	char* block = get_student_block_by_student_no(atoi(st_no));
	int line_start_point = 0;
	
	while( fgets(block, LINESIZE, fp) ) {
		// alttaki kısım tekrar yeni fonksiyon yazılabilir.
		char* st_no_ = get_unique_element(block, log_student_no);
		char* c_code_ = get_unique_element(block, log_course_code);
		if( !strcmp(st_no, st_no_) && !strcmp(c_code, c_code_) ) {
			char* state = get_unique_element(block, log_registration_state);
			char fstate[3] ; sprintf(fstate,",%s,",state) ;
			
			char *stat_pos_ptr = strstr (block, fstate); 
			
			int pos_to_invert = ( stat_pos_ptr - block ) + 1;
			
			fseek(fp, - (ftell(fp)- line_start_point) + pos_to_invert, SEEK_CUR);
			
			printf("%d\t",(int)(fstate[1]));
			printf("%ld ",ftell(fp));
			fputc(!(fstate[1]-48) + 48 , fp); // ascii ile tersini yazdım.0-1
			printf("%c ", !(fstate[1]-48) + 48 );
			printf("id is found");
			
			int credit = get_lec_credit(c_code);
			update_student_lect_rcred_by_std_no(st_no, credit);
		
			free(state); free(block); fclose(fp);
			return SUCCESS;
		}
		line_start_point = ftell(fp);
	}
	
	free(block);
	return FAIL;
}


/*
 * sonradan dizi dönebilir
 * */
void get_courses_by_teacher_id( int teacher_id ) {
	FILE *fp;
	fp = open( LECFILE, "r" );
	
	char* (*cast)() = integer_to_string;
	int is_lec_available = 0;
	
	char* block = (char*) malloc ( LINESIZE );
	while( fgets( block, LINESIZE, fp ) ) {
		char*  t_id = get_unique_element(block, lecture_teacher_id);
		char* t_id_ = cast(teacher_id);
		t_id[strlen(t_id)-1] = '\0';
		
		if( !strcmp( t_id, t_id_ ) ) {
			char* c_name = get_unique_element(block, lecture_course_name);
			printf("%s\n", c_name);
			is_lec_available = 1;
			free(c_name);
		}
		free(t_id);free(t_id_);
	}
	if(is_lec_available == 1 ) {
		fclose(fp);
		printf("derslerine girmektesiniz.");
		return;
	}
	fclose(fp);
	printf("verdiğiniz ders bulunmamaktadır.");
}

/*
 * not bu kodda verilen stringe belli uzulukta veri yazılmakta.
 * 
	Sınıf kodu ile sınıf listesi (menüde id sini giren akademisyene sunulan seçenek ) dosyasını dönen fonksiyon.
 * */
int create_file_for_class_by_course_code(char* c_code) {
	FILE *fw, *fr;
	int len_c_cd = strlen(c_code) ;
	
	char *tail_name = FILE_TAIL_NAME;
	
	strcat(c_code, tail_name);
	int (*cast)() = atoi;
	
	printf("%s ", c_code);
	printf("oluşturulmakta.\n");
	
	fw = open( c_code, "w+" );
	
	c_code[len_c_cd ] = '\0';
	printf("%s", c_code);
	fr = open( "deneme.txt" , "r" );
	
	int list_id = 1;
	char* block = (char*) malloc(LINESIZE);
	while( fgets(block, LINESIZE, fr) ) {
		char *s_c_code = get_unique_element(block, log_course_code);
		char *s_no = get_unique_element(block, log_student_no);
		
		char *s_block = get_student_block_by_student_no(cast(s_no));
		
		if( !strcmp(s_c_code, c_code)) {
			printf("%s", s_block);
		
			//// istenmeyen blokların uzunluğu alınacak . ekleme olursa değils
			char *name_surname = get_name_surname_from_student_id(s_no);
			
			char* l_id = integer_to_string(list_id, PARTSIZE);
			fputs( l_id, fw); 
			fputc(SEP_INFO,fw);
			fputs(s_no,fw );
			fputc(SEP_INFO,fw);
			fputs(name_surname,fw);
			fputc(EOL,fw);
			
			free(name_surname);
			
			list_id++;
		}
		free(s_c_code);free(s_no);free(s_block);
	}
	
	free(block);
	if(feof(fr)) {
		fprintf(fw,"Sınıf mevcudunuz %d\n", list_id);
		printf("oluşturulma başarılı \n.");
		fclose(fw); fclose(fr);
		return SUCCESS;
	}
	fclose(fw); fclose(fr);
	printf("oluşturma başarısız.\n");
	
	return FAIL;
}


int main() {
	//STUDENT* stud = create_student();
	//add_student_to_student_file(stud);
	
	setlocale(LC_ALL,"Turkish");
	
	set_student_newname_by_no("ali",10011000);
	//set_coursename_by_course_code("Yapisal Programlama","MAT1010");
	
	//set_courseoffquota_by_course_code(20, "bbg2");
	
	//int max_lec = 20;
	//int max_credit = 10;
	//add_student_to_lecture_byno("18011078", "BLM1012", max_lec, max_credit );
	//add_student_to_lecture_byno("123", "bbg0", max_lec, max_credit );
	//add_student_to_lecture_byno("13", "AKA1000", max_lec, max_credit );
	//add_student_to_lecture_byno("18011078", "TIP1016", max_lec, max_credit );
	//add_student_to_lecture_byno("125", "MAT1010", max_lec, max_credit );
	//add_student_to_lecture_byno("18011078", "YEME018", max_lec, max_credit );
	
	//update_student_lect_icred_by_std_no("18011078", 2);
	
	//file_initialization();
	//int id = get_last_id_log();
	
	//char code[50] = "mat1010";
	//create_file_for_class_by_course_code(code);
	
	//COURSE* course = create_course();
	
	//add_course_to_course_file( course );
	
	//char* block = get_unique_element("ali baba,çiftlik,inek,kuzu,köpek,kedi",4);
	//printf("%s", block);
	
	
	//char a[LINESIZE] = "3,bbg,18011078,1,Fri Jan  1 17:00:16 2021";
	//char* block = strdup(a);
	
	//int max_lec = 20;
	//int max_credit = 10;
	
	//get_courses_by_teacher_id(3);
	
	//STUDENT student ;
		
	//sscanf("18011078,onur,demir,9,21" ,"%d,%s,%s,%d,%d" , &student.stdno, student.name, student.surname ,&student.numb_of_lect, &student.numb_of_credit);
	//printf("%d",student.stdno);


	//remove_student_from_lecture_byno("18011078","mat1010");
	
	
	
	//add_student_to_lecture_byno("18011078", "BLM1012", max_lec, max_credit );
	//add_student_to_lecture_byno("123", "bbg0", max_lec, max_credit );
	//add_student_to_lecture_byno("13", "AKA1000", max_lec, max_credit );
	//add_student_to_lecture_byno("18011078", "TIP1016", max_lec, max_credit );
	//add_student_to_lecture_byno("125", "MAT1010", max_lec, max_credit );
	//add_student_to_lecture_byno("18011078", "YEME018", max_lec, max_credit );
	
	//is_course_available_quota("BLM1010");

	//invert_log_by_log_id("8");
	
	//move_log_to_end("3", block);
	
	//int available = is_student_add_lec_available("18011078","bbg", 5, 20);
	
	//printf("Available State 0 to false 1 to true 18011078 bbg 5 20 --> %d \n", available);
	//int available2 = is_student_add_lec_available("18011","bbglab", 5, 20);
	//printf("Available State 0 to false 1 to true 18011 bbglab 5 20 --> %d", available2);
	//if(is_course_available_quota("MAT1010")) {
		//printf("MAT1010 kursunun kontenjanı müsaittir.");
	//}
	//update_degree_by_id("5");

	//get_courses_of_student(18011078);

	//char * sample = choosing_degree();
	//printf("%s",sample);
	
	//set_teacher_newname_by_id("faruk", "2");
	
	//char block[5000] = "3,bbg,18011078,1,frijan11700162020\n";
	//char *new_value = set_unique_element(block, BIRAKTI, log_registration_state);

	//printf("%s",new_value);
	
	//remove_teacher_by_id("3");

	//get_teacher_block_by_id("2");

	//TEACHER teacher = { 100, "ferkan", "neyzen", "uzman" };
	
	//add_teacher_to_teacher_file(create_teacher());
	
	//set_coursename_by_course_code("Yapisal Programlama","MAT1010");
	
	//printf("implement_teacher_id %d",implement_teacher_id());
	
	//TEACHER* teacher = create_teacher();
	
	//printf("%d",teacher->id);
	//printf("%s",teacher->name);
	//printf("%s",teacher->surname);
	//printf("%s",teacher->degree);
	
	
	
	//set_courseoffquota_by_course_code(10, "BLM1012");
	
	//COURSE* course = create_course();
	//remove_course("BLM1011");
	
	//get_course_block_by_course_code("BLM1011");
	
	//update_student_lect_rcred_by_std_no("18011078", 5);
	//printf("%s", get_name_surname_from_teacher_id("2"));
	//add_course_to_course_file( course );
	
	//printf("%s",get_student_block_by_student_no(18011078));
	
	//char a[100] = "18011078,Onur,Demir,0,0";
	//char *b = set_unique_element(a,"ahsen",1);
	//printf("%s",b);
	//set_student_newsurname_by_no("atsiz",18011078);
	
	//COURSE *mycourse = create_course();
	//printf("%s", mycourse->name);
	//printf("%s", mycourse->code);
	//printf("%d",mycourse->t_id);
	
	//int true=remove_student_from_student_file_by_no(18011078);
	//char** array;
	//if(true) { array = update_log_file_by_removed_student_no(18011078); } 
	
	////char* a = array[0];
	
	////printf("%s", a);
	////for(int i = 1 ; i <= atoi(a) ; i++) {
		////printf("%s", array[i]);
	////}
	
	//printf("%s",get_name_surname_from_teacher_id("3"));
	//warning_teacher_of_course(array);
	
	//printf("Ders Kayıt Uygulamasına Hoşgeldiniz!\n"
			//"Lütfen bir öğrencinin alabileceği maksimum ders sayısını belirleyiniz. \n");
	//printf("maksimum ders sayısı : ");
	//scanf("%d", &max_lec);
	
	//printf("Lütfen bir öğrencinin alabileceği, maksimum kredi sayısını belirleyiniz. \n");
	//printf("maksimum kredi sayısı : ");
	//scanf("%d", &max_credit);

	//while( option != '0' ){
		//// Ayrıntılar için plan.txt dosyası
		//printf("Admin Menü \n");
		//printf("(1) - Öğrenci Kayıt İşlemleri\n"
			   //"(2) - Ders Ekleme İşlemleri\n" 
			   //"(3) - Akademisyen Ekleme Işlemleri\n");
		//printf("(7) - Akademisyen Sınıf Öğrenme.\n");
		//printf("Kullanıcı Menü \n");
		//printf("(4) - Derse Kayıt\n");
		//printf("(5) - Öğretim görevlisinin verdiği dersler(by id) \n");
		//printf("(6) - Öğrencinin aldığı dersler\n");

		//printf("işleminiz (0 to quit)  : ");
		//scanf("%c", &option);
		
		//STUDENT student;
		//COURSE course;
		//TEACHER teacher;
		
		//char option2, sure, option3;
		//char** course_codes_leave_by_student, teacher_of_course, course_codes;
		
		//char* course_code, course_name,course_quota,newsurname,newname,info;
		//int teacher_id, offset_quota,std_no;
		//char* t_name, t_surname;
		//char* temp;
		//switch(option) {
			//case '1':
				//printf("\tÖğrenci İşlemleri\n");
				
				//printf("(1) Öğrenci kayıt\n"
					   //"(2) Öğrenci sil\n"
					   //"(3) Öğrenci bilgi erişim ve güncelleme\n");
				
				//printf("seçiminiz (0 to quit) : ");
				//scanf("%c", &option2);
				//switch(option2) {
					//case '0':
						//option = '0';
						//break;
					//case '1':
						//student = create_student();
						//if(add_student_to_student_file(student)) != NULL) {
							//printf("öğrenci ekleme başarılı\n");
							//break;	 
						//}
						//printf("öğrenci ekleme başarısız\n");
						//break;
					//case '2':
						
						//printf("Öğrenci silme işlemindesiniz. Onaylıyor musunuz?(y/n) :");
						//scanf("%c", sure);
						//if ( sure == 'y' ) {
							//printf("Silinecek öğrenci numarasını giriniz.");
							//scanf("%d", std_no);
							//if(remove_student_from_student_file_by_no(std_no)) {
								//course_codes = update_log_file_by_removed_student_no(std_no);
								//int erased_reg_size = atoi(course_codes_[0]);
								//for( int i = 1 ; i < erased_reg_size ; i++_) {
									//char* course_code = course_codes[i];
									//warning_teacher_of_course(course_code);
									//print_tab(teacher); printf("\n");
								//}
								
								//printf("\t%d ders kaydı bırakıldı.\n", erased_reg_size);
								//break;
							//}
							//printf("Öğrenci bulunmamaktadır.");
						//} 
						//break;
					//case '3':
						
						//printf("Bilgisini alacağınız öğrencinin numarasını giriniz.");
						//scanf("%d",&std_no);
						//info = get_student_block_by_student_no(std_no);
						//if(info == NULL) { printf("aradığınız öğrenci bulunmamaktadır.\n"); break;}
						//printf("%s\n", info);
						//printf("\t Güncellenecek bilgi ");
						//printf("(1) AD\n");
						//printf("(2) Soyad\n");
						//printf("seçiminiz (0 to quit)");
						//scanf("%c", &option3);
						//switch (option3) {
							//case '1':
								
								//printf("Yeni adı giriniz: ");
								//scanf("%s",newname)
								//set_student_newname_by_no(newname, std_no);
								//break;
							//case '2':
								
								//printf("Yeni soyadı giriniz: ");
								//scanf("%s", newsurname);
								//set_student_newsurname_by_no(newsurname, std_no);
								//break;
							//default:
								//option = '0';
								//break;
						//}	
						//break;
					//default:
						//printf("Böyle bir işlem tanımlanmamıştır.\n"
							   //"çıkılıyor...\n");
						//break;
				//}
				//break;
			//case '2':
				
				//printf("\tDers Ekleme İşlemleri\n");
				//printf("(1) - Ders Ekle\n"
					   //"(2) - Ders Sil\n"
					   //"(3) - Ders Güncelle\n");
				//switch(option2) {
					//case '1':
						//course = create_course();
						//if(add_course_to_course_file(course) ) {
							//printf("ders ekleme başarılı \n");
							//break;	
						//}
						//printf("ders ekleme başarısız.\n");
						//break;
					//case '2':
						
						//printf("ders silme işlemindesiniz onaylıyor musunuz?(y/n)\n");
						//scanf("%c",&sure);
						//if(sure == 'y') {
							//printf("Silmek istediğiniz dersin kodunu giriniz : ");
							//scanf("%s", course_code);
							//if(remove_course(course_code)) {
								//printf("ders başarıyla silindi.");
								//break;
							//} 
							//printf("ders silme başarısız.");
							//break;
						//}
						//break;
					//case '3':
						
						//printf("\tDers güncellemektesiniz.\n");
						//printf("Güncellemek istediğiniz dersin kodunu yazınız : ");
						//scanf("%s", course_code);
						//info = get_course_block_by_course_code(course_code);
						//printf("%s\n", info);
						//printf("(1) Ders adı değiştir.\n" 
							   //"(2) Kontenjan arttır.\n");
						//printf("seçiminiz(0 to quit) : ");
						//scanf("%c", &option3);
						
						//switch(option3){
							//case '1':
								//printf("Yeni ders adı : ");
								//scanf("%s",course_name);
								//if(set_coursename_by_course_code(course_name,course_code)){
									//printf("kurs adı değiştirme başarılı.\n"); break;
								//}
								//printf("ders adı değiştirme başarısız"); break;
							//case '2':
								//printf("Arttırmak istediğiniz kontenjanı giriniz.");
								//scanf("%d", &offset_quota);
								//if(set_courseoffquota_by_course_code(offset_quota, course_code)){
									//printf("kurs kontenjanı %d kadar artırıldı.\n", offset_quota); break; 
								//}
								//printf("kontenjan arttırma başarısız.\n");
								//break;
							//default:
								//break;
						//}
					//default:
						//break;
				//}
				//break;
			//case '3':
				////printf("\tAkademisyen Ekleme İşlemleri\n");
				//printf("(1) Akademisyen oluştur.\n"
					   //"(2) Akademisyen sil.\n" 
					   //"(3) Akademisyen bilgisi gör veya güncelle \n");
				//printf("seçiminiz : ");
				//scanf("%s", option2);
				//switch(option2){
					//case '1':
						//teacher = create_teacher();
						//add_teacher_to_teacher_file(teacher);
						//printf("Akademisyen ekleme işlemi tamamlandı.\n");
						//break;
					//case '2':
						//printf("silmek istediğiniz akademisyenin id sini giriniz.");
						//scanf("%s",temp);
						//if(remove_teacher_by_id(temp) ) {
							//printf("akademisyen silindi.\n");break;
						//}
						//printf("böyle bir akademisyon yok\n");
					//case '3':
						//printf("görmek istediğiniz akademisyenin id sini giriniz.");
						//scanf("%s",temp);
						//printf("Akademisyen bilgisi \n");
						//get_teacher_block_by_id(temp);
						//printf("(1) ad güncelle , (2) soyad güncelle (3) unvan güncelle");
						//scanf("%s"
						//switch(option3){
							//case '1':
								//printf("güncel ad giriniz. : ");
								//scanf("%s",t_name);
								//set_teacher_newname_by_id(t_name, temp);
								//break;
							//case '2':
								//printf("güncel soyad giriniz : ");
								//scanf("%s",t_surname);
								//set_teacher_surname_by_id(t_surname, temp);
								//break;
							//case '3':
								//printf("ünvan bilgisi güncellenecektir.");
								//update_degree_by_id(temp);
								//break;
						//} 
					//default:
						//printf("yanlış seçim");
						//break;
				//}
				//break;
			//case '4':
				//printf("\tDers Kayıt\n");
				//printf("derse kayıt işlemindesiniz öğrenci numaranızı giriniz. ");
				//scanf("%d",std_no);
				//get_courses_of_student(std_no);
				//printf("(0) çıkış yap \n(1) kayıt yap\n");
				//scanf("%s",option2);
				//switch(option2){
					//case '0':
						//printf("ders kayıt işleminden çıkılıyor.");
						//break;
					//case '1':
						//printf("Kayıt işlemleri\n");
						//printf("(1) derse_kayıt_ol\n"
							   //"(2) dersi bırak\n");
						//printf("seçiminizi giriniz(0 to quit)");
						//scanf("%c",&option3);
						//switch(option3){
							//case '1':
								//printf("almak istediğiniz dersin adı nedir? : ");
								//scanf("%s",course_name);
								//if(add_student_to_lecture_byno(std_no,c_code, max_credit, max_lec)){
									//printf("derse kayıt başarılı\n");
									//break;
								//}
								//printf("derse kayıt başarısız.\n");
								//break;
							//case '2':
								//printf("Bırakmak istediğiniz dersin adı nedir? : ");
								//scanf("%s",course_name);
								//if(remove_student_from_lecture_byno(c_code, std_no)){
									//printf("ders silme başarılı\n");
									//break;
								//}
								//printf("ders silme başarısız.\n");
								//break;
							//default:
								//break;
						//}
					//default :
						//printf("yanlıs tuşa basıldı");
						//break;
				//}
				//break;
			//case '5':
				//printf("Öğretim görevlisinin verdiği dersler(by id) \n");
				//printf("Öğretim görevlisinin id si : ");
				//scanf("%d", &teacher_id);
				//printf("akademisyen :%s ;\n", get_teacher_name_by_id(teacher_id));_
				//get_courses_of_teacher(teacher_id);
				//printf("\n bu dersleri vermektedir.\n");
				//break;
			//case '6':
				//printf("\tÖğrencinin aldığı dersler\n");
				//printf("Öğrencinin numarasını giriniz. ");
				//scanf("%d", &std_no);
				//if(get_courses_of_student(std_no)){
					//printf("\n dersleriniz.");
					//break;
				//}
				//printf("numaranızı yanlış girmiş olabilirsiniz.\n");
				//break;
			//case '7':
				//printf("dersin sınıf listesini oluşturma.");
				//printf("akademisyen id_nizi giriniz. : ");
				//scanf("%d",&teacher_id);
				//printf("ilgili ders kodunuzu giriniz");
				//scanf("%s",course_code);
				//if(create_file_for_class(teacher_id,course_code)){
					//printf("Sınıf listeniz oluşturuldu.");
					//break;
				//}
				//printf("hatalı giriş");
				//break;
			//case '0':
				//printf("\tİyi günler dileriz.\n");
				//break;
			//default:
				//printf("\t İstediğiniz işlem tanımlanmamıştır.\n"
						//"\t      lütfen tekrar deneyin.\n");
		//}
	//}
}
