#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 10
#define HEIGHT 10

typedef struct {
    int x, y;
} Cell;

typedef struct {
    Cell cells[WIDTH * HEIGHT];
    int top;
} Stack;

// Initialisation du stack
void initStack(Stack *stack) {
    stack->top = 0;
}

// Fonction pour empiler une cellule
void push(Stack *stack, Cell cell) {
    stack->cells[stack->top++] = cell;
}

// Fonction pour dépiler une cellule
Cell pop(Stack *stack) {
    return stack->cells[--stack->top];
}

// Vérifie si le stack est vide
int isEmpty(Stack *stack) {
    return stack->top == 0;
}

// Directions possibles pour le mouvement dans le labyrinthe
int dx[] = {0, 1, 0, -1};
int dy[] = {-1, 0, 1, 0};

// Fonction pour vérifier si une cellule est valide et non visitée
int isValid(int x, int y, int maze[HEIGHT][WIDTH], int visited[HEIGHT][WIDTH]) {
    return x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT && !visited[y][x];
}

// Fonction pour générer un labyrinthe en utilisant DFS
void generateMaze(int maze[HEIGHT][WIDTH], int visited[HEIGHT][WIDTH]) {
    Stack stack;
    initStack(&stack);
    
    Cell start = {0, 0};
    push(&stack, start);
    visited[start.y][start.x] = 1;

    while (!isEmpty(&stack)) {
        Cell current = stack.cells[stack.top - 1];
        int x = current.x;
        int y = current.y;

        // Trouver les voisins valides non visités
        int neighbors[4][2];
        int n = 0;
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (isValid(nx, ny, maze, visited)) {
                neighbors[n][0] = nx;
                neighbors[n][1] = ny;
                n++;
            }
        }

        if (n > 0) {
            // Choisir un voisin aléatoire
            int r = rand() % n;
            int nx = neighbors[r][0];
            int ny = neighbors[r][1];

            // Briser les murs
            if (nx == x + 1) { // Droite
                maze[y][x] |= 2;
                maze[ny][nx] |= 8;
            } else if (nx == x - 1) { // Gauche
                maze[y][x] |= 8;
                maze[ny][nx] |= 2;
            } else if (ny == y + 1) { // Bas
                maze[y][x] |= 4;
                maze[ny][nx] |= 1;
            } else if (ny == y - 1) { // Haut
                maze[y][x] |= 1;
                maze[ny][nx] |= 4;
            }

            // Marquer comme visité et empiler
            visited[ny][nx] = 1;
            push(&stack, (Cell){nx, ny});
        } else {
            pop(&stack);
        }
    }
}

// Fonction pour afficher le labyrinthe
void printMaze(int maze[HEIGHT][WIDTH]) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            printf("+");
            printf((maze[y][x] & 1) ? "---" : "   ");
        }
        printf("+\n");

        for (int x = 0; x < WIDTH; x++) {
            printf((maze[y][x] & 8) ? " " : "|");
            printf("   ");
        }
        printf("|\n");
    }

    for (int x = 0; x < WIDTH; x++) {
        printf("+---");
    }
    printf("+\n");
}

// Fonction principale
int main() {
    srand(time(NULL));

    int maze[HEIGHT][WIDTH] = {0};       // Grille du labyrinthe
    int visited[HEIGHT][WIDTH] = {0};    // Grille des cellules visitées

    generateMaze(maze, visited);         // Génération du labyrinthe
    printMaze(maze);                     // Affichage du labyrinthe

    return 0;
}
