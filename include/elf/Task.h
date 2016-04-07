#ifndef TASK_H
#define TASK_H

//License: Public Domain
//Author: elf
//EMail: elf@elf0.org

#include "DoubleNode.h"

typedef struct structTask Task;
typedef Bool (* TaskHandler)(Task *pTask);
typedef void (*TaskFinalize)(Task *pTask);

struct structTask{
    DoubleNode node;
    TaskHandler Perform;
    TaskFinalize Finalize;
};

static inline void Task_Initialize(Task *pTask,  TaskHandler Perform, TaskFinalize Finalize){
    pTask->Perform = Perform;
    pTask->Finalize = Finalize;
}

#endif // TASK_H

