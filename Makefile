# O nome dos dos programas que iremos construir
SHARED := shared
PROG1 := manager
PROG2 := feed

# Informação sobre diretórios usados neste projeto
SRC_DIR := src
HEADER_DIR := header
BUILD_DIR := build
BIN_DIR := bin


SHARED_SRC_DIR := $(SHARED)
PROG1_SRC_DIR := $(PROG1)/$(SRC_DIR)
PROG2_SRC_DIR := $(PROG2)/$(SRC_DIR)
SHARED_OBJ_DIR := $(BUILD_DIR)/$(SHARED)
PROG1_OBJ_DIR := $(BUILD_DIR)/$(PROG1)
PROG2_OBJ_DIR := $(BUILD_DIR)/$(PROG2)
# Encontra todos os ficheiros de código fonte 
SHARED_SRCS := $(wildcard $(SHARED_SRC_DIR)/*.c)
PROG1_SRCS := $(wildcard $(PROG1_SRC_DIR)/*.c) 
PROG2_SRCS := $(wildcard $(PROG2_SRC_DIR)/*.c)
# Ficheiros objeto que serão "linked"
# Fazemos isto para evitar compilações desnecessárias, consequente acelerando a
# construção dos binários
SHARED_OBJS := $(SHARED_SRCS:$(SHARED_SRC_DIR)/%.c=$(SHARED_OBJ_DIR)/%.o)
PROG1_OBJS := $(SHARED_OBJS) $(PROG1_SRCS:$(PROG1_SRC_DIR)/%.c=$(PROG1_OBJ_DIR)/%.o)
PROG2_OBJS := $(SHARED_OBJS) $(PROG2_SRCS:$(PROG2_SRC_DIR)/%.c=$(PROG2_OBJ_DIR)/%.o)

PROG1_BUILD_TREE := $(BUILD_DIR) $(PROG1_OBJ_DIR) $(SHARED_OBJ_DIR)  
PROG2_BUILD_TREE := $(BUILD_DIR) $(PROG2_OBJ_DIR) $(SHARED_OBJ_DIR)  
BUILD_TREE := $(PROG1_BUILD_TREE) $(PROG2_BUILD_TREE)
PROG1_BIN_TREE := $(BIN_DIR) $(BIN_DIR)/$(PROG1)  
PROG2_BIN_TREE := $(BIN_DIR) $(BIN_DIR)/$(PROG2)  
BIN_TREE := $(PROG1_BIN_TREE) $(PROG2_BIN_TREE)

CC := gcc
CFLAGS := -Wall -Wextra -lpthread
# Uncomment bellow to enable debugging info 
CFLAGS += -g


all: $(BUILD_TREE) $(BIN_TREE)

broker: $(PROG1_BUILD_TREE) $(PROG1_BIN_TREE)

feed: $(PROG2_BUILD_TREE) $(PROG2_BIN_TREE)


$(BIN_DIR)/$(PROG1): $(PROG1_OBJS)
	@echo A linkar $(PROG1)
	@$(CC) $(PROG1_OBJS) -o $@

$(BIN_DIR)/$(PROG2): $(PROG2_OBJS)
	@echo A linkar $(PROG2)
	@$(CC) $(PROG2_OBJS) -o $@

$(PROG1_OBJ_DIR)/%.o: $(PROG1_SRC_DIR)/%.c
	@echo A compilar $@
	@$(CC) $(CFLAGS) -c $< -o $@

$(PROG2_OBJ_DIR)/%.o: $(PROG2_SRC_DIR)/%.c
	@echo A compilar $@
	@$(CC) $(CFLAGS) -c $< -o $@

$(SHARED_OBJ_DIR)/%.o: $(SHARED_SRC_DIR)/%.c
	@echo A compilar $@
	@$(CC) $(CFLAGS) -c $< -o $@


$(BUILD_DIR):
	@echo "A criar diretório build"
	@mkdir -p $@

$(PROG1_OBJ_DIR):
	@mkdir -p $@

$(PROG2_OBJ_DIR):
	@mkdir -p $@

$(SHARED_OBJ_DIR):
	@mkdir -p $@

$(BIN_DIR):
	@echo "A criar diretório bin"
	@mkdir -p $@

clean: clean_pipes
	@echo "A remover diretórios build e bin"
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

clean_pipes:
	@echo "A remover pipes"
	@rm -f srv_pipe cli_*
