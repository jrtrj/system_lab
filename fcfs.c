#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
struct process {
	int pid;
	int at;
	int bt;
	int ct;
	int tat;
	int wt;
	bool completed;
};

void sort(struct process *p,int size) {
	struct process temp;
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size-i-1; j++) {
			if(p[j].at > p[j+1].at){
				temp = p[j];	
				p[j] = p[j+1];	
				p[j+1] = temp;
			}
		}
	}
}
 
int main() {
	struct process *p;
	int no_of_process;
	float avg_wt,avg_tat;
 
	printf("Enter the number of processes\n");
	scanf("%d",&no_of_process);
 
	p = (struct process *)malloc(no_of_process * sizeof(struct process));
    if(p == NULL){
        printf("memory allocation failed\n");
        return 1;
    }

	for(int i = 0; i < no_of_process; i++) {
		p[i].pid = i+1;
        p[i].completed = false;
	}
 
	printf("Enter the arrival time for processes\n");
	for(int i = 0; i < no_of_process; i++) {
		scanf("%d",&p[i].at);	
	}
 
	printf("Enter the burst time for processes\n");
	for(int i = 0; i < no_of_process; i++) {
		scanf("%d",&p[i].bt);	
	}
 
	sort(p,no_of_process);
 
	//completion time
	p[0].ct = p[0].at + p[0].bt;
	for(int i = 1; i < no_of_process;i++) {
        int start_time = (p[i-1].ct < p[i].at) ? p[i].at:p[i-1].ct;
		p[i].ct = p[i].bt + start_time;
	}
 
	//tat
	for(int i = 0; i < no_of_process;i++) {
		p[i].tat = p[i].ct - p[i].at;
	}
 
	//waiting time
	for(int i = 0; i < no_of_process;i++) {
		p[i].wt = p[i].tat - p[i].bt;
	}
 
	int sum_tat = 0;
	int sum_wt = 0;
	for(int i = 0; i < no_of_process;i++) {
		sum_tat += p[i].tat;
		sum_wt += p[i].wt; 
	}
	avg_tat = (float)sum_tat/no_of_process;
	avg_wt = (float)sum_wt/no_of_process;
 
	printf("Pid\tat\tbt\tct\ttat\twt");
	for(int i=0;i<no_of_process;i++){
		printf("\n%d\t%d\t%d\t%d\t%d\t%d",p[i].pid,p[i].at,p[i].bt,p[i].ct,p[i].tat,p[i].wt);
	}
	printf("\n\naverage turnaround time = %f",avg_tat);
	printf("\naverage waiting time = %f",avg_wt);
    free(p);
    p = NULL;
}
 
