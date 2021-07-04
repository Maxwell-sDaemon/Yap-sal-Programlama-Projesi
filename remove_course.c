#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include "macros.h"
#include "structures.h"

int invert_log_from_id(char* log_id);
char* set_unique_element(char* string, char* value, int tabular_value);
char* get_unique_element(char* string, int tabular_value);
FILE* open(char* filename, char* mode);

STUDENT* create_student() {
	STUDENT* student = (STUDENT*)malloc(sizeof(STUDENT));
	
	printf("Öğrenci numarası giriniz : ");
	scanf("%d", &student->stdno);
	printf("Öğrenci adı : ");
	
	scanf("%s", student->name);
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

char* create_unknown(int size_component) {
	char *block = (char*)malloc(LINESIZE * sizeof(char));
	int i = 0;
	for(int j=0 ; j < size_component ; i+=2, j++) {
		block[i] = UNKNOWN;
		block[i+1] = U_SEP;
	}
	block[2 * size_component -1 ] = '\0';
	return block;
}

void set_cursor_previous_line_begin(FILE* fp) {
	fseek(fp,-2,SEEK_CUR);
	while(fgetc(fp) != '\n') {
		fseek(fp, -2, SEEK_CUR);
	}
}

char* integer_to_string(int integer, int length) {
	char *block = (char*) malloc(sizeof(char) * length);
	sprintf(block, "%d", integer);	
	return block;
}

void write_space_line(FILE *fp) {
	while(fgetc(fp) != '\n') {
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
			invert_log_from_id(block_log_id);
			free(block_log_id);
		}
		free(block_std_no); free(block_rs); 
	}
	
	free(block); fclose(fp);free(st_no);
	return erased_block;
}

int invert_log_from_id(char* log_id) {
	FILE *fp;
	char* block = (char*) malloc(200 * sizeof(char)); 
	fp = fopen(LOGFILE, "r+");
	int line_start_point = 0;
	
	while(fgets(block, 200, fp)) {
		char* id = get_unique_element(block, 0);
		if( !strcmp(id, log_id) ) {
			char* state = get_unique_element(block, 3);
			char fstate[3] ; sprintf(fstate,",%s,",state) ;
			
			char *stat_pos_ptr = strstr (block, fstate); 
			
			int pos_to_invert = ( stat_pos_ptr - block ) + 1;
			
			fseek(fp, - (ftell(fp)- line_start_point) + pos_to_invert, SEEK_CUR);
			
			putc(!(fstate[1]-48) + 48 , fp); // ascii ile tersini yazdım.
			free(block);
			return 1;
		}
		line_start_point = ftell(fp);
	}
	fclose(fp);free(block);
	printf("id is not found");
	return 0;
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
	char* no = integer_to_string(no_,100);
	
	char* block = (char*) malloc(LINESIZE);
	while(fgets(block,LINESIZE,fp)){
		char* block_no = get_unique_element(block,student_std_no);
		if(!strcmp(block_no,no)) {
			return block;
		}
	}
	free(block);
	return NULL;
}

int set_student_newname_by_no(char* newname, int no_){
	FILE *fp;
	
	fp = open(OGRENCIFILE, "r+");
	char *no = integer_to_string(no_, 100);
	
	char* block = (char*) malloc(LINESIZE);
	while(fgets(block, LINESIZE, fp)) {
		char *block_no = get_unique_element(block,student_std_no);
		if( !strcmp(block_no,no)) {
			char* student = set_unique_element(block,newname,student_std_name);
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

int set_student_newsurname_by_no(char* newsurname, int no_){
	FILE *fp;
	
	fp = open(OGRENCIFILE, "r+");
	char *no = integer_to_string(no_, 100);
	
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
int add_teacher_to_teacher_file(TEACHER* teacher) {
	FILE *fp;
	char *block = (char*) malloc(sizeof(char) * TEACHERIDSIZE) ;
	fp = open(TEACHERFILE, "a");
	if( fp != NULL ) {
		fprintf(fp,"%d%s%s%s%s%s%s\n",  teacher->id, SEP,
										teacher->name, SEP,
										teacher->surname, SEP,
										teacher->degree);
	}
	fclose(fp);
	free(block);
	return SUCCESS;
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
	fclose(fp);
	free(block);
	return SUCCESS;
}

int update_lect_rcred_by_std_no(char* stud_no, int stud_credit) {
	FILE *fp;
	fp = open(OGRENCIFILE, "r+");
	char* block = (char*) malloc (LINESIZE);
	while(fgets(block, LINESIZE, fp)) {
		char* st_no = get_unique_element(block, student_std_no);
		if( !strcmp(st_no, stud_no) ) {

			char* c_credit = get_unique_element(block, student_std_course_credit);
			c_credit[strlen(c_credit)-1] = '\0';

			int new_credit = atoi(c_credit);
			new_credit -= stud_credit;
			char* n_lect = get_unique_element(block, student_std_course_number);
			int new_course_numb = atoi(n_lect);
			new_course_numb--;
			sprintf(c_credit, "%d", new_credit);
			sprintf(n_lect, "%d", new_course_numb);
			

			char* nblock = set_unique_element(block, c_credit, student_std_course_credit);
			char* n2block = set_unique_element(nblock, n_lect, student_std_course_number);
			
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
			t_id[strlen(t_id)-2] = '\0'; // sonunda new line karakteri var.
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
			update_lect_rcred_by_std_no(s_no, credit);
			free(n_block);free(s_no);
		}
		free(c_code);
	}
	
	return 0;
}

int main() {
	//STUDENT* stud = create_student();
	//add_student_to_student_file(stud);
	
	setlocale(LC_ALL,"Turkish");
	
	//COURSE* course = create_course();
	remove_course("BLM1011");
	
	//update_lect_rcred_by_std_no("18011078", 5);
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
				//printf("\tAkademisyen Ekleme İşlemleri\n");
				//teacher = create_teacher();
				//add_teacher_to_teacher_file(teacher);
				//printf("Akademisyen ekleme işlemi tamamlandı.\n");
				//break;
			//case '7':
			
				//printf("\tAkademisyen Sınıf Öğrenme.\n");
				//break;
			//case '4':
				//printf("\tDers Kayıt\n");
				//printf("derse kayıt işlemindesiniz öğrenci numaranızı giriniz. ");
				//scanf("%d",std_no);
				//show_courses_of_student(std_no);
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
								//if(add_student_to_lecture_byno(std_no,course_name)){
									//printf("derse kayıt başarılı\n");
									//break;
								//}
								//printf("derse kayıt başarısız.\n");
								//break;
							//case '2':
								//printf("Bırakmak istediğiniz dersin adı nedir? : ");
								//scanf("%s",course_name);
								//if(remove_student_from_lecture_byno(course_name, std_no)){
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
				//list_courses_by_teacher_id(teacher_id);
				//printf("\n bu dersleri vermektedir.\n");
				//break;
			//case '6':
				//printf("\tÖğrencinin aldığı dersler\n");
				//printf("Öğrencinin numarasını giriniz. ");
				//scanf("%d", &std_no);
				//if(list_students_course(std_no)){
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
