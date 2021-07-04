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
	char code[PARTSIZE];
	char name[PARTSIZE];
	int credit;
	int quota;
	int t_id;
	
} COURSE;

typedef struct {
	int stdno;
	char name[PARTSIZE];
	char surname[PARTSIZE];
	int numb_of_lect ;
	int numb_of_credit ;
} STUDENT;

typedef struct {
	int id;
	char name[PARTSIZE];
	char surname[PARTSIZE];
	char degree[PARTSIZE];
} TEACHER;
