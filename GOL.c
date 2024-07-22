#include <SDL2/SDL.h>
#include <stdbool.h>
#include <windows.h>

const int WIDTH = 1920;
const int HEIGHT = 1080;
const int CELL_SIZE = 10;

bool grid[GRID_WIDTH][GRID_HEIGHT];
bool newGrid[GRID_WIDTH][GRID_HEIGHT];

void init(SDL_Window** window, SDL_Renderer** renderer) {
    SDL_Init(SDL_INIT_VIDEO);
    *window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
}

void updateGrid() {
    for (int x = 0; x < GRID_WIDTH; ++x) {
        for (int y = 0; y < GRID_HEIGHT; ++y) {
            int aliveNeighbors = 0;
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    if (dx == 0 && dy == 0) continue;
                    int nx = (x + dx + GRID_WIDTH) % GRID_WIDTH;
                    int ny = (y + dy + GRID_HEIGHT) % GRID_HEIGHT;
                    if (grid[nx][ny]) aliveNeighbors++;
                }
            }
            newGrid[x][y] = (grid[x][y] && (aliveNeighbors == 2 || aliveNeighbors == 3)) || (!grid[x][y] && aliveNeighbors == 3);
        }
    }
    for (int x = 0; x < GRID_WIDTH; ++x) {
        for (int y = 0; y < GRID_HEIGHT; ++y) {
            grid[x][y] = newGrid[x][y];
        }
    }
}

void renderGrid(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    for (int x = 0; x < GRID_WIDTH; ++x) {
        for (int y = 0; y < GRID_HEIGHT; ++y) {
            if (grid[x][y]) {
                SDL_Rect cell = { x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE };
                SDL_RenderFillRect(renderer, &cell);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    init(&window, &renderer);

    // Initialize grid with some pattern or randomly
    // ...

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        updateGrid();
        renderGrid(renderer);
        SDL_Delay(100); // Control the speed of the simulation
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}