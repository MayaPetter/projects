#include <assert.h> /* assert */ 
#include <stdio.h>  /* printf, puts */

#include "scheduler.h" /* scheduler functions */

static task_uid_t SELF_UID = {0};
static task_uid_t OTHER_UID = {0};

static task_status PrintFirst()
{
	puts("I'm first");
	
	return SCHED_NO_REPEAT;
}

static task_status AddTaskBeforeAll(void *param)
{
    time_t time_in_sec = time(NULL) - 10;
    
    OTHER_UID = SchedulerAddTask((scheduler_t *)param, NULL, &PrintFirst, time_in_sec, 0); 
	
	return SCHED_NO_REPEAT;
}

static task_status RemoveOtherAndRepeat(void *param)
{
	static int i = 0;	
	int sch_stat = 0;
	
	sch_stat = SchedulerRemoveTask((scheduler_t *)param, OTHER_UID);
	
	if (SCHED_FAIL == sch_stat)
	{
	    puts("remove other failed\n");
	    
	    return SCHED_NO_REPEAT;
	}

	++i;
	
	printf("Removed other %d time\n", i);
	
	if (3 == i)
	{
	    return SCHED_NO_REPEAT;
	}
	
	return SCHED_REPEAT;
}


static task_status RemoveItselfAndRepeat(void *param)
{
	int sch_stat = 0;
	
	puts("removed myself\n");
	assert(0 == SchedulerRemoveTask((scheduler_t *)param, SELF_UID)); 
	
    return SCHED_NO_REPEAT;
}

static task_status ClearAll(void *param)
{
	puts("Clearing\n");	

    SchedulerClear((scheduler_t *)param);
    	
	return SCHED_NO_REPEAT;
}

static task_status StopAll(void *param)
{
	puts("stops\n");
	
	SchedulerStop((scheduler_t *)param);
	
	return SCHED_NO_REPEAT;
}

static task_status TaskFuncReapeat(void *param)
{
	static size_t counter = 0;
	
	assert(NULL != param);
	
	++counter;
	
	printf("counter %ld\n", counter);
	if (4 == counter)
	{	
		return SCHED_NO_REPEAT;
	}	
	
	return SCHED_REPEAT;
		
}

static task_status TaskFuncNoReapeat(void *param)
{
	static size_t counter = 0;
	
	assert(NULL != param);
	
	counter = *(size_t *)param;
	 
	++counter;
	
	printf("static no repeat counter %lu\n", counter);
	
	return SCHED_NO_REPEAT;
}


static void SchedulerCreateDestroyTest()
{
	scheduler_t *scheduler = SchedulerCreate();
	
	assert(NULL != scheduler);
	
	assert(1 == SchedulerIsEmpty(scheduler));
	assert(0 == SchedulerSize(scheduler));
	
	SchedulerDestroy(scheduler);
}

static scheduler_t *CreateScheduler()
{
	scheduler_t *scheduler = SchedulerCreate();
	
	assert(NULL != scheduler);
	assert(1 == SchedulerIsEmpty(scheduler));
	assert(0 == SchedulerSize(scheduler));
	
	return scheduler;
}

static void AddAndRemoveTasksTest()
{
	scheduler_t *scheduler = CreateScheduler();
	int param = 0;
	time_t time_in_sec = time(NULL) + 10;
	task_uid_t uid1 = {0};
	task_uid_t uid2 = {0};
	
	assert(1 == SchedulerIsEmpty(scheduler));
	assert(0 == SchedulerSize(scheduler));
										
	uid1 = SchedulerAddTask(scheduler, &param, &TaskFuncNoReapeat, time_in_sec, 0); 
	uid2 = SchedulerAddTask(scheduler, &param, &TaskFuncReapeat, time_in_sec, 5); 

	assert(0 == SchedulerIsEmpty(scheduler));
	assert(2 == SchedulerSize(scheduler));
	
	assert(1 == TaskIsMatch(&uid1, &uid1, &param));
	assert(0 == TaskIsMatch(&uid1, &uid2, &param));
	
	assert(SCHED_SUCCESS == SchedulerRemoveTask(scheduler, uid1));
	assert(0 == SchedulerIsEmpty(scheduler));
	assert(1 == SchedulerSize(scheduler));
	
	assert(SCHED_SUCCESS == SchedulerRemoveTask(scheduler, uid2));
	assert(1 == SchedulerIsEmpty(scheduler));
	assert(0 == SchedulerSize(scheduler));
	
	assert(SCHED_FAIL == SchedulerRemoveTask(scheduler, uid2));
	
	SchedulerDestroy(scheduler);
}


