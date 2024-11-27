# 使用 pkg-config 为 GTK4 和 WebKitGTK 定义 CFLAGS
CFLAGS := $(shell pkg-config --cflags --libs gtk4 webkitgtk-6.0)

# 额外的编译器标志
CFLAGS += -Wall -Wextra -O2

# 构建浏览器的目标
browser: browser.c
	gcc browser.c -o browser $(CFLAGS)
