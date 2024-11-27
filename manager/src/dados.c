#include "../headers/dados.h"

/* Esvazia a lista de utilizadores */
void initUsrs(user *u);

/* Inicializa a lista de tópicos */
void initTopics(topic *t);

/* Copia todos os dados de um tópico src para um tópico dest, perdendo este, por
 * sua vez, todos os seus dados originais, passando também a partilhar as
 * mensagens do tópico src (uma alteração a uma mensagem num tópico também
 * altera a mesma mensagem no outro) */
void copyTopic(topic *dest, topic *src);

/* Apaga os dados de um tópico */
void deleteTopic(topic *t);

int findSubbedUsr(pid_t *ul, int uc, pid_t pid);

void initManData(managerData *d) {
  d->ntopics = d->nusers = 0;
  initUsrs(d->users);
  initTopics(d->topics);
}

int addUser(managerData *d, pid_t pid, char *name) {
  for (int i = 0; i < d->nusers; i++)
    if (d->users[i].pid == pid)
      return 0;
  if (d->nusers < MAX_USERS)
    return 1;
  char *newname = malloc(sizeof(char) * (strlen(name) + 1));
  if (newname == NULL)
    return 2;
  strcpy(newname, name);
  d->users[d->nusers].name = newname;
  d->users[d->nusers].pid = pid;
  return 0;
}

int rmUser(managerData *d, pid_t pid) {
  if (findUsr(d, pid) == -1)
    return 1;
  free(d->users[findUsr(d, pid)].name);
  for (int i = 0; i < d->ntopics; i++)
    unsubscribeUser(&d->topics[i], pid);
  d->users[findUsr(d, pid)].name = d->users[d->nusers - 1].name;
  d->users[findUsr(d, pid)].pid = d->users[d->nusers - 1].pid;
  d->nusers--;
  return 0;
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

void swapTopic(topic *t1, topic *t2) {
  topic aux;
  copyTopic(&aux, t1);
  copyTopic(t1, t2);
  copyTopic(t2, &aux);
}

void initUsrs(user *u) {
  for (int i = 0; i < MAX_USERS; i++) {
    u[i].pid = -1;
    u[i].name = NULL;
  }
}

void initTopics(topic *t) {
  for (int i = 0; i < MAX_TOPICS; i++) {
    t[i].nPersistMsgs = 0;
    deleteTopic(&t[i]);
  }
}

void copyTopic(topic *dest, topic *src) {
  strcpy(dest->name, src->name);

  for (int i = 0; i < MAX_USERS; i++)
    dest->subs[i] = src->subs[i];
  dest->nsubs = src->nsubs;

  for (int i = 0; i < MAX_PERSISTENT_MSGS; i++)
    dest->persistMsgs[i].msg = src->persistMsgs[i].msg;
  dest->nPersistMsgs = dest->nPersistMsgs;
}

void deleteTopic(topic *t) {
  strcpy(t->name, "");

  for (int i = 0; i < MAX_USERS; i++)
    t->subs[i] = -1;
  t->nsubs = -1; // nsubs == -1 -> tópico não existe

  for (int i = 0; i < t->nPersistMsgs; i++)
    free(t->persistMsgs[i].msg);
  t->nPersistMsgs = 0;
}

topic *getTopic(topic *tlist, unsigned short tcount, char *tname) {
  for (int i = 0; i < tcount; i++) {
    if (!strcmp(tlist[i].name, tname))
      return &tlist[i];
  }
  return NULL;
}

int findUsr(managerData *d, pid_t pid) {
  for (int i = 0; i < d->nusers; i++)
    if (d->users[i].pid == pid)
      return i;
  return -1;
}

int findSubbedUsr(pid_t *ul, int uc, pid_t pid) {
  for (int i = 0; i < uc; i++)
    if (ul[i] == pid)
      return i;
  return -1;
}
