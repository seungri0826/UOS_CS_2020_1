#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

#define FALSE	0
#define TRUE	1
#define N		2

/*
** 운영체제 과제 1:	상호배제
** 학번과 이름:		2018920031 유승리
** 제출일:			2020-04-09-목
*/

DWORD WINAPI thread_func_1(LPVOID lpParam);
void enter_region(int process);
void leave_region(int process);

volatile double	shared_var = 0.0;
//double	shared_var = 0.0;
volatile int	job_complete[2] = {0, 0};
int	turn;
int	interested[N] = { FALSE, FALSE };


int main(void)
{
	DWORD dwThreadId_1, dwThrdParam_1 = 1; 
    HANDLE hThread_1; 
	int		i, j;

	// Create Thread 1
	hThread_1 = CreateThread( 
        NULL,                        // default security attributes 
        0,                           // use default stack size  
        thread_func_1,                  // thread function 
        &dwThrdParam_1,                // argument to thread function 
        0,                           // use default creation flags 
        &dwThreadId_1);                // returns the thread identifier 
 
   // Check the return value for success. 
 
    if (hThread_1 == NULL) 
    {
       printf("Thread 1 creation error\n");
 	  exit(0);
    }
    else 
    {
       CloseHandle( hThread_1 );
    }

	/* I am main thread */
	/* Now Main Thread and Thread 1 runs concurrently */
	//enter_region(0);
	for (i = 0; i < 10000; i++) {
		for (j = 0; j < 10000; j++) {
			enter_region(0);
			shared_var++;
			leave_region(0);
		}
	}
	//leave_region(0);
	printf("Main Thread completed\n");
	job_complete[0] = 1;
	while (job_complete[1] == 0) ;

	printf("%f\n", shared_var);
	printf("\n(컴퓨터과학부 2018920031 유승리)\n");
	_getch();
	ExitProcess(0);
}


DWORD WINAPI thread_func_1(LPVOID lpParam)
{
	int		i, j;
	//enter_region(1);
	for (i = 0; i < 10000; i++) {
		for (j = 0; j < 10000; j++) {
			enter_region(1);
			shared_var++;
			leave_region(1);
		}
	}
	//leave_region(1);
	printf("Thread_1 completed\n");
	job_complete[1] = 1;
	ExitThread(0);
}


void enter_region(int process)
{
	int	other;
	
	other = 1 - process;
	interested[process] = TRUE;
	turn = process;
	__asm mfence;	// 메모리 배리어
	while (turn == process && interested[other] == TRUE);
}


void leave_region(int process)
{
	interested[process] = FALSE;
}