debug/main: build/bitmap.o build/cmd.o build/dir.o build/init.o build/main.o build/stat.o build/inode.o
	g++ build/bitmap.o build/cmd.o build/dir.o build/init.o build/main.o build/stat.o build/inode.o -o debug/main

build/bitmap.o: src/bitmap.cpp include/fs.h include/debug.h
	g++ -c src/bitmap.cpp -o build/bitmap.o

build/cmd.o: src/cmd.cpp include/fs.h include/debug.h
	g++ -c src/cmd.cpp -o build/cmd.o

build/dir.o: src/dir.cpp include/fs.h include/debug.h
	g++ -c src/dir.cpp -o build/dir.o

build/init.o: src/init.cpp include/fs.h include/debug.h
	g++ -c src/init.cpp -o build/init.o

build/stat.o: src/stat.cpp include/fs.h include/debug.h
	g++ -c src/stat.cpp -o build/stat.o

build/main.o: src/main.cpp include/fs.h include/debug.h
	g++ -c src/main.cpp -o build/main.o

build/inode.o: src/inode.cpp include/fs.h include/debug.h
	g++ -c src/inode.cpp -o build/inode.o

clean:
	rm build/* debug/main