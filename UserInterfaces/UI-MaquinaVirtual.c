#include <gtk/gtk.h>

GtkWidget *win;
GtkTextBuffer *tb;
GFile *file;
char *filename;
GtkWidget *isnew;

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
 
