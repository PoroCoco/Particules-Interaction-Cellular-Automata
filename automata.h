#ifndef _AUTOMATA_H__
#define _AUTOMATA_H__

typedef struct automata automata;

automata* automata_init(void);

void automata_update(automata* autom);

void automata_cleanup(automata* autom);

#endif // _AUTOMATA_H__
