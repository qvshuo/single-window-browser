/*
 * A simple browser based on GTK4 and WebKitGTK 6.0
 * Usage: browser --url <URL> [--title <title>] [--size <width>x<height>]
 * Example: browser --url https://example.com --title "Single-window Browser" --size 1200x800
 */

#include <gtk/gtk.h>
#include <webkit/webkit.h>
#include <string.h>

/* Application settings structure */
typedef struct {
    const char *url;
    const char *title;
    int width;
    int height;
} AppSettings;

/* Window activate callback function */
static void activate(GtkApplication *app, gpointer user_data) 
{
    AppSettings *settings = (AppSettings *)user_data;

    /* Create main window */
    GtkWidget *window = gtk_application_window_new(app);
    if (!window) {
        g_critical("Failed to create window");
        return;
    }

    /* Configure window properties */
    gtk_window_set_title(GTK_WINDOW(window), 
                        settings->title ? settings->title : "Single-window Browser");
    gtk_window_set_default_size(GTK_WINDOW(window), 
                               settings->width, settings->height);

    /* Create WebKit view */
    WebKitWebView *web = WEBKIT_WEB_VIEW(webkit_web_view_new());
    if (!web) {
        g_critical("Failed to create web view");
        return;
    }

    /* Set WebKit view as window's child */
    gtk_window_set_child(GTK_WINDOW(window), GTK_WIDGET(web));

    /* Load URL */
    webkit_web_view_load_uri(web, settings->url);

    /* Set focus to WebKit view */
    gtk_widget_grab_focus(GTK_WIDGET(web));

    /* Show window */
    gtk_window_present(GTK_WINDOW(window));
}

/* Parse window size string (format: widthxheight) */
static gboolean parse_size(const char *size_str, int *width, int *height)
{
    char *endptr;
    
    /* Parse width */
    *width = strtol(size_str, &endptr, 10);
    if (*endptr != 'x' || *width <= 0) {
        return FALSE;
    }

    /* Parse height */
    *height = strtol(endptr + 1, &endptr, 10);
    if (*endptr != '\0' || *height <= 0) {
        return FALSE;
    }

    return TRUE;
}

/* Print usage information */
static void print_usage(const char *program_name)
{
    g_printerr("Usage: %s --url <URL> [--title <title>] [--size <width>x<height>]\n", 
               program_name);
    g_printerr("Example: %s --url https://example.com --title \"Single-window Browser\" --size 1200x800\n", 
               program_name);
}

int main(int argc, char **argv) 
{
    AppSettings settings = {
        .url = NULL,
        .title = NULL,
        .width = 1200,    /* Default width */
        .height = 800     /* Default height */
    };

    /* Parse command line arguments */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--url") == 0 && i + 1 < argc) {
            settings.url = argv[++i];
        } else if (strcmp(argv[i], "--title") == 0 && i + 1 < argc) {
            settings.title = argv[++i];
        } else if (strcmp(argv[i], "--size") == 0 && i + 1 < argc) {
            if (!parse_size(argv[++i], &settings.width, &settings.height)) {
                g_printerr("Error: Invalid size format. Use <width>x<height>\n");
                return 1;
            }
        } else {
            print_usage(argv[0]);
            return 1;
        }
    }

    /* Check if URL is provided and valid */
    if (!settings.url) {
        g_printerr("Error: URL is required\n");
        print_usage(argv[0]);
        return 1;
    }

    if (!g_str_has_prefix(settings.url, "http://") && 
        !g_str_has_prefix(settings.url, "https://")) {
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
    g_signal_connect(app, "activate", G_CALLBACK(activate), &settings);

    /* Run application */
    int status = g_application_run(G_APPLICATION(app), 0, NULL);

    /* Cleanup */
    g_object_unref(app);

    return status;
}
