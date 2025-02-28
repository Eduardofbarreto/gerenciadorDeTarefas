#include <iostream>
#include <fstream>  // Biblioteca para manipulação de arquivos

int main() {
    
    std::ofstream arquivo("tarefas.txt");
    
    int i = 0;
    std::string descricao;
    int id1 = i++;

    std::cout<<"Crie sua tarefa"<<std::endl;
    std::cin>>id1;
    arquivo << id1;
    std::cout<<"Descricao: "<<std::endl;
    std::cin>>descricao;
    arquivo << descricao;

    std::cout<<"Tarefa salva com sucesso!!"<<std::endl;

    return 0;
}
