final: final.o pacman.o gfxnew.o
	g++ -std=c++14 -lX11 final.o pacman.o gfxnew.o -o final

final.o: final.cpp gfxnew.h
	g++ -c -std=c++14 final.cpp -o final.o

pacman.o: pacman.cpp
	g++ -c -std=c++14 pacman.cpp -o pacman.o

clean:
	rm final.o pacman.o final
