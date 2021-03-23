DEPS = input_helper.h huffman_tree.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.cpp
	g++ -std=c++11 *.cpp -o main