CC = gcc
CFLAGS = -Wall -std=c99 -g
TARGET = beale
OBJS = $(TARGET).o liblista_char.o liblista_chave.o libbeale.o libbeale_cod_dec.o
LIBS = liblista_char.h liblista_chave.h libbeale.h libbeale_cod_dec.h

# regra padrao
all: $(TARGET)

# regras de ligacao
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# regras de compilacao
$(TARGET).o: $(TARGET).c $(LIBS)
	$(CC) $(CFLAGS) -c $(TARGET).c

libbeale.o: libbeale.c liblista_char.c libbeale_cod_dec.c
	$(CC) $(CFLAGS) -c libbeale.c

libbeale_cod_dec.o: libbeale_cod_dec.c libbeale_cod_dec.h
	$(CC) $(CFLAGS) -c libbeale_cod_dec.c

liblista_char.o: liblista_char.c liblista_char.h liblista_chave.c liblista_chave.h
	$(CC) $(CFLAGS) -c liblista_char.c

liblista_chave.o: liblista_chave.c liblista_chave.h
	$(CC) $(CFLAGS) -c liblista_chave.c

# auxiliares
clean:
	rm -f *.o

purge: clean
	rm -f $(TARGET)