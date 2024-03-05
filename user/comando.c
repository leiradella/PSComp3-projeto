/***************************************************************************
| File: comando.c  -  Concretizacao de comandos (exemplo)
|
| Autor: Carlos Almeida (IST)
| Data:  Nov 2002
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>

/*-------------------------------------------------------------------------+
| Function: cmd_sair - termina a aplicacao
+--------------------------------------------------------------------------*/ 
void cmd_sair (int argc, char **argv)
{
  exit(0);
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_cts (int argc, char** argv)
{
  int i;

  /* exemplo -- escreve argumentos */
  for (i=0; i<argc; i++)
    printf ("\ncts argv[%d] = %s", i, argv[i]);
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_cps (int argc, char** argv)
{
  int i;

  /* exemplo -- escreve argumentos */
  for (i=0; i<argc; i++)
    printf ("\ncps argv[%d] = %s", i, argv[i]);
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_mpps (int argc, char** argv)
{
  int i;

  /* exemplo -- escreve argumentos */
  for (i=0; i<argc; i++)
    printf ("\nmpps argv[%d] = %s", i, argv[i]);
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_mppa (int argc, char** argv)
{
  int i;

  /* exemplo -- escreve argumentos */
  for (i=0; i<argc; i++)
    printf ("\nmppa argv[%d] = %s", i, argv[i]);
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_mppamb (int argc, char** argv)
{
  int i;

  /* exemplo -- escreve argumentos */
  for (i=0; i<argc; i++)
    printf ("\nmppamb argv[%d] = %s", i, argv[i]);
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_aas (int argc, char** argv)
{
  int i;

  /* exemplo -- escreve argumentos */
  for (i=0; i<argc; i++)
    printf ("\naas argv[%d] = %s", i, argv[i]);
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_cala (int argc, char** argv)
{
  int i;

  /* exemplo -- escreve argumentos */
  for (i=0; i<argc; i++)
    printf ("\ncala argv[%d] = %s", i, argv[i]);
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_dala (int argc, char** argv)
{
  int i;

  /* exemplo -- escreve argumentos */
  for (i=0; i<argc; i++)
    printf ("\ndala argv[%d] = %s", i, argv[i]);
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_cer (int argc, char** argv)
{
  int i;

  /* exemplo -- escreve argumentos */
  for (i=0; i<argc; i++)
    printf ("\ncer argv[%d] = %s", i, argv[i]);
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_aer (int argc, char** argv)
{
  int i;

  /* exemplo -- escreve argumentos */
  for (i=0; i<argc; i++)
    printf ("\naer argv[%d] = %s", i, argv[i]);
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_der (int argc, char** argv)
{
  int i;

  /* exemplo -- escreve argumentos */
  for (i=0; i<argc; i++)
    printf ("\nder argv[%d] = %s", i, argv[i]);
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_tsm (int argc, char** argv)
{
  int i;

  /* exemplo -- escreve argumentos */
  for (i=0; i<argc; i++)
    printf ("\ntsm argv[%d] = %s", i, argv[i]);
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_lreg (int argc, char** argv)
{
  int i;

  /* exemplo -- escreve argumentos */
  for (i=0; i<argc; i++)
    printf ("\nlreg argv[%d] = %s", i, argv[i]);
}

/*-------------------------------------------------------------------------+
| Function: cmd_test - apenas como exemplo
+--------------------------------------------------------------------------*/ 
void cmd_trh (int argc, char** argv)
{
  int i;

  /* exemplo -- escreve argumentos */
  for (i=0; i<argc; i++)
    printf ("\ntrh argv[%d] = %s", i, argv[i]);
}


