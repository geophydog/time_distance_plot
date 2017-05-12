OBJ = dist-time.o sacio.o

dist-time : $(OBJ)
	cc -o dist-time $(OBJ)

$(OBJ) : sacio.h

clean : 
	rm -f $(OBJ) dist-time
