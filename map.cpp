#include "map.h"

#include "globals.h"
#include "graphics.h"

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
struct Map {
    HashTable* items;
    int w, h;
};
#define NUM_BUCKETS 20
#define NUM_MAPS 2
static Map maps[NUM_MAPS];
static int active_map;

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y) {
    // TODO: Fix me!
    return (Y*map_width())+X;
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
    // TODO: Fix me!
    return key%NUM_BUCKETS;
    
    
}

void maps_init()
{
    // TODO: Implement!    
    // Initialize hash table
    // Set width & height
    get_active_map()->items = createHashTable(map_hash,  NUM_BUCKETS);
    get_active_map()->w = map_width();
    get_active_map()->h = map_height();
    
    get_barrier_map()->items = createHashTable(map_hash, NUM_BUCKETS);
    get_barrier_map()->w = map_width();
    get_barrier_map()->h = map_height();
}

Map* get_active_map()
{
    return &maps[active_map];
}

Map* get_barrier_map()
{
    return &maps[1];
}

Map* get_main_map(){
    return &maps[0];
}

Map* set_active_map(int m)
{
    active_map = m;
    return &maps[active_map];
}

void print_map()
{
    char lookup[] = {'W', 'P', 'C', 'C', 'C', 'A', 'S','B','C'};
    Map* map = get_active_map();
    for(int j = 0; j < map->h; j++)
    {
        for (int i = 0; i < map->w; i++)
        {
            MapItem* item = (MapItem*)getItem(map->items, XY_KEY(i, j));
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

int map_width()
{
    // FIX THIS
    return 100;
}

int map_height()
{
    // FIX THIS
    return 50;
}

int map_area()
{
    return map_width()*map_height();
}

MapItem* get_current(int x, int y)
{
    return (MapItem*)getItem(get_active_map()->items, XY_KEY(x, y));
}

MapItem* get_north(int x, int y)
{
    return (MapItem*)getItem(get_active_map()->items, XY_KEY(x, y-1));
}

MapItem* get_south(int x, int y)
{
    return (MapItem*)getItem(get_active_map()->items, XY_KEY(x, y+1));
}

MapItem* get_east(int x, int y)
{
    return (MapItem*)getItem(get_active_map()->items, XY_KEY(x+1, y));
}

MapItem* get_west(int x, int y)
{
    return (MapItem*)getItem(get_active_map()->items, XY_KEY(x-1, y));
}

MapItem* get_here(int x, int y)
{
    return (MapItem*)getItem(get_active_map()->items, XY_KEY(x, y));
}

BarrierItem* get_barrier_here(int x, int y)
{
    return (BarrierItem*)getItem(get_barrier_map()->items, XY_KEY(x, y));
}


void map_erase(int x, int y)
{
    deleteItem(get_active_map()->items, XY_KEY(x, y));
}
void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_barrier(int x, int y)
{
    BarrierItem* w1 = (BarrierItem*)malloc(sizeof(BarrierItem));
    w1->add_right = 0;
    w1->add_left = 0;
    w1->add_top = 0;
    w1->add_bottom = 0;
    void* val = insertItem(get_barrier_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_apple(int x, int y)
{
    MapItem* w1 = (MapItem*)malloc(sizeof(MapItem));
    w1->type = APPLE;
    w1->draw = draw_apple;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_sword(int x, int y)
{
    MapItem* w1 = (MapItem*)malloc(sizeof(MapItem));
    w1->type = SWORD;
    w1->draw = draw_sword;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_bow(int x, int y)
{
    MapItem* w1 = (MapItem*)malloc(sizeof(MapItem));
    w1->type = BOW;
    w1->draw = draw_bow;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}


void add_plant(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_good_character(int x, int y, void* player_data)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = GOOD_C;
    w1->walkable = false;
    w1->data = player_data;
    //w1->data = (Character*) malloc(sizeof(Character));
    //w1->data->health = player_data->health;
    //w1->data->attack = player_data->attack;
    //w1->data->defense = player_data->defense;
    //w1->dat->team = player_data->team;
    w1->draw = draw_player1sprite;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val);
}
void add_bad_character(int x, int y, void* player_data)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = BAD_C;
    w1->walkable = false;
    w1->data = player_data;
    w1->draw = draw_player2sprite;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val);
}

