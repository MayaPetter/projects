#include <assert.h> /*assert*/
#include <stdio.h> /*puts*/

#include "scheduler.h"
#include "task.h"

#define UNUSED(x) (void)(x)

/******************************************************************************/
/************forward declaration - - service functions ************************/
/******************************************************************************/
void Scheduler_Create_Destroy_test();
void Scheduler_Size_of_empty_test();
void Scheduler_IsEmpty_of_empty_test();
void Scheduler_AddTask_test();
void Scheduler_RemoveTask_test();
void Scheduler_Clear_test();
void Scheduler_Run_test();

static task_uid_t uid_for_task_rmoving = {0, 0};
static task_uid_t uid_for_task_rmoving_herself = {0, 0};

task_status Print1(void *param);
task_status Print2(void *param);
task_status Print3(void *param);
task_status stopTask(void *param);
task_status addingTask(void *param);
task_status taskAdded(void *param);
task_status removingTask(void *param);
task_status should_exec_once(void *param);
task_status taskclear(void *param);
task_status removing_myself(void *param);

/******************************************************************************/
/******************************** main ****************************************/
/******************************************************************************/
int main()
{	
	Scheduler_Create_Destroy_test();
	Scheduler_Size_of_empty_test();
	Scheduler_IsEmpty_of_empty_test();
	Scheduler_AddTask_test();
	Scheduler_RemoveTask_test();
	Scheduler_Clear_test();
	Scheduler_Run_test();
	return 0;
}

/******************************************************************************/
/**************************** test functions **********************************/
/******************************************************************************/

/**************************** Create_Destroy_test *****************************/
void Scheduler_Create_Destroy_test()
{
	scheduler_t *new_scheduler = SchedulerCreate();
	assert(NULL != new_scheduler);
	
	SchedulerDestroy(new_scheduler); new_scheduler = NULL;
}

/********************************* Size_test **********************************/
void Scheduler_Size_of_empty_test()
{
	scheduler_t *new_scheduler = SchedulerCreate();
	assert(NULL != new_scheduler);
	
	assert(0 == SchedulerSize(new_scheduler));
	
	SchedulerDestroy(new_scheduler); new_scheduler = NULL;
}

/******************************** IsEmpty_test ********************************/
void Scheduler_IsEmpty_of_empty_test()
{
	scheduler_t *new_scheduler = SchedulerCreate();
	assert(NULL != new_scheduler);
	
	assert(1 == SchedulerIsEmpty(new_scheduler));
	
	SchedulerDestroy(new_scheduler); new_scheduler = NULL;
}

/******************************** AddTask_test ********************************/
void Scheduler_AddTask_test()
{
	time_t curr_time = time(NULL);
	time_t interval = 5;
	scheduler_t *new_scheduler = SchedulerCreate();
	task_uid_t task_uid1 = {0};
	task_uid_t task_uid2 = {0};
	assert(NULL != new_scheduler);
	
	assert(0 == SchedulerSize(new_scheduler));
	assert(1 == SchedulerIsEmpty(new_scheduler));
	
	task_uid1 = SchedulerAddTask(new_scheduler, NULL, Print1, curr_time, 
																	interval);
	task_uid2 = SchedulerAddTask(new_scheduler, NULL, Print1, curr_time, 
																	interval);	
	assert(1 == UIDIsSame(task_uid1, task_uid1));
	assert(0 == UIDIsSame(task_uid1, task_uid2));
	
	assert(2 == SchedulerSize(new_scheduler));
	assert(0 == SchedulerIsEmpty(new_scheduler));
	
	SchedulerDestroy(new_scheduler); new_scheduler = NULL;
}

/****************************** RemoveTask_test *******************************/
void Scheduler_RemoveTask_test()
{
	time_t curr_time = time(NULL);
	time_t interval = 5;
	scheduler_t *new_scheduler = SchedulerCreate();
	task_uid_t task_uid1 = {0};
	task_uid_t task_uid2 = {0};
	task_uid_t no_task_in_pqueue = {0};
	assert(NULL != new_scheduler);
	
	assert(0 == SchedulerSize(new_scheduler));
	assert(1 == SchedulerIsEmpty(new_scheduler));
	
	task_uid1 = SchedulerAddTask(new_scheduler, NULL, Print1, curr_time, 
																	interval);
	task_uid2 = SchedulerAddTask(new_scheduler, NULL, Print1, curr_time, 
																	interval);	
	assert(1 == UIDIsSame(task_uid1, task_uid1));
	assert(0 == UIDIsSame(task_uid1, task_uid2));
	
	assert(2 == SchedulerSize(new_scheduler));
	assert(0 == SchedulerIsEmpty(new_scheduler));

	assert(SCHED_FAIL == SchedulerRemoveTask(new_scheduler, no_task_in_pqueue));
	assert(2 == SchedulerSize(new_scheduler));
	assert(0 == SchedulerIsEmpty(new_scheduler));

	assert(SCHED_SUCCESS == SchedulerRemoveTask(new_scheduler, task_uid1));
	assert(1 == SchedulerSize(new_scheduler));
	assert(0 == SchedulerIsEmpty(new_scheduler));

	assert(SCHED_SUCCESS == SchedulerRemoveTask(new_scheduler, task_uid2));
	assert(0 == SchedulerSize(new_scheduler));
	assert(1 == SchedulerIsEmpty(new_scheduler));
	
	SchedulerDestroy(new_scheduler); new_scheduler = NULL;
}

