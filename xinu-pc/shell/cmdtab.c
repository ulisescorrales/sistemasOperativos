/* shell.c  -  shell */

#include <xinu.h>
#include "shprototypes.h"

/************************************************************************/
/* Table of Xinu shell commands and the function associated with each	*/
/************************************************************************/
const	struct	cmdent	cmdtab[] = {
	{"argecho",	TRUE,	xsh_argecho},
	{"arp",		FALSE,	xsh_arp},
	{"cat",		FALSE,	xsh_cat},
	{"clear",	TRUE,	xsh_clear},
	{"date",	FALSE,	xsh_date},
	{"devdump",	FALSE,	xsh_devdump},
	{"echo",	FALSE,	xsh_echo},
	{"exit",	TRUE,	xsh_exit},
	{"help",	FALSE,	xsh_help},
	{"kill",	TRUE,	xsh_kill},
	{"memdump",	FALSE,	xsh_memdump},
	{"memstat",	FALSE,	xsh_memstat},
	{"netinfo",	FALSE,	xsh_netinfo},
	{"ping",	FALSE,	xsh_ping},
	{"ps",		FALSE,	xsh_ps},
	{"sleep",	FALSE,	xsh_sleep},
	{"udp",		FALSE,	xsh_udpdump},
	{"udpecho",	FALSE,	xsh_udpecho},
	{"udpeserver",	FALSE,	xsh_udpeserver},
	{"uptime",	FALSE,	xsh_uptime},
	{"?",		FALSE,	xsh_help},
    {"miPrograma",FALSE, miPrograma},    
    {"punto6",FALSE,punto6},
    {"tp2punto1",FALSE,tp2punto1},
    {"tp2Punto2",FALSE,tp2Punto2},   
    {"killSleep",FALSE,killSleep},
    {"gestor_bisiestos",FALSE,gestor_bisiestos}
    
};

uint32  ncmd = sizeof(cmdtab) / sizeof(struct cmdent);
