FLAGS=-std=c++17 -masm=intel -fconcepts -pthread -fopenacc -fopenmp -ggdb3 -Wpedantic -Wall -Wextra -Wconversion -Winline -Wsign-conversion -Weffc++ -Wstrict-null-sentinel -Wold-style-cast -Wnoexcept -Wctor-dtor-privacy -Woverloaded-virtual -Wsign-promo -Wzero-as-null-pointer-constant -Wsuggest-final-types -Wsuggest-final-methods -Wsuggest-override

all: main

main: main.cpp Cont.hpp Vect_fix.hpp
	g++ ${FLAGS} -o main main.cpp Cont.hpp Vect_fix.hpp

cont: Cont.hpp
	g++ ${FLAGS} -o cont Cont.hpp

vect: Vect.cpp Vect_fix.hpp
	g++ ${FLAGS} -o vect Vect.cpp Vect_fix.hpp

