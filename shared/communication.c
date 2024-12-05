#include "../include/communication.h"

size_t packetSize(packet p) { return sizeof(packetHeader) + p.head.tam_msg; }

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

  unsigned short offset;

  for (int i = 0, offset = 0;
       i < strsSize && offset + strlen(strs[i]) < TAM_BUF;
       offset += strlen(strs[i++]))
    strcpy(&pack->buf[offset], strs[i]);

  /* TODO: deal with the possibility that not every string was copied due to
   * the final offset going over the allocated size for the buffer
   */

  pack->head.tam_msg = offset;
}

void writeMsgPacket(packet *pack, short msgtype, int msglifetime, char *topic,
                    char *username, char *msg) {
  pack->head.tipo_msg = msgtype;

  uint16_t offset = 0;

  if (msglifetime >= 0) {
    memcpy(&pack->buf[offset], (char *)&msglifetime, sizeof(msglifetime));
    offset += sizeof(msglifetime);
  }

  strcpy(&pack->buf[offset], username);
  offset += strlen(username) + 1;

  strcpy(&pack->buf[offset], topic);
  offset += strlen(topic) + 1;

  strcpy(&pack->buf[offset], msg);
  offset += strlen(msg) + 1;

  pack->head.tam_msg = offset;
}

void writeErrorPacket(packet *p, int errCode) {
  p->head.tipo_msg = P_TYPE_MNGR_ERROR;
  p->head.tam_msg = sizeof(int);
  memcpy(p->buf, &errCode, sizeof(int));
}

void writeSucessPacket(packet *p, int scsCode) {
  p->head.tipo_msg = P_TYPE_MNGR_SUCCESS;
  p->head.tam_msg = sizeof(int);
  memcpy(p->buf, &scsCode, sizeof(int));
}
