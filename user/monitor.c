/***************************************************************************
| File: monitor.c
|
| Autor: Carlos Almeida (IST), from work by Jose Rufino (IST/INESC), 
|        from an original by Leendert Van Doorn
| Data:  Nov 2002
***************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*-------------------------------------------------------------------------+
| Headers of command functions
+--------------------------------------------------------------------------*/ 
extern void cmd_sair (int, char** );
extern void cmd_cts (int, char** );
extern void cmd_cps (int, char** );
extern void cmd_mpps (int, char** );
extern void cmd_mppa (int, char** );
extern void cmd_mppamb (int, char** );
extern void cmd_aas (int, char** );
extern void cmd_cala (int, char** );
extern void cmd_dala (int, char** );
extern void cmd_cer (int, char** );
extern void cmd_aer (int, char** );
extern void cmd_der (int, char** );
extern void cmd_tsm (int, char** );
extern void cmd_lreg (int, char** );
extern void cmd_trh (int, char** );
       void cmd_sos  (int, char** );

/*-------------------------------------------------------------------------+
| Variable and constants definition
+--------------------------------------------------------------------------*/ 
const char TitleMsg[] = "\n Application Control Monitor\n";
const char InvalMsg[] = "\nInvalid command!";

struct 	command_d {
  void  (*cmd_fnct)(int, char**);
  char*	cmd_name;
  char*	cmd_help;
} 
  const commands[] = 
{
  {cmd_sos,    "sos","             help"},
  {cmd_sair,   "sair","            sair"},
  {cmd_cts,    "cts","s            consultar temperatura sector s (0-todos)"},
  {cmd_cps,    "cps","s            consultar parametros (periodos, estado actuador) sector s (0-todos)"},
  {cmd_mpps,   "mpps","s p         modificar periodo proc.sensor do sector s (0-todos) (segundos)"},
  {cmd_mppa,   "mppa","s p         modificar periodo proc.actuador do sector s (0-todos) (segundos)"},
  {cmd_mppamb, "mpamb","s p        modificar periodo proc.ambiente do sector s (0-todos) (segundos)"},
  {cmd_aas,    "aas","s v          activar actuador sector s (0-todos) com variacao"},
  {cmd_cala,   "cala","            consultar limiares de alarme temperatura (minimo e maximo)"},
  {cmd_dala,   "dala","m M         definir limiares de alarme temperatura , minimo m e maximo M"},
  {cmd_cer,    "cer","             consultar estado envio registos para historico"},
  {cmd_aer,    "aer","             activar envio de registos para historico"},
  {cmd_der,    "der","             desactivar envio de registos para historico"},
  {cmd_tsm,    "tsm","             terminar processo sistema de monitorizacao"},
  {cmd_lreg,   "lreg","s [t1[t2]]  listar registos sector s (0-todos) entre t1 (dd/mm/aaaa hh:mm:ss) e t2"},
  {cmd_trh,    "trh","             terminar processo de registo historico"},

};

#define NCOMMANDS  (sizeof(commands)/sizeof(struct command_d))
#define ARGVECSIZE 3
#define MAX_LINE   50

/*-------------------------------------------------------------------------+
| Function: cmd_sos - provides a rudimentary help
+--------------------------------------------------------------------------*/ 
void cmd_sos (int argc, char **argv)
{
  int i;

  printf("%s\n", TitleMsg);
  for (i=0; i<NCOMMANDS; i++)
    printf("%s %s\n", commands[i].cmd_name, commands[i].cmd_help);
}

/*-------------------------------------------------------------------------+
| Function: my_getline        (called from monitor) 
+--------------------------------------------------------------------------*/ 
int my_getline (char** argv, int argvsize)
{
  static char line[MAX_LINE];
  char *p;
  int argc;

  fgets(line, MAX_LINE, stdin);

  /* Break command line into an o.s. like argument vector,
     i.e. compliant with the (int argc, char **argv) specification --------*/

  for (argc=0,p=line; (*line != '\0') && (argc < argvsize); p=NULL,argc++) {
    p = strtok(p, " \t\n");
    argv[argc] = p;
    if (p == NULL) return argc;
  }
  argv[argc] = p;
  return argc;
}

/*-------------------------------------------------------------------------+
| Function: monitor        (called from main) 
+--------------------------------------------------------------------------*/ 
void monitor (void)
{
  static char *argv[ARGVECSIZE+1], *p;
  int argc, i;

  printf("%s Type sos for help\n", TitleMsg);
  for (;;) {
    printf("\nCmd> ");
    /* Reading and parsing command line  ----------------------------------*/
    if ((argc = my_getline(argv, ARGVECSIZE)) > 0) {
      for (p=argv[0]; *p != '\0'; *p=tolower(*p), p++);
      for (i = 0; i < NCOMMANDS; i++) 
	if (strcmp(argv[0], commands[i].cmd_name) == 0) 
	  break;
      /* Executing commands -----------------------------------------------*/
      if (i < NCOMMANDS)
	commands[i].cmd_fnct (argc, argv);
      else  
	printf("%s", InvalMsg);
    } /* if my_getline */
  } /* forever */
}
