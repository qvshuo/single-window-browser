# one-window-browser
Recreation of [one window browser](https://wiki.gnome.org/Projects/WebKitGtk/ProgrammingGuide/Tutorial), but with GTK4 and WebKit GTK 6.0

## Requirements

This requires GTK4 and WebKitGTK 6.0 with development files (includes), on Debian/Ubuntu install using:

```sh
sudo apt install libwebkitgtk-6.0-dev libgtk-4-dev
```

## Build

Build by running `make` or `gcc browser.c -o browser $(pkg-config --cflags --libs gtk4 webkitgtk-6.0)`

## Run

Using `./browser <url>`