#include "../headers/dados.h"
#include "../headers/dadosAux.h"

topic *getTopic(topic *tlist, unsigned short tcount, char *tname) {
  for (int i = 0; i < tcount; i++)
    if (!strcmp(tlist[i].name, tname))
      return &tlist[i];
  return NULL;
}

int addTopic(managerData *d, char *tname) {
  if (getTopic(d->topics, d->ntopics, tname) != NULL)
    return 0;
  if (d->ntopics >= MAX_TOPICS)
    return 1;
  topic *t = &d->topics[d->ntopics];
  deleteTopic(t);
  strcpy(t->name, tname);
  d->ntopics++;
  return 0;
}

void rmTopic(managerData *d, char *tname) {
  topic *t = getTopic(d->topics, d->ntopics, tname);
  swapTopic(t, &d->topics[d->ntopics]);
}

int subscribeUser(topic *t, pid_t pid) {
  if (findSubbedUsr(t->subs, t->nsubs, pid) != -1)
    return 1;
  t->subs[t->nsubs] = pid;
  t->nsubs++;
  return 0;
}

int unsubscribeUser(topic *t, pid_t pid) {
  int idx = findSubbedUsr(t->subs, t->nsubs, pid);
  if (idx == -1)
    return 1;
  t->subs[idx] = t->subs[t->nsubs - 1];
  t->nsubs--;
  return 0;
}

int findSubbedUsr(pid_t *ul, int uc, pid_t pid) {
  for (int i = 0; i < uc; i++)
    if (ul[i] == pid)
      return i;
  return -1;
}

/********************************** PRIVATE **********************************/

void initTopics(topic *t) {
  for (int i = 0; i < MAX_TOPICS; i++) {
    t[i].nPersistMsgs = 0;
    deleteTopic(&t[i]);
  }
}

void swapTopic(topic *t1, topic *t2) {
  topic aux;
  copyTopic(&aux, t1);
  copyTopic(t1, t2);
  copyTopic(t2, &aux);
}

void copyTopic(topic *dest, topic *src) {
  strcpy(dest->name, src->name);
  for (int i = 0; i < MAX_USERS; i++)
    dest->subs[i] = src->subs[i];
  dest->nsubs = src->nsubs;
  for (int i = 0; i < MAX_PERSISTENT_MSGS; i++)
    strcpy(dest->persistMsgs[i].msg, src->persistMsgs[i].msg);
  dest->nPersistMsgs = dest->nPersistMsgs;
}

void deleteTopic(topic *t) {
  strcpy(t->name, "");
  for (int i = 0; i < MAX_USERS; i++)
    t->subs[i] = -1;
  t->nsubs = -1; // nsubs == -1 -> tópico não existe
  for (int i = 0; i < t->nPersistMsgs; i++)
    free(t->persistMsgs[i].uname);
  t->nPersistMsgs = 0;
}
