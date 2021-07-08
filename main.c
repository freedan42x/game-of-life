#include <raylib.h>
#include <string.h>
#include <stdio.h>

#define CELL_WIDTH 12
#define CELL_HEIGHT 12
#define GRID_WIDTH 48
#define GRID_HEIGHT 48

#define SCREEN_WIDTH (CELL_WIDTH * GRID_WIDTH)
#define SCREEN_HEIGHT (CELL_HEIGHT * GRID_HEIGHT)

#define CELL_DEAD 0
#define CELL_LIVE 1

#define FRAME_RATE 240
#define NEXT_GEN_INTERVAL_MIN (1.0f / 64)
#define NEXT_GEN_INTERVAL_MAX 1.0f
#define NEXT_GEN_INTERVAL_STEP (1.0f / 32)
#define NEXT_GEN_INTERVAL_INITIAL (1.0f / 2)

#define CELL_COLOR BLACK
#define BG_COLOR WHITE
#define TEXT_COLOR BLUE
#define TEXT_SIZE 20

#define KEY_PLUS KEY_EQUAL

#define W(s) mod(s, GRID_WIDTH)
#define H(s) mod(s, GRID_HEIGHT)

int mod(int a, int b) {
  int m = a % b;
  if (m < 0) {
    m = (b < 0) ? m - b : m + b;
  }
  return m;
}

int grid[GRID_WIDTH][GRID_HEIGHT];

void DrawCells()
{
  for (int x = 0; x < GRID_WIDTH; x++)
  {
    for (int y = 0; y < GRID_HEIGHT; y++)
    {
      if (grid[x][y] == CELL_LIVE)
      {
        DrawRectangle(x * CELL_WIDTH, y * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, CELL_COLOR);
      }
    }
  }
}

void CellsNextGen()
{
  int g[GRID_WIDTH][GRID_HEIGHT];
  memcpy(g, grid, sizeof(g));

  for (int x = 0; x < GRID_WIDTH; x++)
  {
    for (int y = 0; y < GRID_HEIGHT; y++)
    {
      int neighbours = 
        g[W(x-1)][H(y-1)] + g[x][H(y-1)] + g[W(x+1)][H(y-1)]
      + g[W(x-1)][y]      + 0            + g[W(x+1)][y]
      + g[W(x-1)][H(y+1)] + g[x][H(y+1)] + g[W(x+1)][H(y+1)];

      if (neighbours < 2 || neighbours > 3)
      {
        grid[x][y] = CELL_DEAD;
      }

      if (neighbours == 3)
      {
        grid[x][y] = CELL_LIVE;
      }
    }
  }
}

int main()
{
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Conway's Game of Life");

  SetTargetFPS(FRAME_RATE);

  float nextGenInterval = NEXT_GEN_INTERVAL_INITIAL;
  bool pause = true;
  int frame = 1;
  while (!WindowShouldClose())
  {
    if (IsKeyPressed(KEY_SPACE))
    {
      pause = !pause;
    }

    if (IsKeyPressed(KEY_PLUS))
    {
      if (nextGenInterval - NEXT_GEN_INTERVAL_STEP >= NEXT_GEN_INTERVAL_MIN)
      {
        nextGenInterval -= NEXT_GEN_INTERVAL_STEP;
      }
      frame = 1;
    }

    if (IsKeyPressed(KEY_MINUS))
    {
      if (nextGenInterval + NEXT_GEN_INTERVAL_STEP <= NEXT_GEN_INTERVAL_MAX)
      {
	nextGenInterval += NEXT_GEN_INTERVAL_STEP;
      }
      frame = 1;
    }

    if (IsKeyPressed(KEY_R))
    {
      for (int x = 0; x < GRID_WIDTH; x++)
      {
	for (int y = 0; y < GRID_HEIGHT; y++)
	{
	  grid[x][y] = CELL_DEAD;
	}
      }
    }
    
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) || IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
    {
      Vector2 pos = GetMousePosition();
      if (pos.x * pos.y >= 0 && pos.x < SCREEN_WIDTH && pos.y < SCREEN_HEIGHT)
      {
	grid[(int) pos.x / CELL_WIDTH][(int) pos.y / CELL_HEIGHT]
	  = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? CELL_LIVE : CELL_DEAD;
      }
    }

    BeginDrawing();
    
    ClearBackground(BG_COLOR);
    
    DrawCells();

    char info[15];
    sprintf(info, "Speed: %.5f", nextGenInterval);
    DrawText(info, 10, 10, TEXT_SIZE, TEXT_COLOR);

    if (pause)
    {
      DrawText("Paused", 10, 10 + TEXT_SIZE, TEXT_SIZE, TEXT_COLOR);
    }

    if (!pause)
    {
      if (frame == (int) (FRAME_RATE * nextGenInterval))
      {
        CellsNextGen();
        frame = 1;
      }
      frame++;
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
