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
  	
  	// for every task  we change ordering
    // simple selection
  	for ( i = 0; i < NUMTASKS; ++i)
  	{

	    
	    // loop through all tasks and reorder Tasks by earliest deadline
	  
	  	for (j = i ; j < NUMTASKS; j++)
	  	{
	  		t = &Tasks[j];

  			if(t->NextPendingDeadline   < Tasks[i].NextPendingDeadline )
  			{
  								// swap! 
  				temp = Tasks[i];
  				Tasks[i] = Tasks[j];
  				Tasks[j] = temp;

  			}	

	  	}
	  

  	}


  	// FP for the rest of normal algorithm
	for ( i = 0; i < NUMTASKS; ++i)
  	{


		StartTracking(TT_SCHEDULER);
		Taskp t = &Tasks[i];
		if (t->Flags & BUSY_EXEC)
		{
			StopTracking(TT_SCHEDULER);
			PrintResults();
			break;
		}
		else
		{
			if (!(t->Flags & TRIGGERED))
			{
				t->Activated = t->Invoked;
			}

			while (t->Activated != t->Invoked)
			{
				t->Flags |= BUSY_EXEC;
				_EINT();
				StopTracking(TT_SCHEDULER);
				ExecuteTask(t);
				StartTracking(TT_SCHEDULER);
				_DINT();
				StopTracking(TT_SCHEDULER);
			}
		}
		PrintResults();	

  	}

  

}
