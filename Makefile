cc = clang++
cflags = -O2 -std=c++17 -g
lflags =
starget = ./bin/libpersonal.a
dtarget = ./bin/libpersonal.so

static: $(starget)

dynamic: $(dtarget)

install: $(starget) $(dtarget)
	sudo ./install.sh 

$(starget): ./tmp/personal.o 
	ar cr $@ $^

$(dtarget): ./tmp/personal_share.o 
	$(cc) -shared -fPIC -o $@ $^

./tmp/personal_share.o: ./src/personal.cpp ./src/include/personal.hpp
	$(cc) -fPIC $(cflags) -c -o $@ $<

./tmp/personal.o: ./src/personal.cpp ./src/include/personal.hpp
	$(cc) $(cflags) -c -o $@ $<

.phony: clean

clean:
	rm -f ./tmp/*.o ./bin/*