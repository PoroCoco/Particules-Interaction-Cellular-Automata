#ifndef _PARTICULES_H__
#define _PARTICULES_H__

#include "utils.h"
#include "particules_internal.h"

#define PART_HEATER_TEMP 3000
#define PART_COOLER_TEMP 0


particule particule_create(enum particule_type type);


#endif // _PARTICULES_H__
