#include "builtins.h"
#include "csapp.h"
#include "jobs.h"

#define MAXARGS 128
#define MAX_BG_JOBS 69105

void eval(char *cmdline);
int parseline(char *buf, int *argc, char **argv);
void sigint_handler(int sig);
static int keyboard_terminate = -1;
static jmp_buf env;
static pid_t cpid;

extern Job *first_job;

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
    }
}

void sigint_handler(int sig) {
    keyboard_terminate = sig;
    siglongjmp(env, 1);
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
        return;   /* Ignore empty lines */

    if (!builtin_command(argc, argv)) {        /* if built in, execute directly */
        // create job here (with invalid jid and pid) so that the pointer is
        // valid if signal handler runs

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

        if (sigsetjmp(env, 1) == 1) {
            if(!bg && !kill(-cpid, 0))
                Kill(-cpid, keyboard_terminate);
        }

        /* Parent waits for foreground job to terminate */
        if (!bg) {
            int status;
            if (waitpid(cpid, &status, WUNTRACED) > 0) {
                if (WIFSIGNALED(status)) {
                    remove_job(j);
                    printf(" Job %d terminated by signal: %s\n", cpid, strsignal(WTERMSIG(status)));
                } else if (WIFSTOPPED(status)) {
                    j->stopped = 1;
                    printf(" Job %d stopped by signal: %d\n", cpid, WSTOPSIG(status));
                } else if (WIFEXITED(status)) {
                    remove_job(j);
                    //printf("Job %d exited with status %d\n", cpid, WEXITSTATUS(status));
                } else {
                    remove_job(j);
                    printf("Job %d did not exit with a valid status\n", cpid);
                }
            }
            else
                unix_error("waitfg: waitpid error");
        } else {
            printf("[%d] %d   %s", j->jid, j->pid, j->cmdline);
        }

        Signal(SIGINT, SIG_IGN);
        Signal(SIGTSTP, SIG_IGN);
    }
    return;
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



