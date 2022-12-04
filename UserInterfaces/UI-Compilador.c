#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>


GtkWidget *win;
GtkTextBuffer *tb;
GFile *file;
char *filename;
GtkWidget *isnew;


static void saveFun (GtkNativeDialog *native,int response){

	GtkTextIter start_iter;
	GtkTextIter end_iter;
	char *contents;
	char *pathname;
	
	if (response == GTK_RESPONSE_ACCEPT){
		GtkFileChooser *chooser = GTK_FILE_CHOOSER (native);
		file = gtk_file_chooser_get_file (chooser);
		isnew=0;
		gtk_text_buffer_get_bounds (tb, &start_iter, &end_iter);
		contents = gtk_text_buffer_get_text (tb, &start_iter, &end_iter, FALSE);
		g_print("%s",contents);
		
		if (! g_file_replace_contents (file, contents, strlen (contents), NULL, TRUE, G_FILE_CREATE_NONE, NULL, NULL, NULL)){
			pathname = g_file_get_path (file);
			g_print("%s",pathname);
			g_free (pathname);
		}
		
		g_free (contents);
	}

	g_object_unref (native);
}

static void on_response (GtkNativeDialog *native, int response){
	char *contents;
	gsize length;
	if (response == GTK_RESPONSE_ACCEPT){
		GtkFileChooser *chooser = GTK_FILE_CHOOSER (native);
		file= gtk_file_chooser_get_file (chooser);
		if (g_file_load_contents (file, NULL, &contents, &length, NULL, NULL)){
			gtk_text_buffer_set_text (tb, contents, length);
			g_free (contents);
			
			if ((filename = g_file_get_basename (file)) != NULL) 
				g_free (filename);
		}
	}

	g_object_unref (native);
}

static void save_to_file(){
	GtkFileChooserNative *native;
	GtkFileChooser *chooser;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_SAVE;

	native = gtk_file_chooser_native_new ("Save File", win, action, "_Save", "_Cancel");
	
	chooser = GTK_FILE_CHOOSER (native);

	if (isnew)
		gtk_file_chooser_set_current_name (chooser, "Untitled document");
	else
		gtk_file_chooser_set_file (chooser, file, NULL);

	g_signal_connect (native, "response", G_CALLBACK ( saveFun ), NULL);
	gtk_native_dialog_show (GTK_NATIVE_DIALOG (native));
}


static void click_open (GtkButton *btn, gpointer used_data){
	GtkFileChooserNative *native;
	GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

	native = gtk_file_chooser_native_new ("Open File", win, action, "_Open", "_Cancel");

	g_signal_connect (native, "response", G_CALLBACK (on_response), NULL);
	gtk_native_dialog_show (GTK_NATIVE_DIALOG (native));
}

static void click_run (GtkButton *btn, gpointer used_data){

	if(isnew)
		gtk_text_buffer_set_text (used_data, "Salve antes de compilar", -1);
	
	else{
	
		pid_t cpid;
		int stat;
		
		if(fork() == 0){
		
			char* args[] = {"./compiler", filename, NULL};
			execv("./compiler", args);
		}
		else
			cpid = wait(&stat);
			
		if(WIFEXITED(stat)){
		
			FILE* compLog;
			if((compLog = fopen("erros.txt", "r")) == NULL){
				printf("ERRO");
				exit(1);
			}
			char s[100];
			fgets(s, 100, compLog);
			gtk_text_buffer_set_text(used_data, s, -1);
		}
		else if(WIFSIGNALED(stat))
			psignal(WTERMSIG(stat), "Exit signal");
	
	}
    
}


static void click_new(GtkButton *btn, gpointer used_data){
	file= NULL;
	gtk_text_buffer_set_text (tb, "", -1);
	isnew=1;
}


static void click_save (GtkButton *btn, gpointer used_data){
	GtkTextIter start_iter;
	GtkTextIter end_iter;
	char *contents;
	char *pathname;

	if(!isnew){
		gtk_text_buffer_get_bounds (tb, &start_iter, &end_iter);
		contents = gtk_text_buffer_get_text (tb, &start_iter, &end_iter, FALSE);
		g_print("%s",contents);
		if (! g_file_replace_contents (file, contents, strlen (contents), NULL, TRUE, G_FILE_CREATE_NONE, NULL, NULL, NULL)) {
			pathname = g_file_get_path (file);
			g_print("%s",pathname);
			g_free (pathname);
		}
		
		g_free (contents);
	}
	else
		save_to_file();
}


