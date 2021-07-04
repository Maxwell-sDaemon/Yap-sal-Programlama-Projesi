
	
	

#include<stdio.h>
#include<string.h>
#include<locale.h>
typedef struct{
	char ad[30];
	char soyad[20];
	char poliklinik[20];
	int poliklinikno;
	char tarih[12];
	char saat[6];
}KISI;

int main(){
	setlocale(LC_ALL,"Turkish");
	KISI k;
	int c=0;
	char filename[50];
	printf("Dosya adini giriniz");
	scanf("%s",filename);
	FILE *data=fopen(filename,"r+");
	FILE *fw;
	if(!data){
		printf("Dosya Açılamadı");
		return 0;
	}
	while(!feof(data)){
		filename[0]= '\0';
		fscanf(data,"%s %s %s %d %s %s",k.ad,k.soyad,k.poliklinik,&k.poliklinikno,k.tarih,k.saat);
		
		strcat(filename,k.ad);
		strcat(filename,"_");
		strcat(filename,k.soyad);
		strcat(filename,".txt");
		fw=fopen(filename,"w");
		if(fw){
			printf("%p acıldı \n",fw);
			c++;
			fprintf(fw,"Sayın %s %s,\n Randevu bilgileriniz aşağıdadır.\n"
				"Tarih:%s,\nSaat:%s,\nPoliklinik:%s (%d)\n"
				"Randevu saatinden 15 dk once bekleriz\n",
				k.ad,k.soyad,k.tarih,k.saat,k.poliklinik,k.poliklinikno);
			fclose(fw);
		}
	}
	printf("%d sayıda kayıt oluşturuldu",c);
	fseek(data,0,SEEK_END);
	fprintf(data,"\n%d sayıda kayıt oluşturuldu",c);
	fclose(data);
	return 0;	
}

