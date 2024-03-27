#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <math.h>
#include <bits/stdc++.h>
#include <vector>
#include <math.h>
using namespace std;

// Merges two subarrays of array[].
// First  subarray is arr[begin..mid]
// Second subarray is arr[mid+1..end]
void merge(std::vector<int>& array, int const left, int const mid,
		int const right)
{
	int const subArrayOne = mid - left + 1;
	int const subArrayTwo = right - mid;

	// Create temp vectors
	std::vector<int> leftArray(subArrayOne);
	std::vector<int> rightArray(subArrayTwo);

	// Uncomment this for an array version
	// Create temp arrays
	//auto *leftArray = new int[subArrayOne],
	//	*rightArray = new int[subArrayTwo];

	// Copy data to temp arrays leftArray[] and rightArray[]
	for (auto i = 0; i < subArrayOne; i++)
		leftArray[i] = array[left + i];
	for (auto j = 0; j < subArrayTwo; j++)
		rightArray[j] = array[mid + 1 + j];

	auto indexOfSubArrayOne = 0, indexOfSubArrayTwo = 0;
	int indexOfMergedArray = left;

	// Merge the temp arrays back into array[left..right]
	while (indexOfSubArrayOne < subArrayOne
		&& indexOfSubArrayTwo < subArrayTwo) {
		if (leftArray[indexOfSubArrayOne]
			<= rightArray[indexOfSubArrayTwo]) {
			array[indexOfMergedArray]
				= leftArray[indexOfSubArrayOne];
			indexOfSubArrayOne++;
		}
		else {
			array[indexOfMergedArray]
				= rightArray[indexOfSubArrayTwo];
			indexOfSubArrayTwo++;
		}
		indexOfMergedArray++;
	}

	// Copy the remaining elements of
	// left[], if there are any
	while (indexOfSubArrayOne < subArrayOne) {
		array[indexOfMergedArray]
			= leftArray[indexOfSubArrayOne];
		indexOfSubArrayOne++;
		indexOfMergedArray++;
	}

	// Copy the remaining elements of
	// right[], if there are any
	while (indexOfSubArrayTwo < subArrayTwo) {
		array[indexOfMergedArray]
			= rightArray[indexOfSubArrayTwo];
		indexOfSubArrayTwo++;
		indexOfMergedArray++;
	}
	// Uncomment for an array variant
	//delete[] leftArray;
	//delete[] rightArray;
}

// begin is for left index and end is right index
// of the sub-array of arr to be sorted
void mergeSort(vector<int>& array, int const begin, int const end)
{	// Create a global array and modified mergeSort function that saves mid values and returns in case we reach num_arrays?
	if (begin >= end)
		return;
	int mid = begin + (end - begin) / 2;
	mergeSort(array, begin, mid);
	mergeSort(array, mid + 1, end);
	merge(array, begin, mid, end);
}

void checkSort(const std::vector<int>& array)
{
	int flag = 0;
	int min;
	
	for(int i = 1 ; i < (int)array.size() ; i++)
	{
		min = array[i-1];
		if(array[i] < min)
		{
			flag = 1;
			break;
		}
	}
	
	if(flag == 0)
		printf("\nArray sorted well.\n");
	else 
		printf("\nArray not sorted well.\n");	
}

void printVector(const std::vector<int>& array)
{
	for(int i = 0; i < (int)array.size(); i++)
	{
		printf("%d ",array[i]);
	}
	cout << "\n" << endl;
}

