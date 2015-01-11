#include <stdio.h>
#include <stdlib.h>
#include "elf/WorkerThread.h"

typedef struct{
    Task task;
    U32 nTimes;
    char c;
}MyTask;

static Bool Task_onRun(MyTask *pTask){
    printf("Got: %c\n", pTask->c);
    return !--pTask->nTimes;
}

static void Task_onFinalize(MyTask *pTask){
    free(pTask);
    printf("Free: %p\n", pTask);
}

int main(int argc, char *argv[]){
 WorkerThread thread;
 WorkerThread_Initialize(&thread);
 WorkerThread_Run(&thread);
 //List postingTasks;
 //List_Initialize(&postingTasks);
 int c;
 while((c = getchar()) != 'q'){
     MyTask *pTask = (MyTask*)malloc(sizeof(MyTask));
     printf("Alloc: %p\n", pTask);
     Task_Initialize((Task*)pTask, (TaskEntry)Task_onRun, (TaskFinalize)Task_onFinalize);
     //List_Push(&postingTasks, (Task*)pTask);
     pTask->nTimes = 2;
     pTask->c = c;
     WorkerThread_Post(&thread, (Task*)pTask);
     //WorkerThread_PostTasks(&thread, &postingTasks);
 }
 WorkerThread_Finalize(&thread);
 return 0;
}
