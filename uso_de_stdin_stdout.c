
/*
¿QUE ES UNA TABLA DE DESCRIPTORES DE ARCHIVO?
Es una estructura que gestiona el kernel, nosotros interactuamos
con ella a través de las funciones dup(), dup2() y close(). 
Una tabla de descriptores de archivo contiene pares clave:valor.
Clave es el numerito al que solemos llamar descriptor de archivo.
Y valor es el recurso apuntado. Por ejemplo 1:stdout 0:stdin.
pero es posible asignarle a stdout otro numero, por ejemplo,usando:
dup2(5, STDOUT_FILENO);
La tabla de descriptores quedaría así:

		0:stdin
		1:stdout
		5:stdout	(hemos creado esta nueva entrada)

Ahora cualquier recurso configurado para utilizar el descriptor de
archivo 5 estara utilizando stdout.

Pero lo interesante es justo lo contrario. Imagina que tenemos la
siguiente tabla de descriptores:
	0:extremo_de_lectura_de_un_pipe
	1:stdout
Si en este contexto ejecutamos "wc", al binario wc no le importa
cual es el verdadero stdin, simplemente accede al descriptor de
archivo 0 para obtener una lectura. Y en este caso, 0: es el
extremo de lectura de un pipe.

¿CUAL ES EL ALCANCE DE UNA TABLA DE DESCRIPTORES DE ARCHIVO
Hay una tabla de descriptores de archivos para cada proceso, inicialmente
la tabla de descriptores de archivo del proceso hijo es una copia exacta
de la tabla de descriptores del padre. Podemos aprobechar esta propiedad
para no tener que pasar los extremos del pipe a funciones dentro del hijo.
es decir: 
	1.Configuramos el padre, a imagen de lo que queremos tener en el hijo.
	2.Creamos el proceso hijo.
	3.Devolvemos el proceso padre a una configuracion anterior.*/

#include "minishell.h"

