#include "main.h"
#include "__module.h"


#define WINDOWS_SIZE 20
#define TRIGGER_THRESHOLD 300

extern int is_WF_trigger;
extern float WF_array[WINDOWS_SIZE];
extern int WF_a_current_index;
extern int WF_a_add_up;

extern float WF_a_average;


enum
{
    enum_RESET = 0,
    enum_TRIGGER,
    enum_ADDUP,
    enum_FINISH
};


void WF_Judge_From_Static_To_Dynamic(float input_float);
void WF_Judge_From_Dynamic_To_Static(float input_float);

void WF_update_average_index_and_array(float input_float);