// MAIN CODE ------------------------------------------------------------------------------------
int main(int argc, char*argv[])
{	
	// Number of threads 
	int tc = strtol(argv[1],NULL,10);
	int n = strtol(argv[2],NULL,10); 
	
	std::vector<int> array;
	srand(time(NULL));
	for(int i = 0 ; i < n ; i++)
	{
		array.push_back(rand()%200); 
	}
	
	// Store an array in input file
	FILE *fp_in = fopen("input.txt","w");
	if(fp_in == NULL)
	{
		cout << "Failed to open a file" << endl;
		return 1;
	}
	for(int i = 0; i < n ; i++)
	{
		fprintf(fp_in,"%d ",array[i]);
	}
	fclose(fp_in);
	cout << "Input is stored in input.txt." << endl;

	int depth = (int)ceil(log2(tc));
	int num_arrays = pow(2,depth);	
	int loc_size = (int)floor(((float)n/(float)num_arrays));
	int loc_size_odd = (int)ceil(((float)n/(float)num_arrays));
	int j,loc_start,loc_end;
	int p = 0;
	int t = 0;
	
	// Start measuring time
	double s = omp_get_wtime();
	
	if(n % tc == 0)
	{
		#pragma omp parallel for private(loc_start,loc_end,j) schedule(static,1) num_threads(tc)
		for(int i = 0; i < num_arrays ; i++)
		{
			j = i+1;	
			loc_start = i*loc_size;
			loc_end = j*loc_size-1;
			mergeSort(array,loc_start,loc_end);
			printf("Iteration = %d , loc_start = %d , loc_end = %d \n",i,loc_start,loc_end);  
		}
	}
	else 
	{
		#pragma omp parallel for private(loc_start,loc_end,p) schedule(static,1) num_threads(tc)
		for(int i = 0; i < num_arrays ; i++)
		{
			int trank = omp_get_thread_num();
			if(trank % 2 == 0)
			{
				// Parni podnizovi
				t = trank/2;
				loc_start = loc_size*trank+t;
				loc_end = loc_start + (loc_size-1);
				mergeSort(array,loc_start,loc_end);
				printf("Iteration = %d , t = %d , trank = %d , loc_start = %d , loc_end = %d \n",i,t,trank,loc_start,loc_end);
			}
			else
			{
				// Neparni podnizovi
				if(trank == 1)
				{
					 p = 0;
				}
				else	
				{
					 p = (int)floor((float)trank/2);
				}	
				loc_start = loc_size*trank+p;
				loc_end = loc_start + (loc_size_odd-1);
				mergeSort(array,loc_start,loc_end);
				printf("Iteration = %d , p = %d , trank = %d , loc_start = %d , loc_end = %d \n",i,p,trank,loc_start,loc_end);
			}	
		}
	
	}
		
	printVector(array);
	//-------------------------- REACHED THE DEPTH AND SORTED SUBARRAYS ------------------
	
	/*
	int loc_mid;
	if(n % tc == 0)
	{
		while(num_arrays > 1)
		{
			loc_size = n/num_arrays;
			#pragma omp parallel for private(loc_start,loc_end,loc_mid) schedule (static,1) num_threads(2)
			for(int k = 0; k < num_arrays ; k=k+2)
			{

				loc_start = k*loc_size; 
				loc_end = loc_start+(2*loc_size)-1; 
				loc_mid = (int)floor((loc_start+loc_end)/2);
				merge(array,loc_start,loc_mid,loc_end);
				//printf("k = %d, Loc_start = %d, loc_mid = %d, loc_end = %d, loc_size = %d, num_arrays = %d\n",k,loc_start,loc_mid,loc_end,loc_size,num_arrays); 
			}
			num_arrays = num_arrays/2;
			
		}
	}
	else 
	{
		while(num_arrays > 1)
		{
			loc_size = n/num_arrays;
			loc_size_odd = (int)ceil((float)n/(float)num_arrays);
			#pragma omp parallel for private(loc_start,loc_mid,loc_end) schedule(static,1) num_threads(2)
			for(int k = 0; k < num_arrays ; k=k+2)
			{	
				int trank = omp_get_thread_num();
				loc_start = k*loc_size+trank;
				loc_end = loc_start + (loc_size + loc_size_odd - 1);
				if((loc_start+loc_end) % 2 == 0)
					loc_mid = ((loc_start+loc_end)/2)-1;
				else 
					loc_mid = (int)floor(((float)loc_start + (float)loc_end)/2);
				merge(array,loc_start,loc_mid,loc_end);	
				printf("k = %d, Loc_start = %d, loc_mid = %d, loc_end = %d, loc_size = %d, num_arrays = %d\n",k,loc_start,loc_mid,loc_end,loc_size,num_arrays);
			}
			num_arrays = num_arrays/2;
		}
	}
	
	*/
	//printVector(array); 
	
	
	// End measuring time
 	s = (omp_get_wtime() - s) * 1000.0;	
	cout << "\nTime elapsed is : " << s << "ms" << endl;
	
	
	FILE *fp_out = fopen("res.txt","w");
	if(fp_out == NULL)
	{
		cout << "Failed to open a file" << endl;
		return 1;
	}
	
	for(int i = 0; i < n ; i++)
	{
		fprintf(fp_out,"%d ",array[i]);
	}
	fclose(fp_out);
	cout << "Results are stored in res.txt." << endl;
	
	checkSort(array);
	return 0;
}
