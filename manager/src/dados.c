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
