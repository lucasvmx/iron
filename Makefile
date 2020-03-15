sources=*.c
warnings=-Wall -Wextra
libs=`pkg-config --cflags --libs openssl` -lpthread
output=mdcrack
options=-o $(output)

.PHONY: benchmark

all:
	$(CC) $(sources) $(warnings) $(libs) $(options)

benchmark:
	$(CC) $(sources) $(warnings) $(libs) $(options) -DBENCHMARK

clean:
	$(RM) $(output)