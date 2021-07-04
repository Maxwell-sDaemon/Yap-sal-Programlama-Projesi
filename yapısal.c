#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#define FAIL 0
#define SUCCESS 1

#define KAYITLI "1"
#define BIRAKTI "0"

#define UNVANSZ 14
#define WIDTH 5 
#define HEIGHT 10  

#define STNUMSZ 7
#define TEACHERIDSIZE 4

#define LINESIZE 256

#define EOL '\n'

#define SEP ","
#define TEACHERFILE "akademisyen.txt"
#define OGRENCIFILE "öğrenci.txt"
#define LECFILE "dersler.txt"
#define LOGFILE "ÖğrenciDersKayıt.txt"

enum mylog {log_id, ders_kodu, ogrenci_no, kayit_durumu, tarih};
enum mylec {ders_kodum,dersin_adi,dersin_kredisi,kontenjan,ogretim_uyesi_id};
typedef struct {
	unsigned int day: 5;
	unsigned int month: 4;
	unsigned int year: 11;
} DATE;

typedef struct {
	int stdId;
	int course_code;
	int register_state;
	char* date;
} LOG;

typedef struct {
	char* code;
	char* name;
	int credit;
	int quota;
	int teacher_id;
	
} COURSE;

typedef struct {
	int stno;
	char* name;
	char* surname;
	int total_lect ;
	int total_lect_credit ;
} STUDENT;

typedef struct {
	int id;
	char* name;
	char* surname;
	char* degree;
} TEACHER;

FILE* open(char* filename, char* mode);
time_t get_system_date();

int list_teacher_courses(char* id) {
	return 0;
}

int list_course_students(char* course_id) {
	return 0;
}

int list_teaher_students(char* id) {
	return 0;
}

int list_students_course(char *id) {
	return 0;
}

int dosya_ekle() {
	return 0;
}

int dosya_sil() {
	return 0;
}

int dosya_guncelle() {
	return 0;
}

int add_student(STUDENT* student) {
	FILE *fp;
	char *block = (char*) malloc(sizeof(char) * STNUMSZ) ;
	fp = open(OGRENCIFILE, "a");
	if( fp != NULL ) {
		sprintf(block ,"%d", student->stno);  
		fputs(block, fp);
		fputs(SEP,fp);
		fputs(student->name, fp);
		fputs(SEP,fp);
		fputs(student->surname, fp);
		fputs(SEP,fp);
		sprintf(block ,"%d", student->total_lect);
		fputs(block, fp);
		fputs(SEP,fp);
		sprintf(block ,"%d", student->total_lect_credit); 
		fputs(block, fp);
		fputs("\n", fp);
		fputs("\0", fp);
	} 
	fclose(fp);
	free(block);
	return SUCCESS;
}

// artan numara öğrenciNO- Derscode 
int is_course_available_quota(char* course_code) {
	FILE *fp; 
	fp = open(LOGFILE, "r");
	char* block = (char*) malloc(sizeof(char)*200);
	char* temp; // 1 block erken kesmeden dolayı 1 kere get_unique içinde kullanılıyor.(optimize edilmeli.)
	int lec_students = 0;
	while( fgets(block, LINESIZE, fp) ) { // ders kodu , kayıt durumu enum sayilar
		temp = strdup(block);
		//printf("s: %s", block);
		//printf("t: %d\n", !strcmp(get_unique_element(block, 1), course_code));
		//printf("s: %s\n", temp);
		//printf("k: %d\n", !strcmp(get_unique_element(temp, 4), "1"));
		
		if(strcmp(get_unique_element(block, 1), course_code) && strcmp(get_unique_element(temp, 4), "1")) 
			lec_students++;
		free(temp);
	}
	fclose(fp);
	fp = open(LECFILE, "r") ;
	while( fgets(block, LINESIZE, fp) != NULL ) {
		temp = strdup(block);
		if ( !strcmp(get_unique_element(block,0), course_code) ) {
			if(atoi(get_unique_element(temp, 3)) > lec_students) {
				free(block);
				return SUCCESS ;
			} 
		}
		free(temp);
	}
	free(block);
	return FAIL;
}