static void ClearTest()
{
	scheduler_t *scheduler = CreateScheduler();
	int param = 0;
	time_t time_in_sec = time(NULL) + 10;
	task_uid_t uid1 = {0};
	task_uid_t uid2 = {0};
	
	assert(1 == SchedulerIsEmpty(scheduler));
	assert(0 == SchedulerSize(scheduler));
										
	uid1 = SchedulerAddTask(scheduler, &param, &TaskFuncNoReapeat, time_in_sec, 0); 
	uid2 = SchedulerAddTask(scheduler, &param, &TaskFuncReapeat, time_in_sec, 5); 
	uid1 = SchedulerAddTask(scheduler, &param, &TaskFuncNoReapeat, time_in_sec, 0); 
	uid2 = SchedulerAddTask(scheduler, &param, &TaskFuncReapeat, time_in_sec, 5); 
	
	assert(0 == SchedulerIsEmpty(scheduler));
	assert(4 == SchedulerSize(scheduler));
	
	SchedulerClear(scheduler);
	
	assert(1 == SchedulerIsEmpty(scheduler));
	assert(0 == SchedulerSize(scheduler));
	
	SchedulerDestroy(scheduler);
}

static void RunAndStop()
{
    scheduler_t *scheduler = CreateScheduler();

    time_t time_in_sec = time(NULL);
    size_t param = 0, i = 0;
    task_uid_t print_first = {0};
	task_uid_t task_repeat = {0};
	task_uid_t clear_all = {0};
	task_uid_t stop_all = {0};
    sched_status status = SCHED_SUCCESS;
    
	assert(NULL != scheduler);
    assert(1 == SchedulerIsEmpty(scheduler));
	assert(0 == SchedulerSize(scheduler));
	
	for(i = 0; i < 3; ++i)
	{
		time_t time_in_sec = time(NULL)+i;
		time_t interval = 3;
		
		task_repeat = SchedulerAddTask(scheduler, scheduler, &TaskFuncNoReapeat,
									   time_in_sec, interval);
		assert(!UIDIsSame(task_repeat, BAD_UID));
	}
	
    OTHER_UID = SchedulerAddTask(scheduler, &param, &TaskFuncReapeat, time(NULL)+20, 1);
	assert(!UIDIsSame(OTHER_UID, BAD_UID));
	
/*    SELF_UID = SchedulerAddTask(scheduler, NULL, &RemoveItselfAndRepeat, time(NULL)+2, 0);*/
/*	assert(!UIDIsSame(SELF_UID, BAD_UID));*/
	
	stop_all = SchedulerAddTask(scheduler, scheduler, &StopAll, time(NULL)+4, 0);
	assert(!UIDIsSame(stop_all, BAD_UID));
	
	print_first = SchedulerAddTask(scheduler, scheduler, &AddTaskBeforeAll, time(NULL)+2, 0);
	assert(!UIDIsSame(print_first, BAD_UID));
	
/*	stop_all = SchedulerAddTask(scheduler, NULL, &RemoveOtherAndRepeat, time(NULL)+2, 1);*/
/*	assert(!UIDIsSame(stop_all, BAD_UID));*/
	
	clear_all = SchedulerAddTask(scheduler, scheduler, &ClearAll, time(NULL) + 6, 0);
	assert(!UIDIsSame(clear_all, BAD_UID));
	
	SchedulerRun(scheduler);
	puts("stopped");
	SchedulerRun(scheduler);
	
	
	assert(1 == SchedulerIsEmpty(scheduler));
	assert(0 == SchedulerSize(scheduler));
	
	SchedulerDestroy(scheduler); scheduler = NULL;	
}



int main()
{
	SchedulerCreateDestroyTest();
	AddAndRemoveTasksTest();
	ClearTest();
	RunAndStop();
	
	return 0;
}
