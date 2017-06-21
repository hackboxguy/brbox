how to compile:
gcc -Wall playloop.c -o playloop $(pkg-config --cflags --libs gstreamer-1.0)

how to run:
./playloop file://`pwd`/sample.mkv
