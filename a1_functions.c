/********* a1_functions.c ********
	
	Student Name 	= Cienna Van
	Student Number	= 101302963
*/

#include <stdio.h>
#include <math.h>
#include "a1_functions.h"
#include "a1_data_structures.h"

#define AVERAGE_HOURLY_SALARY 40.0f
#define HOURS_PER_DAY 7.5f

/*
initiallizes activity with id, hours, cost. empty values
*/
void init_activity(activity_t * activity_to_int, unsigned short int id,  unsigned short int planned_duration){
	if (activity_to_int == NULL) return;

	activity_to_int -> id = id;
	activity_to_int -> planned_duration = planned_duration; 
	activity_to_int->planned_cost = (float)planned_duration * AVERAGE_HOURLY_SALARY;
	activity_to_int -> actual_cost = 0.0f;
	activity_to_int -> actual_duration = 0;
	activity_to_int ->completed = 0;

} 

void init_milestone(milestone_t * milestone_to_int, unsigned short int id, unsigned short int num_activities, unsigned short int * planned_duration){
	if (milestone_to_int == NULL) return;

	milestone_to_int -> id = id;
	milestone_to_int -> completed = 0;
	milestone_to_int -> actual_cost = 0.0f;
	milestone_to_int -> actual_duration = 0;

	unsigned short to_init = (unsigned short) (num_activities <= NUM_ACTIVITIES ? num_activities : NUM_ACTIVITIES);
	if ((int) num_activities < 0) to_init = 0;

	for (unsigned short i = 0; i < NUM_ACTIVITIES; ++i){
		unsigned short pd = 0;
		if(i < to_init && planned_duration != NULL){
			pd = planned_duration [i];
		}
		init_activity(&milestone_to_int -> activity_list[i], (unsigned short)(i + 1), pd);
	}



}


project_t init_project(unsigned short int id, milestone_t *milestone_list, int number_milestones, const int * number_activities) {
    project_t p;
    p.id = id;
    p.planned_cost = 0.0f;
    p.actual_cost = 0.0f;
    p.planned_duration = 0;
    p.actual_duration = 0;
    p.completed = 0;

    if (milestone_list == NULL || number_milestones <= 0) {
        return p;
    }

    double total_planned_hours = 0.0;
    /* Sum planned cost and planned hours across milestones and their active activities */
    for (int i = 0; i < number_milestones; ++i) {
        int act_count = NUM_ACTIVITIES;
        if (number_activities != NULL) {
            int na = number_activities[i];
            if (na < 0) na = 0;
            if (na < act_count) act_count = na;
        }
        for (int a = 0; a < act_count; ++a) {
            activity_t *act = &milestone_list[i].activity_list[a];
            p.planned_cost += act->planned_cost;
            total_planned_hours += (double) act->planned_duration;
            /* actuals remain zero initially */
        }
    }

    if (total_planned_hours > 0.0) {
        p.planned_duration = (unsigned short) ceil(total_planned_hours / HOURS_PER_DAY);
    } else {
        p.planned_duration = 0;
    }

    /* actuals already zero */
    p.actual_cost = 0.0f;
    p.actual_duration = 0;
    p.completed = 0;

    return p;
}

void print_main_menu(void) {
    printf("Please select an option:\n");
    printf("1. Update activity\n");
    printf("2. Print stats\n");
    printf("3. Exit\n");
}

static void compute_milestone_aggregates(const milestone_t *m, int act_count, unsigned int *out_planned_hours, float *out_planned_cost, unsigned int *out_completed_count) {
    unsigned int ph = 0;
    float pc = 0.0f;
    unsigned int cc = 0;
    if (m != NULL) {
        for (int a = 0; a < act_count; ++a) {
            const activity_t *act = &m->activity_list[a];
            ph += (unsigned int) act->planned_duration;
            pc += act->planned_cost;
            if (act->completed) cc++;
        }
    }
    if (out_planned_hours) *out_planned_hours = ph;
    if (out_planned_cost) *out_planned_cost = pc;
    if (out_completed_count) *out_completed_count = cc;
}

void print_milestone_stats(const milestone_t * list_milestones, int num_milestones, const int * number_activities) {
    if (list_milestones == NULL || num_milestones <= 0) {
        printf("No milestones available.\n");
        return;
    }

    for (int i = 0; i < num_milestones; ++i) {
        const milestone_t *m = &list_milestones[i];

        int act_count = NUM_ACTIVITIES;
        if (number_activities != NULL) {
            int na = number_activities[i];
            if (na < 0) na = 0;
            if (na < act_count) act_count = na;
        }

        unsigned int planned_hours = 0;
        float planned_cost = 0.0f;
        unsigned int completed_count = 0;
        compute_milestone_aggregates(m, act_count, &planned_hours, &planned_cost, &completed_count);

        if (!m->completed) {
            /* not completed message */
            printf("Milestone %u is not completed yet.\n", (unsigned int)m->id);
            printf(" %u out of %d activities completed.\n", completed_count, act_count);
            printf(" The milestone planned duration is %u hours.\n", planned_hours);
            printf(" The milestone planned cost is $%.2f.\n\n", planned_cost);
        } else {
            unsigned int planned_days = (unsigned int) ceil((double)planned_hours / HOURS_PER_DAY);
           
            int actual_hours = m->actual_duration;
			float actual_cost = m->actual_cost;

			float diff_cost = planned_cost - actual_cost; 
			int diff_hours = 0;
			if (actual_hours >= 0) {
    
    		int actual_days = (int) ceil((double)actual_hours / HOURS_PER_DAY);
    		diff_hours = (int)planned_days - actual_days;
}


            printf("Milestone %u is completed.\n", (unsigned int)m->id);

            if (fabsf(diff_cost) < 0.005f && diff_hours == 0) {
                printf("Finished on budget and on time.\n");
            } else {
                if (diff_cost > 0.005f) {
                    /* underbudget */
                    printf("Finished $%.2f underbudget", diff_cost);
                } else if (diff_cost < -0.005f) {
                    printf("Finished $%.2f overbudget", -diff_cost);
                } else {
                    printf("Finished on budget");
                }

                /* time phrase */
                if (diff_hours > 0) {
                    printf(" and %d days early.\n", diff_hours);
                } else if (diff_hours < 0) {
                    printf(" and %d days late.\n", -diff_hours);
                } else {
                    printf(" and on time.\n");
                }
            }
        }
    }
}