int executor(t_tree *node, char **envp)
{
	int previous_stdin;
    int original_stdout;
	int pipefd[2];
	t_pipe *pipe_node;
	int pid;
	t_task *task;

    if (!node)
        return (0);
    
    if (node->type == PIPE)
    {
        pipe_node = (t_pipe *)node; // castear nodo
        previous_stdin = dup(STDIN_FILENO); //guardamos copia de stdin, para poder reestablecerlo despues de hacer fork, en el proceso padre
        original_stdout = dup(STDOUT_FILENO); //lo mismo para stdout
		/*La tabla de descriptores de archivo queda así
			0: stdin
			1: stdout
			2: ...
			3: stdin		(!!! cambio)
			4: stdout		(!!! cambio)
		*/
//PROCESANDO LADO IZQUIERDO:
//1.Configuramos el padre, a imagen de lo que queremos tener en el primer hijo:
        pipe(pipefd); //creamos pipe, un nuevo pipe añade dos entradas a la tabla de descriptores de archivo (5,6),
        /*La tabla de descriptores de archivo queda así:
			0: stdin
			1: stdout;
			2: ...
			3: stdin
			4: stdout
			5: extremo de lectura del pipe		(!!! cambio)
			6: extremo de escritura del pipe	(!!! cambio)
		printf("%d\n", pipefd[0]);// imprimiria "5"
		printf("%d\n", pipefd[1]);// imprimiria "6"
		*/
		dup2(pipefd[1], STDOUT_FILENO);//Aqui cogemos el extremo de escritura del pipe y lo duplicamos en el fd 1(stdout).
		/*La tabla de descriptores de archivo queda así:
			0: stdin
			1: extremo de escritura del pipe;	(!!! cambio)
			2: ...
			3: stdin
			4: stdout
			5: extremo de lectura del pipe
			6: extremo de escritura del pipe
		*/
		close(pipefd[1]); // Ya tenemos "1:" para escribir en el pipe. Es MUY importante no dejarse extremos innecesarios abiertos
		/*
			0: stdin
			1: extremo de escritura del pipe;
			2: ...
			3: stdin
			4: stdout
			5: extremo de lectura del pipe
			----- recurso liberado--------	(!!! cambio)
		*/
//2.Creamos el proceso hijo. 
		executor((t_tree *)pipe_node->left, envp);//En esta llamada se cumple if (node->type == TASK) y se hará un fork (linea182)
//3.Devolvemos el proceso padre a una configuracion estandard.*/
		dup2(original_stdout, STDOUT_FILENO);
		/*
			0: stdin
			1: stdout	(!!! cambio)
			2: ...
			3: stdin
			4: stdout
			5: extremo de lectura del pipe
		*/
//PROCESANDO LADO DERECHO:
//1.Configuramos el padre, a imagen de lo que queremos tener en el siguiente hijo:
		dup2(pipefd[0], STDIN_FILENO);
		/*
			0: extremo de lectura del pipe  (!!! cambio)
			1: stdout			
			2: ...
			3: stdin
			4: stdout
			5: extremo de lectura del pipe
		*/
		close(pipefd[0]); //liberamos fd 5
		/*
			0: extremo de lectura del pipe 
			1: stdout			
			2: ...
			3: stdin
			4: stdout
			----- recurso liberado--------	(!!! cambio)
		*/
		
		executor(pipe_node->rigth, envp); //dentro de las siguientes llamadas recursivas, se creará almenos un hijo.
		//-Conforme avanzamos iterando executor(pipe_node->rigth, envp), previous_stdin pierde el rastro del stdin original.
		//Pero al finalizar las llamadas recursivas, el flujo original regresa aqui, a la primera llamada a executor(),
		//donde previous_stdin contiene el stdin original.
//3.Devolvemos el proceso padre a una configuracion estandard.*/
		dup2(previous_stdin, STDIN_FILENO); //Restauramos stdin original en el proceso padre
		/*
			0: stdin 
			1: stdout			
			2: ...
			3: stdin
			4: stdout
		*/
		// Cerramos los descriptores duplicados
		close(previous_stdin);
		close(original_stdout);
		/*
				0: stdin 
				1: stdout			
				2: ...
		*/
	}
    else if (node->type == TASK)
    {
	/*
	Si executor no encontro pipes en su primera llamada desde main:
			0: stdin 
			1: stdout			
			2: ...

	Si executor fue llamado por linea 237---->> executor((t_tree *)pipe_node->left, envp);
				0: extremo de lectura del pipe  anterior (pipe creado dos nodos mas atras).
				1: extremo de escritura del ultimo pipe (pipe creado en el nodo anterior).
				2: ...
				3: extremo de lectura del pipe  anterior
				4: stdout
				5: extremo de lectura del pipe

	Si executor fue llamado por linea 271----->> executor(pipe_node->rigth, envp);
				0: extremo de lectura del pipe actual
				1: stdout			
				2: ...
				3: extremo de lectura del pipe  anterior
				4: stdout
	*/
        task = (t_task *)node;
        pid = fork();
        
        if (pid == -1)
            return (1);
            
        if (pid == 0)
        {
			//Este seria el momento de meter redirecciones si hubiera en task->redir...
			//puesto que estamos en el proceso hijo, no alteramos la tabla de descriptores del padre, sobre la que trabajan las redirecciones de los pipes.
            execve(task->cmd, task->argv, envp);
            exit(EXIT_FAILURE);
        }
        
        task->pid = pid; //Almacenamos el pid en el propio arbol, para poder hacer wait a cada proceso despues
    }
    return (0);
}

