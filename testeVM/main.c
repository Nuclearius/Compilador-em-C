#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

int Memoria[50];	//memoria
int l = 0;				//linha
int p;				//flag
FILE *arquivo;
char f[200][26] = {0};

int main()

{
    printf("vm teste go\n");
    openFILE();
    int I = 0;
    while ( I < 200 && f[I][0] != 0 )
    {
    printf("%d:\t %s\n", I+1, f[I]);
    I++;
    }
    printf("read file end\n");
    while (I != -1)
    {
        I = execucao(f[l]);
    }

    printf("\nFIM\n");
    return 0;
}



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
     arquivo=fopen("codigo.txt","r");
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
	char rotulo [5] = {0};
	char comando [9] = {0};
	int j,k = 0, aux;
	int flag = 0;


/*
	for(j=0;j<4;j++){
		rotulo[j] = f[k][j];
	}

	rotulo[4] = '\0';

	for(j=0;j<8;j++){
		comando[j] = f[k][j+4];
	}
	comando[8] = '\0';
*/
	while(strcmp(comando,"HLT") && atoi(rotulo) != label){

		k++;
		j = 0;
		aux = 0;

        while (f[k][j] != 9)
        {
            rotulo[j] = f[k][j];
            j++;
        }
        aux = j+1;
        rotulo[aux] = 0;
        j = 0;
        while (f[k][aux+j] != 9)
        {
            comando[j] = f[k][aux+j];
            j++;
        }
         comando[aux+j+1] = 0;

		if(!strcmp(comando,"HLT")){
			flag = 1;
		}

	}
	if(flag){
		return -1;
	}
	else{
        printf ( " jump to: %d \n", k+1);
		return k;
	}
}
int execucao(char *line)
{

	char rotulo [10] = {0};
	char comando [10] = {0};
	char param1 [10] = {0};
	char param2 [10] = {0};



	int I = 0, aux = 0;
	while (line[I] != 9)
    {
     rotulo[I] = line[I];
     I++;
    }
    aux = I + 1;
    I=0;

    while (line[aux+I] != 9)
    {
        comando[I] = line[aux+I];
        I++;
    }
    aux = aux+ I + 1;
    I = 0;

    while(line[aux+I] != 9)
    {
        param1[I] = line[aux+I];
        I++;
    }
    aux = aux+ I + 1;
    I = 0;


    while(line[aux+I] != 9)
    {
        param2[I] = line[aux+I];
        I++;
    }


    printf("teste:  %s\t%s\t%s\t%s\t\n", rotulo, comando, param1, param2);

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
                    int r;
                    r=atoi(param1);
                    p++;
                    Memoria[p]=l+1;				//chamada
                    l = busca(r);
                   printf("call %d\n",Memoria[p]);
                }
     else if(!strcmp(comando,"RETURN")){
                    printf("RETURN %d",Memoria[p]);		//RETURN
                    l=Memoria[p];
                    p--;

          }
     else if(!strcmp(comando,"HLT")){
                printf("HALT");//halt
                return -1;
          }
     else if(!strcmp(comando,"RD")){
                printf("READ\n");
                l++;                        //comando ler
                p++;
                scanf("%d",&Memoria[p]);

          }

     else if(!strcmp(comando,"PRN")){
        printf("PRINT\n%d\n",Memoria[p]);    //comando printar
        p--;
        l++;
        }
     else if(!strcmp(comando,"LDC")){
            int k;
            k=atoi(param1);
            printf("constante %d\n",k); 		//carregar cosntante
            p++;
            Memoria[p]=k;
            l++;
        }
	 else if(!strcmp(comando,"LDV")){
		int n = atoi(param1);
		p++;
		Memoria[p] = Memoria[n];		//carregar variavel
		printf("variavel %d\n",Memoria[p]);
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
        }
        p--;
        l++;
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
		Memoria[n] = Memoria[p];
		p--;
		l++;
	}else if(!strcmp(comando,"JMP")){
		int u;
		u = atoi(param1);
		l = busca(u);
	}else if(!strcmp(comando,"JMPF")){
		if(Memoria[p] == 0){
			int u = atoi(param1);
			l = busca(u);
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



