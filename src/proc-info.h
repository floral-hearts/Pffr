#ifndef PROC__INFO_H
#define PROC__INFO_H

typedef struct tagProcInfo {
    char title[255];
    char author[255];
} ProcInfo;

ProcInfo setDefaultProcInfo(void);

#endif
