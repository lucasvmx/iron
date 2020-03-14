sources=*.c
warnings=-Wall -Wextra
libs=`pkg-config --cflags --libs openssl`
output=mdcrack
options=-o $(output)

all:
	$(CC) $(sources) $(warnings) $(libs) $(options)

clean:
	$(RM) $(output)