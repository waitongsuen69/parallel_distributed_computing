#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#include <CL/cl.h>
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define MAX_SOURCE_SIZE 100000
int source_num;
int num_list[MAX_SOURCE_SIZE];

void check_err(cl_int err, char* err_output) {
    if (err != CL_SUCCESS) {
        fprintf(stderr, "Error %i: %s\n", (int)err, err_output);
        exit(1);
    }
}

//void swap(int x, int y){
//    int a = num_list[x];
//    num_list[x] = num_list[y];
//    num_list[y] = a;
//    return arr;
//}

void set_up(){
    for (int i = 0; i < source_num; i++) {
        num_list[i] = rand();
    }
}

void O_E_sort(){

// Load the kernel source code into the array source_str
    FILE *fp;
    char *source_str;
    size_t source_size;

    fp = fopen("kernel.cl", "r");
    if (!fp) {
        fprintf(stderr, "Failed to load kernel.\n");
        exit(1);
    }
    source_str = (char *) malloc(MAX_SOURCE_SIZE);
    source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
    fclose(fp);

    // variable used for OpenCL error handling
    cl_int err;

    // Get ID of platform, device
    cl_platform_id platform_id;
    cl_uint ret_num_platforms;
    err = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    check_err(err, "clGetPlatformIDs");
    printf("%i Platforms found\n", ret_num_platforms);

    cl_device_id device_id;
    cl_uint ret_num_devices;
    err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);
    check_err(err, "clGetPlatformIDs");
    printf("%i Devices found\n", ret_num_devices);

    //get work group size !!!!!!
    size_t max_work_group_size;
    err = clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(max_work_group_size), &max_work_group_size, NULL);
    check_err(err, "clGetDeviceInfo");
    printf("Max workgroup size is %i\n", (int)max_work_group_size);

    // Create an OpenCL context
    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);
    check_err(err, "clCreateContext");

    // Create Queue with Profiling enabled
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &err);
    check_err(err, "clCreateCommandQueue");

    // Create memory buffers on the device for each vector
    cl_mem mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, source_num * sizeof(int), NULL, &err);
    check_err(err, "clCreateBuffer a");

    // Copy the lists to buffers
    err = clEnqueueWriteBuffer(command_queue, mem_obj, CL_TRUE, 0, source_num * sizeof(int), num_list, 0, NULL, NULL);
    check_err(err, "clEnqueueWriteBuffer a");

    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &err);
    check_err(err, "clCreateProgramWithSource");

    // Build the program
    err = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    check_err(err, "clBuildProgram");

    //for even_sort
    // Create the OpenCL kernel
    cl_kernel kernel_1 = clCreateKernel(program, "even_sort", &err);
    check_err(err, "clCreateKernel");

    // Set the arguments of the kernel
    err = clSetKernelArg(kernel_1, 0, sizeof(cl_mem), (void *)&mem_obj);
    check_err(err, "clSetKernelArg C");

    //for odd_sort
    // Create the OpenCL kernel
    cl_kernel kernel_2 = clCreateKernel(program, "odd_sort", &err);
    check_err(err, "clCreateKernel");

    // Set the arguments of the kernel
    err = clSetKernelArg(kernel_2, 0, sizeof(cl_mem), (void *)&mem_obj);
    check_err(err, "clSetKernelArg C");
//
//    for (int i = 0; i < (source_num/2)+1; ++i) {
//        size_t g_item = source_num/2;
//        size_t l_item = 1;
//
//    }

    // Ensure to have executed all enqueued tasks
    err = clFinish(command_queue);
    check_err(err, "clFinish");

    size_t local_work_size = ((size_t)source_num > max_work_group_size)? max_work_group_size : (size_t)source_num;
    size_t num_work_groups = (source_num + local_work_size - 1) / local_work_size; // divide rounding up
    size_t global_work_size = num_work_groups * local_work_size;

    for (int i = 0; i < source_num; ++i) {


        err = clEnqueueNDRangeKernel(command_queue, kernel_1, 1, NULL, &global_work_size, &local_work_size, 0, NULL, NULL);
        check_err(err, "even sort Finish");

        err = clEnqueueNDRangeKernel(command_queue, kernel_2, 1, NULL, &global_work_size, &local_work_size, 0, NULL, NULL);
        check_err(err, "odd sort Finish");
    }
    // Launch Kernel linked to an event
//    cl_event event;
//    err = clEnqueueNDRangeKernel(command_queue, kernel_1, 1, NULL, &global_work_size, &local_work_size, 0, NULL, &event);
//    check_err(err, "clEnqueueNDRangeKernel");
////
//    // Ensure kernel execution is finished
//    err = clWaitForEvents(1, &event);
//    check_err(err, "clWaitForEvents");
//
//    // Get the Profiling data
//    cl_ulong time_start, time_end;
//    double total_time;
//    err = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
//    check_err(err, "clGetEventProfilingInfo Start");
//    err = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
//    check_err(err, "clGetEventProfilingInfo End");
//    total_time = time_end - time_start;
//
//    // Read the memory buffer C on the device to the local variable C
//    int *C = (int*)malloc(sizeof(int)*LIST_SIZE);
    err = clEnqueueReadBuffer(command_queue, mem_obj, CL_TRUE, 0, source_num * sizeof(int), num_list, 0, NULL, NULL);
    check_err(err, "clEnqueueReadBuffer c");
    // Clean up
    err = clFlush(command_queue);
    err = clFinish(command_queue);
    err = clReleaseKernel(kernel_1);
    err = clReleaseKernel(kernel_2);
    err = clReleaseProgram(program);
    err = clReleaseMemObject(mem_obj);
    err = clReleaseCommandQueue(command_queue);
    err = clReleaseContext(context);
    free(source_str);
    return;
}

int check(){
    for (int i = 0; i < source_num-1; ++i) {
        if ( num_list[i] > num_list[i+1]){
            return 0;
        }
    }
    return 1;
}



int main(int argc, char** argv){
    clock_t start, end;
    double time_use;
    source_num = atoi(argv[1]);
    set_up();
    start = clock();
    O_E_sort();
    end = clock();
    time_use = ((double)(end - start))/ CLOCKS_PER_SEC;
    printf("check (1:0 T:F):%d\n",check());
    printf("Time for %d numbers sort in serial is %f second\n",source_num,time_use);


    return 0;
}