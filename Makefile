CC = gcc
CFLAGS = -Wall -Wextra -g

build: tema2

tema2: stack.o queue.o utils.o main.o linked_list.o browser.o
	$(CC) $(CFLAGS) $^ -o tema2

main.o: main.c
	$(CC) $(CFLAGS) $^ -c -o $@

linked_list.o: linked_list.c
	$(CC) $(CFLAGS) $^ -c -o $@

browser.o: browser.c
	$(CC) $(CFLAGS) $^ -c -o $@

stack.o: stack.c
	$(CC) $(CFLAGS) $^ -c -o $@

queue.o: queue.c
	$(CC) $(CFLAGS) $^ -c -o $@

utils.o: utils.c
	$(CC) $(CFLAGS) $^ -c -o $@

run: tema2
	./tema2

clean:
	rm *.o tema2
