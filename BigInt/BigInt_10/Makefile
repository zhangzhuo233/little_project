ALL=TestMain.o BigInt.o
OBJ=TestMain
$(OBJ):$(ALL)
	g++ $^ -o $@ -g
%.o:%.c
	g++ -c $< -o $@
.PHONY:clean
clean:
	@/bin/rm -f *.o $(OBJ)
	@echo "clean ok."
