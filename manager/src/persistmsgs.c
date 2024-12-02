#include "../headers/dados.h"
#include "../headers/dadosAux.h"

int addPersistMsg(managerData *d, char *tname, char *uname, char *msg,
                  int lifetime) {
  topic *t = getTopic(d->topics, d->ntopics, tname);
  if (t == NULL || t->nPersistMsgs == MAX_PERSISTENT_MSGS)
    return -1;
  t->persistMsgs[t->nPersistMsgs].uname =
      malloc(sizeof(char) * strlen(uname) + 1);
  if (t->persistMsgs[t->nPersistMsgs].uname == NULL) {
    return -2;
  }
  strcpy(t->persistMsgs[t->nPersistMsgs].uname, uname);
  strcpy(t->persistMsgs[t->nPersistMsgs].msg, msg);
  t->persistMsgs[t->nPersistMsgs].lifetime = lifetime;
  t->nPersistMsgs++;
  return 0;
}

int rmPersistMsg(topic *t, int idx) {
  if (t->nPersistMsgs <= idx)
    return 1;
  free(t->persistMsgs[idx].uname);
  for (int i = idx; i < t->nPersistMsgs - 1; i++)
    /* move a msg que queremos apagar para o fim e tudo o resto 1 posição para
     * trás */
    swapPersistMsg(&t->persistMsgs[i], &t->persistMsgs[i + 1]);
  t->nPersistMsgs--;
  return 0;
}

/********************************** PRIVATE **********************************/

int swapPersistMsg(persistMsg *msg1, persistMsg *msg2) {
  persistMsg aux;
  cpyPersistMsg(&aux, msg1);
  cpyPersistMsg(msg1, msg2);
  cpyPersistMsg(msg2, &aux);
  return 0;
}

int cpyPersistMsg(persistMsg *msg1, persistMsg *msg2) {
  strcpy(msg1->msg, msg2->msg);
  msg1->uname = msg2->uname;
  msg1->lifetime = msg2->lifetime;
  return 0;
}
