#include "../headers/dados.h"
#include "../headers/dadosAux.h"

void initManData(managerData *d) {
  d->ntopics = d->nusers = 0;
  initUsrs(d->users);
  initTopics(d->topics);
}
