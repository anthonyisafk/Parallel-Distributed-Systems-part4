for thread in 2 4 6 8 12 16
    do 
        src/./pthread $thread 1000000 0.5;
    done

for thread in 2 4 6 8 12 16
    do 
        src/./axpycilk $thread 1000000 0.5;
    done