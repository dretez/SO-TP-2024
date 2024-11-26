#ifndef MANAGER_MANAGER_H
#define MANAGER_MANAGER_H

// Limites predefinidos
#define MAX_USERS 10
#define MAX_TOPICS 20
#define MAX_PERSISTENT_MSGS 5

typedef struct {
  int lifetime;
  char *msg;
} persistMsg;

#endif // !MANAGER_MANAGER_H
