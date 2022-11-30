#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char Memoria[50];//memoria
int l;//linha
int p;//flag
FILE *arquivo;
char f[200][26];

int pilha()
{

    int j = 50;
	for(j=0;j>=0;j--)
    {
		printf("%d\n",Memoria[j]);
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
int execucao()
{
	char rotulo [10];
	char comando [10];
	char param1 [10];
	char param2 [10];

	if(!strcmp(comando,"START   "))
	{
		printf("start\n");
		p=-1;
		l++;
	}//start
		else{	
			if(!strcmp(comando,"ALLOC   "))
			{
				int m,n,k;
				m=atoi(param1);
				n=atoi(param2);
				printf("alloc %d %d\n",m,n);//alocacao da memoria
				for(k=0;k<n;k++)
				{
					p++;
					Memoria[p]=Memoria[m+k];
				}
				l++;
			}
		else
		{

			if(!strcmp(comando,"DALLOC  "))
			{
				int m,n,k;
				m=atoi(param1);
				n=atoi(param2);
				printf("dalloc %d %d\n",m,n);//dealocacao
				for(k=n-1;k>=0;k--)
				{
					Memoria[m+k]= Memoria[p];
					p--;
				}
				l++;
			}
		}
	}
}