/******************************** Clear_test ********************************/
void Scheduler_Clear_test()
{
	time_t curr_time = time(NULL);
	time_t interval = 5;
	scheduler_t *new_scheduler = SchedulerCreate();
	task_uid_t task_uid1 = {0};
	task_uid_t task_uid2 = {0};
	assert(NULL != new_scheduler);
	
	assert(0 == SchedulerSize(new_scheduler));
	assert(1 == SchedulerIsEmpty(new_scheduler));
	
	task_uid1 = SchedulerAddTask(new_scheduler, NULL, Print1, curr_time, 
																	interval);
	task_uid2 = SchedulerAddTask(new_scheduler, NULL, Print1, curr_time, 
																	interval);	
	assert(1 == UIDIsSame(task_uid1, task_uid1));
	assert(0 == UIDIsSame(task_uid1, task_uid2));
	
	assert(2 == SchedulerSize(new_scheduler));
	assert(0 == SchedulerIsEmpty(new_scheduler));
	
	SchedulerClear(new_scheduler);
	
	assert(0 == SchedulerSize(new_scheduler));
	assert(1 == SchedulerIsEmpty(new_scheduler));
	
	SchedulerDestroy(new_scheduler); new_scheduler = NULL;
}

/******************************** AddTask_test ********************************/
void Scheduler_Run_test()
{
	time_t curr_time = time(NULL);
	time_t plus_1s_time = time(NULL) + 1;
	time_t plus_2s_time = time(NULL) + 2;
	time_t plus_5s_time = time(NULL) + 5;
	time_t plus_8s_time = time(NULL) + 8;
	
	time_t interval = 3;
	time_t interval0 = 0;
	scheduler_t *new_scheduler = SchedulerCreate();
	task_uid_t task_uid1 = {0};
	task_uid_t task_uid2 = {0};
	task_uid_t task_uid3 = {0};
	assert(NULL != new_scheduler);
	
	assert(0 == SchedulerSize(new_scheduler));
	assert(1 == SchedulerIsEmpty(new_scheduler));
	
	/*********************** TASK: PRINT 2 **************************/
	/*************** print current size and if sched empty **********/
	task_uid2 = SchedulerAddTask(new_scheduler, new_scheduler, Print2, 
												plus_1s_time, interval);
								 
	/*********************** TASK: PRINT 3 **************************/
	/*************** print current size and if sched empty **********/
	task_uid3 = SchedulerAddTask(new_scheduler, new_scheduler, Print3, 
												plus_2s_time, interval);

	/*********************** TASK: PRINT 1 **************************/
	/*************** print current size and if sched empty **********/
	task_uid1 = SchedulerAddTask(new_scheduler, new_scheduler, Print1, 
												curr_time, interval);

	/********************* TASK: ADDING TASK ************************/
	/*************** print current size and if sched empty **********/
	SchedulerAddTask(new_scheduler, new_scheduler, addingTask, plus_5s_time,
															   interval0);
															   
	/******************** TASK: REMOVING TASK ***********************/
	/*************** print current size and if sched empty **********/
	SchedulerAddTask(new_scheduler, new_scheduler, removingTask, curr_time,
															   interval0);
	
	/******************** TASK: STOPPING TASK ***********************/
	/*************** print current size and if sched empty **********/
	SchedulerAddTask(new_scheduler, new_scheduler, stopTask, plus_5s_time,
														 3);
														 
	/******************** TASK: REMOVED TASK ************************/
	/*************** print current size and if sched empty **********/
						/*exec 1 instead of 3 times*/
	uid_for_task_rmoving = SchedulerAddTask(new_scheduler, new_scheduler, 
										should_exec_once, curr_time, interval);
	
	uid_for_task_rmoving_herself = SchedulerAddTask(new_scheduler,new_scheduler, 
										removing_myself, curr_time, interval);

	
	/*TODO*/
	SchedulerAddTask(new_scheduler, new_scheduler, taskclear, plus_8s_time,
															  interval0);
	
	
	
	assert(9 == SchedulerSize(new_scheduler));
	assert(0 == SchedulerIsEmpty(new_scheduler));
								 
	assert(1 == UIDIsSame(task_uid1, task_uid1));
	assert(0 == UIDIsSame(task_uid1, task_uid2));
	assert(0 == UIDIsSame(task_uid1, task_uid3));		
	assert(0 == UIDIsSame(task_uid2, task_uid3));	
	
	/*run including stop task*/
	assert(SCHED_SUCCESS == SchedulerRun(new_scheduler)); 
	puts("**************stop_success**************");
	/*run again from task you stoped in, excluding stop task*/
	assert(SCHED_SUCCESS == SchedulerRun(new_scheduler));

	assert(0 == SchedulerSize(new_scheduler));
	assert(1 == SchedulerIsEmpty(new_scheduler));
	
	SchedulerDestroy(new_scheduler); new_scheduler = NULL;
}


