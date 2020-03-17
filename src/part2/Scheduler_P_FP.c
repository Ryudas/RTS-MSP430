#include "Scheduler.h"
#include "Led.h"
#include "TimeTracking.h"

static void ExecuteTask(Taskp t)
{
	t->Invoked++;
	t->Taskf(t->ExecutionTime); // execute task
	t->Flags ^= BUSY_EXEC;
}

void Scheduler_P_FP(Task Tasks[])
{
	int i;
	for (i = 0; i < NUMTASKS; i++)
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