#include <gtk/gtk.h>
#include <webkit/webkit.h>

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "One Window Browser");
    gtk_window_set_default_size(GTK_WINDOW(window), 1024, 768);

    WebKitWebView *web = WEBKIT_WEB_VIEW(webkit_web_view_new());
    gtk_window_set_child(GTK_WINDOW(window), GTK_WIDGET(web));

    gchar *url = user_data;
    webkit_web_view_load_uri(web, url);
    gtk_widget_grab_focus(GTK_WIDGET(web));

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc,char **argv) {
    gchar *url = "https://kernel.org";
    if(argc == 2)
        url = argv[1];

    GtkApplication *app = gtk_application_new("org.gtk.one-window-browser", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), url);
    int status = g_application_run(G_APPLICATION(app), 0, NULL);

    g_object_unref(app);

    return status;
}
