#include "../headers/emissor.h"

void processCmd(packet *p, char *input, char *username) {
  char cmd[12];
  sscanf(input, "%s", cmd);

  char *offset = strchr(input, ' ');

  if (strcmp(cmd, "topics")) {
    writeEmptyPacket(p, 0);
  } else if (strcmp(cmd, "exit")) {
    writeEmptyPacket(p, 4);
  } else if (strcmp(cmd, "subscribe")) {
    char topic[TAM_NOME_TOPICO];
    strcpy(topic, offset + 1);
    writeSingleValPacket(p, 2, topic, strlen(topic));
  } else if (strcmp(cmd, "unsubscribe")) {
    char topic[TAM_NOME_TOPICO];
    strcpy(topic, offset + 1);
    writeSingleValPacket(p, 3, topic, strlen(topic));
  } else if (strcmp(cmd, "msg")) {
    char topic[TAM_NOME_TOPICO];
    int i;
    for (i = 0; i < TAM_NOME_TOPICO - 1 || offset[i + 1] == ' '; i++)
      topic[i] = offset[i + 1];
    topic[i] = '\0';

    offset = strchr(offset + 1, ' ');
    int duracao;
    sscanf(offset + 1, "%d", &duracao);

    offset = strchr(offset + 1, ' ');
    char msg[TAM_CORPO_MSG];
    sscanf(offset + 1, "%s", msg);

    writeMsgPacket(p, 1, duracao, topic, username, msg);
  } else if (strcmp(cmd, "help")) {
    printf("\ntopics - Mostrar uma lista com todos os tópicos ");
    printf("\nmsg <topico> <duração> <mensagem> - Enviar mensagem para um "
           "determinado tópico");
    printf("\nsubscribe <nome do tópico> - Subscrever um determinado tópico");
    printf("\nunsubscribe <nome do topico> - Deixar de subscrever um "
           "determinado tópico");
    printf("\nexit - Sair\n ");
  } else {
    printf("Comando não reconhecido");
  }
}
