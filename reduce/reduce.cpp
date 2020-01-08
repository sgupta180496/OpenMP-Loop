#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>
#include<string.h>

using namespace std;

#ifdef __cplusplus
extern "C" 
{
	#endif
	void generateReduceData (int* arr, size_t n);
	#ifdef __cplusplus
}
#endif

int main (int argc, char* argv[]) 
{
	//forces openmp to create the threads beforehand
	#pragma omp parallel
	{
		int fd = open (argv[0], O_RDONLY);
		if (fd != -1) 
		{
			close (fd);
		}
		else 
		{
			std::cerr<<"something is amiss"<<std::endl;
		}
	}

	if (argc < 5) 
	{
		std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads> <scheduling> <granularity>"<<std::endl;
		return -1;
	}
	//input variables needed
	int n = atoi(argv[1]);
	int * arr = new int [n];
	// condition to check if the array is a valid one
	if(arr==NULL)
	{
		std::cout<<"Array allocation failed"<<std::endl;
		return 0;
	}
	generateReduceData (arr, n);
	int nbthreads = atoi(argv[2]);
	char* scheduling = argv[3];
	int granularity = atoi(argv[4]);
	int res = 0;
	// setting number of threads
	omp_set_num_threads(nbthreads);

	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

	if(strcmp(scheduling , "static") == 0)
  	{
    		if (granularity<0)
    		{
       			#pragma omp parallel for schedule(static) reduction(+:res)
        		for (int i = 0; i < n; ++i)
       	 		{
          			res+=arr[i];
        		}  
    		}
    		else
    		{
      			#pragma omp parallel for schedule(static,granularity) reduction(+:res)
      			for (int i = 0; i < n; ++i)
      			{
        			res+=arr[i];
      			}
    		}  
  	}
  	else if(strcmp(scheduling, "dynamic") == 0)
  	{
    		if (granularity<0)
    		{  
     			#pragma omp parallel for schedule(dynamic) reduction(+:res)
     			for (int i = 0; i < n; ++i)
     			{
       				res+=arr[i];
     			}
    		}
    		else
    		{
      			#pragma omp parallel for schedule(dynamic,granularity) reduction(+:res)
      			for (int i = 0; i < n; ++i)
      			{
       				res+=arr[i];
      			}
    		} 
  	}
	// end calculation of time
	std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  	std::chrono::duration<double> elapased_seconds = end-start;
  	cerr<<elapased_seconds.count()<<std::endl;
	std::cout<<res<<std::endl;
	delete[] arr;
	return 0;
}
