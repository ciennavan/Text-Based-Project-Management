/********* a1_data_structures.h ********
	
	Student Name 	= Cienna Van
	Student Number	= 101302963 
*/

#ifndef A1_DATA_STRUCTURES_H
#define A1_DATA_STRUCTURES_H
#include <stdbool.h>

#define NUM_ACTIVITIES 3


typedef struct activity {
    unsigned short int id;
    float planned_cost, actual_cost;
    unsigned short int planned_duration, actual_duration;
    _Bool completed;
}activity_t;

typedef struct milestone {
    unsigned short int id;
    activity_t activity_list[NUM_ACTIVITIES];
    _Bool completed;
    float actual_cost;
    short int actual_duration;
}milestone_t;

typedef struct project {
    unsigned short int id;
    float planned_cost, actual_cost;
    unsigned short int planned_duration, actual_duration;
    _Bool completed;
}project_t;

#endif 
