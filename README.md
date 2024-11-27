# single-window-browser
A lightweight, single-window browser inspired by the [one window browser](https://wiki.gnome.org/Projects/WebKitGtk/ProgrammingGuide/Tutorial), utilizing GTK4 and WebKit GTK 6.0.

---

## Requirements

Ensure you have the following development packages installed:


### Fedora:
```sh
sudo dnf install gtk4-devel webkitgtk6.0-devel
```

### Debian/Ubuntu:
```sh
sudo apt install libwebkitgtk-6.0-dev libgtk-4-dev
```

## Build

### Using Make:
```sh
make
```

### Using GCC directly:
```sh
gcc browser.c -o browser $(pkg-config --cflags --libs gtk4 webkitgtk-6.0) -Wall -Wextra -O2
```

## Usage

```sh
./browser https://example.com  # Usage example
./browser                      # Display usage instructions
./browser example.com          # Show URL format error message
```
