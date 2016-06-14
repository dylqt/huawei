#For Gurobj-Project
CPP = g++
CARGS = -m64 -g
CPPLIB =-Wl,-rpath . -L./lib/ -llpsolve55

DIR = ./
LIB =$(DIR)lib/

Object = future_net.o route.o io.o
future : $(Object)
	$(CPP) $(CARGS) -o ./future_net $(Object) $(CPPLIB) -lm -ldl

future_net.o:$(DIR)future_net.cpp $(DIR)route.h $(LIB)lib_io.h $(LIB)lib_time.h
	g++ -c $(DIR)future_net.cpp -I$(LIB) $(DIR)

route.o:$(DIR)route.cpp $(DIR)route.h $(LIB)lib_record.h $(LIB)lp_lib.h
	$(CPP) $(CARGS) -c $(DIR)route.cpp -I$(LIB) $(DIR)

io.o:$(DIR)io.cpp
	g++ -c $(DIR)io.cpp
clean:
	rm ./future_net $(Object)
