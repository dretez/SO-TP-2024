#include "../headers/login.h"

int login(packet *p, char *nome, int fd, int fd_cli) {
  writeSingleValPacket(p, P_TYPE_USER_HANDSHK, nome, strlen(nome) + 1);
  p->head.pid = getpid();
  write(fd, p, packetSize(*p));

  read(fd_cli, &p->head, sizeof(packetHeader));
  read(fd_cli, &p->buf, p->head.tam_msg);

  switch (p->head.tipo_msg) {
  case P_TYPE_MNGR_SUCCESS: {
    int scsCode;
    memcpy(&scsCode, p->buf, sizeof(int));
    if (scsCode == P_SCS_HANDSHK)
      return 0;
    printf("[ERRO] Imposibilidade de aceder ao servidor.\n");
    return 1;
  }
  case P_TYPE_MNGR_ERROR: {
    int errCode;
    memcpy(&errCode, p->buf, sizeof(int));

    switch (errCode) {
    case P_ERR_ALREADY_LOGGED:
      printf("[ERRO] Este utilizador já está ligado.\n");
      break;
    default:
      printf("[ERRO] Imposibilidade de aceder ao servidor.\n");
      break;
    }
    return 1;
  }
  default:
    printf("[ERRO] Imposibilidade de aceder ao servidor.\n");
    return 1;
  }
}
