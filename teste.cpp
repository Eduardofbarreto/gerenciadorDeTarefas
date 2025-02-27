#include<iostream>
#include<string>

using namespace std;

class gerenciadorDeTarefas{
    public:
    int id;
    std::string descricao;
    bool concluida;

    gerenciadorDeTarefas(int id, string descricao, bool concluida){
        this -> id = id;
        this -> descricao = descricao;
        this -> concluida = concluida;
    }

    void setDescricao(std::string descricao){
        this -> descricao = descricao;
    }
    
    
};

int main(){

    int opcao;

    //Menu interativo que irá aparecer para o cliente!
    cout<<"Bem-vindo ao gerenciador de tarefas!"<<endl;
    cout<<"\n1 - Adicionar tarefa"<<endl;
    cout<<"2 - Listar tarefas"<<endl;
    cout<<"3 - Concluir tarefa"<<endl;
    cout<<"4 - Remover tarefa"<<endl;
    cout<<"5 - Salvar e sair"<<endl;
    cout<<"Escolha uma opcao: "<<endl;

    
    

    return 0;
}