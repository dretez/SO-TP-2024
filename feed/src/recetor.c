#include "../headers/recetor.h"

int processPacket(packet *p) {
  switch (p->head.tipo_msg) {
  default:
    printf("[ERRO] Tipo de mensagem desconhecido: %d\n", p->head.tipo_msg);
    break;
  }

  return 0;
}
