/*
 * A simple browser based on GTK4 and WebKitGTK 6.0
 * Usage: browser <URL>
 */

#include <gtk/gtk.h>
#include <webkit/webkit.h>

/* Window activate callback function */
static void activate(GtkApplication *app, gpointer user_data) 
{
    const char *url = (const char *)user_data;

    /* Create main window */
    GtkWidget *window = gtk_application_window_new(app);
    if (!window) {
        g_critical("Failed to create window");
        return;
    }

    /* Configure window properties */
    gtk_window_set_title(GTK_WINDOW(window), "One Window Browser");
    gtk_window_set_default_size(GTK_WINDOW(window), 1024, 768);

    /* Create WebKit view */
    WebKitWebView *web = WEBKIT_WEB_VIEW(webkit_web_view_new());
    if (!web) {
        g_critical("Failed to create web view");
        return;
    }

    /* Set WebKit view as window's child */
    gtk_window_set_child(GTK_WINDOW(window), GTK_WIDGET(web));

    /* Load URL */
    webkit_web_view_load_uri(web, url);

    /* Set focus to WebKit view */
    gtk_widget_grab_focus(GTK_WIDGET(web));

    /* Show window */
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) 
{
    /* Check command line arguments */
    if (argc != 2) {
        g_printerr("Usage: %s <URL>\n", argv[0]);
        g_printerr("Example: %s https://example.com\n", argv[0]);
        return 1;
    }

    /* Validate URL format */
    if (!g_str_has_prefix(argv[1], "http://") && 
        !g_str_has_prefix(argv[1], "https://")) {
        g_printerr("Error: URL must start with 'http://' or 'https://'\n");
        return 1;
    }

    /* Create application instance */
    GtkApplication *app = gtk_application_new("org.gtk.one-window-browser",
                                            G_APPLICATION_DEFAULT_FLAGS);
    if (!app) {
        g_critical("Failed to create application");
        return 1;
    }

    /* Connect activate signal */
    g_signal_connect(app, "activate", G_CALLBACK(activate), argv[1]);

    /* Run application */
    int status = g_application_run(G_APPLICATION(app), 0, NULL);

    /* Cleanup */
    g_object_unref(app);

    return status;
}
