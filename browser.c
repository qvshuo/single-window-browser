/*
 * 基于 GTK4 和 WebKitGTK 6.0 的简单浏览器
 * 用法: browser --url <URL> [--title <标题>] [--size <宽度>x<高度>]
 * 示例: browser --url https://example.com --title "单窗口浏览器" --size 1200x800
 */

#include <gtk/gtk.h>
#include <webkit/webkit.h>
#include <string.h>

/* 应用程序设置结构体 */
typedef struct {
    const char *url;
    const char *title;
    int width;
    int height;
} AppSettings;

/* 窗口激活回调函数 */
static void activate(GtkApplication *app, gpointer user_data) 
{
    AppSettings *settings = (AppSettings *)user_data;

    /* 创建主窗口 */
    GtkWidget *window = gtk_application_window_new(app);
    if (!window) {
        g_critical("Failed to create window");
        return;
    }

    /* 配置窗口属性 */
    gtk_window_set_title(GTK_WINDOW(window), 
                        settings->title ? settings->title : "Single-window Browser");
    gtk_window_set_default_size(GTK_WINDOW(window), 
                               settings->width, settings->height);

    /* 创建 WebKit 视图 */
    WebKitWebView *web = WEBKIT_WEB_VIEW(webkit_web_view_new());
    if (!web) {
        g_critical("Failed to create web view");
        return;
    }

    /* 将 WebKit 视图设置为窗口的子部件 */
    gtk_window_set_child(GTK_WINDOW(window), GTK_WIDGET(web));

    /* 加载 URL */
    webkit_web_view_load_uri(web, settings->url);

    /* 设置焦点到 WebKit 视图 */
    gtk_widget_grab_focus(GTK_WIDGET(web));

    /* 显示窗口 */
    gtk_window_present(GTK_WINDOW(window));
}

/* 解析窗口尺寸字符串（格式：宽度x高度） */
static gboolean parse_size(const char *size_str, int *width, int *height)
{
    char *endptr;
    
    /* 解析宽度 */
    *width = strtol(size_str, &endptr, 10);
    if (*endptr != 'x' || *width <= 0) {
        return FALSE;
    }

    /* 解析高度 */
    *height = strtol(endptr + 1, &endptr, 10);
    if (*endptr != '\0' || *height <= 0) {
        return FALSE;
    }

    return TRUE;
}

/* 打印使用信息 */
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
        .width = 1200,    /* 默认宽度 */
        .height = 800     /* 默认高度 */
    };

    /* 解析命令行参数 */
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

    /* 检查是否提供了 URL 且格式有效 */
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

    /* 创建应用程序实例 */
    GtkApplication *app = gtk_application_new("org.gtk.single-window-browser",
                                            G_APPLICATION_DEFAULT_FLAGS);
    if (!app) {
        g_critical("Failed to create application");
        return 1;
    }

    /* 连接激活信号 */
    g_signal_connect(app, "activate", G_CALLBACK(activate), &settings);

    /* 运行应用程序 */
    int status = g_application_run(G_APPLICATION(app), 0, NULL);

    /* 清理 */
    g_object_unref(app);

    return status;
}
