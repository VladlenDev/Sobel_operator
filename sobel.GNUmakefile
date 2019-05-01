SRC_DIR = src
OBJ_DIR = obj
CC = gcc
CFLAGS = -c -Wall -I"./include"
LDFLAGS =
SOURCES = $(SRC_DIR)/my_sobel.c $(SRC_DIR)/typeppm.c $(SRC_DIR)/sobel_operator.c
OBJECTS = $(OBJ_DIR)/my_sobel.o $(OBJ_DIR)/typeppm.o $(OBJ_DIR)/sobel_operator.o
EXECUTABLE = ppmred+

all: $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*.o $(EXECUTABLE)