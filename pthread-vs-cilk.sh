for thread in 2 4 6 8 12 16
    do 
        build/axpy_pthread $thread 1000000 0.5;
    done

for thread in 2 4 6 8 12 16
    do 
        build/axpy_cilk $thread 1000000 0.5;
    done