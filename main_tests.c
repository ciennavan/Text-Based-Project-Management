/********* main.c ********
	
	Student Name 	= Chat GPT

/********* test_a1_functions.c *********/
// DISCLAIMER: This test harness has been partially generated using prompt engineering in ChatGPT
// It has been widely modified and tested by the course instructors to satisfy the needs of the course.


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sput.h"
#include "a1_data_structures.h"
#include "a1_functions.h"

/* --- Helper to capture stdout --- */
static char *capture_output(void (*func)(void *), void *arg) {
    char *buffer = calloc(1, 4096);
    FILE *stream = fmemopen(buffer, 4096, "w");
    if (!stream) return NULL;

    FILE *old = stdout;
    stdout = stream;

    func(arg);

    fflush(stream);
    fclose(stream);
    stdout = old;

    return buffer;
}

/* --- Wrappers for functions with no params --- */
static void wrapper_print_main_menu(void *arg) {
    (void)arg;
    print_main_menu();
}

/* --- Wrappers for milestone/project print functions --- */
struct milestone_print_args {
    const milestone_t *milestones;
    int num;
    const int *num_activities;
};

static void wrapper_print_milestones(void *arg) {
    struct milestone_print_args *a = arg;
    print_milestone_stats(a->milestones, a->num, a->num_activities);
}

struct project_print_args {
    project_t project;
    const milestone_t *milestones;
    int num;
    const int *num_activities;
};

static void wrapper_print_project(void *arg) {
    struct project_print_args *a = arg;
    print_project_stats(a->project, a->milestones, a->num, a->num_activities);
}

/* --- Tests --- */
static void test_init_activity(void) {
    activity_t act;
    init_activity(&act, 1, 10);

    sput_fail_unless(act.id == 1, "activity id set");
    sput_fail_unless(act.planned_duration == 10, "activity planned_duration set");
    sput_fail_unless(act.completed == 0, "activity not completed by default");
    sput_fail_unless(act.planned_cost == 400 , "activity planned_cost based on the value used in the function call");
    sput_fail_unless(act.actual_cost == 0.0f, "activity actual_cost default 0");
}

static void test_init_milestone(void) {
    milestone_t ms;
    unsigned short durations[NUM_ACTIVITIES] = {5, 10};
    init_milestone(&ms, 1, 2, durations);

    sput_fail_unless(ms.id == 1, "milestone id set");
    sput_fail_unless(ms.completed == 0, "milestone not completed by default");
    sput_fail_unless(ms.actual_cost == 0, "milestone actual cost default 0");
    sput_fail_unless(ms.actual_duration == 0, "milestone actual duration default 0");
    for (int i = 0; i < 2; i++) {
        sput_fail_unless(ms.activity_list[i].id == i+1, "activity id sequential");
        sput_fail_unless(ms.activity_list[i].planned_duration == durations[i], "activity planned durations set");
    }
}

static void test_init_project(void) {
    milestone_t ms[2];
    unsigned short durations[2][NUM_ACTIVITIES] = {{3}, {2,4}};
    for (int i = 0; i<2; i++){
        init_milestone(&ms[i], i, i+1, durations[i]);
    }
    
    int num_activities[2] = {1,2};
    project_t p = init_project(200, ms, 2, num_activities);

    sput_fail_unless(p.id == 200, "project id set");
    sput_fail_unless(p.completed == 0, "project not completed by default");
    sput_fail_unless(p.planned_cost == 360, "project planned cost based on values used to call the function");
    sput_fail_unless(p.actual_duration == 0, "project actual duration by default 0");
    sput_fail_unless(p.planned_duration == 2, "project plan duration based on input data. Plan duration is in days");

}

static void test_print_main_menu(void) {
    char *out = capture_output(wrapper_print_main_menu, NULL);

    const char *expected =
        "Please select an option:\n"
        "1. Update activity\n"
        "2. Print stats\n"
        "3. Exit\n";

    sput_fail_unless(strcmp(out, expected) == 0, "print_main_menu exact match");
    printf("\nThe expected menu is:\n\n%s\n", expected);

    free(out);
}

