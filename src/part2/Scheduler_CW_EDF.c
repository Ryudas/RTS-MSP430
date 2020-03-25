#include "Scheduler.h"
#include "Led.h"
#include "TimeTracking.h"


static void ExecuteTask (Taskp t)
{
  t->Invoked++;
  t->Taskf(t->ExecutionTime); // execute task
  t->Flags ^= BUSY_EXEC;
}

uint16_t min(uint16_t a, uint16_t b)
{
	return( (a<b)? a:b);
}

void Scheduler_CW_EDF (Task Tasks[])
{ 
  /// Order ready queue
  int j = 0, i = 0;
  Taskp t;
  Task temp;
  	
  uint16_t current_time = Tasks[0].global_time + uint16_t(TAR * 1000.0/4096.0);
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

  	// now Tasks[0] has earliest deadline.

  	// Calculate L1 

  	uint8_t sorted_no_pend_tasks_idx[NUMTASKS];
  	uint8_t last_idx = -1;
  	// part 1 - sorted list (ascending by deadlines) of indices 
  	// of the tasks that  do not have a pending tasks at time
  	for(i = 0 ; i< NUMTASKS ; i++)
  	{
  		sorted_no_pend_tasks_idx[i] = -1; // by default 

  		// check if task has no pending jobs 
  		if(Tasks[i].Activated == Tasks[i].Invoked){
  			sorted_no_pend_tasks_idx[i] = i;
  			last_idx = i;
  		}
  	}

  	// going from latest deadline to earliest,
  	// iterate over L 
  	// initial value of Li is Lm, end value will be L1
  	uint16_t Li  = Tasks[last_idx].NextPendingDeadline - Tasks[last_idx].ExecutionTime;

  	// start at latest job that isnt m 
  	for ( i = last_idx-1 ; i >= 0; i--)
  	{
  		if( sorted_no_pend_tasks_idx[i] != -1)
  		{
  			// calculate latest start time of task given by index
  			// using previous Li
  			Li = min(Tasks[i].NextPendingDeadline, Li) - Tasks[i].ExecutionTime;
  		}
  	}

  	// FP for the rest of normal algorithm
	for (i = 0; i < NUMTASKS; i++)
	{

		// TAR keeps current time 
		StartTracking(TT_SCHEDULER);
		Taskp t = &Tasks[i];

		// if current task is not being  executed.
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
				// if current task will be finished before
				// next relead of highest prio in system

				// OR  t0  is latest to execute and end of candidate task 
				// is lesser than the latest possible time for T0 to start 	
				if( (current_time + t->ExecutionTime) <= Li )
				{

					t->Flags |= BUSY_EXEC;
					_EINT();
					StopTracking(TT_SCHEDULER);
					ExecuteTask(t);
					StartTracking(TT_SCHEDULER);
					_DINT();
					StopTracking(TT_SCHEDULER);
				} else {
					// schedule n idle interval from t until the release
					// of the critical job
					break;
				}


			}
		}
		PrintResults();
	}
}


