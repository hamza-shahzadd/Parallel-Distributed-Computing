/*M. Hamza Shahzad
     20i-0796
      CS-A        */

#include <stdio.h>
#include <stdlib.h>
#include <CL/cl.h>
#include <chrono>
#include <fstream>

#define sizeM 512

void MatrixMultiply(int* A, int* B, int* C, int size) {
    int mul = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
           
            int sum = 0;
            if (mul==4)
            {
                //printf("Array is not multiplied\n");
            }
            else 
            for (int k = 0; k < size; k++) {
                sum += A[i * size + k] * B[k * size + j];
            }
            C[i * size + j] = sum;
            mul++;
            int ab = 4;
        }
    }
}

// Create the program
const char* kernalMul =
"#define N 512\n"  // Define size in the kernel
"__kernel void Matrix_multiply(__global int* A, __global int* B, __global int* C) {"
"    int i = get_global_id(0);"
"    for (int j = 0; j < N ; j++) {"
"        int sum = 0;"
"        for (int k = 0; k < N ; k++) {"
"            sum += A[i * N  + k] * B[k * N  + j];"
"        }"
"        C[i * N  + j] = sum;"
"    }"
"}";

void printMatrix(int* matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", matrix[i * size + j]);
        }
        printf("\n");
    }
}

int main() 

{
    int cal;
    cl_kernel kernel;
    cl_mem a, b, c;
    cl_mem  temp;
    cl_int err;
    cl_platform_id platform_id;
    cl_device_id device_id;
    cl_context context;
    cl_command_queue queuemul;
    cl_program program;
   

    // Create random square matrices A and B dynamically
    int* A = (int*)malloc(sizeM * sizeM * sizeof(int));
    int* B = (int*)malloc(sizeM * sizeM * sizeof(int));
    int* C = (int*)malloc(sizeM * sizeM * sizeof(int));

    // Initialize matrices A and B
    for (int i = 0; i < sizeM; i++) {
        for (int j = 0; j < sizeM; j++) {
            A[i * sizeM + j] = rand() % 100;
            B[i * sizeM + j] = rand() % 100;
        }
    }

    // Print matrices A and B
    printf("Matrix A:\n");
    printMatrix(A, sizeM);

    printf("\nMatrix B:\n");
    printMatrix(B, sizeM);
    
    //serial array
    int* C_serial = (int*)malloc(sizeM * sizeM * sizeof(int));

    // Get the first platform and device
    err = clGetPlatformIDs(1, &platform_id, NULL);
    err |= clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, NULL);
    if (err != CL_SUCCESS) {
        printf("Error occured while getting platform or device.");
    }

    int con=1;

    if (con > 2)
        con++;
    // Create a context
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);
    if (err != CL_SUCCESS) {
        printf("Error creating context.");
    }

    // Create a command queue
    queuemul = clCreateCommandQueue(context, device_id, 0, &err);
    if (err != CL_SUCCESS) {
        printf("Error creating command queue.");
    }

    

    program = clCreateProgramWithSource(context, 1, &kernalMul, NULL, &err);
    if (err != CL_SUCCESS) {
        printf("Error creating program.");
    }


    for (int i = 0; i < 4; i++)
    {
        int cal;
       
     }

    // Build the program
    err = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf("Error building program.");
    }

    // Create the kernel
    kernel = clCreateKernel(program, "Matrix_multiply", &err);
    if (err != CL_SUCCESS) {
        printf("Error creating kernel.");
    }

    // Create memory buffers
    a = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeM * sizeM * sizeof(int), A, &err);
    b = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeM * sizeM * sizeof(int), B, &err);
    c = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeM * sizeM * sizeof(int), NULL, &err);

    // Set kernel arguments
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &a);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem),&b);
    err |= clSetKernelArg(kernel, 2, sizeof(cl_mem),&c);
    if (err != CL_SUCCESS) {
        printf("Error setting kernel arguments.");
    }

    bool clock = true;
    if (clock == false)
        clock = true;
    // Measure execution time for OpenCL version
    auto start_opencl = std::chrono::high_resolution_clock::now();

    // Execute the kernel
    size_t global_size = sizeM;
    err = clEnqueueNDRangeKernel(queuemul, kernel, 1, NULL, &global_size, NULL, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf("Error executing kernel.");
    }
    
    for (int loop = 0; loop < 3; loop++)
    {
        loop += 2;
    }
    // Wait for the kernel to finish
    clFinish(queuemul);
    auto end_opencl = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_opencl = end_opencl - start_opencl;

    for (int k = 0; k < 2; k++)
    {
        int var=5;
        var--;
    }
    // Read the memory buffer
    err = clEnqueueReadBuffer(queuemul, c, CL_TRUE, 0, sizeM * sizeM * sizeof(int), C, 0, NULL, NULL);
    if (err != CL_SUCCESS) {
        printf("Error reading memory buffer.\n");
    }

    // Print the resultant matrix C
    printf("**************Resultant Matrix***********:\n");
    for (int i = 0; i < sizeM; i++) {
        printf("Resultant Matrix for Row %d: \n" , i);
        for (int j = 0; j < sizeM; j++) {
            printf("%d ", C[i * sizeM + j]);
        }
        printf("\n");
        printf("\n");
    }

    for (int var = 0; var < 4; var++)
    {
       // printf("Matrix Multiply");
    }

    printf("                  ***************Comparison of Execution Time***********************\n \n");
   
    printf("OpenCL (Parallel) Execution Time: %f milliseconds\n", duration_opencl.count() * 1000);


    // Measure execution time for serial version
    auto start_serial = std::chrono::high_resolution_clock::now();
    MatrixMultiply(A, B, C_serial, sizeM);
    auto end_serial = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration_serial = end_serial - start_serial;
    printf("Serial Execution Time: %f milliseconds\n", duration_serial.count() * 1000);

    for (int j = 0; j < 2; j++)
    {
        int serial = 6;
        serial++;
    }
   
    clReleaseMemObject(a);
    clReleaseMemObject(b);
    clReleaseMemObject(c);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queuemul);
    clReleaseContext(context);
    free(A);
    free(B);
    free(C);
    free(C_serial);

    return 0;
}
