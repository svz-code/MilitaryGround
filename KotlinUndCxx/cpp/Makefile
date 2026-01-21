all:
	clang++ -c -fPIC -I/usr/lib/jvm/java-17-openjdk/include -I/usr/lib/jvm/java-17-openjdk/include/linux main.cpp -o main.o
	clang++ -shared -fPIC -o libarducat.so main.o -lc

clear:
	rm -rf main.o libarducat.so
