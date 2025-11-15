#include <iostream>
#include <fstream>
#include <sstream>
#include "parser.h"

using namespace std;

string readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Erro: Nao foi possivel abrir o ficheiro de entrada '" << filename << "'." << endl;
        exit(EXIT_FAILURE);
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char* argv[]) 
{
    if (argc != 2) {
        cerr << "Erro: Uso incorreto." << endl;
        cerr << "Exemplo: ./xpp_compiler arquivo.xpp" << endl;
        return 1;
    }

    string filename = argv[1];
    string input;

    try {
        input = readFile(filename);

        Parser* parser = new Parser(input);

        parser->run();

        delete parser;

    } catch (const std::exception& e) {
        cerr << "Erro fatal: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}