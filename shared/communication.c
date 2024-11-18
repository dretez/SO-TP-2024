#include "../include/communication.h"
#include <string.h>

void writeEmptyPacket(packet *pack, short msgtype) {
  pack->head.tipo_msg = msgtype;
  pack->head.tam_msg = 0;
}

void writeStringListPacket(packet *pack, short msgtype, char **strs,
                           int strsSize) {
  pack->head.tipo_msg = msgtype;

  unsigned short offset = 0;
  snprintf(pack->buf, sizeof(pack->buf), "%s", (char *)&strsSize);
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

void writeSingleValPacket(packet *pack, short msgtype, char *value,
                          int valuesize) {
  pack->head.tipo_msg = msgtype;

  unsigned short offset = 0;

  pack->head.tam_msg = offset;
}

void writeMsgPacket(packet *pack, short msgtype, int msglifetime, char *topic,
                    char *username, char *msg) {
  pack->head.tipo_msg = msgtype;

  unsigned short offset = 0;

  pack->head.tam_msg = offset;
}
