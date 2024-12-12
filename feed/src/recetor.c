#include "../headers/recetor.h"

int processPacket(packet *p) {
  switch (p->head.tipo_msg) {
  case P_TYPE_MNGR_SUCCESS: {
    if (processSucess(p))
      return 1;
    break;
  }

  case P_TYPE_MNGR_ERROR: {
    processError(p);
    break;
  }

  case P_TYPE_MNGR_TOPIC: {
    int offset = 0;
    unsigned short ntopics;
    memcpy(&ntopics, &p->buf[offset], sizeof(unsigned short));
    offset += sizeof(unsigned short);
    for (int i = 0; i < ntopics; i++) {
      printf("%s\n", &p->buf[offset]);
      offset += strlen(&p->buf[offset]) + 1;
    }
    break;
  }

  case P_TYPE_MNGR_MSG: {
    int offset = 0;

    char *uname = malloc(sizeof(char) * (strlen(&p->buf[offset] + 1)));
    if (uname == NULL) {
      printf("Erro ao receber mensagem\n");
      break;
    }
    strcpy(uname, &p->buf[offset]);
    offset += strlen(uname) + 1;
    char topico[TAM_NOME_TOPICO];
    strcpy(topico, &p->buf[offset]);
    offset += strlen(topico) + 1;
    char msg[TAM_CORPO_MSG];
    strcpy(msg, &p->buf[offset]);
    printf(NOTIF_FEED_MSG, topico, uname, msg);
    free(uname);
    break;
  }

  case P_TYPE_MNGR_RMUSR: {
    printf("%s saiu\n", p->buf);
    break;
  }

  case P_TYPE_MNGR_CLOSE: {
    printf("O manager foi terminado.\n");
    return 1;
  }

  default:
    printf("[ERRO] Tipo de mensagem desconhecido: %d\n", p->head.tipo_msg);
    break;
  }

  return 0;
}

int processSucess(packet *p) {
  int scsCode;
  memcpy(&scsCode, p->buf, sizeof(int));

  switch (scsCode) {
  case P_SCS_GENERIC: {
    break;
  }
  case P_SCS_SUB: {
    printf(NOTIF_SCS_SUB);
    break;
  }
  case P_SCS_UNSUB: {
    printf(NOTIF_SCS_UNSUB);
    break;
  }
  case P_SCS_EXIT: {
    return 1;
  }
  case P_SCS_FORCE_EXIT: {
    printf("Foi removido pelo administrador.\n");
    return 1;
  }
  default: {
    // TODO: check for invalid sucess code
  }
  }
  return 0;
}

int processError(packet *p) {
  int errCode;
  memcpy(&errCode, p->buf, sizeof(int));

  switch (errCode) {
  case P_ERR_GENERIC: {
    break;
  }
  case P_ERR_TOPIC_LIST_FULL: {
    printf(NOTIF_ERR_TOPIC_LIST_FULL);
    break;
  }
  case P_ERR_ALREADY_SUBBED: {
    printf("Já está subscrito\n");
    break;
  }
  case P_ERR_INVALID_TOPIC: {
    printf("O tópico não existe\n");
    break;
  }
  case P_ERR_NOT_SUBBED: {
    printf("Ainda não está subscrito\n");
    break;
  }
  case P_ERR_IS_LOCKED: {
    printf("O tópico não está a aceitar mensagens neste momento.");
    break;
  }
  case P_ERR_EXIT: {
    printf("Ocorreu um erro ao tentar sair.\n");
  }
  default: {
    // TODO: check for invalid error code
  }
  }
  return 0;
}
