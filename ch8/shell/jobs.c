#include "jobs.h"

Job *first_job = NULL;

Job *make_job(unsigned jid, pid_t pid, char *cmdline) {
    Job *job = malloc(sizeof(Job));

    if (!job) {
        fprintf(stderr, "unable to allocate memory for new job");
        return NULL;
    }

    job->jid = jid;
    job->pid = pid;
    job->completed = 0;
    job->stopped = 0;
    job->notified = 0;
    job->last = NULL;
    job->next = NULL;
    strcpy(job->cmdline, cmdline);

    if (!first_job) {
        first_job = job;
        return job;
    }

    /* add to list of bg jobs */
    Job *j = first_job;
    while (j->next)
        j = j->next;
    j->next = job;
    job->last = j;

    return job;
}

void remove_job(Job *job) {
    Job *j;
    for (j = first_job; j; j = j->next) {
        if (j->pid == job->pid) {
            if (!j->last && !j->next)
                first_job = NULL;
            else {
                if (!j->last)
                    first_job = j->next;
                else 
                    j->last->next = j->next;

                if (j->next)
                    j->next->last = j->last;
            }
            free(job);
            return;
        }
    }
    fprintf(stderr, "Job %d not in bg job list\n", job->pid);
}

Job *job_with_jid(unsigned jid) {
    Job *j;
    for (j = first_job; j; j = j->next)
        if (j->jid == jid)
            return j;
    return NULL;
}

Job *job_with_pid(pid_t pid) {
    Job *j;
    for (j = first_job; j; j = j->next)
        if (j->pid == pid)
            return j;
    return NULL;
}

int mark_process_status(pid_t pid, int status) {
    Job *j;

    if (pid > 0) {
        /* Update the record for the process. */
        for (j = first_job; j; j = j->next) {
            if (j->pid == pid) {
                if (WIFSTOPPED(status)) {
                    printf("job %d stopped\n", pid);
                    j->stopped = 1;
                } else {
                    j->completed = 1;
                    if (WIFSIGNALED(status))
                        fprintf(stderr, "Job %d terminated by signal: %d\n", j->pid, WTERMSIG(status));
                    else
                        printf("job %d exited normally\n", pid);
                }
                return 0;
            }
        }
        fprintf(stderr, "No child process %d\n", pid);
        return -1;
    }
    else if (pid == 0 || errno == ECHILD)
        /* No processes ready to report. */
        return -1;
    else {
        /* Other weird errors.  */
        perror("waitpid");
        return -1;
    }
}

void update_jobs() {
    pid_t pid;
    int status;

    do 
        pid = waitpid(-1, &status, WUNTRACED|WNOHANG);
    while (!mark_process_status(pid, status));

    Job *j;
    for (j = first_job; j; j = j->next) {
        if (j->completed)
            remove_job(j);
        else if (j->stopped && !j->notified)
            j->notified = 1;
    }
}


