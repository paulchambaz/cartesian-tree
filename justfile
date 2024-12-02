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

publish:
  mkdir complex-damienassire-paulchambaz
  cp -r paper/rapport.pdf Makefile src include complex-damienassire-paulchambaz
  tar czf complex-damienassire-paulchambaz.tar.gz complex-damienassire-paulchambaz
  rm -fr complex-damienassire-paulchambaz
