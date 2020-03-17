#include "Scheduler.h"
#include "Led.h"
#include "TimeTracking.h"


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
  int j = 0, i = 0;
  Taskp t;
  uint8_t prio_tsk;
  Task temp;
  	
  	// for every task  we run in order 
  	for ( i = 0; i < NUMTASKS; ++i)
  	{

	    
	    // loop through all tasks and find earliest deadline
	  
	  	for (j = 0 ; j < NUMTASKS -1; j++)
	  	{
	  		t = &Tasks[j];

  			if(t->NextPendingDeadline   < Tasks[i].NextPendingDeadline )
				prio_tsk = j;

	  	}
	  


	  	while (t->Activated != t->Invoked)
		{
			t->Flags |= BUSY_EXEC;
			_EINT();
			StopTracking(TT_SCHEDULER);
			ExecuteTask(&Tasks[prio_tsk]);
			StartTracking(TT_SCHEDULER);
			_DINT();
			StopTracking(TT_SCHEDULER);
		}

  	}


  
  

}
