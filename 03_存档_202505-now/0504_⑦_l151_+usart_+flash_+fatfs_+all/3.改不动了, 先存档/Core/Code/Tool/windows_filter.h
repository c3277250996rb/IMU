#include "main.h"



#define WF_WINDOWS_SIZE 20
#define WF_TRIGGER_THRESHOLD 300

extern int WF_Status;
extern float WF_Window[WF_WINDOWS_SIZE];
extern int WF_W_Current_Index;
extern int WF_Counter_AddUp;

extern float WF_W_Average;


enum
{
    F_WF_Reset = 0,
    F_WF_Trigger,
    F_WF_AddUp,
    F_WF_Finish
};


void WF_Judge_From_Static_To_Dynamic(float input_float);
void WF_Judge_From_Dynamic_To_Static(float input_float);

void WF_update_average_index_and_array(float input_float);
