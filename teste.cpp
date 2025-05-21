#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <limits> // Para numeric_limits
#include <filesystem> // Para obter o diretório atual (C++17 e posterior)

#ifdef __linux__
#include <unistd.h>
#include <limits.h>
#endif

// Classe base para Tarefa
class Tarefa {
protected:
    int id;
    std::string descricao;
    bool concluida;

public:
    Tarefa(int id, const std::string& descricao) : id(id), descricao(descricao), concluida(false) {}

    // Métodos para acessar e modificar atributos
    int getId() const { return id; }
    std::string getDescricao() const { return descricao; }
    bool isConcluida() const { return concluida; }

    void setDescricao(const std::string& descricao) { this->descricao = descricao; }
    void setConcluida(bool concluida) { this->concluida = concluida; }

    // Método virtual para exibir informações da tarefa (para polimorfismo)
    virtual void exibirTarefa() const {
        std::cout << "ID: " << id << ", Descrição: " << descricao << ", Concluída: " << (concluida ? "Sim" : "Não");
    }

    // Método virtual para salvar a tarefa em arquivo
    virtual std::string salvarTarefa() const {
        return std::to_string(id) + ";" + descricao + ";" + (concluida ? "1" : "0");
    }

    // Método virtual estático para carregar a tarefa de uma string
    static Tarefa* carregarTarefa(const std::string& linha) {
        std::stringstream ss(linha);
        std::string segmento;
        std::vector<std::string> dados;

        while (std::getline(ss, segmento, ';')) {
            dados.push_back(segmento);
        }

        if (dados.size() >= 3) {
            int id = std::stoi(dados[0]);
            std::string descricao = dados[1];
            bool concluida = (dados[2] == "1");
            return new Tarefa(id, descricao);
        }
        return nullptr;
    }
};

// Classe TarefaSimples (herda de Tarefa sem alterações)
class TarefaSimples : public Tarefa {
public:
    TarefaSimples(int id, const std::string& descricao) : Tarefa(id, descricao) {}

    void exibirTarefa() const override {
        Tarefa::exibirTarefa();
        std::cout << " (Simples)" << std::endl;
    }

    std::string salvarTarefa() const override {
        return "SIMPLES;" + Tarefa::salvarTarefa();
    }

    static TarefaSimples* carregarTarefa(const std::string& linha) {
        std::stringstream ss(linha);
        std::string segmento;
        std::vector<std::string> dados;

        while (std::getline(ss, segmento, ';')) {
            dados.push_back(segmento);
        }

        if (dados.size() == 4 && dados[0] == "SIMPLES") {
            int id = std::stoi(dados[1]);
            std::string descricao = dados[2];
            bool concluida = (dados[3] == "1");
            return new TarefaSimples(id, descricao);
        }
        return nullptr;
    }
};

// Classe TarefaComPrazo (herda de Tarefa e adiciona prazo)
class TarefaComPrazo : public Tarefa {
private:
    std::string prazo;

public:
    TarefaComPrazo(int id, const std::string& descricao, const std::string& prazo)
        : Tarefa(id, descricao), prazo(prazo) {}

    std::string getPrazo() const { return prazo; }
    void setPrazo(const std::string& prazo) { this->prazo = prazo; }

    void exibirTarefa() const override {
        Tarefa::exibirTarefa();
        std::cout << ", Prazo: " << prazo << " (Com Prazo)" << std::endl;
    }

    std::string salvarTarefa() const override {
        return "PRAZO;" + Tarefa::salvarTarefa() + ";" + prazo;
    }

    static TarefaComPrazo* carregarTarefa(const std::string& linha) {
        std::stringstream ss(linha);
        std::string segmento;
        std::vector<std::string> dados;

        while (std::getline(ss, segmento, ';')) {
            dados.push_back(segmento);
        }

        if (dados.size() == 5 && dados[0] == "PRAZO") {
            int id = std::stoi(dados[1]);
            std::string descricao = dados[2];
            bool concluida = (dados[3] == "1");
            std::string prazo = dados[4];
            return new TarefaComPrazo(id, descricao, prazo);
        }
        return nullptr;
    }
};

