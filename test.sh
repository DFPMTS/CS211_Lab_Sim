riscv64-unknown-elf-gcc $1.c ../test/lib.c -ffast-math -o $1.rv &&
riscv64-unknown-elf-objdump -d $1.rv > $1.S &&
pushd /home/dfpmts/cs211/lab_sim/build && make Simulator -C /home/dfpmts/cs211/lab_sim/build && popd &&
./Simulator $1.rv