while true; do
    ./a.out
    ./run.sh test.c
    if diff my.out spike.out; then
        printf "PASS\n"
    else
        printf "ERROR!\n"
        exit 0
    fi
done
