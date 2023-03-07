all: build run clean

build:
	@gcc main.c -o tema1 -Wall -Werror -lpthread -lm

run:
	@./tema1 2 4 test0/test.txt

clean:
	@rm tema1
	
# clean-files:
# 	@rm out*
