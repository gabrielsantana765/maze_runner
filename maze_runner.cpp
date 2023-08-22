#include <stdio.h>
#include <stack>
#include <cstdlib>

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
	for (int i = 0; i < num_rows; ++i) {
		for (int j = 0; j < num_cols; ++j) {
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}


// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada
bool walk(pos_t pos) {
	
	// Repita até que a saída seja encontrada ou não existam mais posições não exploradas
	 while (true) {
		// Marcar a posição atual com o símbolo '.'
		maze[pos.i][pos.j] = '.';
		// Limpa a tela
		system("cls");
		// Imprime o labirinto
		for (int i = 0; i < num_rows; ++i) {
            for (int j = 0; j < num_cols; ++j) {
                printf("%c ", maze[i][j]);
            }
            printf("\n");
        }

        
		/* Dado a posição atual, verifica quais sao as próximas posições válidas
			Checar se as posições abaixo são validas (i>0, i<num_rows, j>0, j <num_cols)
		 	e se são posições ainda não visitadas (ou seja, caracter 'x') e inserir
		 	cada uma delas no vetor valid_positions
		 		- pos.i, pos.j+1
		 		- pos.i, pos.j-1
		 		- pos.i+1, pos.j
		 		- pos.i-1, pos.j
		 	Caso alguma das posiçÕes validas seja igual a 's', retornar verdadeiro
	 	
		if (maze[pos.i][pos.j] == 's') {
            return true;
        }
        */
		 // Verifica se a posição atual é a saída
        if (maze[pos.i][pos.j] == 's') {
            return true;
        }
        
        // Verifica as próximas posições válidas
        // Checa posições abaixo, acima, à direita e à esquerda
        pos_t next_positions[4] = {
            {pos.i, pos.j + 1},  // Direita
            {pos.i, pos.j - 1},  // Esquerda
            {pos.i + 1, pos.j},  // Abaixo
            {pos.i - 1, pos.j}   // Acima
        };
        
        for (int i = 0; i < 4; ++i) {
            int new_i = next_positions[i].i;
            int new_j = next_positions[i].j;
            
            // Verifica se a nova posição está dentro dos limites do labirinto
            if (new_i >= 0 && new_i < num_rows && new_j >= 0 && new_j < num_cols) {
                // Verifica se a nova posição é válida para explorar
                if (maze[new_i][new_j] == 'x') {
                    valid_positions.push({new_i, new_j});
                    // Se a nova posição for a saída, retornar verdadeiro
                    if (maze[new_i][new_j] == 's') {
                        return true;
                    }
                }
            }
        }
		
	
		// Verifica se a pilha de posições nao esta vazia 
		//Caso não esteja, pegar o primeiro valor de  valid_positions, remove-lo e chamar a funçao walk com esse valor
		// Caso contrario, retornar falso
		if (!valid_positions.empty()) {
            pos_t next_position = valid_positions.top();
            valid_positions.pop();
            pos = next_position;
            // Chame recursivamente a função walk com a próxima posição
            if (walk(next_position)) {
                return true;  // Se a saída foi encontrada
            }
        } else {
            return false;  // Se não há mais posições a serem exploradas
        }
    }
}

int main(int argc, char* argv[]) {
	// carregar o labirinto com o nome do arquivo recebido como argumento
	pos_t initial_pos = load_maze(argv[1]);
	// chamar a função de navegação
	bool exit_found = walk(initial_pos);
	
	// Tratar o retorno (imprimir mensagem)
	
	return 0;
}