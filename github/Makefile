comp: 
	gcc -o cm -g ./src/*.c

test: comp
	./cm input 
	cc -z noexecstack out.s
	./a.out
	@echo "Test 1"
	./cm input2
	cc -z noexecstack out.s
	@echo "Test 2"
	./a.out
	./cm input3
	cc -z noexecstack out.s
	@echo "Test 3"
	./a.out
clean:
	rm -f out* *.o cm a.out
