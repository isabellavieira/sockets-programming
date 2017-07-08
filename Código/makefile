CC     = gcc -std=c99
CFLAGS = -g 
OBJS1 = oplista.o produtor_consumer.o
OBJS2 = fork.o 
OBJS3 = iterativo.o
OBJS4 = thread.o
OBJS5 = concorrente.o
TARGET = produtor_consumer fork iterativo thread concorrente

all: $(TARGET)

produtor_consumer: $(OBJS1) 
	$(CC) -pthread  $(CFLAGS) $(OBJS1) -o produtor_consumer

thread: $(OBJS4) 
	$(CC) -pthread  $(CFLAGS) $(OBJS4) -o thread

fork: $(OBJS2) 
	$(CC) -pthread  $(CFLAGS) $(OBJS2) -o fork

iterativo: $(OBJS3) 
	$(CC) -pthread  $(CFLAGS) $(OBJS3) -o iterativo

concorrente: $(OBJS5)
	$(CC) $(CFLAGS) $(OBJS5) -o concorrente

produtor_consumer.o: produtor_consumer.c  oplista.h
		$(CC) -pthread  $(CFLAGS) -c produtor_consumer.c

thread.o: thread.c
	$(CC) -pthread  $(CFLAGS) -c thread.c

oplista.o: oplista.c oplista.h
	$(CC) $(CFLAGS)  -c oplista.c

fork.o: fork.c
	$(CC) $(CFLAGS) -c fork.c

concorrente.o: concorrente.c oplista.h
	$(CC) $(CFLAGS) -c concorrente.c

clean: 
	rm -rf *.o ./$(TARGET)
