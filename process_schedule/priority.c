//Lower the value higher the priority
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 20

struct Process {
	int pid;
	int at;
	int bt;
	int ct;
	int tat;
	int wt;
	int priority;
	bool completed;
};

// insertion sort
void sort(struct Process process[],int size) {
    for(int j = 1; j < size; j++){
        struct Process key = process[j];
        int i = j-1;
        while(i > -1 && process[i].at > key.at) {
            process[i+1] = process[i];
            i--;
        }
        process[i+1] = key;
    }
}
 
int main() {
	struct Process process[MAX];
	int no_of_process;
	float avg_wt,avg_tat;
 
	printf("Enter the number of processes\n");
	scanf("%d",&no_of_process);
 
	for(int i = 0; i < no_of_process; i++) {
		process[i].pid = i+1;
        process[i].completed = false;
	}
 
	printf("Enter the arrival time, burst time and priority for processes\n");
	for(int i = 0; i < no_of_process; i++) {
		scanf("%d",&process[i].at);	
		scanf("%d",&process[i].bt);	
		scanf("%d",&process[i].priority);	
	}
 
	sort(process,no_of_process);

	int current_time = 0;
	int completed = 0;

    int log_index = 0;
    int execution_log[1000];

    while(completed < no_of_process) {
       int idx = -1; 
       int max_priority = 999999; //lower the value higher the priority
       for(int i = 0; i < no_of_process; i++) {
            if(process[i].at <= current_time && 
                    process[i].completed == false) {
               if(process[i].priority < max_priority) {
                    max_priority = process[i].priority;
                    idx = i;
               }
            }
       }
       if(idx != -1) {
            process[idx].ct = process[idx].bt + current_time;
            process[idx].tat = process[idx].ct - process[idx].at;
            process[idx].wt = process[idx].tat - process[idx].bt;
            process[idx].completed = true;
            current_time += process[idx].bt;
            completed++;
            //for gantt chart
            for(int i = 0; i<process[idx].bt; i++){
                execution_log[log_index++] = process[idx].pid;
            }
       }
       else {
            current_time++;
       }

    }

	int sum_tat = 0;
	int sum_wt = 0;
	for(int i = 0; i < no_of_process;i++) {
		sum_tat += process[i].tat;
		sum_wt += process[i].wt; 
	}
	avg_tat = (float)sum_tat/no_of_process;
	avg_wt = (float)sum_wt/no_of_process;
 
	printf("Pid\tat\tbt\tct\ttat\twt");
	for(int i=0;i<no_of_process;i++){
		printf("\n%d\t%d\t%d\t%d\t%d\t%d",
                process[i].pid,process[i].at,process[i].bt,process[i].ct,process[i].tat,process[i].wt);
	}
	printf("\n\naverage turnaround time = %.2f",avg_tat);
	printf("\naverage waiting time = %.2f",avg_wt);

    //printing gantt chart
    printf("\nGantt Chart\n");
    for(int i = 0;i < log_index;i++) {
        printf("P%d|",execution_log[i]);
    }
    printf("\n");
    return 0;
}
