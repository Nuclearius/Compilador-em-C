#include <gtk/gtk.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>


GtkWidget *win;
GtkTextBuffer *tb;
GFile *file;
char *filename;
GtkWidget *isnew;

static void click_run(GtkButton *btn, gpointer used_data)
{
    execucaoVM(false);
}

static void click_run_step(GtkButton *btn, gpointer used_data)
{
    execucaoVM(true);
}

static void click_new(GtkButton *btn, gpointer used_data){
    file= NULL;
    gtk_text_buffer_set_text (tb, "", -1);
    isnew=1;
}

static void on_response (GtkNativeDialog *native, int response){
    char *contents;
    gsize length;
    if (response == GTK_RESPONSE_ACCEPT)
    {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (native);
        file= gtk_file_chooser_get_file (chooser);
        if (g_file_load_contents (file, NULL, &contents, &length, NULL, NULL)){
            gtk_text_buffer_set_text (tb, contents, length);
            g_free (contents);
                if ((filename = g_file_get_basename (file)) != NULL) {
                        g_free (filename);
            }
        }
    }
    g_object_unref (native);
}

static void click_open (GtkButton *btn, gpointer used_data){
    GtkFileChooserNative *native;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

    native = gtk_file_chooser_native_new ("Open File",win,action,"_Open","_Cancel");

    g_signal_connect (native, "response", G_CALLBACK (on_response), NULL);
    gtk_native_dialog_show (GTK_NATIVE_DIALOG (native));
}

 static void app_activate (GApplication *app, gpointer user_data) {
    GtkWidget *scr;
    GtkWidget *scr2;
    GtkWidget *scr3;
    GtkWidget *scr4;

    GtkEntryBuffer* eb;
    GtkWidget* entry;

    GtkWidget *tv;
    GtkWidget *tv2;
    GtkWidget *tv3;
    GtkWidget *tv4;
    GtkWidget *tv5;
    GtkWidget *tv6;
    GtkWidget *tv7;

    GtkWidget *boxv;
    GtkWidget *boxv2;
    GtkWidget *boxv3;

    GtkWidget *boxh;
    GtkWidget *boxh2;

    GtkWidget *btnn;
    GtkWidget *btno;

    GtkWidget *dmy1;
    GtkWidget *dmy2;
    GtkWidget *dmy3;

    GtkTextBuffer *tb2;
    GtkTextBuffer *tb3;
    GtkTextBuffer *tb4;
    GtkTextBuffer *tb5;
    GtkTextBuffer *tb6;
    GtkTextBuffer *tb7;

    gchar *text;
    gchar *text2;
    gchar *text3;
    gchar *text4;

    text = "\n";
    text2= "\n\t\tCodigo de Maquina\n";
    text3= "\n\tMemoria (Stack)\n";
    text4= "\n\t\tSaida de Dados\n";

    win = gtk_application_window_new (GTK_APPLICATION (app));
    gtk_window_set_title (GTK_WINDOW (win), "Maquina Virtual");
    gtk_window_maximize(GTK_WINDOW (win));

    boxv = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_window_set_child (GTK_WINDOW (win), boxv);

    boxh = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_append (GTK_BOX (boxv), boxh);
    dmy1 = gtk_label_new(NULL); /* dummy label for left space */
    gtk_label_set_width_chars (GTK_LABEL (dmy1), 10);
    dmy2 = gtk_label_new(NULL); /* dummy label for center space */
    gtk_widget_set_hexpand (dmy2, TRUE);
    dmy3 = gtk_label_new(NULL); /* dummy label for right space */
    gtk_label_set_width_chars (GTK_LABEL (dmy3), 10);
    btnn = gtk_button_new_with_label ("New");
    btno = gtk_button_new_with_label ("Open");


    gtk_box_append (GTK_BOX (boxh), dmy1);
    gtk_box_append (GTK_BOX (boxh), btno);
    gtk_box_append (GTK_BOX (boxh), btnn);
    gtk_box_append (GTK_BOX (boxh), dmy2);

    g_signal_connect(btno, "clicked", G_CALLBACK(click_open),NULL);
    g_signal_connect(btnn, "clicked", G_CALLBACK(click_new),NULL);

    boxh2 = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_append (GTK_BOX (boxv), boxh2);

    boxv2 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append (GTK_BOX (boxh2), boxv2);

///titulo do codigo
    tv4 = gtk_text_view_new ();
    tb4 = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tv4));
    gtk_text_buffer_set_text (tb4, text2, -1);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (tv4), GTK_WRAP_WORD_CHAR);
    gtk_box_append(GTK_BOX (boxv2),tv4);
    gtk_text_view_set_editable(tv4, FALSE);
    gtk_widget_set_hexpand(tv4, TRUE);
    gtk_widget_set_vexpand(tv4, FALSE);

