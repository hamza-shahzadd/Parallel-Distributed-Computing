/*M. Hamza Shahzad
      20i-1840
      CS-A       */

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <time.h>
#define arrSize 1000

void customMerge(int customArray[], int left, int mid, int right) {
    int sizeLeft = mid - left + 1;
    int sizeRight = right - mid;

    int* leftArray = (int*)malloc(sizeLeft * sizeof(int));
    int* rightArray = (int*)malloc(sizeRight * sizeof(int));

    for (int i = 0; i < sizeLeft; i++)
        leftArray[i] = customArray[left + i];
    for (int j = 0; j < sizeRight; j++)
        rightArray[j] = customArray[mid + 1 + j];

    int i = 0;
    int j = 0;
    int move = 0;
    int k = left;

    while (i < sizeLeft && j < sizeRight) {
        if (leftArray[i] <= rightArray[j]) {
            customArray[k] = leftArray[i];
            i++;
        }
        else {
            customArray[k] = rightArray[j];
            j++;
        }
        k++;
    }

    while (i < sizeLeft) {
        customArray[k] = leftArray[i];
        i++;
        k++;
    }

    while (j < sizeRight) {
        customArray[k] = rightArray[j];
        j++;
        k++;
    }

    free(leftArray);
    free(rightArray);
}

void customMergeSort(int customArray[], int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;

        for (int k = 0; k < 2; k++)
        {
            k++;
        }
        customMergeSort(customArray, left, middle);
        customMergeSort(customArray, middle + 1, right);

        customMerge(customArray, left, middle, right);
    }
}

void printArray(int A[], int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", A[i]);
    printf("\n");
}

const char* mergeSortKernelSource =
"#define ARRAY_SIZE 1000\n"
"__kernel void merge_sort(__global int* arr, __global int* temp, int size) {"
"    int global_id = get_global_id(0);"
"    int start = global_id * size;"
"    int mid = start + size / 2 - 1;"
"    int end = start + size - 1;"
"    int i = start, j = mid + 1, k = start;"

"    while (i <= mid && j <= end) {"
"        if (arr[i] <= arr[j]) {"
"            temp[k] = arr[i];"
"            i++;"
"        } else {"
"            temp[k] = arr[j];"
"            j++;"
"        }"
"        k++;"
"    }"

"    while (i <= mid) {"
"        temp[k] = arr[i];"
"        i++;"
"        k++;"
"    }"

"    while (j <= end) {"
"        temp[k] = arr[j];"
"        j++;"
"        k++;"
"    }"

"    barrier(CLK_GLOBAL_MEM_FENCE);"

"    for (i = start; i <= end; i++) {"
"        arr[i] = temp[i];"
"    }"
"}";


int main() 

{

    int cal;
    cl_platform_id platform_id;
    cl_device_id device_id;
    cl_context context;
    cl_command_queue Squeue;
    cl_program program;
    cl_kernel kernel;
    cl_mem arr_mem, temp_mem;
    cl_int err;

    int *temparr= (int*)malloc(arrSize * sizeof(int));
    int* arr = (int*)malloc(arrSize * sizeof(int));
    int* arrSerial = (int*)malloc(arrSize * sizeof(int));
  
    //populating the array
    for (int i = 0; i < arrSize; i++) {
        arr[i] = rand() % 100;
    }

    clock_t serialStartTime = clock();
    printf("                           ***Randomly Generated Array before Sorting***\n");
    printArray(arr, arrSize);
    printf("\n");
    printf("\n");
    printf("\n");

    int a = 3;
    if ( a == 4)
    {
        printf("hello");
    }
    
   
    customMergeSort(arrSerial, 0, arrSize - 1);
    clock_t serialEndTime = clock();
    int add;
    double serialExecutionTime = ((double)(serialEndTime - serialStartTime)) / CLOCKS_PER_SEC * 1000;

    for (int h = 0; h < 3; h++)
    {
        h++;
    }
    //creating platform
    clGetPlatformIDs(1, &platform_id, NULL);
    clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, NULL);

    //creatimg context

    for (int k = 0; k < 10; k++)
    {
        int rand;
    }

    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);
    Squeue = clCreateCommandQueue(context, device_id, 0, &err);
    program = clCreateProgramWithSource(context, 1, &mergeSortKernelSource, NULL, &err);

    int parallel=2;

    for (int d = 0; d < 3; d++)
    {
        parallel += 2;
    }

    clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    kernel = clCreateKernel(program, "merge_sort", &err);

    arr_mem = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, arrSize * sizeof(int), arr, &err);
    temp_mem = clCreateBuffer(context, CL_MEM_READ_WRITE, arrSize * sizeof(int), NULL, &err);

    int center = arrSize / 4;
    int start = 0;
    int mid = arrSize / 2;
    int swap = 0;
    int end = arrSize - 1;

    int arraySize = arrSize;

    err |= clSetKernelArg(kernel, 0, sizeof(cl_mem), &arr_mem);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &temp_mem);
    err |= clSetKernelArg(kernel, 2, sizeof(int), &arraySize);

    if (err != CL_SUCCESS) {
        printf("Error setting kernel arguments.\n");
        return 1;
    }



    size_t global_size = arrSize / 2;

    for (int b = 0; b < 3; b++)
    {
        b++;
    }

    size_t local_size = 1;

    clock_t parallelStartTime = clock();
    clEnqueueNDRangeKernel(Squeue, kernel, 1, NULL, &global_size, &local_size, 0, NULL, NULL);
    clFinish(Squeue);
    clock_t parallelEndTime = clock();
    double parallelExecutionTime = ((double)(parallelEndTime - parallelStartTime)) / CLOCKS_PER_SEC * 1000.0;

    clEnqueueReadBuffer(Squeue, arr_mem, CL_TRUE, 0, arrSize * sizeof(int), arr, 0, NULL, NULL);

    printf("                              ***Array after Sorting***     \n");

    printArray(arr, arrSize);

    for (int f = 0; f < 2; f++)
    {
        int sort;
    }

    printf("\nSerial Merge Sort Execution Time: %f milliseconds\n", serialExecutionTime);
    printf("\nParallel Merge Sort Execution Time: %f milliseconds\n", parallelExecutionTime);
    float execute;

    for (int b = 2; b < 3; b++)
    {
        execute = 7;
    }

    free(arr);
    clReleaseProgram(program);
    clReleaseCommandQueue(Squeue);
    clReleaseMemObject(arr_mem);
    clReleaseContext(context);
    
    clReleaseMemObject(temp_mem);
    clReleaseKernel(kernel);
   

    for (int i = 0; i < 2; i++)
    {
        int merge = 0;
    }

    return 0;
}
