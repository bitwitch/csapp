#include "builtins.h"
#include "csapp.h"
#include "jobs.h"

#define MAXARGS 128
#define MAX_BG_JOBS 69105

void eval(char *cmdline);
int parseline(char *buf, int *argc, char **argv);
void sigint_handler(int sig);

static pid_t cpid;

int main()
{
    Signal(SIGINT, SIG_IGN);
    Signal(SIGTSTP, SIG_IGN);

    char cmdline[MAXLINE];

    printf("Parent pid: %d\n", getpid());

    while (1) {
        /* Read */
        printf("> ");
        Fgets(cmdline, MAXLINE, stdin);
        if (feof(stdin))
            exit(0);

        /* Evaluate */
        eval(cmdline);

        /* Update job statuses */
        update_jobs();
    }
}

void sigint_handler(int sig) {
    if(!kill(-cpid, 0))
        Kill(-cpid, sig);
}

/* eval - Evaluate a command line */
void eval(char *cmdline)
{
    int argc;            /* argument count */
    char *argv[MAXARGS]; /* Argument list execve() */
    char modified_cmdline[MAXLINE];   /* Holds modified command line */
    int bg;              /* Should the job run in bg or fg? */

    static unsigned jid = 0; /* job id */

    strcpy(modified_cmdline, cmdline);

    bg = parseline(modified_cmdline, &argc, argv);
    if (argv[0] == NULL)
        return;  /* Ignore empty lines */

    /* if built in, execute directly */
    if (builtin_command(argc, argv))
        return;

    Signal(SIGINT, sigint_handler);
    Signal(SIGTSTP, sigint_handler);

    jid++;

    if ((cpid = Fork()) == 0) {   /* Child runs user job */
        setpgid(getpid(), 0);
        if (execve(argv[0], argv, environ) < 0) {
            printf("%s: Command not found.\n", argv[0]);
            exit(0);
        }
    } 

    Job *j = make_job(jid, cpid, cmdline); 

    /* Parent waits for foreground job to terminate */
    if (!bg)
        wait_for_job(j);
    else
        printf("[%d] %d   %s", j->jid, j->pid, j->cmdline);

    Signal(SIGINT, SIG_IGN);
    Signal(SIGTSTP, SIG_IGN);

    return;
}

void wait_for_job(Job *j) {
    int status;
    if (waitpid(j->pid, &status, WUNTRACED) > 0) {
        if (WIFSTOPPED(status)) {
            j->stopped = 1;
            printf(" Job %d stopped by signal: %d\n", j->pid, WSTOPSIG(status));
        } else {
            remove_job(j);
            if (WIFSIGNALED(status))
                printf(" Job %d terminated by signal: %s\n", j->pid, strsignal(WTERMSIG(status)));
            else if (!WIFEXITED(status))
                printf(" Job %d did not exit with a valid status\n", j->pid);
        }
    }
    else
        unix_error("waitfg: waitpid error");
}
   
/* parseline - Parse the command line and build the argv array */
int parseline(char *buf, int *arg_count, char **argv)
{
    char *delim;         /* Points to first space delimiter */
    int argc;            /* Number of args */
    int bg;              /* Background job? */

    buf[strlen(buf)-1] = ' ';  /* Replace trailing '\n' with space */
    while (*buf && (*buf == ' ')) /* Ignore leading spaces */
        buf++;

    /* Build the argv list */
    argc = 0;
    while ((delim = strchr(buf, ' '))) {
        argv[argc++] = buf;
        *delim = '\0';
        buf = delim + 1;
        while (*buf && (*buf == ' ')) /* Ignore spaces */
                buf++;
    }
    argv[argc] = NULL;

    if (argc == 0)  /* Ignore blank line */
        return 1;

    /* Should the job run in the background? */
    if ((bg = (*argv[argc-1] == '&')) != 0)
        argv[--argc] = NULL;

    *arg_count = argc;
    return bg;
}

