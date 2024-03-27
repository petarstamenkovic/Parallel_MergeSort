// C++ program for Merge Sort 
#include <iostream> 
#include<cstdlib>
#include <chrono>
#include <vector>
using namespace std::chrono;
using namespace std; 

// Merges two subarrays of array[]. 
// First subarray is arr[begin..mid] 
// Second subarray is arr[mid+1..end] 
void merge(std::vector<int>& array, int const left, 
		int const mid, int const right) 
{ 
	auto const subArrayOne = mid - left + 1; 
	auto const subArrayTwo = right - mid; 

	// Create temp vectors
	std::vector<int> leftArray(subArrayOne);
	std::vector<int> rightArray(subArrayTwo);

	// Uncomment for an array variant
	// Create temp arrays 
	//auto *leftArray = new int[subArrayOne], 
	//	*rightArray = new int[subArrayTwo]; 

	// Copy data to temp arrays leftArray[] 
	// and rightArray[] 
	for (auto i = 0; i < subArrayOne; i++) 
		leftArray[i] = array[left + i]; 
	for (auto j = 0; j < subArrayTwo; j++) 
		rightArray[j] = array[mid + 1 + j]; 

	// Initial index of first sub-array 
	// Initial index of second sub-array 
	auto indexOfSubArrayOne = 0, 
		indexOfSubArrayTwo = 0; 

	// Initial index of merged array 
	int indexOfMergedArray = left; 

	// Merge the temp arrays back into 
	// array[left..right] 
	while (indexOfSubArrayOne < subArrayOne && 
		indexOfSubArrayTwo < subArrayTwo) 
	{ 
		if (leftArray[indexOfSubArrayOne] <= 
			rightArray[indexOfSubArrayTwo]) 
		{ 
			array[indexOfMergedArray] = 
			leftArray[indexOfSubArrayOne]; 
			indexOfSubArrayOne++; 
		} 
		else
		{ 
			array[indexOfMergedArray] = 
			rightArray[indexOfSubArrayTwo]; 
			indexOfSubArrayTwo++; 
		} 
		indexOfMergedArray++; 
	} 
	
	// Copy the remaining elements of 
	// left[], if there are any 
	while (indexOfSubArrayOne < subArrayOne) 
	{ 
		array[indexOfMergedArray] = 
		leftArray[indexOfSubArrayOne]; 
		indexOfSubArrayOne++; 
		indexOfMergedArray++; 
	} 

	// Copy the remaining elements of 
	// right[], if there are any 
	while (indexOfSubArrayTwo < subArrayTwo) 
	{ 
		array[indexOfMergedArray] = 
		rightArray[indexOfSubArrayTwo]; 
		indexOfSubArrayTwo++; 
		indexOfMergedArray++; 
	} 
} 

// begin is for left index and end is 
// right index of the sub-array 
// of arr to be sorted */ 
void mergeSort(vector<int>& array, 
			int const begin, 
			int const end) 
{ 
	// Returns recursively 
	if (begin >= end) 
		return; 

	auto mid = begin + (end - begin) / 2; 
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

// Driver code 
int main() 
{ 
	time_t t;
   	int n,x;
   	cout << "Insert a lenght of an array: " << endl;
  	cin >> n ;
    
    	std::vector<int> array;
    
	for(int i = 0 ; i < n ; i++)
	{
		srand((unsigned) time(&t) + i);
		array.push_back(rand()%500); 
	}
	
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
	cout << "Results are stored in input.txt." << endl;
	
	
	auto start = high_resolution_clock::now();
	
	mergeSort(array, 0, n - 1);
	 
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	cout << "Elapsed time is : " << duration.count() << "ms."<< endl;
	
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