// 2ª iteracion:
int executor(t_tree *node, char **envp)
{
	/*
		0: extremo de lectura del pipe anterior 
		1: stdout			
		2: ...
		3: stdin
		4: stdout
	*/
	int previous_stdin;
    int original_stdout;
	int pipefd[2];
	t_pipe *pipe_node;
	int pid;
	t_task *task;

    if (!node)
        return (0);
    
    if (node->type == PIPE)
    {
        pipe_node = (t_pipe *)node;
        previous_stdin = dup(STDIN_FILENO);
        original_stdout = dup(STDOUT_FILENO);
		/*La tabla de descriptores de archivo queda así
			0: extremo de lectura del pipe  anterior
			1: stdout
			2: ...
			3: extremo de lectura del pipe 	(!!! cambio)
			4: stdout						(!!! cambio)
		*/
		//PROCESANDO LADO IZQUIERDO:
        pipe(pipefd);
        /*La tabla de descriptores de archivo queda así:
			0: extremo de lectura del pipe  anterior
			1: stdout;
			2: ...
			3: extremo de lectura del pipe 
			4: stdout
			5: extremo de lectura del pipe actual		(!!! cambio)
			6: extremo de escritura del pipe actual		(!!! cambio)
		*/
        if (pipe_node->left)
        {
			dup2(pipefd[1], STDOUT_FILENO);
			/*La tabla de descriptores de archivo queda así:
				0: extremo de lectura del pipe  anterior
				1: extremo de escritura del pipe actual	(!!! cambio)
				2: ...
				3: extremo de lectura del pipe  anterior
				4: stdout
				5: extremo de lectura del pipe actual
				6: extremo de escritura del pipe actual
			*/
			close(pipefd[1]);
			/*
				0: extremo de lectura del pipe  anterior
				1: extremo de escritura del pipe;
				2: ...
				3: extremo de lectura del pipe  anterior
				4: stdout
				5: extremo de lectura del pipe actual
				----- recurso liberado--------	(!!! cambio)
			*/
			executor((t_tree *)pipe_node->left, envp);
			//3.Devolvemos el proceso padre a una configuracion estandard.*/
			dup2(original_stdout, STDOUT_FILENO);
			/*
				0: extremo de lectura del pipe  anterior
				1: stdout	(!!! cambio)
				2: ...
				3: extremo de lectura del pipe  anterior
				4: stdout
				5: extremo de lectura del pipe actual
			*/
		}
		//PROCESANDO LADO DERECHO:
        if (pipe_node->rigth)
        {
				dup2(pipefd[0], STDIN_FILENO);
			/*
				0: extremo de lectura del pipe actual  (!!! cambio)
				1: stdout			
				2: ...
				3: extremo de lectura del pipe  anterior
				4: stdout
				5: extremo de lectura del pipe actual
			*/
				close(pipefd[0]); //liberamos fd 5
			/*
				0: extremo de lectura del pipe actual
				1: stdout			
				2: ...
				3: extremo de lectura del pipe  anterior
				4: stdout
				----- recurso liberado--------	(!!! cambio)
			*/
				executor(pipe_node->rigth, envp);// Si rigth es otro pipe, su left (left es t_task por fuerza) encontrara que "0: extremo de lectura del pipe", justo lo que queremos.
				//esta iteracion guarda en
				// Restauramos stdin previo en el proceso padre
				dup2(previous_stdin, STDIN_FILENO);
			/*
				0: extremo de lectura del pipe  anterior
				1: stdout			
				2: ...
				3: extremo de lectura del pipe  anterior
				4: stdout
				----- recurso liberado--------	(!!! cambio)
			*/
		}

        // Cerramos los descriptores duplicados
        close(previous_stdin);
        close(original_stdout);
    }
    else if (node->type == TASK)
    {
	
	/* Si executor fue llamado por linea 237---->> executor((t_tree *)pipe_node->left, envp);
				0: extremo de lectura del pipe  anterior
				1: extremo de escritura del ultimo pipe;
				2: ...
				3: extremo de lectura del pipe  anterior
				4: stdout
				5: extremo de lectura del pipe

	Si executor fue llamado por linea 271----->> executor(pipe_node->rigth, envp);
				0: extremo de lectura del pipe actual
				1: stdout			
				2: ...
				3: extremo de lectura del pipe  anterior
				4: stdout
	*/

        task = (t_task *)node;
        pid = fork();
        
        if (pid == -1)
            return (1);
            
        if (pid == 0)
        {
            execve(task->cmd, task->argv, envp);
            exit(EXIT_FAILURE);
        }
        
        task->pid = pid;
    }
    return (0);
}