run: compile
  ./target/release/bin

compile: clean
  make

clean:
  make clean

valgrind:
  valgrind ./target/release/bin

debug:
  make debug
