for size in 1000000 2000000 5000000 
    do
    for thread in {1..16}
        do 
            build/axpy_pthread $thread $size 0.5;
        done

    for thread in {1..16}
        do 
            build/axpy_cilk $thread $size 0.5;
        done
    done