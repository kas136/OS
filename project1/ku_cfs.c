#include "ku_cfs.h"

int main(int argc, char *argv[])
{
    ts_num = 0;

    for(i = 0; i < 5; i++)
    {
	n[i] = atoi(argv[i+1]);
	total += n[i];
    }

    ts = atoi(argv[6]);

    L = (linkedList *)malloc(sizeof(linkedList));
    L->head = NULL;
    L->tail = NULL;

    createNode();

    sleep(3);

    timer_handler();

    while(ts_num != ts)
    {
	cont_handler();
	pause();
	float clock_end = (float)(clock() - start)/CLOCKS_PER_SEC;
	vruntime_cal(clock_end);
	clock_end = 0;
	list_sort();
	ts_num++;
    }
}

void app_play(char *alp)
{
    char *app_arr[3];
    app_arr[0] = "./ku_app";
    app_arr[1] = alp;
    app_arr[2] = NULL;

    execvp("./ku_app", app_arr);
}

void createNode()
{
    temp = 0;

    for(i = 0; i < 5; i++)
    {
	for(node_num = 0; node_num < n[i]; node_num++)
	{
	    node *newNode = (node *)malloc(sizeof(node));
	    newNode->data = alp_data[temp];
	    newNode->nice_cal = nice_cal[i];
	    newNode->deltaexec = 0;
	    newNode->vruntime = 0;;
	    pid = fork();
	    
	    if(pid == 0)
	    {
		app_play(alp_data[temp]);
	    }

	    else
	    {
		newNode->check_pid = pid;
	    }

	    temp++;

	    newNode->next = NULL;

	    if(L->head == NULL && L->tail == NULL)
		L->head = L->tail = newNode;

	    else
	    {
		L->tail->next = newNode;
		L->tail = newNode;
	    }
	}
    }
}

void vruntime_cal(float clock_end)
{
    L->head->deltaexec = L->head->deltaexec + clock_end;
    L->head->vruntime = L->head->vruntime + (L->head->deltaexec * L->head->nice_cal);
}

void list_sort()
{
    node *curNode = (node *)malloc(sizeof(node));
    node *curnextNode = (node *)malloc(sizeof(node));
    node *changeNode = (node *)malloc(sizeof(node));

    for(i = 0; i < total; i++)
    {
	curNode = L->head;

	for(j = 0; j < total-1; j++)
	{
	    if(curNode->vruntime > curNode->next->vruntime)
	    {
		curnextNode = curNode->next;
		curNode->next = curnextNode->next;
		curnextNode->next = curNode;

		if(curNode == L->head)
		{
		    L->head = curnextNode;
		    curNode = curnextNode;
		}
		
		else
		{
		    curNode = curnextNode;
		    changeNode->next = curnextNode;
		}
	    }

	changeNode = curNode;
	curNode = curNode->next;
	}
    }
}

void timer_handler()
{
    memset(&sa, 0, sizeof(sa));

    sa.sa_handler = &stop_handler;
    sigaction(SIGALRM, &sa, NULL);

    settimer.it_value.tv_sec = 1;
    settimer.it_value.tv_usec = 0;

    settimer.it_interval.tv_sec = 1;
    settimer.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &settimer, NULL);

    kill(getppid(), SIGCONT);
}

void cont_handler()
{
    pid_t cont_number = L->head->check_pid;
    kill(cont_number, SIGCONT);
    start = clock();
}

void stop_handler()
{
    pid_t stop_number = L->head->check_pid;
    kill(stop_number, SIGSTOP);
}
