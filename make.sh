gcc main.c stella.c memory.c cpu.c globals.c tia.c color.c pia.c addressing.c ops.c \
    -lglut -lGL -ggdb -Wall -Werror -o atari -pthread -lm -I/usr/local/include/SDL2 -D_REENTRANT \
    -L/usr/local/lib -Wl,-rpath,/usr/local/lib -Wl,--enable-new-dtags -lSDL2
