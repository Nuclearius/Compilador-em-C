#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char m[50];//memoria
int l;//linha
int p;//flag
FILE *arquivo;
char f[200][26];

int pilha()
{

    int j = 50;
	for(j=0;j>=0;j--)
    {
		printf("%d\n",m[j]);
	}
	return 0;
}
void openFILE(){
     arquivo=fopen("","r");
    if(arquivo == NULL) {
        printf("ERRO");
        exit(1);
    }
char charATUAL;
	charATUAL = fgetc(arquivo);
	int r = 0;
	int c = 0;
	while(charATUAL != EOF){
		if(charATUAL == '\n'){
			f[r][c] = '\0';
			r++;
			c=0;
		}
		else{
			f[r][c] = charATUAL;
			c++;
		}
		charATUAL = fgetc(arquivo);
	}
	return r;
}