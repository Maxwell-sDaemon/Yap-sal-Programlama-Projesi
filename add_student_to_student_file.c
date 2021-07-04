#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include "macros.h"
#include "structures.h"

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
	fclose(fp);
	return SUCCESS;
}

int main() {
	STUDENT* stud = create_student();
	add_student_to_student_file(stud);
	
	setlocale(LC_ALL,"Turkish");
	
	int max_lec, max_credit;
	char option = 8;
	
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
								//course_codes = update_log_file_by_student_no(std_no);
								//int erased_reg_size = atoi(course_codes_[0]);
								//for( int i = 1 ; i < erased_reg_size ; i++_) {
									//char* course_code = course_codes[i];
									//int teacher_id = teacher_id_of_course(course_code);
									//char* teacher = get_teacher_name_surname_by_id( teacher_id );
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
