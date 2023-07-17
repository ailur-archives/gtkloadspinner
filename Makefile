CC := gcc
CFLAGS := $(shell pkg-config --cflags gtk+-3.0)
LIBS := $(shell pkg-config --libs gtk+-3.0)
TARGET := loadspinner

all: $(TARGET)

$(TARGET): spinner.c
	$(CC) -o $@ $< $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)
