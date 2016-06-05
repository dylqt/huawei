#For Gurobj-Project
CPP = g++
CARGS = -m64 -g
CPPLIB =-Wl,-rpath ./so/lib/ -L./so/lib/ -lgurobi_g++4.2 -lgurobi65

DIR = ./future_net/
LIB =$(DIR)lib/

Object = future_net.o route.o io.o
future : $(Object)
	$(CPP) $(CARGS) -o future $(Object) $(CPPLIB) -lm

future_net.o:$(DIR)future_net.cpp $(DIR)route.h $(LIB)lib_io.h $(LIB)lib_time.h
	g++ -c $(DIR)future_net.cpp -I$(LIB)

route.o:$(DIR)route.cpp $(DIR)route.h $(LIB)lib_record.h $(LIB)gurobi_c++.h
	$(CPP) $(CARGS) -c $(DIR)route.cpp -I$(LIB) $(DIR)

io.o:$(DIR)io.cpp
	g++ -c $(DIR)io.cpp
clean:
	rm future $(Object)
