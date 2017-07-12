#ifndef TASK_H
#define TASK_H

//License: Public Domain
//Author: elf
//EMail: elf@iamelf.com

#include "DoubleNode.h"

typedef struct structTask Task;
typedef B (* TaskHandler)(Task *pTask);
typedef void (*TaskFinalize)(Task *pTask);

struct structTask{
    DoubleNode node;
    TaskHandler Perform;
    TaskFinalize Finalize;
    B bCancel;
};

static inline void Task_Initialize(Task *pTask, TaskHandler Perform, TaskFinalize Finalize){
    pTask->Perform = Perform;
    pTask->Finalize = Finalize;
    pTask->bCancel = 0;
}

static inline void Task_Cancel(Task *pTask){
    pTask->bCancel = 1;
}

#endif // TASK_H

