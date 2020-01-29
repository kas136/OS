#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

void app_play(char *alp);
void createNode();
void vruntime_cal(float clock_end);
void list_sort();
void timer_handler();
void cont_handler();
void stop_handler();

typedef struct _list {
    struct _node *head;
    struct _node *tail;
} linkedList;

typedef struct _node {
    char *data;
    pid_t check_pid;
    float vruntime;
    float nice_cal;
    float deltaexec;
    struct _node *next;
} node;

int n[5];
int ts;
int status;
int ts_num;
int node_num;
int i;
int j;
int temp;
int total;
char tdata;
clock_t start;
pid_t pid;
struct sigaction sa;
struct itimerval settimer;
float nice_cal[5] = {0.64, 0.8, 1, 1.25, 1.5625};
char *alp_data[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};
linkedList *L;
