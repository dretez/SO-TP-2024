#ifndef MANAGER_DADOS_H
#define MANAGER_DADOS_H

#include "../../include/globals.h"
#include "../../include/limites.h"

// Limites predefinidos
#define MAX_USERS 10
#define MAX_TOPICS 20
#define MAX_PERSISTENT_MSGS 5

typedef struct {
  int lifetime;
  char *msg;
} persistMsg;

typedef struct {
  short nsubs;
  char name[TAM_NOME_TOPICO];
  pid_t subs[MAX_USERS];
  short nPersistMsgs;
  persistMsg persistMsgs[MAX_PERSISTENT_MSGS];
} topic;

typedef struct {
  pid_t pid;
  char *name;
} user;

typedef struct {
  unsigned short ntopics;
  topic topics[MAX_TOPICS];
  unsigned short nusers;
  user users[MAX_USERS];
} managerData;

/* Inicializa dados do manager */
void initManData(managerData *d);

int addUser(managerData *d, pid_t pid, char *name);

int addTopic(managerData *d, char *tname);

int rmUser(managerData *d, pid_t pid);

void rmTopic(managerData *d, char *tname);

void swapTopic(topic *t1, topic *t2);

topic *getTopic(topic *tlist, unsigned short tcount, char *tname);

pid_t *getSubscribedUser(topic t, char *name);

int subscribeUser(topic *t, pid_t pid);

int unsubscribeUser(topic *t, pid_t pid);

int findUsr(managerData *d, pid_t pid);

#endif // !MANAGER_DADOS_H
