/opt/riscv/rv64imf/bin/riscv64-unknown-elf-gcc -I../test $1 ../test/lib.c -o my -ffast-math &&
/opt/riscv/bin/riscv64-unknown-elf-gcc -I../test $1 ../test/lib_spike.c -o spike -ffast-math &&
../build/Simulator my  &&
spike pk spike -isa=rv64gc
