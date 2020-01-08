#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <omp.h>
#include <chrono>

using namespace std;

#ifdef __cplusplus
extern "C" 
{
	#endif
	void generateMergeSortData (int* arr, size_t n);
	void checkMergeSortResult (int* arr, size_t n);
	#ifdef __cplusplus
}
#endif

void merge(int* arr, int l, int m, int r);
void mergeSort(int* arr, int l, int r, int num);

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

  	if (argc < 3) 
	{ 
		std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
	    	return -1;
	}

  	int n=atoi(argv[1]);
  	int num=atoi(argv[2]);
  	int * arr = new int [n];
	// condition to check if the array is a valid one
	if(arr==NULL)
	{
		std::cout<<"Array allocation failed"<<std::endl;
		return 0;
	}
  	generateMergeSortData(arr,n);


  	// start calculating time
  	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
	
	// call mergesort function to begin sorting
  	mergeSort(arr,0,n-1,num);

  	// end calculating time
  	std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  	std::chrono::duration<double> elapased_seconds = end-start;
  	cerr<<elapased_seconds.count()<<std::endl;
  	checkMergeSortResult(arr,n);
	delete[] arr;
	return 0;
}

void merge(int* arr, int left, int mid, int right)
{
	int i,j,k;
    	int left_sub=mid-left+1;
    	int right_sub=right-mid;

    	// temporary arrays for storing left and right sub array
    	int* Left_arr=new int[left_sub];
    	int* Right_arr=new int[right_sub];

    	// Copy left and right subarrays in temporary arrays
    	for (i=0;i<left_sub;i++)
        	Left_arr[i]=arr[left+i];
    	for (j=0;j<right_sub;j++)
        	Right_arr[j]=arr[mid+1+j];

    	i=0; 
    	j=0; 
    	k=left; 
	//Merging left and right sub arrays after comparing first elements
    	while (i<left_sub && j<right_sub)
    	{
		if (Left_arr[i]<=Right_arr[j])
    	    	{
    	        	arr[k]=Left_arr[i];
    	        	i++;
    	    	}
    	    	else
   	    	{
            		arr[k]=Right_arr[j];
            		j++;
            	}
            	k++;
    	}

    	//Copy the remaining elements of Left array
    	while (i<left_sub)
    	{
    	    	arr[k]=Left_arr[i];
        	i++;
        	k++;
    	}

       //Copy the remaining elements of Right array
    	while (j<right_sub)
    	{
        	arr[k]=Right_arr[j];
        	j++;
        	k++;
    	}
    	delete[] Left_arr;
    	delete[] Right_arr;
}

void mergeSort(int* arr,int left,int right,int num)
{
   	//Set the number of threads
    	omp_set_num_threads(num);
    	int n=right+1;

    	for(int x=1;x<n;x *= 2)
    	{
        	#pragma omp parallel for schedule(static, 1)
        	for(int i=0;i<n;i += (2*x))
        	{
            		int start=i;
            		int mid=i+(x-1);
            		int end=i+((2*x)-1);
            		if(mid>=n)
            		{
                		mid=(i+n-1)/2;
                		end=n-1;
            		}
            		else if(end>=n)
            		{
                		end=n-1;
            		}
			// call merge function
            		merge(arr,start,mid,end);
        	}
    	}
}




