all: ipc.c
	gcc -Wall -shared -fPIC -o ipc.so ipc.c -ldl

run:
	(LD_PRELOAD="./ipc.so" python3 test.py)
	
clean:
	rm -f ipc.so example
