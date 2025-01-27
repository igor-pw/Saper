CFLAGS = $(shell pkg-config --cflags gtk+-3.0)
LDFLAGS = $(shell pkg-config --libs gtk+-3.0)
TARGET = bin/saper

saper: src/main.c src/menu.c src/board.c src/after_game.c
	@gcc -o $(TARGET) src/main.c src/menu.c src/board.c src/after_game.c src/mouse_click.c $(CFLAGS) $(LDFLAGS)

run: 
	./$(TARGET)

clean:
	rm -f $(TARGET)
