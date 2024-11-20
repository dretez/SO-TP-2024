#include "../include/communication.h"

void writeEmptyPacket(packet *pack, short msgtype) {
  pack->head.tipo_msg = msgtype;
  pack->head.tam_msg = 0;
}

void writeSingleValPacket(packet *pack, short msgtype, char *value,
                          short valuesize) {
  pack->head.tipo_msg = msgtype;
  pack->head.tam_msg = valuesize;
  memcpy(pack->buf, value, valuesize);
}

void writeStringListPacket(packet *pack, short msgtype, char **strs,
                           int strsSize) {
  pack->head.tipo_msg = msgtype;

  unsigned short offset = 0;
  memcpy(pack->buf, (char *)&strsSize, sizeof(strsSize));
  offset += sizeof(strsSize);

  for (int i = 0, len = strlen(strs[i]);
       i < strsSize && offset + len + sizeof(len) < TAM_BUF;
       i++, len = strlen(strs[i])) {
    memcpy(pack->buf + offset, (char *)&len, sizeof(len));
    offset += sizeof(len);
    memcpy(pack->buf + offset, strs[i], len);
    offset += len;
  }

  /* TODO: deal with the possibility that not every string was copied due to
   * the final offset going over the allocated size for the buffer
   */

  pack->head.tam_msg = offset;
}

void writeMsgPacket(packet *pack, short msgtype, int msglifetime, char *topic,
                    char *username, char *msg) {
  pack->head.tipo_msg = msgtype;

  unsigned short offset = 0;
  memcpy(pack->buf + offset, (char *)&msglifetime, sizeof(msglifetime));
  offset += sizeof(msglifetime);

  int len = strlen(username);
  memcpy(pack->buf + offset, (char *)&len, sizeof(len));
  offset += sizeof(len);
  memcpy(pack->buf + offset, username, len);
  offset += len;

  len = strlen(topic) <= TAM_NOME_TOPICO ? strlen(topic) : TAM_NOME_TOPICO;
  memcpy(pack->buf + offset, (char *)&len, sizeof(len));
  offset += sizeof(len);
  memcpy(pack->buf + offset, topic, len);
  offset += len;

  len = strlen(msg) <= TAM_CORPO_MSG ? strlen(topic) : TAM_CORPO_MSG;
  memcpy(pack->buf + offset, (char *)&len, sizeof(len));
  offset += sizeof(len);
  memcpy(pack->buf + offset, msg, len);
  offset += len;

  pack->head.tam_msg = offset;
}
