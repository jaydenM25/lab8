#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int extraMemoryAllocated;

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(int array[], int N, int oriVal) {
    int largest = oriVal;

    int left = 2 * oriVal + 1;
    int right = 2 * oriVal + 2;

    if (left < N && array[left] > array[largest])
        largest = left;
    if (right < N && array[right] > array[largest])
        largest = right;

    if (largest != oriVal) {
        swap(&array[oriVal], &array[largest]);
        heapify(array, N, largest);
    }
}

void heapSort(int array[], int N) {
    for (int i = N / 2 - 1; i >= 0; i--)
        heapify(array, N, i);

    for (int i = N - 1; i >= 0; i--) {
        swap(&array[0], &array[i]);
        heapify(array, i, 0);
    }
}

void mergeSort(int *pData, int left, int right) {
    if (left < right) {
        int mid = (left+right)/2;
        mergeSort(pData, left, mid);
        mergeSort(pData, mid+1, right);
        
        int q1 = mid - left + 1;
        int q2 = right - mid;
        
        extraMemoryAllocated += (q1*sizeof(int)) + (q2*sizeof(int));
        
        int *leftList = (int*) malloc(q1*sizeof(int));
        int *rightList = (int*) malloc(q2*sizeof(int));
        
        for (int i = 0; i < q1; i++)
        	leftList[i] = pData[left + i];
        for (int i = 0; i < q2; i++) 
        	rightList[i] = pData[mid + 1 + i];
        
        int leftCounter = 0;
        int rightCounter = 0;
        
        int k = left;
        
        while (leftCounter < q1 && rightCounter < q2) {
        	if(leftList[leftCounter] <= rightList[rightCounter]) {
        		pData[k] = leftList[leftCounter];
        		leftCounter++;
        	}
        	else {
        		pData[k] = rightList[rightCounter];
        		rightCounter++;
        	}
        k++;
        }
        	while(leftCounter < q1) {
        		pData[k] = leftList[leftCounter];
        		leftCounter++;
        		k++;
        	}
        	
        	while (rightCounter < q2) {
        		pData[k] = rightList[rightCounter];
        		rightCounter++;
        		k++;
        	}
        
        free(leftList);
        free(rightList);
        
    } 
}

int parseData(char *inputFileName, int **ppData) {
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile) {
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL) {
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i) {
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz) {
	int i, sz = dataSz - 100;
	
	printf("\tData:\n\t");
	
	for (i=0;i<100;++i) {
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i) {
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void) {
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i) {
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.6lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.6lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}