void print_project_stats(project_t details, const milestone_t * list_milestones, int num_milestones, const int * number_activities) {
    printf("Project: %u\n", (unsigned int)details.id);
    if (!details.completed) {
        printf("The project is not completed.\n");
    } else {
        printf("The project is completed.\n");
        float planned_cost = details.planned_cost;
        float actual_cost = details.actual_cost;
        int planned_days = details.planned_duration;
        int actual_days = details.actual_duration;

        float diff_cost = actual_cost - planned_cost;
        int diff_days = actual_days - planned_days;

        if (fabsf(diff_cost) < 0.005f && diff_days == 0) {
            printf("Finished on budget and on time.\n");
        } else {
            if (diff_cost > 0.005f) {
                printf("Finished $%.2f overbudget", diff_cost);
            } else if (diff_cost < -0.005f) {
                printf("Finished $%.2f underbudget", -diff_cost);
            } else {
                printf("Finished on budget");
            }

            if (diff_days > 0) {
                printf(" and %d days late.\n", diff_days);
            } else if (diff_days < 0) {
                printf(" and %d days early.\n", -diff_days);
            } else {
                printf(" and on time.\n");
            }
        }
    }

    print_milestone_stats(list_milestones, num_milestones, number_activities);
}

void update_milestone(milestone_t * milestone_to_update, int activities_in_milestone) {
    if (milestone_to_update == NULL) return;

    int act_count = NUM_ACTIVITIES;
    if (activities_in_milestone >= 0 && activities_in_milestone <= NUM_ACTIVITIES) act_count = activities_in_milestone;

    int all_complete = 1;
    unsigned int total_actual_hours = 0;
    float total_actual_cost = 0.0f;

    for (int a = 0; a < act_count; ++a) {
        activity_t *act = &milestone_to_update->activity_list[a];
        if (!act->completed) {
            all_complete = 0;
        }
        total_actual_hours += (unsigned int) act->actual_duration;
        total_actual_cost += act->actual_cost;
    }

    if (all_complete && act_count > 0) {
        milestone_to_update->actual_duration = (short) total_actual_hours;
        milestone_to_update->actual_cost = total_actual_cost;
        milestone_to_update->completed = 1;
    } else {
        /* leave actuals as zero and completed as 0 */
        milestone_to_update->actual_duration = 0;
        milestone_to_update->actual_cost = 0.0f;
        milestone_to_update->completed = 0;
    }
}

void update_project(const milestone_t * milestone_array, int num_milestones, const int * number_activities, project_t * my_project) {
    if (my_project == NULL) return;

    float sum_planned_cost = 0.0f;
    double total_planned_hours = 0.0;
    double total_actual_hours = 0.0;
    float sum_actual_cost = 0.0f;
    int all_complete = 1;

    if (milestone_array != NULL && num_milestones > 0) {
        for (int i = 0; i < num_milestones; ++i) {
            const milestone_t *m = &milestone_array[i];
            int act_count = NUM_ACTIVITIES;
            if (number_activities != NULL) {
                int na = number_activities[i];
                if (na < 0) na = 0;
                if (na < act_count) act_count = na;
            }

            for (int a = 0; a < act_count; ++a) {
                const activity_t *act = &m->activity_list[a];
                sum_planned_cost += act->planned_cost;
                total_planned_hours += (double) act->planned_duration;

                /* For completeness check, every considered activity must be completed */
                if (!act->completed) all_complete = 0;

                /* Only add actuals (hours/cost) regardless — we will choose to store them only if all_complete */
                total_actual_hours += (double) act->actual_duration;
                sum_actual_cost += act->actual_cost;
            }
        }
    }

    my_project->planned_cost = sum_planned_cost;
    if (total_planned_hours > 0.0) {
        my_project->planned_duration = (unsigned short) ceil(total_planned_hours / HOURS_PER_DAY);
    } else {
        my_project->planned_duration = 0;
    }

    if (all_complete && num_milestones > 0) {
        my_project->actual_cost = sum_actual_cost;
        my_project->actual_duration = (unsigned short) ceil(total_actual_hours / HOURS_PER_DAY);
        my_project->completed = 1;
    } else {
        my_project->actual_cost = 0.0f;
        my_project->actual_duration = 0;
        my_project->completed = 0;
    }
}

