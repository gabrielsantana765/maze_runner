#include <stdio.h>
#include <stack>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

// Matriz de char representnado o labirinto
char** maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Representação de uma posição
struct pos_t {
	int i;
	int j;
};

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
std::stack<pos_t> valid_positions;
/* Inserir elemento: 

	 pos_t pos;
	 pos.i = 1;
	 pos.j = 3;
	 valid_positions.push(pos)
 */
// Retornar o numero de elementos: 
//    valid_positions.size();
// 
// Retornar o elemento no topo: 
//  valid_positions.top(); 
// 
// Remover o primeiro elemento do vetor: 
//    valid_positions.pop();


// Função que le o labirinto de um arquivo texto, carrega em 
// memória e retorna a posição inicial
pos_t load_maze(const char* file_name) {
	pos_t initial_pos;
	// Abre o arquivo para leitura (fopen)
	 FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }
	// Le o numero de linhas e colunas (fscanf) 
	// e salva em num_rows e num_cols
		fscanf(file, "%d %d\n", &num_rows, &num_cols);
	// Aloca a matriz maze (malloc)
maze = (char**)malloc(num_rows * sizeof(char*));
    if (maze == NULL) {
        perror("Erro ao alocar matriz");
        exit(1);
    }
    for (int i = 0; i < num_rows; ++i) {
        maze[i] = (char*)malloc(num_cols * sizeof(char));
        if (maze[i] == NULL) {
            perror("Erro ao alocar linha da matriz");
            exit(1);
        }
    }

    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
			// Le o valor da linha i+1,j do arquivo e salva na posição maze[i][j]
			fscanf(file, " %c", &maze[i][j]);
			// Se o valor for 'e' salvar o valor em initial_pos
			if (maze[i][j] == 'e') {
                initial_pos.i = i;
                initial_pos.j = j;
            }
		}
	}
	fclose(file);
	return initial_pos;
}

// Função que imprime o labirinto
void print_maze() {
	system("clear");
        // Imprimir o labirinto (para fins de depuração)
        for (int i = 0; i < num_rows; ++i) {
            for (int j = 0; j < num_cols; ++j) {
                std::cout << maze[i][j];
            }
            std::cout << '\n';
        }
        std::cout << '\n';

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

bool walk(pos_t initial_pos) {
    valid_positions.push(initial_pos);

    while (!valid_positions.empty()) {
        pos_t pos = valid_positions.top();
        
        if (maze[pos.i][pos.j] == 's') {
            return true;
        }

        bool position_found = false;

        pos_t next_positions[4] = {
            {pos.i, pos.j + 1},
            {pos.i, pos.j - 1},
            {pos.i + 1, pos.j},
            {pos.i - 1, pos.j}
        };

        for (int i = 0; i < 4; ++i) {
            int new_i = next_positions[i].i;
            int new_j = next_positions[i].j;

            if (new_i >= 0 && new_i < num_rows && new_j >= 0 && new_j < num_cols) {
                if (maze[new_i][new_j] == 's') {
                    
            return true;
            }
                if (maze[new_i][new_j] == 'x') {
                    position_found = true;
                    maze[new_i][new_j] = '.'; // Marcar como visitada
                    valid_positions.push({new_i, new_j});
                    break;
                }
            }
        }
        
        if (!position_found) {
            valid_positions.pop();
        }
        system ("clear");
        // Imprimir o labirinto (para fins de depuração)
        for (int i = 0; i < num_rows; ++i) {
            for (int j = 0; j < num_cols; ++j) {
                std::cout << maze[i][j];
            }
            std::cout << '\n';
        }
        std::cout << '\n';

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return false;
}

bool explore(pos_t initial_pos) {
    valid_positions.push(initial_pos);

    while (!valid_positions.empty()) {
        pos_t pos = valid_positions.top();

        if (maze[pos.i][pos.j] == 's') {
            return true;
        }

        bool position_found = false;
        int valid_neighbor_count = 0;
        pos_t next_positions[4] = {
            {pos.i, pos.j + 1},
            {pos.i, pos.j - 1},
            {pos.i + 1, pos.j},
            {pos.i - 1, pos.j}
        };

        std::vector<std::thread> threads; // Para armazenar as threads dinâmicas
pos_t new_position = {0, 0};
        for (int i = 0; i < 4; ++i) {
            int new_i = next_positions[i].i;
            int new_j = next_positions[i].j;

            if (new_i >= 0 && new_i < num_rows && new_j >= 0 && new_j < num_cols) {
                if (maze[new_i][new_j] == 's') {
                    return true;
                }
                if (maze[new_i][new_j] == 'x') {
                    position_found = true;
                    valid_neighbor_count++;
                    maze[new_i][new_j] = '.'; // Marcar como visitada
                    valid_positions.push({new_i, new_j});
                    new_position = {new_i, new_j};
                    // Criar uma nova thread para explorar o próximo caminho
                }
            }
        }
        if (valid_neighbor_count > 0) {                  
        threads.emplace_back(explore, new_position);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
            if (!position_found) {
                valid_positions.pop();
            }
        }

        if (!position_found) {
            valid_positions.pop();
        }


        // Aguardar todas as threads dinâmicas criadas terminarem
        for (auto& thread : threads) {
            thread.join();
        }
    std::thread print_thread(print_maze);
    print_thread.join();

    }
    
    return false;
}


//fim

int main(int argc, char* argv[]) {
	// carregar o labirinto com o nome do arquivo recebido como argumento
	pos_t initial_pos = load_maze(argv[1]);
	// chamar a função de navegação
    //std::thread print_thread(print_maze);
	bool exit_found = explore(initial_pos);
    //print_thread.join();
	
	// Tratar o retorno (imprimir mensagem)
	if (exit_found) {
        std::cout << "Saída encontrada!\n";
    } else {
        std::cout << "Saída não encontrada.\n";
    }
	return 0;
}