///codigo
    scr = gtk_scrolled_window_new ();
    tv = gtk_text_view_new ();
    tb = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tv));
    gtk_text_buffer_set_text (tb, text, -1);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (tv), GTK_WRAP_WORD_CHAR);
    gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (scr), tv);
    gtk_box_append(GTK_BOX (boxv2),scr);
    gtk_scrolled_window_set_has_frame(scr, TRUE);
    gtk_widget_set_size_request (scr, 720, 350);


    boxv3 = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_append (GTK_BOX (boxh2), boxv3);

///titilo memoria
    tv5 = gtk_text_view_new ();
    tb5 = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tv5));
    gtk_text_buffer_set_text (tb5, text3, -1);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (tv5), GTK_WRAP_WORD_CHAR);
    gtk_box_append(GTK_BOX (boxv3),tv5);
    gtk_text_view_set_editable(tv5, FALSE);
    gtk_widget_set_hexpand(tv5, TRUE);
    gtk_widget_set_vexpand(tv5, FALSE);

///memoria
    scr2 = gtk_scrolled_window_new ();
    tv2 = gtk_text_view_new ();
    tb2 = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tv2));
    gtk_text_buffer_set_text (tb2, text, -1);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (tv2), GTK_WRAP_WORD_CHAR);
    gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (scr2), tv2);
    gtk_box_append(GTK_BOX (boxv3),scr2);
    gtk_text_view_set_editable(tv2, FALSE);
    gtk_scrolled_window_set_has_frame(scr2, TRUE);
    gtk_scrolled_window_get_max_content_height(scr2);
    gtk_widget_set_hexpand(scr2, TRUE);
    gtk_widget_set_size_request (scr2, 0,350);


///titulo saida
    tv6 = gtk_text_view_new ();
    tb6 = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tv6));
    gtk_text_buffer_set_text (tb6, text4, -1);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (tv6), GTK_WRAP_WORD_CHAR);
    gtk_box_append(GTK_BOX (boxv),tv6);
    gtk_text_view_set_editable(tv6, FALSE);
    gtk_widget_set_hexpand(tv6, TRUE);
    gtk_widget_set_vexpand(tv6, FALSE);

///saida
    scr3 = gtk_scrolled_window_new ();
    tv3 = gtk_text_view_new ();
    tb3 = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tv3));
    gtk_text_buffer_set_text (tb3, text, -1);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (tv3), GTK_WRAP_WORD_CHAR);
    gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (scr3), tv3);
    gtk_box_append(GTK_BOX (boxv),scr3);
    gtk_text_view_set_editable(tv3, FALSE);
    gtk_scrolled_window_set_has_frame(scr3, TRUE);
    gtk_scrolled_window_get_min_content_height(scr3);
    gtk_widget_set_hexpand(scr3, TRUE);
    gtk_widget_set_vexpand(scr3, TRUE);

///entrada
    entry= gtk_entry_new();
    eb=gtk_entry_get_buffer(entry);
    gtk_box_append(GTK_BOX (boxv),entry);
    gtk_entry_set_has_frame(entry,TRUE);
    gtk_widget_set_vexpand(entry, FALSE);
    gtk_widget_set_hexpand(entry, TRUE);

    gtk_widget_show (win);
 }

int main (int argc, char **argv) {
   GtkApplication *app;
   int stat;

   app = gtk_application_new ("c.c", G_APPLICATION_FLAGS_NONE);
   g_signal_connect (app, "activate", G_CALLBACK (app_activate), NULL);
   stat =g_application_run (G_APPLICATION (app), argc, argv);
   g_object_unref (app);
   return stat;
 }


int Memoria[50];	//memoria
int l = 0;				//linha
int p;				//flag
FILE *arquivo;
char f[200][26] = {0};


void execucaoVM(bool step)
{   int I =0;
    openFILE();
     while (I != -1)
    {
        I = lerLinha(f[l]);
        if(step)
            getchar();
    }
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
     arquivo=fopen(filename,"r");
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

int lerLinha(char *line)
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


