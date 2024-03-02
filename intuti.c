#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIM 50

char userinput[DIM]; 
char command[DIM]; 
int comarg1; comarg2;

void main ()
{
    while (1)
    {
        fgets(userinput,DIM, stdin);
        sscanf(userinput,"%s %d %d", &command, &comarg1, &comarg2);
        
        if (strcmp("cts",command) == 0)
        {
            printf("you used the cts command with arguments %d %d\n", comarg1, comarg2);
        }

        else if (strcmp("cps",command) == 0)
        {
            printf("you used the cps command with arguments %d %d\n", comarg1, comarg2);
        }

        else if (strcmp("mpps",command) == 0)
        {
            printf("you used the mpps command with arguments %d %d\n", comarg1, comarg2);
        }
        
        else if (strcmp("mppa",command) == 0)
        {
            printf("you used the mppa command with arguments %d %d\n", comarg1, comarg2);
        }
        
        else if (strcmp("mpamb",command) == 0)
        {
            printf("you used the mppamb command with arguments %d %d\n", comarg1, comarg2);
        }
        
        else if (strcmp("aas",command) == 0)
        {
            printf("you used the aas command with arguments %d %d\n", comarg1, comarg2);
        }
        
        else if (strcmp("cala",command) == 0)
        {
            printf("you used the cala command with arguments %d %d\n", comarg1, comarg2);
        }
        
        else if (strcmp("dala",command) == 0)
        {
            printf("you used the dala command with arguments %d %d\n", comarg1, comarg2);
        }
        
        else if (strcmp("cer",command) == 0)
        {
            printf("you used the cer command with arguments %d %d\n", comarg1, comarg2);
        }
        
        else if (strcmp("cer",command) == 0)
        {
            printf("you used the cer command with arguments %d %d\n", comarg1, comarg2);
        }
        
        else if (strcmp("aer",command) == 0)
        {
            printf("you used the aer command with arguments %d %d\n", comarg1, comarg2);
        }
        
        else if (strcmp("der",command) == 0)
        {
            printf("you used the der command with arguments %d %d\n", comarg1, comarg2);
        }
        
        else if (strcmp("tsm",command) == 0)
        {
            printf("you used the tsm command with arguments %d %d\n", comarg1, comarg2);
        }
        
        else if (strcmp("lreg",command) == 0)
        {
            printf("you used the lreg command with arguments %d %d\n", comarg1, comarg2);
        }
        
        else if (strcmp("trh",command) == 0)
        {
            printf("you used the trh command with arguments %d %d\n", comarg1, comarg2);
        }
        
        else if (strcmp("sos",command) == 0 || strcmp("help",command) == 0)
        {
            printf("cts s - consultar temperatura sector s (0-todos)\ncps s - consultar parametros (periodos, estado actuador) sector s (0-todos)\nmpps s p - modificar periodo proc.sensor do sector s (0-todos) (segundos)\nmppa s p - modificar periodo proc.actuador do sector s (0-todos) (segundos)\nmpamb s p - modificar periodo proc.ambiente do sector s (0-todos) (segundos)\naas s v - activar actuador sector s (0-todos) com variacao v (-+0)\ncala - consultar limiares de alarme temperatura (minimo e maximo)\ndala m M - definir limiares de alarme temperatura, minimo m e maximo M\ncer - consultar estado envio registos para historico\naer - activar envio de registos para historico\nder - desactivar envio de registos para historico\ntsm - terminar processo sistema de monitorizacao (sismon)\nlreg s [t1 [t2]] - listar registos sector s (0-todos) entre t1 (dd/mm/aaaa hh:mm:ss) e t2\ntrh - terminar processo de registo historico (reghist)\nsos ou help - mostrar comandos disponiveis\nsair - sair da interface utilizador (intuti)\n");
        }

        else if (strcmp("sair",command) == 0)
        {
            return;
        }
        
        else
        {
            printf("command not found\n");
        }

    }
}
