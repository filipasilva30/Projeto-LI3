CC = gcc
CFLAGS = -Wall -Wextra -pedantic-errors -O3 -g -I./include
LIBS = -lm -lncurses# Adicione outras bibliotecas se necessário

# Nome do executável
EXECUTABLE = programa-principal

# Arquivos fonte
SOURCE_FILES = $(wildcard src/*.c)

# Arquivos objeto
OBJ_FILES = $(SOURCE_FILES:.c=.o)

# Regra padrão
all: $(EXECUTABLE)

# Como construir o executável
$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(EXECUTABLE) $(LIBS)

# Como construir os arquivos objeto
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpar arquivos objeto e executável
clean:
	rm -f $(EXECUTABLE) $(OBJ_FILES)
	rm -f Resultados/*.txt Resultados/*.csv