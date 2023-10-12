#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "raylib.h"
#include "raymath.h"

#include "perlinNoise.c"

#define WorkingMapWidth 124
#define WorkingMapHeight 64

int seed = 0;
int tileSize = 32;

float *baseTerrienMap;
float *moreDetailMap;

float *endMap;

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
    // printf("test 1 %f \n", baseTerrienMap[arrayNum]);
    // printf("test %d \n", arrayNum);
    if (endMap[arrayNum] < 0.3f)
    {
        return TILE_DEEP_WATER;
    }
    else if (endMap[arrayNum] < 0.4f)
    {
        return TILE_WATER;
    }
    else if (endMap[arrayNum] < 0.6f)
    {
        return TILE_GRASS;
    }
    else if (endMap[arrayNum] < 0.69f)
    {
        return TILE_MOUNTAIN;
    }
    else if (endMap[arrayNum] < 1)
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

// malloc Arrays
//------------------------------------------------------------------------

float *MallocFloatArrys(float *array, int width, int height)
{
    array = (float *)malloc(width * height * sizeof(float));
    return array;
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

void clearData(int width, int height, float *dataArray)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            dataArray[y * width + x] = 0;
        }
    }
}
// noise gen
//------------------------------------------------------------------------
void PopPerlinNoiseArray(int width, int height, float *dataArray, double freq, int depth)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            dataArray[y * width + x] += Perlin_Get2d(x, y, freq, depth);
            // printf("; %f", baseTerrienMap[y * width + x]);
        }
        // printf("\n--------------------------\n");
    }
}

void Addmaps(int width, int height)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            endMap[y * width + x] = (baseTerrienMap[y * width + x] + moreDetailMap[y * width + x] / 2);
            printf("; %f", endMap[y * width + x]);
        }
        printf("\n--------------------------\n");
    }
}

int main()
{
    // setup
    //------------------------------------------------------------------------------
    ComfigSeed();
    gen_seed();

    // Makes maps
    baseTerrienMap = MallocFloatArrys(baseTerrienMap, WorkingMapWidth, WorkingMapHeight);
    moreDetailMap = MallocFloatArrys(moreDetailMap, WorkingMapWidth, WorkingMapHeight);

    endMap = MallocFloatArrys(endMap, WorkingMapWidth, WorkingMapHeight);

    // Clear data
    clearData(WorkingMapWidth, WorkingMapHeight, baseTerrienMap);
    clearData(WorkingMapWidth, WorkingMapHeight, moreDetailMap);
    clearData(WorkingMapWidth, WorkingMapHeight, endMap);

    // makes Hight map
    PopPerlinNoiseArray(WorkingMapWidth, WorkingMapHeight, baseTerrienMap, 0.09, 2);

    TurnOnSeedOffset(true);
    PopPerlinNoiseArray(WorkingMapWidth, WorkingMapHeight, moreDetailMap, 0.01, 1);

    // Puts maps togeder
    Addmaps(WorkingMapWidth, WorkingMapHeight);

    // creats tiles + map
    map_t main_map = InitializeMap(WorkingMapWidth, WorkingMapHeight, 16);

    // makes window
    InitWindow(main_window_C.width, main_window_C.height, main_window_C.title.str);
    //------------------------------------------------------------------------------
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        DrawMap(&main_map, WorkingMapWidth, WorkingMapHeight);

        EndDrawing();
    }

    FreeMap(&main_map);
    free(baseTerrienMap);

    return 0;
}