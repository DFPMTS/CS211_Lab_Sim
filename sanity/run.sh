/opt/riscv/rv64imf/bin/riscv64-unknown-elf-gcc -O3 -I../test $1 ../test/lib.c -o my -ffast-math &&
/opt/riscv/bin/riscv64-unknown-elf-gcc -O3 -I../test $1 ../test/lib_spike.c -o spike -ffast-math &&
../build/Simulator my > my.out &&
spike pk spike -isa=rv64gc > spike_raw.out &&
tail -n +2 spike_raw.out > spike.tmp && 
mv spike.tmp spike.out
