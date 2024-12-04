#ifndef INCLUDE_INCLUDE_NOTIFICACOES_H_
#define INCLUDE_INCLUDE_NOTIFICACOES_H_

#define NOTIF_ERR "[ERRO] %s\n"
#define NOTIF_SINT "[SINTAXE] %s\n"

#define NOTIF_PROG_ARGS "Número de argumentos incorreto."

#define NOTIF_ERR_PACK_INIT "Falha ao iniciar pacote de dados."

#define NOTIF_LOGGING_IN "A fazer login...\n"
#define NOTIF_NO_SRV "Manager não está a correr!"
#define NOTIF_SRV_FULL "Servidor cheio."
#define NOTIF_ALREADY_LOGGED "Este utilizador já está ligado."
#define NOTIF_ERR_LOGIN "Imposibilidade de aceder ao servidor."

// feed message format
// {topic}> {username}: {message}
#define NOTIF_FEED_MSG "%s> %s: %s\n"

#endif // INCLUDE_INCLUDE_NOTIFICACOES_H_