static void app_activate (GApplication *app, gpointer user_data) {
	GtkWidget *scr;
	GtkWidget *scr2;
	gchar *text;
	GtkWidget *tv;
	GtkWidget *tv2;
	GtkWidget *dmy10;
	GtkWidget *dmy15;
	GtkTextBuffer *tb2;

	GtkWidget *dmy20;
	GtkWidget *dmy30;
	GtkWidget *dmy35;
	GtkWidget *boxv;
	GtkWidget *boxh;
	GtkWidget *btnn;
	GtkWidget *btns;
	GtkWidget *btno;
	GtkWidget *btnc;

	text = "";

	win = gtk_application_window_new (GTK_APPLICATION (app));
	gtk_window_set_title (GTK_WINDOW (win), "Compilador");
	gtk_window_maximize(GTK_WINDOW (win));

	boxv = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
	gtk_window_set_child (GTK_WINDOW (win), boxv);

	boxh = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_append (GTK_BOX (boxv), boxh);
	dmy10 = gtk_label_new(NULL);
	gtk_label_set_width_chars (GTK_LABEL (dmy10), 5);
	dmy15 = gtk_label_new(NULL);
	gtk_label_set_width_chars (GTK_LABEL (dmy15), 2);
	dmy20 = gtk_label_new(NULL);
	gtk_widget_set_hexpand (dmy20, TRUE);
	dmy30 = gtk_label_new(NULL);
	gtk_label_set_width_chars (GTK_LABEL (dmy30), 2);
	dmy35 = gtk_label_new(NULL);
	gtk_label_set_width_chars (GTK_LABEL (dmy35), 5);
	
	btnn = gtk_button_new_with_label ("New");
	btno = gtk_button_new_with_label ("Open");
	btns = gtk_button_new_with_label ("Save");
	btnc = gtk_button_new_with_label ("Run");
	
	gtk_box_append (GTK_BOX (boxh), dmy10);
	gtk_box_append (GTK_BOX (boxh), btnn);
	gtk_box_append (GTK_BOX (boxh), dmy15);
	gtk_box_append (GTK_BOX (boxh), btno);
	gtk_box_append (GTK_BOX (boxh), dmy20);
	gtk_box_append (GTK_BOX (boxh), btns);
	gtk_box_append (GTK_BOX (boxh), dmy30);
	gtk_box_append (GTK_BOX (boxh), btnc);
	gtk_box_append (GTK_BOX (boxh), dmy35);

	g_signal_connect(btno, "clicked", G_CALLBACK(click_open),NULL);
	g_signal_connect(btns, "clicked", G_CALLBACK(click_save),NULL);
	g_signal_connect(btnc, "clicked", G_CALLBACK(click_run),tb2);
	g_signal_connect(btnn, "clicked", G_CALLBACK(click_new),NULL);

	scr = gtk_scrolled_window_new ();
	tv = gtk_text_view_new ();
	tb = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tv));
	gtk_text_buffer_set_text (tb, text, -1);
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (tv), GTK_WRAP_WORD_CHAR);
	gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (scr), tv);
	gtk_box_append(GTK_BOX (boxv),scr);
	gtk_widget_set_hexpand(scr, TRUE);
	gtk_widget_set_vexpand(scr, TRUE);

	scr2 = gtk_scrolled_window_new ();
	tv2 = gtk_text_view_new ();
	tb2 = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tv2));
	gtk_text_buffer_set_text (tb2, text, -1);
	gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (tv2), GTK_WRAP_WORD_CHAR);
	gtk_scrolled_window_set_child (GTK_SCROLLED_WINDOW (scr2), tv2);
	gtk_box_append(GTK_BOX (boxv),scr2);
	gtk_text_view_set_editable(tv2, FALSE);
	gtk_scrolled_window_set_has_frame(scr2, TRUE);
	gtk_scrolled_window_get_min_content_height(scr2);
	gtk_widget_set_hexpand(scr2, TRUE);


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
