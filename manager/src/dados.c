#include "../headers/dados.h"
#include "../headers/dadosAux.h"

void initManData(managerData *d) {
  d->ntopics = d->nusers = 0;
  initUsrs(d->users);
  initTopics(d->topics);
  char *savedata = getenv("MSG_FICH");
  if (savedata == NULL) {
    return;
  }
  FILE *f = fopen(savedata, "r");
  if (f == NULL) {
    printf("%s: ficheiro não encontrado, não foram recuperadas mensagens\n",
           savedata);
    return;
  }
  loadPMsgs(d, f);
  fclose(f);
}

void clearManData(managerData *d) {
  for (int i = 0; i < d->ntopics; i++)
    rmTopic(d, d->topics[i].name);
  for (int i = 0; i < d->nusers; i++)
    free(d->users[i].name);
}
