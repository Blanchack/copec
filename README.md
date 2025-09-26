# miShell - Intérprete de Comandos

## Integrantes

* Alex Blanchard
* Vicente Hernandez
* Ignacio Díaz
* Vicente Wiederhold

## Requisitos

* Compilador **g++** compatible con C++11 o superior.
* Sistema operativo tipo Unix/Linux (se usan llamadas al sistema `fork`, `execvp`, `pipe`, `waitpid`, `getrusage`, etc.).

## Compilación

En la carpeta donde se encuentra el archivo `main.cpp`, ejecutar en consola:

"g++ main.cpp -o mishell"

Esto generará el binario `mishell`.

## Ejecución

Para iniciar el intérprete:

./mishell

El programa mostrará un prompt: "Enter a command:$"

Desde ahí se pueden ejecutar comandos del sistema o los comandos especiales implementados.

## Ejemplos de uso

### Comandos simples

Enter a command:$ ls
Enter a command:$ cat archivo.txt


### Uso de pipes

Enter a command:$ ls -l | grep cpp | wc -l


### Comando `miprof`

Ejecutar un comando midiendo tiempos:


Enter a command:$ miprof ejec ls -l
