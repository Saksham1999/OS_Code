#include <bits/stdc++.h>
using namespace std;

struct Process
{
	int arrival_time; //Process Arrival Time
	int burst_time; //Process Burst Time
	int Priority; //Process Priority
	int finish_time; //Process Finish Time
	int remaining_time; //Process Remaining  Time During Execution
	int waiting_time; //Waiting Time
	int start_time; //Process start Time
	int response_time;// Process response time
	int Num;
	int Pid;  //Process Id

};

struct Process current;

bool idsort(const Process& x , const Process& y)
{
	return x.Pid < y.Pid;
}
//Sorting on the base of arrival time if they are same then on Priority basis if Priority also  matches than on the basis of process id
bool arrivalsort( const Process& x ,const Process& y)
{
	if(x.arrival_time < y.arrival_time)
		return true;
	else if(x.arrival_time > y.arrival_time)
		return false;
	if(x.Priority < y.Priority)
		return true;
	else if(x.Priority > y.Priority)
		return false;
	if(x.Pid < y.Pid)
		return true;

	return false;
}


struct compare
{
	bool operator()(const Process& x ,const Process& y)
	{
		if( x.Priority > y.Priority )
			return true;
		else if( x.Priority < y.Priority )
			return false;
		if( x.Pid > y.Pid )
			return true;

		return false;
		
	}
	
};


