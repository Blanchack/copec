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
    vector<string> parts;
    stringstream iss(command);
    string word;
    while (iss >> word)
        parts.push_back(word);

    vector<char *> argv;
    for (auto &p : parts)
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
        /////
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
        else if (command.rfind("myproof", 0) == 0)
        {
            exemiprof(command);
        }
        else
            exeCommand(command);
    }
    return 0;
}