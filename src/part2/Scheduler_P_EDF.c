#include "Scheduler.h"
#include "Led.h"

static void ExecuteTask (Taskp t)
{
  /* insert code */
  t->Invoked++;
  t->Taskf(t->ExecutionTime); // execute task
  /* insert code */
}

void Scheduler_P_EDF (Task Tasks[])
{ 

  // Order ready queue
  uint8_t high_prio_idx = 0;
  int i = 0;
  for ( i = 0; i < NUMTASKS; ++i)
  {

	    Taskp t = &Tasks[i]; 
	  	// check ready 
	  	if (t->Activated != t->Invoked)
	  	{
	  		  	
		  	if(t->NextPendingDeadline < Tasks[high_prio_idx].NextPendingDeadline )
				high_prio_idx = i;
	  	}


  }

  	ExecuteTask(&Tasks[high_prio_idx]);

}
