CC=g++
FLAG=-c
OUTPUT=-o
ALL=convert.o
OBJ=convert

$(OBJ):$(ALL)
	$(CC) $^ $(OUTPUT) $@
%.o:%.c
	$(CC) $(FLAG) $< $(OUTPUT) $@
.PHONY:clean
clean:
	@rm -f *.o  $(OBJ)
	@echo "del ok."