/******************************************************************************/
/******************************** task_func ***********************************/
/******************************************************************************/

/******************************** TASK: PRINT 1 *******************************/
/******************* print current size and if sched empty ********************/
task_status Print1(void *param)
{
	static size_t count = 1;
	printf("num: 1 size: %lu is empty? %d\n", 
			SchedulerSize((scheduler_t *)param),
			SchedulerIsEmpty((scheduler_t *)param));
	
	if (4 == count)
	{
			return SCHED_NO_REPEAT;	
	}
	
	++count;
		
	return SCHED_REPEAT;
}

/******************************** TASK: PRINT 2 *******************************/
/******************* print current size and if sched empty ********************/
task_status Print2(void *param)
{
	static size_t count = 1;
	printf("num: 2 size: %lu is empty? %d\n", 
			SchedulerSize((scheduler_t *)param), 
			SchedulerIsEmpty((scheduler_t *)param));
	
	if (4 == count)
	{
		return SCHED_NO_REPEAT;	
	}
	
	++count;
	
	return SCHED_REPEAT;
}

/******************************** TASK: PRINT 3 *******************************/
/******************* print current size and if sched empty ********************/
task_status Print3(void *param) 
{
	static size_t count = 1;
	printf("num: 3 size: %lu is empty? %d\n", 
			SchedulerSize((scheduler_t *)param),
			SchedulerIsEmpty((scheduler_t *)param));
	
	if (4 == count)
	{
			return SCHED_NO_REPEAT;	
	}
	
	++count;
	
	return SCHED_REPEAT;
}

/****************************** TASK: STOPPING  *******************************/
/******************* print current size and if sched empty ********************/
task_status stopTask(void *param)
{
	printf("just stopping task! size: %lu is empty? %d\n",
			SchedulerSize((scheduler_t *)param),
			SchedulerIsEmpty((scheduler_t *)param));
	
	SchedulerStop(param);
	
	return SCHED_NO_REPEAT;
}

/******************************* TASK: ADDING  ********************************/
/******************* print current size and if sched empty ********************/
task_status addingTask(void *param)
{
	time_t add_time = time(NULL) + 1;
	time_t interval = 0;

	SchedulerAddTask(param, param, taskAdded, add_time, interval);

	printf("just adding task! size: %lu is empty? %d\n",
			SchedulerSize((scheduler_t *)param),
			SchedulerIsEmpty((scheduler_t *)param));
	
	return SCHED_NO_REPEAT;
}

/***************************** TASK: ADDED TASK  ******************************/
/******************* print current size and if sched empty ********************/
task_status taskAdded(void *param)
{
	printf("added task. size: %lu is empty? %d\n",
			SchedulerSize((scheduler_t *)param),
			SchedulerIsEmpty((scheduler_t *)param));
	
	return SCHED_NO_REPEAT;
}

/****************************** TASK: REMOVING  *******************************/
/******************* print current size and if sched empty ********************/
task_status removingTask(void *param)
{
	printf("just removing task! size: %lu is empty? %d\n",
			SchedulerSize((scheduler_t *)param),
			SchedulerIsEmpty((scheduler_t *)param));
	
	SchedulerRemoveTask((scheduler_t *)param, uid_for_task_rmoving);
	
	return SCHED_NO_REPEAT;
}

/******************** TASK: TO BE REMOVED AFTER ONE EXEC  *********************/
/******************* print current size and if sched empty ********************/
task_status should_exec_once(void *param)
{		
	printf("!!!!!!!!!!!!!!!!!!!!!!should_exec_just_once!!!!!!!!!!!!!!!!!!!");	
	
	printf("size: %lu is empty? %d\n",
			SchedulerSize((scheduler_t *)param),
			SchedulerIsEmpty((scheduler_t *)param));
		
	return SCHED_REPEAT;
}

task_status taskclear(void *param) 
{
	printf("clear task: befor clear. size: %lu is empty? %d\n",
			SchedulerSize((scheduler_t *)param),
			SchedulerIsEmpty((scheduler_t *)param));
	
	SchedulerClear((scheduler_t *)param);

	printf("clear task: after clear. size: %lu is empty? %d\n",
			SchedulerSize((scheduler_t *)param),
			SchedulerIsEmpty((scheduler_t *)param));
		
	return SCHED_REPEAT;
}


/************************* TASK: REMOVING HERSELF  ****************************/
/******************* print current size and if sched empty ********************/
task_status removing_myself(void *param)
{		
	printf("!*!*!*!*!*!      i'm_removinf_myself        !*!*!*!*!*!");	
	
	printf("size: %lu is empty? %d\n",
			SchedulerSize((scheduler_t *)param),
			SchedulerIsEmpty((scheduler_t *)param));
	
	SchedulerRemoveTask((scheduler_t *)param, uid_for_task_rmoving_herself);
	
	return SCHED_REPEAT;
}