static void test_print_milestone_stats(void) {
    milestone_t ms[1];
    unsigned short durations[NUM_ACTIVITIES] = {4, 5, 6};
    init_milestone(&ms[0], 300, NUM_ACTIVITIES, durations);

    int num_activities[1] = {NUM_ACTIVITIES};


    printf("\n****CHECK MANUALLY****\n");
    printf("The formating does not need to be exactly the same\n");
    printf("\n*Actual Output when the milestone is not completed*\n");
    print_milestone_stats(ms, 1, num_activities);
    printf("\n*Expected Output when the milestone is not completed*\n");
    printf("Milestone 300 is not completed yet.\n 0 out of 3 activities completed.\n The milestone planned duration is 15 hours.\n The milestone planned cost is $600.00.\n\n");

    ms[0].completed = true;
    ms[0].actual_cost = 100;
    ms[0].actual_duration = 6;

    printf("\n*Actual Output when the milestone is completed - test 1*\n");
    print_milestone_stats(ms, 1, num_activities);
    printf("\n*Expected Output when the milestone is completed - test 1*\n");
    printf("Milestone 300 is completed.\nFinished $500.00 underbudget and 1 days early.\n");

    ms[0].actual_cost = 800;
    ms[0].actual_duration = 80;


    printf("\n*Actual Output when the milestone is completed - test 2*\n");
    print_milestone_stats(ms, 1, num_activities);
    printf("\n*Expected Output when the milestone is completed - test 2*\n");
    printf("Milestone 300 is completed.\nFinished $200.00 overbudget and 9 days late.\n");  

    ms[0].actual_cost = 600;
    ms[0].actual_duration = 15;


    printf("\n*Actual Output when the milestone is completed - test 3*\n");
    print_milestone_stats(ms, 1, num_activities);
    printf("\n*Expected Output when the milestone is completed - test 3*\n");
    printf("Milestone 300 is completed.\nFinished on budget and on time.\n");  
}

static void test_print_project_stats(void) {
    
    milestone_t ms[2];
    unsigned short d1[NUM_ACTIVITIES] = {5, 5, 5};
    unsigned short d2[NUM_ACTIVITIES] = {2, 2, 2};
    init_milestone(&ms[0], 10, NUM_ACTIVITIES, d1);
    init_milestone(&ms[1], 20, NUM_ACTIVITIES, d2);

    int num_activities[2] = {NUM_ACTIVITIES, NUM_ACTIVITIES};
    project_t proj = init_project(400, ms, 2, num_activities);

    printf("\n****CHECK MANUALLY****\n");
    printf("The formating does not need to be exactly the same\n");
    printf("\n*Actual Output when the project is not completed*\n");
    print_project_stats(proj, ms, 2, num_activities);
    printf("\n*Expected Output when the project is not completed*\n");
    printf("Project: 400\nThe project is not completed.\nMilestone 10 is not completed yet.\n0 out of 3 activities completed.\n");
    printf("The milestone planned duration is 15 hours.\nThe milestone planned cost is $600.00.\nMilestone 20 is not completed yet.\n");
    printf("0 out of 3 activities completed.\nThe milestone planned duration is 6 hours.\nThe milestone planned cost is $240.00.\n");
   
    proj.completed = true;
    proj.actual_cost = 1000;
    proj.actual_duration = 20;
    printf("\n****CHECK MANUALLY****\n");
    printf("The formating does not need to be exactly the same\n");
    printf("\n*Actual Output when the project is completed*\n");
    print_project_stats(proj, ms, 2, num_activities);
    printf("\n*Expected Output when the project is completed*\n");
    printf("Project: 400\nThe project is completed.\nFinished $160.00 overbudget and 17 days late.\n");

}

