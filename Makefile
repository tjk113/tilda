CC = g++
S = src/
I = include/
B = bin/

FLAGS = -Iinclude/ -std=c++20

OBJ_FILES = $(B)main.o $(B)expression.o $(B)scanner.o $(B)statement.o $(B)token.o

$(B)tilda.exe: $(OBJ_FILES)
	$(CC) $^ -o $@ $(FLAGS)

$(B)main.o: $(S)main.cpp $(I)scanner.hpp $(I)token.hpp
	$(CC) -c $< -o $@ $(FLAGS)

$(B)expression.o: $(S)expression.cpp $(I)expression.hpp
	$(CC) -c $< -o $@ $(FLAGS)

$(B)scanner.o: $(S)scanner.cpp $(I)scanner.hpp $(I)token.hpp
	$(CC) -c $< -o $@ $(FLAGS)

$(B)statement.o: $(S)statement.cpp $(I)statement.hpp
	$(CC) -c $< -o $@ $(FLAGS)

$(B)token.o: $(S)token.cpp $(I)token.hpp
	$(CC) -c $< -o $@ $(FLAGS)

tilda:
	exe
	@del $(OBJ_FILES)
exe:
	tilda.exe
clean:
	@del tilda.exe $(OBJ_FILES)