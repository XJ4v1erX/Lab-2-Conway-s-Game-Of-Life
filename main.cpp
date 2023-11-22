#include <SDL.h>
#include <vector>
#include <random>
#include <algorithm>  // For std::swap

constexpr int SCREEN_WIDTH = 150;
constexpr int SCREEN_HEIGHT = 150;
constexpr int CELL_SIZE = 5;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distrX(0, SCREEN_WIDTH - 50);
std::uniform_int_distribution<> distrY(0, SCREEN_HEIGHT - 50);

bool checkNeighbor(const std::vector<std::vector<bool>>& grid, int x, int y) {
    int aliveNeighbors = 0;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (!(i == 0 && j == 0) && grid[(x + i + SCREEN_WIDTH) % SCREEN_WIDTH][(y + j + SCREEN_HEIGHT) % SCREEN_HEIGHT]) {
                aliveNeighbors++;
            }
        }
    }

    if (grid[x][y] && aliveNeighbors >= 2 && aliveNeighbors <= 3) {
        return true;
    }

    if (!grid[x][y] && aliveNeighbors == 3) {
        return true;
    }

    return false;
}

void addPattern(const std::vector<std::pair<int, int>>& pattern, std::vector<std::vector<bool>>& grid) {
    int offsetX = distrX(gen), offsetY = distrY(gen);

    for(auto cell : pattern) {
        grid[cell.first+offsetX][cell.second+offsetY] = true;
    }
}

// Function for linear interpolation between two values
float lerp(float v0, float v1, float t) {
    return v0 + t * (v1 - v0);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Conway's Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH*CELL_SIZE, SCREEN_HEIGHT*CELL_SIZE, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    std::vector<std::vector<bool>> grid(SCREEN_WIDTH, std::vector<bool>(SCREEN_HEIGHT, false));
    std::vector<std::vector<bool>> newGrid(SCREEN_WIDTH, std::vector<bool>(SCREEN_HEIGHT, false));

    // Define patterns
    std::vector<std::pair<int, int>> glider = {{1, 0}, {2, 1}, {0, 2}, {1, 2}, {2, 2}};
    std::vector<std::pair<int, int>> lwss = {{1, 0}, {4, 0}, {0, 1}, {0, 2}, {4, 2}, {0, 3}, {1, 4}, {2, 4}, {3, 4}, {4, 4}};
    std::vector<std::pair<int, int>> pulsar = {{2,4}, {2,5}, {2,6}, {2,10}, {2,11}, {2,12}, {4,2}, {4,7}, {4,9}, {4,14}, {5,2}, {5,7}, {5,9}, {5,14}, {6,2}, {6,7}, {6,9}, {6,14}, {7,4}, {7,5}, {7,6}, {7,10}, {7,11}, {7,12}, {9,4}, {9,5}, {9,6}, {9,10}, {9,11}, {9,12}, {10,2}, {10,7}, {10,9}, {10,14}, {11,2}, {11,7}, {11,9}, {11,14}, {12,2}, {12,7}, {12,9}, {12,14}, {14,4}, {14,5}, {14,6}, {14,10}, {14,11}, {14,12}};
    std::vector<std::pair<int, int>> pentadecathlon = {{4,5}, {5,5}, {6,4}, {6,6}, {7,5}, {8,5}, {9,5}, {10,5}, {11,4}, {11,6}, {12,5}, {13,5}};
    std::vector<std::pair<int, int>> rPentomino = {{1,0}, {2,0}, {0,1}, {1,1}, {1,2}};
    std::vector<std::pair<int, int>> diehard = {{0,1}, {1,1}, {1,2}, {5,0}, {6,0}, {7,0}, {6,2}};
    std::vector<std::pair<int, int>> acorn = {{1,0}, {3,1}, {0,2}, {1,2}, {4,2}, {5,2}, {6,2}};

    // Add patterns to the grid at random positions
    for (int i = 0; i < 100; ++i) {  // 100 instances of each pattern
        addPattern(glider, grid);
        addPattern(lwss, grid);
        addPattern(pulsar, grid);
        addPattern(pentadecathlon, grid);
        addPattern(rPentomino, grid);
        addPattern(diehard, grid);
        addPattern(acorn, grid);
    }

    bool gameRunning = true;

    int r1 = 255, g1 = 0, b1 = 0;  // Start color (red in this case)
    int r2 = 0, g2 = 0, b2 = 255;  // End color (blue in this case)
    float t = 0.0f;
    float dt = 0.01f;

    while (gameRunning) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                gameRunning = false;
            }
        }

        for (int x = 0; x < SCREEN_WIDTH; x++) {
            for (int y = 0; y < SCREEN_HEIGHT; y++) {
                newGrid[x][y] = checkNeighbor(grid, x, y);
            }
        }

        grid = newGrid;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Update color values
        int r = static_cast<int>(std::round(lerp(r1, r2, t))); // NOLINT
        int g = static_cast<int>(std::round(lerp(g1, g2, t))); // NOLINT
        int b = static_cast<int>(std::round(lerp(b1, b2, t))); // NOLINT



        // Update the interpolation parameter and swap colors if necessary
        t += dt;
        if (t >= 1.0) {
            std::swap(r1, r2);
            std::swap(g1, g2);
            std::swap(b1, b2);
            t = 0.0f;
        }

        for (int x = 0; x < SCREEN_WIDTH; x++) {
            for (int y = 0; y < SCREEN_HEIGHT; y++) {
                if (grid[x][y]) {
                    SDL_Rect cell;
                    cell.x = x * CELL_SIZE;
                    cell.y = y * CELL_SIZE;
                    cell.w = CELL_SIZE;
                    cell.h = CELL_SIZE;

                    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
                    SDL_RenderFillRect(renderer, &cell);
                }
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(100);  // Pause for 100 milliseconds
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
