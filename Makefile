CC      = gcc
CFLAGS  = -g
RM      = rm -f
RF      = rd /s /q
MF 		= mkdir -p
FILES	= src/**.c
default: all

all: build run

build:
	$(MF) bin
	$(CC) -o bin/n++.exe -Wall -O3 -Isrc/ $(FILES) -DCOMPILER
clean:
	rm -rf bin

update:
	clean
	Push.bat
run:
	echo on
	bin/n++ -i
	echo %ERRORLEVEL%