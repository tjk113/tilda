CC = g++
S = src/
I = include/
B = bin/

FLAGS = -Iinclude/ -std=c++20

OBJ_FILES = $(B)main.o $(B)expression.o $(B)scanner.o $(B)statement.o $(B)token.o $(B)ast.o $(B)parser.o $(B)error.o $(B)interpreter.o

$(B)tilda.exe: $(OBJ_FILES)
	$(CC) $^ -o $@ $(FLAGS)

$(B)main.o: $(S)main.cpp $(I)scanner.hpp $(I)token.hpp $(I)ast.hpp $(I)expression.hpp $(I)parser.hpp $(I)tilda.hpp $(I)interpreter.hpp
	mkdir -p $(B)
	$(CC) -c $< -o $@ $(FLAGS)

$(B)expression.o: $(S)expression.cpp $(I)expression.hpp $(I)common.hpp $(I)token.hpp
	$(CC) -c $< -o $@ $(FLAGS)

$(B)scanner.o: $(S)scanner.cpp $(I)scanner.hpp $(I)token.hpp
	$(CC) -c $< -o $@ $(FLAGS)

$(B)statement.o: $(S)statement.cpp $(I)statement.hpp
	$(CC) -c $< -o $@ $(FLAGS)

$(B)token.o: $(S)token.cpp $(I)token.hpp
	$(CC) -c $< -o $@ $(FLAGS)

$(B)ast.o: $(S)ast.cpp $(I)ast.hpp $(I)expression.hpp $(I)token.hpp $(I)common.hpp
	$(CC) -c $< -o $@ $(FLAGS)

$(B)parser.o: $(S)parser.cpp $(I)parser.hpp $(I)expression.hpp $(I)token.hpp $(I)common.hpp $(I)error.hpp $(I)tilda.hpp
	$(CC) -c $< -o $@ $(FLAGS)

$(B)error.o: $(S)error.cpp $(I)error.hpp $(I)tilda.hpp
	$(CC) -c $< -o $@ $(FLAGS)

$(B)tilda.o: $(S)tilda.cpp $(I)tilda.hpp
	$(CC) -c $< -o $@ $(FLAGS)

$(B)interpreter.o : $(S)interpreter.cpp $(I)interpreter.hpp $(I)expression.hpp
	$(CC) -c $< -o $@ $(FLAGS)

exe: $(B)tilda.exe
	$(RM) $(OBJ_FILES)

debug: FLAGS += -g
debug: $(B)tilda.exe

clean:
	$(RM) $(B)tilda.exe $(OBJ_FILES)