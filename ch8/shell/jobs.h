#ifndef _JOBS_H
#define _JOBS_H

#include "csapp.h"

typedef struct Job {
    unsigned jid;
    pid_t pid;
    int completed;
    int stopped;
    int notified;
    struct Job *last;
    struct Job *next;
    char cmdline[MAXLINE];
} Job;

Job *make_job(unsigned jid, pid_t pid, char *cmdline);
void remove_job(Job *job);
Job *job_with_jid(unsigned jid);
Job *job_with_pid(pid_t pid);
int mark_process_status(pid_t pid, int status);
void update_jobs();
void wait_for_job(Job *j);

extern Job *first_job;

#endif 