int is_student_add_lec_available(char* student_no, int max_credit, int max_lec ) {
	
	// öğrencinin derse seçebilmesi için maksimum ders sayısı ve krediyi aşmaması gerekir.
	// bunun için derlogunu
	FILE *flog;
	flog = open(LOGFILE, "r");
	char* block = (char*)malloc(LINESIZE*sizeof(char));
	while( fgets(block, LINESIZE, flog) ) {
		get_unique_element
	}
	return 0;
}

int add_course_register(int student_no, char* course_code) {
	FILE * fp;
	fp = open(LOGFILE, "r");
	int id=0;
	char* block = (char*) malloc(sizeof(char)*LINESIZE);
	if ( fp ) {
		while( fgets(block, LINESIZE, fp) !=NULL ) {
		// Just search for the latest line, do nothing in the loop
		} 
		fclose(fp);
		sscanf(block,"%d",&id);
		id++;
	}
	
	time_t date = get_system_date();
	fp = open(LOGFILE, "a");
	
	fprintf(fp, "%d%s%d%s%s%s%s\n",id,SEP,
						  student_no,SEP,
						  course_code,SEP,
						  ctime(&date));
	return 0;
}

time_t get_system_date() {
	time_t mytime ;
	mytime = time(NULL);
	return mytime;
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

int add_course(COURSE *course) {
	FILE *fp ;
	char *block = (char*) malloc(sizeof(char) * TEACHERIDSIZE) ;
	
	fp = open(LECFILE, "a");
	
	if( fp ) {
		printf("%p acıldı \n",fp);
		fprintf(fp,"%s%s%s%s%d%s%d%s%d \n",   course->code, SEP,
										course->name, SEP,
										course->credit, SEP,
										course->quota, SEP,
										course->teacher_id);
	}
	fclose(fp);
	free(block);
	return SUCCESS;
}

int is_course_possible_id(COURSE *course) {
	FILE *fp;

	FILE* (*open_lecturer_file) () = open;
	char* (*cast) () = integer_to_string;
	
	char *block = (char*) malloc(sizeof(char) * LINESIZE);
	
	fp = open_lecturer_file(TEACHERFILE, "r");
	
	if( fp && fread(block , sizeof(char), TEACHERIDSIZE, fp) != TEACHERIDSIZE ) {
		return FAIL; // -1
	} 
	char* comparated = (char*) malloc(4);
	while( fgets(block, LINESIZE, fp) ) {
		memcpy(comparated, block, 4 * sizeof(char));
		if(strcmp(comparated, cast(course->teacher_id, TEACHERIDSIZE)) == 0) 
			return SUCCESS;
	}
	
	fclose(fp);
	free(block);
	return FAIL ;
}

FILE* open(char* filename, char* mode) {
	FILE *fp;
	if((fp = fopen(filename, mode)) == NULL) {
		printf("Couldn't open the output file"); fclose(fp); return NULL;
	}	
	return fp; 
}

char* integer_to_string(int integer, int length) {
	char *block = (char*) malloc(sizeof(char) * length);
	sprintf(block, "%d", integer);	
	return block;
}




char* get_unique_element(char* string, int tabular_value) {
	char* result = strtok(string, ",") ;
	
	for(int i = 1 ; i < tabular_value ; i++) {
		result = strtok(NULL, ",");
	}
	return result;
}

// artan numara öğrenciNO- Derscode 
int is_course_available_quota(char* course_code) {
	FILE *fp; 
	fp = open(LOGFILE, "r");
	char* block = (char*) malloc(sizeof(char)*200);
	char* temp; // 1 block erken kesmeden dolayı 1 kere get_unique içinde kullanılıyor.(optimize edilmeli.)
	int lec_students = 0;
	while( fgets(block, LINESIZE, fp) ) { // ders kodu , kayıt durumu enum sayilar
		temp = strdup(block);
		//printf("s: %s", block);
		//printf("t: %d\n", !strcmp(get_unique_element(block, 1), course_code));
		//printf("s: %s\n", temp);
		//printf("k: %d\n", !strcmp(get_unique_element(temp, 4), "1"));
		
		if(strcmp(get_unique_element(block, 1), course_code) && strcmp(get_unique_element(temp, 4), "1")) 
			lec_students++;
		free(temp);
	}
	fclose(fp);
	fp = open(LECFILE, "r") ;
	while( fgets(block, LINESIZE, fp) != NULL ) {
		temp = strdup(block);
		if ( !strcmp(get_unique_element(block,0), course_code) ) {
			if(atoi(get_unique_element(temp, 3)) > lec_students) {
				free(block);
				return SUCCESS ;
			} 
		}
		free(temp);
	}
	free(block);
	return FAIL;
}

int is_student_add_lec_available(char* student_no, int max_credit, int max_lec ) {
	
	// öğrencinin derse seçebilmesi için maksimum ders sayısı ve krediyi aşmaması gerekir.
	// bunun için derlogunu
	FILE *flog;
	flog = open(LOGFILE, "r");
	char* block = (char*)malloc(LINESIZE*sizeof(char));
	while( fgets(block, LINESIZE, flog) ) {
		get_unique_element
	}
	return 0;
}

int add_course_register(int student_no, char* course_code) {
	FILE * fp;
	fp = open(LOGFILE, "r");
	int id=0;
	char* block = (char*) malloc(sizeof(char)*LINESIZE);
	if ( fp ) {
		while( fgets(block, LINESIZE, fp) !=NULL ) {
		// Just search for the latest line, do nothing in the loop
		} 
		fclose(fp);
		sscanf(block,"%d",&id);
		id++;
	}
	
	time_t date = get_system_date();
	fp = open(LOGFILE, "a");
	
	fprintf(fp, "%d%s%d%s%s%s%s\n",id,SEP,
						  student_no,SEP,
						  course_code,SEP,
						  ctime(&date));
	return 0;
}

time_t get_system_date() {
	time_t mytime ;
	mytime = time(NULL);
	return mytime;
}

int main() {
	//DEGREE uzmanlik = uz_;
	//converter.tgt[0] = '2';
	
	setlocale(LC_ALL,"Turkish"); // set locale yerleştirilen ve yerleştirilmeden İLK açmalar farklı şeylerdir.
	
	char* unvanlar[] = {"Uzman", "Öğretim Planlamacısı", "Çevirmen", "Okutman",
		"Öğretim Görevlisi", "Öğretim Üyesi", "Araştırma Görevlisi", 
		"Araştırma Görevlisi Doktor", "Yardımcı Doçent", "Yardımcı Doçent Doktor", 
		"Doçent", "Doçent Doktor", "Profesör", "Profesör Doktor", "Ordinaryüs"
	};
	
	int max_credit = 20;
	int max_lect = 10;
	
	STUDENT* sample = (STUDENT*) malloc(sizeof(STUDENT));
	sample->stno = 18011078;
	sample->name = "Onur";
	sample->surname = "Demir";
	sample->total_lect = 20;
	sample->total_lect_credit = 25;
	char array[64]; 
    int myInteger = 4711; 
    sprintf( array, "%d", myInteger ); 
    printf( "%s", array ); 
	printf("%s", unvanlar[0]);
	
	add_student(sample);
	
	TEACHER* teacher = (TEACHER*) malloc(sizeof(TEACHER));
	teacher-> id = 1000;
	teacher-> name = "ferkan";
	teacher-> surname = "neysene";
	teacher-> degree = unvanlar[8];
	
	add_lecturer(teacher);
	
	COURSE* course = (COURSE*) malloc(sizeof(TEACHER));
	course->code = "BLM1011";
	course->name = "Bilgisayar Bilimlerine Giriş";
	course->credit = 5;
	course->quota = 120;
	course->teacher_id	= 1000;
	
	if(is_course_possible_id(course)) {
		add_course(course);
	}
	
	//fseek trial
	//int randomLineWidth = 100;
	
	char k[10] ;
	
	scanf("%[0-9]+", k);
	printf("%s", k);
		
	//öğrenci kursu seçti. o kursta yeterli yer var mı 
	// öğrenci kredisi ve öğrencinin seçtiği ders maksimum ders sayısının altında mı ?
	if(is_course_available_quota("BLM2021") && is_student_add_lec_available(sample, max_lect, max_credit) ) {
		printf("success");
	}


	//converter.source = 1;
	//block = converter.tgt[0];
	//printf("me thatch lee %d\n", block);
	
	return 0;
}
