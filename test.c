#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 10
#define HEIGHT 10

// Définition de la structure Cell
typedef struct {
    int x, y;
} Cell;

// Définition de la structure Stack pour la génération DFS
typedef struct {
    Cell cells[WIDTH * HEIGHT];
    int top;
} Stack;

// Initialisation de la pile
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

// Vérifier si la pile est vide
int isEmptyStack(Stack *stack) {
    return stack->top == 0;
}

// Définition de la structure Queue pour BFS
typedef struct {
    Cell cells[WIDTH * HEIGHT];
    int front, rear;
} Queue;

// Initialisation de la file
void initQueue(Queue *queue) {
    queue->front = 0;
    queue->rear = 0;
}

// Ajouter une cellule à la file
void enqueue(Queue *queue, Cell cell) {
    queue->cells[queue->rear++] = cell;
}

// Retirer une cellule de la file
Cell dequeue(Queue *queue) {
    return queue->cells[queue->front++];
}

// Vérifier si la file est vide
int isEmptyQueue(Queue *queue) {
    return queue->front == queue->rear;
}

// Directions possibles pour le mouvement dans le labyrinthe
int dx[] = {0, 1, 0, -1};
int dy[] = {-1, 0, 1, 0};

// Fonction pour vérifier si une cellule est valide et non visitée
int isValid(int x, int y, int visited[HEIGHT][WIDTH]) {
    return x >= 0 && y >= 0 && x < WIDTH && y < HEIGHT && !visited[y][x];
}

// Fonction pour générer un labyrinthe en utilisant DFS
void generateMaze(int maze[HEIGHT][WIDTH], int visited[HEIGHT][WIDTH]) {
    Stack stack;
    initStack(&stack);
    
    Cell start = {0, 0};
    push(&stack, start);
    visited[start.y][start.x] = 1;

    while (!isEmptyStack(&stack)) {
        Cell current = stack.cells[stack.top - 1];
        int x = current.x;
        int y = current.y;

        // Trouver les voisins valides non visités
        int neighbors[4][2];
        int n = 0;
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (isValid(nx, ny, visited)) {
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

// Fonction pour vérifier s'il est possible de se déplacer dans une certaine direction sans traverser de mur
int canMove(int maze[HEIGHT][WIDTH], int x, int y, int direction) {
    switch (direction) {
        case 0: return maze[y][x] & 1;  // Haut
        case 1: return maze[y][x] & 2;  // Droite
        case 2: return maze[y][x] & 4;  // Bas
        case 3: return maze[y][x] & 8;  // Gauche
    }
    return 0;
}

// Fonction pour trouver le chemin le plus court avec BFS
int findShortestPath(int maze[HEIGHT][WIDTH], int path[HEIGHT][WIDTH], Cell start, Cell end) {
    Queue queue;
    initQueue(&queue);
    enqueue(&queue, start);
    path[start.y][start.x] = 1;

    Cell parent[HEIGHT][WIDTH];  // Stocker le parent de chaque cellule pour retracer le chemin
    parent[start.y][start.x] = (Cell){-1, -1};

    while (!isEmptyQueue(&queue)) {
        Cell current = dequeue(&queue);
        int x = current.x;
        int y = current.y;

        if (x == end.x && y == end.y) {
            // Retrouver le chemin depuis la sortie
            Cell trace = end;
            while (trace.x != -1 && trace.y != -1) {
                path[trace.y][trace.x] = 2; // Marquer le chemin
                trace = parent[trace.y][trace.x];
            }
            return 1; // Chemin trouvé
        }

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            // Vérifier si le déplacement est possible et sans traverser de mur
            if (nx >= 0 && ny >= 0 && nx < WIDTH && ny < HEIGHT && !path[ny][nx] && canMove(maze, x, y, i)) {
                path[ny][nx] = 1;
                enqueue(&queue, (Cell){nx, ny});
                parent[ny][nx] = current;
            }
        }
    }

    return 0; // Chemin non trouvé
}

// Fonction pour afficher le labyrinthe avec le chemin, l'entrée et la sortie
void printMazeWithPath(int maze[HEIGHT][WIDTH], int path[HEIGHT][WIDTH], Cell start, Cell end) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            printf("+");
            printf((maze[y][x] & 1) ? "---" : "   ");
        }
        printf("+\n");

        for (int x = 0; x < WIDTH; x++) {
            printf((maze[y][x] & 8) ? " " : "|");

            if (y == start.y && x == start.x) {
                printf(" S "); // Marquer l'entrée
            } else if (y == end.y && x == end.x) {
                printf(" E "); // Marquer la sortie
            } else if (path[y][x] == 2) {
                printf(" @ "); // Marquer le chemin le plus court
            } else {
                printf("   ");
            }
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
    int path[HEIGHT][WIDTH] = {0};       // Grille pour le chemin

    generateMaze(maze, visited);         // Génération du labyrinthe

    // Choisir une entrée et une sortie aléatoires
    Cell start = {rand() % WIDTH, rand() % HEIGHT};
    Cell end = {rand() % WIDTH, rand() % HEIGHT};

    // Trouver le chemin le plus court entre l'entrée et la sortie
    findShortestPath(maze, path, start, end);

    // Afficher le labyrinthe avec le chemin, l'entrée, et la sortie
    printMazeWithPath(maze, path, start, end);

    return 0;
}
