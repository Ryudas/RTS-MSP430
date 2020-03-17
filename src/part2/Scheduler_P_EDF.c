#include "Scheduler.h"
#include "Led.h"

static void ExecuteTask (Taskp t)
{
  /* insert code */
  t->Invoked++;
  t->Taskf(t->ExecutionTime); // execute task
  t->Flags ^= BUSY_EXEC;
  /* insert code */
}

void Scheduler_P_EDF (Task Tasks[])
{ 

  // Order ready queue
  uint8_t high_prio_idx = 0;
  int j = 0, i = 0;
  Taskp t;

  for(j = 0; j< NUMTASKS; j++)
  { 
  	

  	for ( i = 0; i < NUMTASKS; ++i)
  	{

	    t = &Tasks[i]; 
	  	// check ready 
	  	if (t->Activated != t->Invoked)
	  	{
	  		  	
		  	if(t->NextPendingDeadline < Tasks[high_prio_idx].NextPendingDeadline )
				high_prio_idx = i;
	  	}


  	}

	while (t->Activated != t->Invoked)
	{
		t->Flags |= BUSY_EXEC;
		_EINT();
		StopTracking(TT_SCHEDULER);
		ExecuteTask(&Tasks[high_prio_idx]);
		StartTracking(TT_SCHEDULER);
		_DINT();
		StopTracking(TT_SCHEDULER);
	}

  }
  

}
