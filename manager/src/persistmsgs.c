#include "../headers/dados.h"
#include "../headers/dadosAux.h"

void savePMsgs(managerData d, FILE *f) {
  for (int i = 0; i < d.ntopics; i++) {
    topic t = d.topics[i];
    for (int j = 0; j < t.nPersistMsgs; j++) {
      persistMsg pmsg = t.persistMsgs[j];
      char aux[TAM_BUF];
      sprintf(aux, "%s %s %d %s\n", t.name, pmsg.uname, pmsg.lifetime,
              pmsg.msg);
      fputs(aux, f);
    }
  }
}

int addPersistMsg(managerData *d, char *tname, char *uname, char *msg,
                  int lifetime) {
  if (getTopic(d->topics, d->ntopics, tname) == NULL && addTopic(d, tname)) {
    return 1;
  }
  topic *t = getTopic(d->topics, d->ntopics, tname);
  if (t->nPersistMsgs == MAX_PERSISTENT_MSGS)
    return -1;
  persistMsg *pmsg = &t->persistMsgs[t->nPersistMsgs];
  pmsg->uname = malloc(sizeof(char) * strlen(uname) + 1);
  if (pmsg->uname == NULL) {
    return -2;
  }
  strcpy(pmsg->uname, uname);
  strcpy(pmsg->msg, msg);
  pmsg->lifetime = lifetime;
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

void loadPMsgs(managerData *d, FILE *f) {
  char line[TAM_BUF];
  while (fgets(line, TAM_BUF, f)) {
    int offset = 0;
    char tname[TAM_NOME_TOPICO];
    wordncpy(tname, &line[offset], TAM_NOME_TOPICO);
    offset = nextword(line, offset, TAM_BUF);
    char *uname = malloc(sizeof(char) * wordlen(&line[offset]));
    if (uname == NULL) {
      printf("[ERRO] Falha ao iniciar mensagens armazenadas.");
      return;
    }
    wordncpy(uname, &line[offset], wordlen(&line[offset]) + 1);
    offset = nextword(line, offset, TAM_BUF);
    int lt = atoi(&line[offset]);
    offset = nextword(line, offset, TAM_BUF);
    char msg[TAM_CORPO_MSG];
    strncpy(msg, &line[offset], TAM_CORPO_MSG);
    char *aux = &msg[strlen(msg) - 1];
    if (*aux == '\n')
      *aux = '\0';

    addPersistMsg(d, tname, uname, msg, lt);
    free(uname);
  }
}

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
