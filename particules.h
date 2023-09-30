#ifndef _PARTICULES_H__
#define _PARTICULES_H__

#include "utils.h"


enum particule_type{
    TYPE_AIR,
    TYPE_SAND
};

typedef struct particule{
    enum particule_type type;
    float melting_point;
    float temperature;
} particule;

particule particule_create(enum particule_type type);


#endif // _PARTICULES_H__
