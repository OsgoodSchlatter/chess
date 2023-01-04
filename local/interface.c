#include <gtk/gtk.h>

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "My Application");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    GtkWidget *label = gtk_label_new("<b>Hello, World!</b>");
    gtk_label_set_use_markup(GTK_LABEL(label), TRUE);

    gtk_container_add(GTK_CONTAINER(window), label);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
