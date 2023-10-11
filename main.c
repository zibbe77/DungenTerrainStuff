#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

int seed = 0;
int tileSize = 32;
float output[4096];

typedef enum tile_types_e
{
    TILE_GRASS,
    TILE_WATER,
    TILE_FLOOR,
    TILE_WALL
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
    if (output[arrayNum] < 0.5f)
    {
        return 0;
    }
    else
    {
        return 1;
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

void PerlinNoise2D(int width, int height, int octaves, float *output, float *noiceSeed)
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            float noise = 0.0f;
            float scale = 1.0f;
            float scaleAcc = 0.0f;

            for (int o = 0; o < octaves; o++)
            {
                // printf("y %d\n", o);

                int pitch = width >> o;

                int sampleX1 = (x / pitch) * pitch;
                int sampleY1 = (y / pitch) * pitch;

                int sampleX2 = (sampleX1 + pitch) % width;
                int sampleY2 = (sampleY1 + pitch) % width;

                float blendX = (float)(x - sampleX1) / (float)pitch;
                float blendY = (float)(y - sampleY1) / (float)pitch;

                float sample1 = (1.0f - blendX) * noiceSeed[sampleY1 * width + sampleX1] + blendX * noiceSeed[sampleY1 * width + sampleX2];
                float sample2 = (1.0f - blendX) * noiceSeed[sampleY2 * width + sampleX1] + blendX * noiceSeed[sampleY2 * width + sampleX2];

                noise += (blendX * (sample2 - sample1) + sample1) * scale;
                scaleAcc += scale;
            }
            printf("test %f \n ", noise / scaleAcc);
            printf("num %d \n ", y * width + x);
            output[y * width + x] = noise / scaleAcc;
        }
    }
}

void DrawMap(map_t *map, int x, int y)
{
    for (int y = 0; y < map->height; y++)
    {
        for (int x = 0; x < map->width; x++)
        {
            switch (map->tiles[(y * map->width) + x].type)
            {
            case TILE_GRASS:

                DrawRectangle(x * map->tileSize, y * map->tileSize, tileSize, tileSize, GREEN);
                break;
            case TILE_WATER:
                DrawRectangle(x * map->tileSize, y * map->tileSize, tileSize, tileSize, BLUE);
                break;

            default:
                break;
            }
            //  map->tiles[(y * map->width) + x];
        }
    }
}

void RandomNoice(float *noiceSeed)
{
    for (int i = 0; i < 64; i++)
    {
        noiceSeed[i] = rand();
        // printf("Rand: %f", noiceSeed[i]);
    }
}

int main()
{
    // setup
    //------------------------------------------------------------------------------
    ComfigSeed();

    float noiceSeed[4096];
    RandomNoice(noiceSeed);

    printf("Seed: %d \n", seed);

    PerlinNoise2D(64, 64, 4, output, noiceSeed);

    map_t main_map = InitializeMap(64, 64, 32);

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