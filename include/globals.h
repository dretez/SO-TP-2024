// Dados mensagens
#define TAM_NOME_TOPICO 20
#define TAM_CORPO_MSG 300

typedef struct msg {
  // outros dados relevantes
  char topico[TAM_NOME_TOPICO];
  char msg[TAM_CORPO_MSG];
} mensagem;
