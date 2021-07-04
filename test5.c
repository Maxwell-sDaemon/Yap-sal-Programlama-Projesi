// ders silinirse , ders kayıtlarının hepsi bıraktıya dönmeli,
// ilgili ders derslerden siinmeli yada üzerine unknown yazılabilir.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINESIZE 200

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
	printf("%d", start_ptr);
	printf("%d\n", end_ptr);
	char* block = (char*) malloc(LINESIZE * sizeof(char));
	// \0 ekleme yolu ile stringin baş ve son parçasını kullandım.

	// 3 tane char bloğunu birleştireceğim 
	string[start_ptr] = '\0';
	int first_partial_len = strlen(string);
	memcpy(block, string, first_partial_len);
	int second_partial_len = strlen(value);
	memcpy(block + first_partial_len, value, second_partial_len);
	char* third_partial = string + first_partial_len + second_partial_len;
	printf("%s\n", third_partial);
	int third_partial_len = strlen( third_partial ) ;
	memcpy(block + first_partial_len + second_partial_len ,third_partial, third_partial_len);

	return block;

}


int main() {
	char a[200] = "abc,def,cde,lll,124\n";
	char* b = set_unique_element(a, "123", 4);
	printf("%s", b);
	return 0;
}
