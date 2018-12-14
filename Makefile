debug/main: build/bitmap.o build/cmd.o build/dir.o build/init.o build/main.o build/stat.o build/inode.o build/read_write.o build/block.o
	g++ build/bitmap.o build/cmd.o build/dir.o build/init.o build/main.o build/stat.o build/inode.o build/read_write.o build/block.o -o debug/main

include/fs.h: include/buffer.h
	touch include/fs.h

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

build/read_write.o: src/read_write.cpp include/fs.h include/debug.h
	g++ -c src/read_write.cpp -o build/read_write.o

build/block.o: src/block.o include/fs.h include/debug.h
	g++ -c src/block.cpp -o build/block.o

clean:
	rm build/* debug/main