/* Add update tests once implementations are available */
static void test_update_milestone(void) {

    //READY UNTIL HERE
    milestone_t ms;
    unsigned short durations[NUM_ACTIVITIES] = {5, 10, 15};
    init_milestone(&ms, 50, NUM_ACTIVITIES, durations);
    ms.activity_list[0].actual_cost = 80;
    ms.activity_list[0].actual_duration = 6;
    ms.activity_list[0].completed = true;

    update_milestone(&ms, NUM_ACTIVITIES);

    sput_fail_unless(ms.actual_cost == 0, "milestone does not modify cost unless all activities are completed");
    sput_fail_unless(ms.actual_duration == 0, "milestone does not modify duration unless all activities are completed");

    ms.activity_list[1].actual_cost = 100;
    ms.activity_list[1].actual_duration = 11;
    ms.activity_list[1].completed = true;

    ms.activity_list[2].actual_cost = 800;
    ms.activity_list[2].actual_duration = 16;
    ms.activity_list[2].completed = true;

    update_milestone(&ms, NUM_ACTIVITIES);

    sput_fail_unless(ms.actual_cost == 980, "milestone correctly updates actual cost once all activities are completed");
    sput_fail_unless(ms.actual_duration == 33, "milestone correctly updates actual duration once all activities are completed");


}

static void test_update_project(void) {
    milestone_t ms[2];
    unsigned short durations[NUM_ACTIVITIES] = {5, 10, 15};
    unsigned short durations2[2] = {25, 100,};
    init_milestone(&ms[0], 60, NUM_ACTIVITIES, durations);
    init_milestone(&ms[1], 2, 2, durations2);

    int num_activities[2] = {NUM_ACTIVITIES, 2};
    project_t proj = init_project(500, ms, 2, num_activities);
    float planned_cost = proj.planned_cost;
    unsigned short int planned_duration = proj.planned_duration;

    update_project(ms, 2, num_activities, &proj);

    sput_fail_unless(proj.planned_cost == planned_cost, "project does not update planned cost");
    sput_fail_unless(proj.planned_duration == planned_duration, "project does not update planned duration");
    sput_fail_unless(proj.actual_cost == 0, "project actual cost is zero until the project finishes");
    sput_fail_unless(proj.actual_duration == 0, "project actual duration is zero until the project finishes");


    ms[0].activity_list[0].actual_cost = 80;
    ms[0].activity_list[0].actual_duration = 6;
    ms[0].activity_list[0].completed = true;

    ms[0].activity_list[1].actual_cost = 100;
    ms[0].activity_list[1].actual_duration = 11;
    ms[0].activity_list[1].completed = true;

    ms[0].activity_list[2].actual_cost = 800;
    ms[0].activity_list[2].actual_duration = 16;
    ms[0].activity_list[2].completed = true;

    update_milestone(&ms[0], NUM_ACTIVITIES);

    ms[1].activity_list[0].actual_cost = 80;
    ms[1].activity_list[0].actual_duration = 6;
    ms[1].activity_list[0].completed = true;

    ms[1].activity_list[1].actual_cost = 100;
    ms[1].activity_list[1].actual_duration = 11;
    ms[1].activity_list[1].completed = true;

    update_milestone(&ms[1], 2);

    update_project(ms, 2, num_activities, &proj);

    sput_fail_unless(proj.planned_cost == planned_cost, "project does not update planned cost");
    sput_fail_unless(proj.planned_duration == planned_duration, "project does not update planned duration");
    sput_fail_unless(proj.actual_cost == 1160, "project actual cost is correctly updated when the project finishes");
    sput_fail_unless(proj.actual_duration == 7, "project actual duration is correctly updated when the project finishes");



}

/* --- Main --- */
int main(void) {
    sput_start_testing();

    sput_enter_suite("***INITIALIZATION TESTS***");
    sput_run_test(test_init_activity);
    sput_run_test(test_init_milestone);
    sput_run_test(test_init_project);

    sput_enter_suite("***PRINT TESTS***");
    sput_run_test(test_print_main_menu);
    sput_run_test(test_print_milestone_stats);
    sput_run_test(test_print_project_stats);

    sput_enter_suite("***UPDATE TESTS***");
    sput_run_test(test_update_milestone);
    sput_run_test(test_update_project);

    sput_finish_testing();
    return sput_get_return_value();
}
