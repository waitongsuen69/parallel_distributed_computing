#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define MAX_SOURCE_SIZE 100000
int source_num;
int num_list[MAX_SOURCE_SIZE];

void swap(int x, int y){
    int a = num_list[x];
    num_list[x] = num_list[y];
    num_list[y] = a;
//    return arr;
}

void set_up(){
    for (int i = 0; i < source_num; i++) {
        num_list[i] = rand();
    }
}

void O_E_sort(){
    int n = source_num;
    int full_sort = 0;
    while (full_sort != 1){
        full_sort = 1;
        for(int i = 0; i<= n-2;i = i+2){
            if(num_list[i]>num_list[i+1]){
                swap(i,i+1);
                full_sort = 0 ;
            }

        }

        for(int i = 1 ; i <= n-2;i = i+2) {
            if (num_list[i] > num_list[i + 1]) {
                swap(i, i + 1);
                full_sort = 0;
            }
        }
    }
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
    if(1) {
        O_E_sort();
    }
    end = clock();
    time_use = ((double)(end - start))/ CLOCKS_PER_SEC;
    printf("check (1:0 T:F):%d\n",check());
    printf("Time for %d numbers sort in serial is %f second\n",source_num,time_use);


    return 0;
}
