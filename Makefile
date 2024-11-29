# O nome dos dos programas que iremos construir
PROG1 := manager
PROG2 := feed

# Informação sobre diretórios usados neste projeto
SRC_DIR := src
HEADER_DIR := header
BUILD_DIR := build
BIN_DIR := bin


PROG1_SRC_DIR := $(PROG1)/$(SRC_DIR)
PROG2_SRC_DIR := $(PROG2)/$(SRC_DIR)
SHARED_SRC_DIR := shared
# Encontra todos os ficheiros de código fonte 
SHARED_SRCS := $(wildcard $(SHARED_SRC_DIR)/*.c)
PROG1_SRCS := $(wildcard $(PROG1_SRC_DIR)/*.c) 
PROG2_SRCS := $(wildcard $(PROG2_SRC_DIR)/*.c)
# Caso o projeto cresca muito, pode ser conveniente compilar ficheiros objeto
# antes de fazer o seu linking
SHARED_OBJS := $(SHARED_SRCS:$(SHARED_SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
PROG1_OBJS := $(PROG1_SRCS:$(PROG1_SRC_DIR)/%.c=$(BUILD_DIR)/%.o) $(SHARED_OBJS)
PROG2_OBJS := $(PROG2_SRCS:$(PROG2_SRC_DIR)/%.c=$(BUILD_DIR)/%.o) $(SHARED_OBJS)

# Ferramentas para processamento e compilação de ficheiros de código fonte
CC := gcc
FORMATTER = clang-format

CFLAGS := -Wall -lpthread


all: $(BUILD_DIR) $(BIN_DIR) $(BIN_DIR)/$(PROG1) $(BIN_DIR)/$(PROG2)


$(BIN_DIR)/$(PROG1): $(PROG1_OBJS) $(SHARED_OBJS)
	@echo A linkar $(PROG1)
	@$(CC) $(CFLAGS) $(PROG1_OBJS) -o $@

$(BIN_DIR)/$(PROG2): $(PROG2_OBJS) $(SHARED_OBJS)
	@echo A linkar $(PROG2)
	@$(CC) $(CFLAGS) $(PROG2_OBJS) -o $@

$(BUILD_DIR)/%.o: $(PROG1_SRC_DIR)/%.c
	@echo A compilar $@
	@$(CC) -c $< -o $@

# $(BUILD_DIR)/$(PROG2_OBJS): %.o: %.c
$(BUILD_DIR)/%.o: $(PROG2_SRC_DIR)/%.c
	@echo A compilar $@
	@$(CC) -c $< -o $@

$(BUILD_DIR)/%.o: $(SHARED_SRC_DIR)/%.c
	@echo A compilar $@
	@$(CC) -c $< -o $@


$(BUILD_DIR):
	@echo "A criar diretório build"
	@mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	@echo "A criar diretório bin"
	@mkdir -p $(BIN_DIR)

clean:
	@echo "A remover diretórios build e bin"
	@rm -rf $(BUILD_DIR) $(BIN_DIR)
	@rm srv_pipe cli_*
