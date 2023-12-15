#ifndef _PARTICULES_INTERNAL_H__
#define _PARTICULES_INTERNAL_H__

#include <stdint.h>

enum particule_type{
    TYPE_AIR,
    TYPE_SAND,
    TYPE_HEATER,
    TYPE_COOLER,
    TYPE_COUNT
};


typedef struct automata automata;


typedef struct particule{
    enum particule_type type;
    float melting_point;
    float temperature;
    void (*update)(automata *, uint32_t );
} particule;


#endif // _PARTICULES_INTERNAL_H__
