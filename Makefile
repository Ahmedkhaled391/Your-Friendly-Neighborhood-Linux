CC = gcc
CFLAGS = -Wall -Wextra -O2 $(shell pkg-config --cflags gtk4)
LDFLAGS = $(shell pkg-config --libs gtk4)
TARGET = yfnl

SRCDIR = .
OBJDIR = obj

SOURCES = \
    main.c \
    app/window.c \
    app/sidebar.c \
    app/pages/pages.c \
    app/pages/page_setup.c \
    app/pages/page_perf.c \
    app/pages/page_custom.c \
    app/pages/page_tools.c \
    app/pages/page_generator.c \
    app/pages/page_about.c \
    app/pages/page_firewall.c \
    app/widgets/card.c \
    logic/actions.c \
    logic/runner.c \
    logic/passwordGenerator.c

OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean