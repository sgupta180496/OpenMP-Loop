#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<chrono>
#include<string.h>

using namespace std;

#ifdef __cplusplus
extern "C" 
{
	#endif

	float f1(float x, int intensity);
	float f2(float x, int intensity);
	float f3(float x, int intensity);
	float f4(float x, int intensity);
	
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

  	if (argc < 9) 
	{
		std::cerr<<"Usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <scheduling> <granularity>"<<std::endl;
    		return -1;
  	}
	//variables needed
	int functionid=atoi(argv[1]);
	float a=atof(argv[2]);
	float b=atof(argv[3]);
	int n=atoi(argv[4]);
	int intensity=atoi(argv[5]);
	int nbthreads=atoi(argv[6]);
	char* sync=argv[7];
	int granularity=atoi(argv[8]);
	float res=0;
	omp_set_num_threads(nbthreads);

	//using two schedules, static and dynamic

	if(strcmp(sync,"static")==0)
	{
		omp_set_schedule(omp_sched_static,granularity);
	}
	else if(strcmp(sync,"dynamic")==0)	
	{
		omp_set_schedule(omp_sched_dynamic,granularity);
	}
	else if(strcmp(sync,"guided")==0)
	{
    		omp_set_schedule(omp_sched_guided, granularity);
  	}

	//start calculation of time
	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
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

		#pragma omp for reduction (+ : res) schedule (runtime)
		for(int i=0;i<n;i++)
		{
			float x=(a+(i+.5)*((b-a)/n));
			switch(functionid)
			{
				case 1: 
				res +=(f1(x, intensity));
				break;
				case 2: 
				res +=(f2(x, intensity));
				break;
				case 3: 
				res +=(f3(x, intensity));
				break;
				case 4: 
				res +=(f4(x, intensity));
				break;
				default: std::cout<<"\nWrong functionid";
			}
		}
	}
	// end calculation of time
	std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
	std::chrono::duration<double> time = end-start;
	std::cout<<(res*((b-a)/n))<<std::endl;
	std::cerr<<time.count()<<std::endl;
  	return 0;
}
