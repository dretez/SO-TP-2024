#include "../headers/emissor.h"

int processCmd(packet *p, char *input, char *username) {
  char cmd[12];
  int offset = 0;
  if (input[0] == ' ') {
    offset = nextword(input, 0, TAM_CMD_INPUT);
    offset = offset == -1 ? 0 : offset;
    wordncpy(cmd, &input[offset], 12);
  } else {
    wordncpy(cmd, input, 12);
  }

  if (!strcmp(cmd, "topics")) {
    writeEmptyPacket(p, P_TYPE_USER_TOPIC);

  } else if (!strcmp(cmd, "exit")) {
    writeEmptyPacket(p, P_TYPE_USER_EXIT);

  } else if (!strcmp(cmd, "subscribe")) {
    offset = nextword(input, offset, TAM_CMD_INPUT);
    if (offset == -1) {
      printf(FEED_ERR_FMT FEED_CMD_FMT_SUB);
      return -1;
    }
    if (wordlen(&input[offset]) >= TAM_NOME_TOPICO) {
      printf("Nome de tópico muito longo\n");
      return -1;
    }
    char topic[TAM_NOME_TOPICO];
    wordncpy(topic, &input[offset], TAM_NOME_TOPICO);
    writeSingleValPacket(p, P_TYPE_USER_SUB, topic, strlen(topic) + 1);

  } else if (!strcmp(cmd, "unsubscribe")) {
    offset = nextword(input, offset, TAM_CMD_INPUT);
    if (offset == -1) {
      printf(FEED_ERR_FMT FEED_CMD_FMT_UNSUB);
      return -1;
    }
    if (wordlen(&input[offset]) >= TAM_NOME_TOPICO) {
      printf("Nome de tópico muito longo\n");
      return -1;
    }
    char topic[TAM_NOME_TOPICO];
    wordncpy(topic, &input[offset], TAM_NOME_TOPICO);
    writeSingleValPacket(p, P_TYPE_USER_UNSUB, topic, strlen(topic) + 1);

  } else if (!strcmp(cmd, "msg")) {
    offset = nextword(input, offset, TAM_CMD_INPUT);
    if (offset == -1) {
      printf("%s%s\n", FEED_ERR_FMT, FEED_CMD_FMT_MSG);
      return -1;
    }
    if (wordlen(&input[offset]) >= TAM_NOME_TOPICO) {
      printf("Nome de tópico muito longo\n");
      return -1;
    }
    char topic[TAM_NOME_TOPICO];
    wordncpy(topic, &input[offset], TAM_NOME_TOPICO);

    offset = nextword(input, offset, TAM_CMD_INPUT);
    if (offset == -1) {
      printf("%s%s\n", FEED_ERR_FMT, FEED_CMD_FMT_MSG);
      return -1;
    }
    int duracao = atoi(&input[offset]);
    duracao = duracao > 0 ? duracao : 0;

    offset = nextword(input, offset, TAM_CMD_INPUT);
    if (offset == -1) {
      printf("%s%s\n", FEED_ERR_FMT, FEED_CMD_FMT_MSG);
      return -1;
    }
    if (strlen(&input[offset]) > TAM_CORPO_MSG - 1) {
      printf("Mensagem muito longa\n");
      return -1;
    }
    char msg[TAM_CORPO_MSG];
    strncpy(msg, &input[offset], TAM_CORPO_MSG);

    writeMsgPacket(p, P_TYPE_USER_MSG, duracao, topic, username, msg);

  } else if (!strcmp(cmd, "help")) {
    printf(FEED_CMD_FMT_TOPIC " - Mostrar uma lista com todos os tópicos\n");
    printf(FEED_CMD_FMT_MSG " - Enviar mensagem para um determinado tópico\n");
    printf(FEED_CMD_FMT_SUB " - Subscrever um determinado tópico\n");
    printf(FEED_CMD_FMT_UNSUB
           " - Deixar de subscrever um determinado tópico\n");
    printf(FEED_CMD_FMT_EXIT " - Sair\n");
    return -1;

  } else {
    printf("Comando não reconhecido\n");
    return -1;
  }

  return 0;
}