// Classe TarefaPrioritaria (herda de Tarefa e adiciona nivelPrioridade)
class TarefaPrioritaria : public Tarefa {
private:
    int nivelPrioridade;

public:
    TarefaPrioritaria(int id, const std::string& descricao, int nivelPrioridade)
        : Tarefa(id, descricao), nivelPrioridade(nivelPrioridade) {
        if (nivelPrioridade < 1 || nivelPrioridade > 5) {
            this->nivelPrioridade = 1; // Define um valor padrão se estiver fora do intervalo
        }
    }

    int getNivelPrioridade() const { return nivelPrioridade; }
    void setNivelPrioridade(int nivelPrioridade) {
        if (nivelPrioridade >= 1 && nivelPrioridade <= 5) {
            this->nivelPrioridade = nivelPrioridade;
        }
    }

    void exibirTarefa() const override {
        Tarefa::exibirTarefa();
        std::cout << ", Prioridade: " << nivelPrioridade << " (Prioritária)" << std::endl;
    }

    std::string salvarTarefa() const override {
        return "PRIORITARIA;" + Tarefa::salvarTarefa() + ";" + std::to_string(nivelPrioridade);
    }

    static TarefaPrioritaria* carregarTarefa(const std::string& linha) {
        std::stringstream ss(linha);
        std::string segmento;
        std::vector<std::string> dados;

        while (std::getline(ss, segmento, ';')) {
            dados.push_back(segmento);
        }

        if (dados.size() == 5 && dados[0] == "PRIORITARIA") {
            int id = std::stoi(dados[1]);
            std::string descricao = dados[2];
            bool concluida = (dados[3] == "1");
            int nivelPrioridade = std::stoi(dados[4]);
            return new TarefaPrioritaria(id, descricao, nivelPrioridade);
        }
        return nullptr;
    }
};

// Classe GerenciadorDeTarefas
class GerenciadorDeTarefas {
private:
    std::vector<Tarefa*> tarefas;
    int proximoId;
    std::string nomeArquivo = "tarefas.txt"; // Nome do arquivo na mesma pasta

public:
    GerenciadorDeTarefas() : proximoId(1) {}

    ~GerenciadorDeTarefas() {
        // Limpar a memória alocada para as tarefas
        for (Tarefa* tarefa : tarefas) {
            delete tarefa;
        }
        tarefas.clear();
    }

    void adicionarTarefa() {
        int tipoTarefa;
        std::cout << "Escolha o tipo de tarefa a adicionar:\n";
        std::cout << "1. Simples\n";
        std::cout << "2. Com Prazo\n";
        std::cout << "3. Prioritária\n";
        std::cout << "Opção: ";
        std::cin >> tipoTarefa;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpar o buffer do teclado

        std::string descricao;
        std::cout << "Digite a descrição da tarefa: ";
        std::getline(std::cin, descricao);

        switch (tipoTarefa) {
            case 1: {
                tarefas.push_back(new TarefaSimples(proximoId++, descricao));
                std::cout << "Tarefa simples adicionada com ID: " << proximoId - 1 << std::endl;
                break;
            }
            case 2: {
                std::string prazo;
                std::cout << "Digite o prazo da tarefa (dd/mm/aaaa): ";
                std::getline(std::cin, prazo);
                tarefas.push_back(new TarefaComPrazo(proximoId++, descricao, prazo));
                std::cout << "Tarefa com prazo adicionada com ID: " << proximoId - 1 << std::endl;
                break;
            }
            case 3: {
                int nivelPrioridade;
                std::cout << "Digite o nível de prioridade da tarefa (1-5): ";
                std::cin >> nivelPrioridade;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                tarefas.push_back(new TarefaPrioritaria(proximoId++, descricao, nivelPrioridade));
                std::cout << "Tarefa prioritária adicionada com ID: " << proximoId - 1 << std::endl;
                break;
            }
            default:
                std::cout << "Opção inválida." << std::endl;
                break;
        }
    }

    void listarTarefas() const {
        if (tarefas.empty()) {
            std::cout << "Não há tarefas cadastradas." << std::endl;
            return;
        }
        std::cout << "\n--- Lista de Tarefas ---" << std::endl;
        for (const Tarefa* tarefa : tarefas) {
            tarefa->exibirTarefa();
        }
        std::cout << "-------------------------" << std::endl;
    }

