run: compile
  ./target/release/bin

compile:
  make

clean:
  make clean

valgrind:
  valgrind ./target/release/bin

debug:
  make debug
  gdb ./target/debug/bin
