CC=g++
FLAG=-c
OUTPUT=-o
GDB=-g
ALL=main.o convertcomment.o
OBJ=main

$(OBJ):$(ALL)
	$(CC) $^ $(OUTPUT) $@
%.o:%.c
	$(CC) $(FLAG) $< $(OUTPUT) $@
.PHONY:clean
clean:
	@rm -f *.o output.c $(OBJ)
	@echo "del ok."


