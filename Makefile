CPPFLAGS = -std=c++17 -Wall -Wextra -Wshadow -pedantic -march=native

.PHONY: clean

main: main.cpp VByte.hpp bitarray.hpp
	g++ $(CPPFLAGS) -DNDEBUG -Ofast -o main main.cpp


clean:
	rm -f main