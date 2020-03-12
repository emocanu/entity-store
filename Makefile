# I am deeply sorry that I cannot make a more reliable makefile 
# I have not worked in years with makefiles
#
program = main

CXXFLAGS = -std=c++17 -Wall -O3

all: $(program)

main: main.o Store.o SimpleStore.o Test.o
	$(CXX) $(CXXFLAGS) $(CXXFLAGS) -o main main.o Store.o SimpleStore.o Test.o

main.o: main.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c main.cpp

Store.o: Store.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c Store.cpp

SimpleStore.o: SimpleStore.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c SimpleStore.cpp

Test.o: Test.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c Test.cpp

run: $(program)
	./$(program)

clean:
	$(RM) -f $(program)
