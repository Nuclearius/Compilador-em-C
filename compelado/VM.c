#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

char Memoria[50];	//memoria
int l;				//linha
int p;				//flag
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
char charA;
	charA = fgetc(arquivo);
	int r = 0;
	int c = 0;
	while(charA != EOF){
		if(charA == '\n'){
			f[r][c] = '\0';
			r++;
			c=0;
		}
		else{
			f[r][c] = charA;
			c++;
		}
		charA = fgetc(arquivo);
	}
	return r;
}
int busca(int label){
	char rotulo [5];
	char comando [9];
	int j,k;
	int flag = 0;
	
	for(j=0;j<4;j++){
		rotulo[j] = f[k][j];
	}
	rotulo[4] = '\0';
	
	for(j=0;j<8;j++){
		comando[j] = f[k][j+4];	
	}
	comando[8] = '\0';
	
	while(strcmp(comando,"HLT") && atoi(rotulo) != label){
		
		k++;
		for(j=0;j<4;j++){
			rotulo[j] = f[k][j];
		}
		rotulo[4] = '\0';
		
		for(j=0;j<8;j++){
			comando[j] = f[k][j+4];	
		}
		comando[8] = '\0';
		
		if(!strcmp(comando,"HLT")){
			flag = 1;
		}
	}
	if(flag){
		return -1;
	}
	else{
		return k;
	}
}
int execucao()
{
	char rotulo [10];
	char comando [10];
	char param1 [10];
	char param2 [10];

	if(!strcmp(comando,"START"))
	{
		printf("start\n");//start
		p=-1;
		l++;
	}
	else if(!strcmp(comando,"ALLOC")){

				int m,n,k;
				m=atoi(param1);
				n=atoi(param2);
				printf("alloc %d %d\n",m,n);			//alocacao da memoria
				for(k=0;k<n;k++)
				{
					p++;
					Memoria[p]=Memoria[m+k];
				}
				l++;}

    else if(!strcmp(comando,"DALLOC")){

				int m,n,k;
				m=atoi(param1);
				n=atoi(param2);
				printf("dalloc %d %d\n",m,n);			//dealocacao da memoria
				for(k=n-1;k>=0;k--)
				{
					Memoria[m+k]= Memoria[p];
					p--;
				}
				l++;
			}

    else if(!strcmp(comando,"CALL")){
                    int p;
                    l++;
                    Memoria[l]=p+1;				//chamada
                    p=atoi(param1);
                   printf("call %d\n",p);
                }
     else if(!strcmp(comando,"RETURN")){
                    printf("RETURN %d",p);		//RETURN
                    l=Memoria[p];
                    p--;

          }
     else if(!strcmp(comando,"HLT")){
                printf("HALT");//halt
                return -1;
          }
     else if(!strcmp(comando,"RD")){
            if(!strcmp(comando,"RD")){
                printf("READ");
                l++;                        //comando ler
                scanf("%d",Memoria[l]);
                p++;}
          }

     else if(!strcmp(comando,"PRN")){
        printf("PRINT %d",Memoria[l]);    //comando printar
        p--;
        l++;
        }
     else if(!strcmp(comando,"LDC")){
            int k;
            k=atoi(param1);
            printf("%d",k); 		//carregar cosntante
            p++;
            Memoria[p]=k;
            l++;
        }
	 else if(!strcmp(comando,"LDV")){
		int n = atoi(param1);
		p++;
		Memoria[p] = Memoria[n];		//carregar variavel
		l++;
		}
	 else if (!strcmp(comando,"ADD")){
		Memoria[p-1]= Memoria[p-1]+Memoria[p];		//adicao
		p--;
		l++;
	}
	 else if (!strcmp(comando,"SUB")){
		Memoria[p-1]= Memoria[p-1]-Memoria[p];		//subtraçao
		p--;
		l++;
	}
	 else if (!strcmp(comando,"MULT")){
		Memoria[p-1]= Memoria[p-1]*Memoria[p];		//multiplicaçao
		p--;
		l++;
	}
	 else if (!strcmp(comando,"DIV")){
		Memoria[p-1]= Memoria[p-1]/Memoria[p];		//divisao
		p--;
		l++;
	}
	 else if (!strcmp(comando,"INV")){
		Memoria[p]= -(Memoria[p]);				//inverter sinal
		l++;
	}else if(!strcmp(comando,"AND")){
			if(Memoria[p-1] == 1 && Memoria[p] == 1){
			Memoria[p-1] = 1;
			}
		else{
			Memoria[p-1] = 0;
				p--;
				l++;
			}
	}else if(!strcmp(comando,"OR")){
		if(Memoria[p-1] == 1 || Memoria[p] == 1){
			Memoria[p-1] = 1;
		}
		else{
		Memoria[p-1] = 0;
		}
		p--;
		l++;
	}else if(!strcmp(comando,"NEG")){
		Memoria[p] = 1-Memoria[p];
		l++;
	} else if(!strcmp(comando,"CME")){
		if(Memoria[p-1] < Memoria[p]){
			Memoria[p-1] = 1;
		}
		else{
		Memoria[p-1] = 0;
		}
		p--;
		l++;
	}else if(!strcmp(comando,"CMA")){
		if(Memoria[p-1] > Memoria[p]){
			Memoria[p-1] = 1;
		}
		else{
			Memoria[p-1] = 0;
		}
		p--;
		l++;
	}else if(!strcmp(comando,"CEQ")){
		if(Memoria[p-1] == Memoria[p]){
			Memoria[p-1] = 1;
		}
		else{
			Memoria[p-1] = 0;
		}
		p--;
		l++;
	}else if(!strcmp(comando,"CDIF")){
		if(Memoria[p-1] != Memoria[p]){
			Memoria[p-1] = 1;
		}
		else{
			Memoria[p-1] = 0;
		}
		p--;
		l++;
	}else if(!strcmp(comando,"CMEQ")){
	if(Memoria[p-1] <= Memoria[p]){
			Memoria[p-1] = 1;
		}
		else{
			Memoria[p-1] = 0;
		}
		p--;
		l++;
	}else 	if(!strcmp(comando,"CMAQ")){
		if(Memoria[p-1] >= Memoria[p]){
			Memoria[p-1] = 1;
		}
		else{
			Memoria[p-1] = 0;
		}
		p--;
		l++;
	}else if(!strcmp(comando,"STR")){
		int n;
		n = atoi(param1);
		Memoria[p] = Memoria[p];
		p--;
		l++;
	}else if(!strcmp(comando,"JMP")){
		int u;
		u = atoi(param1);
		l = busca(u);
	}else if(!strcmp(comando,"JMPF    ")){
		if(Memoria[p] == 0){
			int u = atoi(param1);
			l = FetchLabel(u);
			}
		else{
			l++;
			}
			p--;
	}else if(strcmp(comando,"NULL")){
			return 1;
		}
		else{
			l++;
		}
}



