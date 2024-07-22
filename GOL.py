import pygame
import random
import ctypes
import os
import time

WIDTH, HEIGHT = (1920+1920), 1000
CELL_SIZE = 5
GRID_WIDTH, GRID_HEIGHT = WIDTH // CELL_SIZE, HEIGHT // CELL_SIZE

# Initialize Pygame
pygame.init()
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("Conway's Game of Life")

# Initialize Grid
grid = [[False for _ in range(GRID_HEIGHT)] for _ in range(GRID_WIDTH)]
new_grid = [[False for _ in range(GRID_HEIGHT)] for _ in range(GRID_WIDTH)]

# Randomly populate the grid
for x in range(GRID_WIDTH):
    for y in range(GRID_HEIGHT):
        grid[x][y] = random.choice([False, True])

def update_grid():
    global grid, new_grid
    for x in range(GRID_WIDTH):
        for y in range(GRID_HEIGHT):
            alive_neighbors = 0
            for dx in range(-1, 2):
                for dy in range(-1, 2):
                    if dx == 0 and dy == 0:
                        continue
                    nx, ny = (x + dx) % GRID_WIDTH, (y + dy) % GRID_HEIGHT
                    if grid[nx][ny]:
                        alive_neighbors += 1
            if grid[x][y]:
                new_grid[x][y] = alive_neighbors in [2, 3]
            else:
                new_grid[x][y] = alive_neighbors == 3
    grid, new_grid = new_grid, grid

def draw_grid():
    screen.fill((0, 0, 0))
    for x in range(GRID_WIDTH):
        for y in range(GRID_HEIGHT):
            if grid[x][y]:
                pygame.draw.rect(screen, (255, 255, 255), (x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE))
    pygame.display.flip()

def save_frame_as_image():
    temp_image_path = os.path.join(os.getcwd(), "current_frame.bmp")
    pygame.image.save(screen, temp_image_path)
    return temp_image_path

def set_desktop_background(image_path):
    ctypes.windll.user32.SystemParametersInfoW(20, 0, image_path, 3)

# Main Loop
running = True
clock = pygame.time.Clock()

while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    update_grid()
    draw_grid()
    image_path = save_frame_as_image()
    set_desktop_background(image_path)
    clock.tick(15)  # Control the speed of the simulation

pygame.quit()

# Clean up the temporary image file
if os.path.exists(image_path):
    os.remove(image_path)
