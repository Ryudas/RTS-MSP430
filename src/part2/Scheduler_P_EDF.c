#include "Scheduler.h"
#include "Led.h"

static void ExecuteTask (Taskp t)
{
  /* insert code */
  t->Taskf(t->ExecutionTime); // execute task
  /* insert code */
}

void Scheduler_P_EDF (Task Tasks[])
{ 

  // Order ready queue
  uint8_t high_prio_idx = 0;
  for (int i = 0; i < NUMTASKS; ++i)
  {
  	if(Tasks[i].NextPendingDeadline < Tasks[high_prio_idx].NextPendingDeadline )
  		high_prio_idx = i;
  }

  if(Tasks[high_prio_idx]>Activated != Tasks[high_prio_idx]->Invoked)
  	ExecuteTask(&Tasks[high_prio_idx]);

}
