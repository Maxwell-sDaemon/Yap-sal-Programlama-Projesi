#define FAIL 0
#define SUCCESS 1

#define KAYITLI "1"
#define BIRAKTI "0"

#define UNVANSZ 14
#define WIDTH 5 
#define HEIGHT 10  

#define STNUMSZ 7
#define TEACHERIDSIZE 4

#define LINESIZE 500
#define PARTSIZE 500

#define EOL '\n'

#define SEP ","

#define UNKNOWN '?'
#define SEP_U ','
#define SEP_INFO ' '

#define UNKNOWN_STUDENT_COMPONENT 5
#define UNKNOWN_TEACHER_COMPONENT 4
#define UNKNOWN_COURSE_COMPONENT 5

#define FILE_TAIL_NAME "_SINIFLİSTESI.txt"
#define TEMPFILE "temp.txt"
#define TEACHERFILE "akademisyen.txt"
#define OGRENCIFILE "öğrenci.txt"
#define LECFILE "dersler.txt"
#define LOGFILE "ÖğrenciDersKayıt.txt"


enum mylog {log_id, log_course_code, log_student_no, log_registration_state, log_date};
enum mylec {lecture_course_code, lecture_course_name, lecture_course_credit, lecture_quota, lecture_teacher_id};
enum mystudent {student_std_no, student_std_name, student_std_surname, student_std_course_number, student_std_course_credit};
enum myteacher {teacher_id, teacher_name, teacher_surname, teacher_degree};
