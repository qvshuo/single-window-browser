CFLAGS := $(shell pkg-config --cflags --libs gtk4 webkitgtk-6.0)

browser: browser.c
	gcc browser.c -o browser $(CFLAGS)
