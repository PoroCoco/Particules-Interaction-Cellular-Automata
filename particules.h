#ifndef _PARTICULES_H__
#define _PARTICULES_H__

#include "utils.h"

#define PART_HEATER_TEMP 3000
#define PART_COOLER_TEMP 0


enum particule_type{
    TYPE_AIR,
    TYPE_SAND,
    TYPE_HEATER,
    TYPE_COOLER,
    TYPE_COUNT
};

typedef struct particule{
    enum particule_type type;
    float melting_point;
    float temperature;
} particule;

particule particule_create(enum particule_type type);


#endif // _PARTICULES_H__
