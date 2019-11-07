CC:=g++
exe:=sandbox
obj:=main.o
san:=sandbox.so

all: sand main
main:$(obj)
	$(CC) -o $(exe) $(obj)  
sand:
	$(CC) -shared -fPIC -o $(san) sandbox.c -ldl
%.o:%.c
	$(CC) -c $^ -o $@
clean:
	rm -rf $(obj) $(exe)
