# Nome do compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -pthread -g

# Nome do programa
TARGET = unisex_bathroom

# Regras principais
all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c

# Limpar arquivos gerados
clean:
	rm -f $(TARGET) *.o

# Executar o programa
run: $(TARGET)
	./$(TARGET)
