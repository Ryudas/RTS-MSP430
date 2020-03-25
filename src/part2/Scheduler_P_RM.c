#include "Scheduler.h"
#include "Led.h"
#include "TimeTracking.h"

static void ExecuteTask (Taskp t)
{
  t->Invoked++;
  t->Taskf(t->ExecutionTime); // execute task
  t->Flags ^= BUSY_EXEC;
}

void Scheduler_P_RM (Task Tasks[], uint16_t current_time)
{ 
  /* insert code */
	//. current time - > TAR 



	// select highest priority task (RM)

	//

	// else dispatch 

  /* Use ExecuteTask function to execute a task */
  /* insert code */
	// rnext should be done in interrupt 
	// for all tasks 	
	int i;
    
    // holds last executed task as bit (can only handle up to 16 tasks)
    uint16_t latest_to_exec = 0;

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
				if( (current_time + (t->ExecutionTime)) <= Tasks[0].NextRelease ||  
					((latest_to_exec & 0x1) && (current_time + (t->ExecutionTime)) <= (Tasks[0].NextRelease + Tasks[0].Period - Tasks[0].ExecutionTime))
				){

					t->Flags |= BUSY_EXEC;

					latest_to_exec &= 0; // clear buffer, track latest to exec
					latest_to_exec |= 1<<i; // set latest to exec as current task

					_EINT();
					StopTracking(TT_SCHEDULER);
					ExecuteTask(t);
					StartTracking(TT_SCHEDULER);
					_DINT();
					StopTracking(TT_SCHEDULER);
				} else {
					break;
				}


			}
		}
		PrintResults();
	}
}
