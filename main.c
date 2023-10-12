#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

#include "perlinNoise.c"

int seed = 0;
int tileSize = 32;
float output[4096];

typedef enum tile_types_e
{
    TILE_DEEP_WATER,
    TILE_WATER,
    TILE_GRASS,
    TILE_MOUNTAIN,
    TILE_HIGH_MOUNTAIN,
    TILE_ERROR
} tile_types_e;

typedef struct tile_t
{
    tile_types_e type;
    int x;
    int y;
} tile_t;

typedef struct string_C
{
    char *str;
    int length;
} string_C;

typedef struct window_C
{
    int height;
    int width;
    string_C title;
} window_C;

window_C main_window_C = {
    .width = 1920,
    .height = 1080,
    .title = {"My Window", 8}};

// map + tiles
//------------------------------------------------------------------------

typedef struct map_t
{
    int width;
    int height;
    int tileSize;
    tile_t *tiles;
} map_t;

tile_types_e TypeControler(int arrayNum)
{
    // printf("test 1 %f \n", output[arrayNum]);
    // printf("test %d \n", arrayNum);
    if (output[arrayNum] < 0.3f)
    {
        return TILE_DEEP_WATER;
    }
    else if (output[arrayNum] < 0.4f)
    {
        return TILE_WATER;
    }
    else if (output[arrayNum] < 0.6f)
    {
        return TILE_GRASS;
    }
    else if (output[arrayNum] < 0.69f)
    {
        return TILE_MOUNTAIN;
    }
    else
    {
        return TILE_HIGH_MOUNTAIN;
    }

    return 3;

    // return rand() % 2;
}

map_t InitializeMap(int width, int height, int tileSize)
{
    map_t output = (map_t){
        .width = width,
        .height = height,
        .tileSize = tileSize,
        .tiles = (tile_t *)malloc(width * height * sizeof(tile_t))};

    for (int y = 0; y < output.height; y++)
    {
        for (int x = 0; x < output.width; x++)
        {
            output.tiles[y * output.width + x] = (tile_t){
                .type = TypeControler(y * output.width + x),
                .x = x,
                .y = y};
        }
    }
    return output;
}

void FreeMap(map_t *map)
{
    free(map->tiles);
}

void DrawMap(map_t *map, int x, int y)
{
    for (int y = 0; y < map->height; y++)
    {
        for (int x = 0; x < map->width; x++)
        {
            switch (map->tiles[(y * map->width) + x].type)
            {
            case TILE_DEEP_WATER:
                DrawRectangle(x * map->tileSize, y * map->tileSize, tileSize, tileSize, DARKBLUE);
                break;
            case TILE_WATER:
                DrawRectangle(x * map->tileSize, y * map->tileSize, tileSize, tileSize, BLUE);
                break;
            case TILE_GRASS:
                DrawRectangle(x * map->tileSize, y * map->tileSize, tileSize, tileSize, GREEN);
                break;
            case TILE_MOUNTAIN:
                DrawRectangle(x * map->tileSize, y * map->tileSize, tileSize, tileSize, LIGHTGRAY);
                break;
            case TILE_HIGH_MOUNTAIN:
                DrawRectangle(x * map->tileSize, y * map->tileSize, tileSize, tileSize, GRAY);
                break;
            case TILE_ERROR:
                DrawRectangle(x * map->tileSize, y * map->tileSize, tileSize, tileSize, PURPLE);
                break;
            default:
                DrawRectangle(x * map->tileSize, y * map->tileSize, tileSize, tileSize, PURPLE);
                break;
            }
            //  map->tiles[(y * map->width) + x];
        }
    }
}

// random seed
//------------------------------------------------------------------------
void ComfigSeed()
{
    if (seed == 0)
    {
        seed = time(NULL);
        srand(seed);
    }
    else
    {
        srand(seed);
    }
}

void CreatTileMap()
{
    float width = main_window_C.width / tileSize;
    float height = main_window_C.height / tileSize;
}

// noise gen
//------------------------------------------------------------------------
void PopPerlinNoiseArray(int width, int height)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            output[y * width + x] = Perlin_Get2d(x, y, 0.08, 7);
            printf("; %f", output[y * width + x]);
        }
        // printf("\n--------------------------\n");
    }
}

int main()
{
    // setup
    //------------------------------------------------------------------------------
    ComfigSeed();
    gen_seed();

    PopPerlinNoiseArray(64, 64);
    map_t main_map = InitializeMap(64, 64, 16);

    InitWindow(main_window_C.width, main_window_C.height, main_window_C.title.str);
    //------------------------------------------------------------------------------
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        DrawMap(&main_map, 64, 64);

        EndDrawing();
    }

    FreeMap(&main_map);

    return 0;
}