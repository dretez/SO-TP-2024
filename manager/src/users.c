#include "../headers/dados.h"
#include "../headers/dadosAux.h"

int addUser(managerData *d, pid_t pid, char *name) {
  for (int i = 0; i < d->nusers; i++)
    if (d->users[i].pid == pid)
      return 0;
  if (d->nusers == MAX_USERS)
    return 1;
  char *newname = malloc(sizeof(char) * (strlen(name) + 1));
  if (newname == NULL)
    return 2;
  strcpy(newname, name);
  d->users[d->nusers].name = newname;
  d->users[d->nusers].pid = pid;
  d->nusers++;
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

int findUsr(managerData *d, pid_t pid) {
  for (int i = 0; i < d->nusers; i++)
    if (d->users[i].pid == pid)
      return i;
  return -1;
}

pid_t findUsrByName(managerData *d, char *name) {
  for (int i = 0; i < d->nusers; i++)
    if (!strcmp(d->users[i].name, name))
      return d->users[i].pid;
  return -1;
}

/********************************** PRIVATE **********************************/

void initUsrs(user *u) {
  for (int i = 0; i < MAX_USERS; i++) {
    u[i].pid = -1;
    u[i].name = NULL;
  }
}
