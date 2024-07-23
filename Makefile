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
	gcc -o bin/n++.exe -Isrc/ $(FILES) -DCOMPILER
	gcc -o bin/n++I.exe -Isrc/ $(FILES) -DINTERPRETER
clean:
	rm -rf bin

update:
	clean
	Push.bat
run:
	echo on
	bin/n++ -i
	echo %ERRORLEVEL%