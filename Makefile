# O nome dos dos programas que iremos construir
PROG1 := manager
PROG2 := feed

# Informação sobre diretórios usados neste projeto
SRC_DIR := src
HEADER_DIR := header
BUILD_DIR := build
BIN_DIR := bin
SHARED_SRC_DIR := shared

# Ferramentas para processamento e compilação de ficheiros de código fonte
CC := gcc
FORMATTER = clang-format


# Encontra todos os ficheiros de código fonte 
PROG1_SRCS := $(wildcard $(PROG1)/$(SRC_DIR)/*.c)
PROG2_SRCS := $(wildcard $(PROG2)/$(SRC_DIR)/*.c)
PROG1_SRCS += $(wildcard $(SHARED_SRC_DIR)/*.c)
PROG2_SRCS += $(wildcard $(SHARED_SRC_DIR)/*.c)
# Caso o projeto cresca muito, pode ser conveniente compilar ficheiros objeto
# antes de fazer o seu linking
PROG1_OBJS := $(patsubst %.c, %.o, $(PROG1_SRCS))
PROG2_OBJS := $(patsubst %.c, %.o, $(PROG2_SRCS))


all: $(BUILD_DIR) $(BIN_DIR) compile


# Compilação dos executáveis
compile: $(BIN_DIR)/$(PROG1) $(BIN_DIR)/$(PROG2)

$(BIN_DIR)/$(PROG1): $(PROG1_SRCS)
	@echo A compilar $(PROG1)
	@$(CC) $(PROG1_SRCS) -o $@

$(BIN_DIR)/$(PROG2): $(PROG2_SRCS)
	@echo A compilar $(PROG2)
	@$(CC) $(PROG2_SRCS) -o $@


$(BUILD_DIR) $(BIN_DIR):
	@echo "A criar diretórios build e bin"
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

clean:
	@echo "A remover diretórios build e bin"
	@rm -rf $(BUILD_DIR) $(BIN_DIR)