    void concluirTarefa() {
        int idConcluir;
        std::cout << "Digite o ID da tarefa a marcar como concluída: ";
        std::cin >> idConcluir;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        for (Tarefa* tarefa : tarefas) {
            if (tarefa->getId() == idConcluir) {
                tarefa->setConcluida(true);
                std::cout << "Tarefa com ID " << idConcluir << " marcada como concluída." << std::endl;
                return;
            }
        }
        std::cout << "Tarefa com ID " << idConcluir << " não encontrada." << std::endl;
    }

    void removerTarefa() {
        int idRemover;
        std::cout << "Digite o ID da tarefa a remover: ";
        std::cin >> idRemover;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        for (auto it = tarefas.begin(); it != tarefas.end(); ++it) {
            if ((*it)->getId() == idRemover) {
                delete *it;
                tarefas.erase(it);
                std::cout << "Tarefa com ID " << idRemover << " removida." << std::endl;
                return;
            }
        }
        std::cout << "Tarefa com ID " << idRemover << " não encontrada." << std::endl;
    }

    void salvarTarefas() const {
        std::ofstream arquivo(nomeArquivo); // Abre o arquivo "tarefas.txt" na mesma pasta
        if (arquivo.is_open()) {
            for (const Tarefa* tarefa : tarefas) {
                arquivo << tarefa->salvarTarefa() << std::endl;
            }
            arquivo.close();
            std::cout << "Tarefas salvas em " << nomeArquivo << std::endl;
        } else {
            std::cerr << "Erro ao abrir o arquivo para salvar." << std::endl;
        }
    }

    void carregarTarefas() {
        std::ifstream arquivo(nomeArquivo); // Abre o arquivo "tarefas.txt" na mesma pasta
        if (arquivo.is_open()) {
            std::string linha;
            while (std::getline(arquivo, linha)) {
                Tarefa* tarefa = nullptr;
                if (linha.rfind("SIMPLES", 0) == 0) {
                    tarefa = TarefaSimples::carregarTarefa(linha);
                } else if (linha.rfind("PRAZO", 0) == 0) {
                    tarefa = TarefaComPrazo::carregarTarefa(linha);
                } else if (linha.rfind("PRIORITARIA", 0) == 0) {
                    tarefa = TarefaPrioritaria::carregarTarefa(linha);
                } else {
                    tarefa = Tarefa::carregarTarefa(linha);
                }

                if (tarefa) {
                    tarefas.push_back(tarefa);
                    if (tarefa->getId() >= proximoId) {
                        proximoId = tarefa->getId() + 1;
                    }
                }
            }
            arquivo.close();
            std::cout << "Tarefas carregadas de " << nomeArquivo << std::endl;
        } else {
            std::cout << "Nenhum arquivo de tarefas encontrado. Iniciando com uma lista vazia." << std::endl;
        }
    }
};

int main() {
    GerenciadorDeTarefas gerenciador;
    gerenciador.carregarTarefas();

    int escolha;
    do {
        std::cout << "\nBem-vindo ao Gerenciador de Tarefas!\n";
        std::cout << "1. Adicionar Tarefa\n";
        std::cout << "2. Listar Tarefas\n";
        std::cout << "3. Concluir Tarefa\n";
        std::cout << "4. Remover Tarefa\n";
        std::cout << "5. Salvar e Sair\n";
        std::cout << "Escolha uma opção: ";
        std::cin >> escolha;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpar o buffer

        switch (escolha) {
            case 1:
                gerenciador.adicionarTarefa();
                break;
            case 2:
                gerenciador.listarTarefas();
                break;
            case 3:
                gerenciador.concluirTarefa();
                break;
            case 4:
                gerenciador.removerTarefa();
                break;
            case 5:
                gerenciador.salvarTarefas();
                std::cout << "Saindo do Gerenciador de Tarefas. Até logo!\n";
                break;
            default:
                std::cout << "Opção inválida. Tente novamente.\n";
        }
    } while (escolha != 5);

    return 0;
}