#include "../headers/recetor.h"
#include <stdio.h>

int processPacket(packet *p) {
  switch (p->head.tipo_msg) {
  case P_TYPE_MNGR_MSG: {
    // print mensagem
    break;
  }
  case P_TYPE_MNGR_RMUSR: {
    // se o nome recebido for igual ao próprio nome, sai;
    // senão mostra uma mensagem a indicar o nome do feed que saiu.
    break;
  }
  case P_TYPE_MNGR_TOPIC: {
    uint16_t offset = 0;
    unsigned short ntopics;
    memcpy(&ntopics, &p->buf[offset], sizeof(unsigned short));
    // printf("%x\n", *((unsigned short *)&p->buf[offset]));
    // printf("%x\n", (unsigned short)ntopics);
    offset += sizeof(unsigned short);
    for (unsigned short i = 0; i < ntopics; i++) {
      printf("%s\n", &p->buf[offset]);
    }
    break;
  }
  case P_TYPE_MNGR_LOCKD: {
    // print "Tópico {nome tópico} não está a receber mensagens"
    break;
  }
  case P_TYPE_MNGR_USERS: {
    // print todos os nomes recebidos
    break;
  }
  case P_TYPE_MNGR_ERROR: {
    // processa erro
    break;
  }
  case P_TYPE_MNGR_SUCCESS: {
    // processa sucesso
    break;
  }
  default:
    printf("[ERRO] Tipo de mensagem desconhecido: %d\n", p->head.tipo_msg);
    break;
  }

  return 0;
}
