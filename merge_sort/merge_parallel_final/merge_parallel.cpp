#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <math.h>
#include <bits/stdc++.h>
#include <vector>
#include <math.h>
using namespace std;

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

// Function that checks if a vector is sorted well
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

// Function that prints the vector
void printVector(const std::vector<int>& array)
{
	for(int i = 0; i < (int)array.size(); i++)
	{
		printf("%d ",array[i]);
	}
	cout << "\n" << endl;
}

// Help function used in merge process - Sums first k elements in a vector
int sum(const std::vector<int> vector,int k)
{
	int res = 0;
	if(k == 0)
		return res;
	for(int i = 0 ; i < k ; i++)
	{
		res = res + vector[i];
	}
	return res;
}

// Function that gets the sizes of subarrays
void divide(vector<int>& subSizes, int size,int num_arrays)
{
    int help = num_arrays;
    int left_size,right_size;
    
    if(help == 1)
    	return;
 	
    if(help > 1)
    {
	    left_size = size/2;
	    right_size = size - left_size;
	    if(help == 2)
	    {
	    	subSizes.push_back(left_size);
	    	subSizes.push_back(right_size);
	    }
	    help = help / 2;
	  
	    divide(subSizes,left_size,help);
	    divide(subSizes,right_size,help);
    }
}

// MAIN CODE ------------------------------------------------------------------------------------
int main(int argc, char*argv[])
{	
	// Number of threads 
	int tc = strtol(argv[1],NULL,10);
	int n = strtol(argv[2],NULL,10); 
	
	// Used vectors
	vector<int> subSizes;
	vector<int> array;
	vector<int> help;
	
	// Creating a random array of numbers
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
	int loc_start,loc_end;

	divide(subSizes,n,num_arrays);
	
	// Start measuring time
	double s = omp_get_wtime();
	
	#pragma omp parallel for private(loc_start,loc_end) schedule(static,1) num_threads(tc)
	for(int i = 0 ; i < num_arrays; i++)
	{
			loc_start = sum(subSizes,i);
			loc_end = loc_start + subSizes[i] - 1;
			mergeSort(array,loc_start,loc_end);
	}
	
	//-------------------------- REACHED THE DEPTH AND SORTED SUBARRAYS ------------------
	
	int loc_mid;
	while(num_arrays > 1)
	{
		#pragma omp parallel for private(loc_start,loc_mid,loc_end) schedule(static,1) num_threads(tc)
		for(int k = 0 ; k < num_arrays ; k = k + 2)
		{
			int trank = omp_get_thread_num();
			if(trank == 0)
			{
				loc_start = 0;
				loc_end = subSizes[k] + subSizes[k+1] - 1;
				if((loc_start+loc_end) % 2 == 0)
					loc_mid = ((loc_start+loc_end)/2)-1;
				else 
					loc_mid = (int)floor(((float)loc_start + (float)loc_end)/2);
				merge(array,loc_start,loc_mid,loc_end);

			}
			else
			{
				loc_start = sum(subSizes,k);
				loc_end = (loc_start + subSizes[k] + subSizes[k+1])-1;
				if((loc_start+loc_end) % 2 == 0)
					loc_mid = ((loc_start+loc_end)/2)-1;
				else 
					loc_mid = (int)floor(((float)loc_start + (float)loc_end)/2);
				merge(array,loc_start,loc_mid,loc_end);
			}
		}
		num_arrays = num_arrays/2;
		// Due to shrinking of a subSize, we need to transfer elements in help and clear out the subSizes
		// and then move new values to shrinked subSizes.
		for(int s = 0 ; s < (int)subSizes.size() ; s=s+2)
		{
			int elem = subSizes[s] + subSizes[s+1];
			help.push_back(elem);
		}
		subSizes.clear();
		for(int s = 0 ; s < (int)help.size();s++)
		{
			subSizes.push_back(help[s]);
		}
		help.clear();
	}
	
	// Uncomment to view a sorted array 
	// printVector(array); 
	
	
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
