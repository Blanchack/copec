#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

using namespace std;

vector<char *> makeArgv(const string &command)
{
    stringstream ss(command);
    string word;
    vector<string> parts;
    while (ss >> word)
        parts.push_back(word);

        static vector<string> aliveStrings;
    aliveStrings = parts;

    vector<char *> argv;
    for (auto &p : aliveStrings)
        argv.push_back(const_cast<char *>(p.c_str()));
    argv.push_back(nullptr);
    return argv;
}

void exeCommand(const string &command)
{
    vector<char *> argv = makeArgv(command);
    if (argv[0] == nullptr)
        return;

    unsigned int pid = fork();
    if (pid == 0)
    {                                 // Soy el hijo? EEE MI hijo
        execvp(argv[0], argv.data()); // Ejecuto el comando y no vuelvo
        perror("execvp");             // Volvi, entonces error
        _exit(127);                   // command not found
    }
    else if (pid > 0)
    { // Soy el padre?
        int status;
        waitpid(pid, &status, 0); // Espero a que mi hijo termine
    }
    else
    {
        perror("fork"); // error en el fork
    }
}

void exemiprof(const string &command)
{
    vector<char *> argv = makeArgv(command);
    if (argv.size() < 2)
    {
        cerr << "Uso: miprof [ejec|ejecsave archivo|maxtiempo N] comando args\n";
        return;
    }
    string mode = argv[1];

   if (mode == "ejec")
{
    // Armar el comando real (todo lo que viene después de "miprof ejec")
    string cmd;
    for (int i = 2; argv[i]; i++) {
        if (i > 2) cmd += " ";
        cmd += argv[i];
    }

    if (cmd.empty()) {
        cerr << "Falta comando a ejecutar\n";
        return;
    }

    // Medir tiempos
    struct rusage usage;
    struct timespec start, end;
    clock_gettime(CLOCK_REALTIME, &start);

    // Ejecutar con o sin pipes
    if (cmd.find('|') != string::npos) {
        vector<string> commands = split(cmd, '|');
        executePipe(commands);   // función ya implementada
    } else {
        exeCommand(cmd);         // función ya implementada
    }

    clock_gettime(CLOCK_REALTIME, &end);
    getrusage(RUSAGE_CHILDREN, &usage);

    // Calcular tiempos
    double real_time = (end.tv_sec - start.tv_sec) +
                       (end.tv_nsec - start.tv_nsec) / 1e9;
    double user_time = usage.ru_utime.tv_sec + usage.ru_utime.tv_usec / 1e6;
    double sys_time  = usage.ru_stime.tv_sec + usage.ru_stime.tv_usec / 1e6;

    // Mostrar resultados
    cout << "---- Resultados miprof ----\n";
    cout << "Comando: " << cmd << "\n";
    cout << "Tiempo real: " << real_time << " s\n";
    cout << "Tiempo usuario: " << user_time << " s\n";
    cout << "Tiempo sistema: " << sys_time << " s\n";
    cout << "Memoria máxima (RSS): " << usage.ru_maxrss << " KB\n";
}


    else if (mode == "ejecsave")
    {
        ///////
    }

    else if (mode == "maximoresidentset")
    {
        //////
    }

    else
    {
        cerr << "Modo incorrecto\n";
    }
}

vector<string> split(const string &str, char delimiter)
{
    vector<string> tokens;
    stringstream ss(str);
    string token;

    while (getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }

    return tokens;
}

void executePipe(const vector<string> &commands)
{
    int numCommands = commands.size();
    if (numCommands == 0)
        return;

    vector<int> pfd;
    if (numCommands > 1)
        pfd.resize(2 * (numCommands - 1));
    for (int i = 0; i < numCommands - 1; ++i)
    {
        if (pipe(&pfd[2 * i]) == -1)
        {
            perror("pipe");
            return;
        }
    }

    vector<pid_t> children;

    for (int i = 0; i < numCommands; ++i)
    {
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            return;
        }

        if (pid == 0)
        {
            // --- hijo ---
            // stdin del comando actual
            if (i > 0)
            {
                dup2(pfd[2 * (i - 1)], STDIN_FILENO);
            }
            // stdout del comando actual
            if (i < numCommands - 1)
            {
                dup2(pfd[2 * i + 1], STDOUT_FILENO);
            }

            // Cerrar todos los fds de pipes
            for (size_t j = 0; j < pfd.size(); ++j)
                close(pfd[j]);

            // Preparar argv y ejecutar
            vector<char *> argv = makeArgv(commands[i]);
            execvp(argv[0], argv.data());

            // Si exec falla
            perror("execvp");
            _exit(127);
        }
        else
        {
            // --- padre ---
            children.push_back(pid);
        }
    }

    // Padre cierra todos los fds de los pipes
    for (size_t j = 0; j < pfd.size(); ++j)
        close(pfd[j]);

    // Esperar a todos los hijos
    int status;
    for (pid_t c : children)
    {
        waitpid(c, &status, 0);
    }
}

int main()
{
    bool shouldExit = false;

    string command;
    while (!shouldExit)
    {
        // prompt para el comando
        // para este tipo de prompts, no se necesitan llamadas a sistema
        cout << "Enter a command:$ ";
        getline(cin, command);

        if (command.empty())
            continue;
        else if (command == "exit")
            shouldExit = true;
        else if (command.rfind("miproof", 0) == 0)
        {
            exemiprof(command);
        }
        else if (command.find('|') != string::npos)
        {
            vector<string> commands = split(command, '|');
            // funcion para las pipes
            executePipe(commands);
        }
        else
        {
            // funcion para comandos simples
            exeCommand(command);
        }
    }
    return 0;
}