int main()
{
	int i;
	vector< Process > input;
	Process temp;
	int priority_queue_process = 0; // for Priority queue process
	int ready_queue_process = 0; // for Ready queue process
	int arrival_time;
	int burst_time;
	int Pid;
	int Priority;
	int n;
	int clock;
	int total_exection_time = 0;
	int Pid_check=0;
	int c;
	cout<<"Enter number of processes:\t";
	cin>>n;
	if(n<=0) // If number of process is 0 or less thn 0 then exit from the program
	{
		cout<<"There must be at least one process to execute";
	exit(0);
    }   
	cout<<"Enter in the format:\n";
	cout<<"Pid"<<" "<<"Arrival_Time"<<" "<<"Burst_Time"<<" "<<"Priority:\n";
	for( i= 0; i< n; i++ )
	{
		cin>>Pid>>arrival_time>>burst_time>>Priority;
		if(Pid<=0) //if Pid is less 1
		{
			cout<<"Pid cannot be less than 1";
		exit(0);
		}
		else if(arrival_time<0) //if arrival time is less than 0
		{
			cout<<"Arrival time cannot be negative";
		exit(0);
		}
		else if(burst_time<1) //if burst time is less than 1
		{
			cout<<"Burst time cannot be less than 1";
		exit(0);
		}
		else if(Priority<0) //if priority is less than 0
		{
			cout<<"Priority cannot be negative";
		exit(0);
		}
		temp.Num = i+1;
		temp.arrival_time = arrival_time;
		temp.burst_time = burst_time;
		temp.remaining_time = burst_time;
		temp.Pid = Pid;
		temp.Priority = Priority;
		input.push_back(temp);
		for(c=0;c<=i;c++)
		{
			if(Pid==input[c].Pid)
			Pid_check++;
		}
		if(Pid_check>1) //Check if two Pid's are same then exit from the program
		{
			cout<<"Two process Pid's cannot be same";
		exit(0);
	    }
		Pid_check=0;
	}
	
	sort( input.begin(), input.end(), arrivalsort ); //Arrange the process as per their arrival time and if that is same then on priority basis
    total_exection_time = total_exection_time + input[0].arrival_time;
    for( i= 0 ;i< n; i++ ) // Finding the total execution time
    {
    	if( total_exection_time >= input[i].arrival_time ) // If CPU is not idle then just add the burst time
    	{
    		total_exection_time = total_exection_time +input[i].burst_time;
    	}
    	else
    	{
    		int diff = (input[i].arrival_time - total_exection_time); // If CPU is idle and process arrives later then we need to add the difference
    		total_exection_time = total_exection_time + diff + burst_time;

    	}
    }
	int Gantt[total_exection_time]={-1};//creating an array of size total execution time with an intial value of -1

	priority_queue < Process ,vector<Process> ,compare> priority_queue; //Priority Queue priority_queue
	
	queue< Process > ready_queue; //Round Robin Queue ready_queue
	int cpu_state = 0; // if 0 then Idle if 1 then Busy
	int quantum = 4 ; //Time Quantum
	current.Pid = -5;
	current.Priority = 999999;

	for ( clock = 0; clock< total_exection_time; clock++ )
	{
		//Insert the process with same Arrival time in Priority Queue
		for( int j = 0; j< n ; j++ )
		{
			if(clock == input[j].arrival_time)
			{
				priority_queue.push(input[j]);
			}
		}

		if(cpu_state == 0) //If CPU is idle
		{
			if(!priority_queue.empty())
			{
				current = priority_queue.top();
				cpu_state = 1;
				priority_queue_process = 1;
				priority_queue.pop();
				quantum = 4; 
			}
			else if(!ready_queue.empty())
			{
				current = ready_queue.front();
				cpu_state = 1;
				ready_queue_process = 1;
				ready_queue.pop();
				quantum = 4;
			}
		}
		else if(cpu_state == 1) //If cpu has any procss
		{
			if(priority_queue_process == 1 && (!priority_queue.empty()))
			{
				if(priority_queue.top().Priority < current.Priority ) //If new process has high priority
				{
					ready_queue.push(current); //push current in ready_queue
					current = priority_queue.top();
					priority_queue.pop();
					quantum = 4; 
				}
			}
			else if(ready_queue_process == 1 && (!priority_queue.empty())) //If process is from ready_queue and new process come  in priority_queue
			{
				ready_queue.push(current);
				current = priority_queue.top();
				priority_queue.pop();
				ready_queue_process = 0;
				priority_queue_process = 1;
				quantum = 4 ;
			}
			

		}


		if(current.Pid != -5) // Process Execution
		{
			current.remaining_time--;
			quantum--;
			Gantt[clock] = current.Pid;
			if(current.remaining_time == 0) //If process Finish
			{
				cpu_state = 0 ;
				quantum = 4 ;
				current.Pid = -5;
				current.Priority = 999999;
				ready_queue_process = 0;
				priority_queue_process = 0;
			}
			else if(quantum == 0 ) //If time Qunatum of a current running process Finish
			{
				ready_queue.push(current);
				current.Pid = -5;
				current.Priority = 999999;
				ready_queue_process = 0;
				priority_queue_process = 0;
				cpu_state=0;

			}

		}
		
	}


	sort( input.begin(), input.end(), idsort );
	
	for(int i=0;i<n;i++)
	{
		for(int k=total_exection_time;k>=0;k--)
		{
			if(Gantt[k]==i+1)
			{
				input[i].finish_time=k+1;
				break;

			}
		}
	}
	for(int i=0;i<n;i++)
	{
		for(int k=0;k<total_exection_time;k++)
		{

			if(Gantt[k]==i+1)
			{
				input[i].start_time=k;
				break;
			}
		}
	}
	

	for(int i=0;i<n;i++)
	{
		input[i].response_time=input[i].start_time-input[i].arrival_time; // Finding response time of every process
		input[i].waiting_time=(input[i].finish_time-input[i].arrival_time)-input[i].burst_time; // Finding waiting time of every process

	}
	cout<<"--------------------------------------------------------------------------------------------------------\n";
	cout<<"--------------------------------------------------------------------------------------------------------\n";

	
	cout<<"Pid"<<"\t"<<"Response Time"<<"\t"<<"Finish time"<<"\t"<<"Waiting Time"<<endl;
	for(int i=0;i<n;i++)
	{
		cout<<input[i].Pid<<"\t"<<"    "<<input[i].response_time<<"\t\t"<<"   "<<input[i].finish_time<<"\t\t"<<"   "<<input[i].waiting_time<<endl;
		
	}	
	return 0;
}
