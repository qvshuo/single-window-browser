# Define CFLAGS using pkg-config for GTK4 and WebKitGTK
CFLAGS := $(shell pkg-config --cflags --libs gtk4 webkitgtk-6.0)

# Additional compiler flags
CFLAGS += -Wall -Wextra -O2

# Target for building the browser
browser: browser.c
	gcc browser.c -o browser $(CFLAGS)
