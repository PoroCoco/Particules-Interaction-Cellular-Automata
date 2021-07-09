//définitions des paramètre physique des particules. 
#define water_spread_speed 3
#define sable_max_y_velocity 5
#define feu_max_y_velocity 33
#define eau_max_y_velocity 6
#define vapeur_max_y_velocity 3

//def des id de chaques particules
#define particules_number 7
#define mat_id_air 0
#define mat_id_sable 1
#define mat_id_eau 2
#define mat_id_bois 3
#define mat_id_feu 4
#define mat_id_vapeur 5
#define mat_id_explorer 6

//flammabilité des particules
#define bois_flammability 40

//temps de vie des particules
#define lifetime_wood 250
#define lifetime_feu 355
#define lifetime_vapeur 150


#define def_gravity 1
int gravity;
int gravity_direction;

typedef unsigned int uint;

struct vector{
    float x;
    float y;
};
typedef struct vector vector_t;

struct color{
    uint R;
    uint G;
    uint B;
    uint ALPHA;
};
typedef struct color color_t;

color_t RED = {255,0,0,255};


struct particule{
    unsigned int id;
    float life_time;
    vector_t velocity;
    color_t color;
    bool has_been_updated;

};
typedef struct particule particule_t;



bool isInWorldBoundaries(uint x, uint y);

//regarde si les coordonnées souhaitées existe dans l'array. Renvoie true si oui sinon false.
bool isInWorldBoundaries(uint y, uint x){    
    if(x>=0 && x<=COLONNE-1 && y>=0 && y<= LIGNE-1){
        return true;
    }
    return false;
}

//change la couleur d'une particule 
void highlightParticule(particule_t world[LIGNE][COLONNE], uint x, uint y, int R, int G, int B){ 
    if(isInWorldBoundaries(y,x)){
        world[y][x].color.R = R;
        world[y][x].color.G = G;
        world[y][x].color.B = B;
    }
}

//informations sur chaque type de particules
struct particule particule_air = { .id = mat_id_air, .life_time = 0, .velocity = {.x = 0, .y = 0}, .color = {125,125,125,255}, .has_been_updated = false};
struct particule particule_sable = { .id = mat_id_sable, .life_time = -1, .velocity = {.x = 0, .y = 0}, .color = {255,255,0,255}, .has_been_updated = false};
struct particule particule_eau = { .id = mat_id_eau, .life_time = -1, .velocity = {.x = 0, .y = 0}, .color = {0,0,255,255}, .has_been_updated = false};
struct particule particule_bois = { .id = mat_id_bois, .life_time = lifetime_wood, .velocity = {.x = 0, .y = 0}, .color = {139,69,19,255}, .has_been_updated = false};
struct particule particule_feu = { .id = mat_id_feu, .life_time = lifetime_feu, .velocity = {.x = 0, .y = 0}, .color = {255,92,15,255}, .has_been_updated = false};
struct particule particule_vapeur = { .id = mat_id_vapeur, .life_time = lifetime_vapeur, .velocity = {.x = 0, .y = 0}, .color = {220,220,220,125}, .has_been_updated = false};
struct particule particule_explorer = { .id = mat_id_explorer, .life_time = -1, .velocity = {.x = 0, .y = 0}, .color = {255,255,255,255}, .has_been_updated = false};
