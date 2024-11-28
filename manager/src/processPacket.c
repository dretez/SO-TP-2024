#include "../headers/recetor.h"

int processPacket(packet *p, managerData *d) {
  switch (p->head.tipo_msg) {
  case P_TYPE_USER_TOPIC: // Pedido de lista de tópicos
  {
    p->head.tipo_msg = P_TYPE_MNGR_TOPIC;
    unsigned short offset;
    for (int i = 0, offset = 0; i < d->ntopics;
         offset += strlen(d->topics[i++].name) + 1)
      strcpy(&p->buf[offset], d->topics[i].name);
    p->head.tam_msg = offset;
    break;
  }
  case P_TYPE_USER_MSG: { // Recebida mensagem para um tópico
    p->head.tipo_msg = P_TYPE_MNGR_MSG;
    break;
  }
  case P_TYPE_USER_SUB: { // Pedido de subscrição a um tópico
    topic *topico = getTopic(d->topics, d->ntopics, p->buf);
    if (topico == NULL && addTopic(d, p->buf) == 1) {
      writeErrorPacket(p, P_ERR_TOPIC_LIST_FULL);
      break;
    }
    if (subscribeUser(topico, p->head.pid)) {
      writeErrorPacket(p, P_ERR_ALREADY_SUBBED);
      break;
    }
    p->head.tipo_msg = P_TYPE_MNGR_SUCCESS;
    p->head.tam_msg = 0;
    break;
  }
  case P_TYPE_USER_UNSUB: { // Pedido de remoção de subscrição a um tópico
    topic *topico = getTopic(d->topics, d->ntopics, p->buf);
    if (topico == NULL) {
      writeErrorPacket(p, P_ERR_INVALID_TOPIC);
      break;
    }
    if (unsubscribeUser(topico, p->head.pid)) {
      writeErrorPacket(p, P_ERR_NOT_SUBBED);
      break;
    }
    p->head.tipo_msg = P_TYPE_MNGR_SUCCESS;
    p->head.tam_msg = 0;
    break;
  }
  case P_TYPE_USER_EXIT: // Pedido de saída de um cliente feed
    // remove utilizador da lista de utilizadores connectados
    // envia informação a todos os utilizadores que o utilizador saiu
    break;

  default: // Tipo de mensagem desconhecido
    writeErrorPacket(p, P_ERR_GENERIC);
    break;
  }

  return 0;
}
