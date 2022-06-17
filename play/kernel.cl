__kernel void even_sort(__global int* C ) {

    // Get the index of the current element to be processed
    int i = get_global_id(0);
 
    // Do the operation
    if(C[ i ] > C[i + 1 ] ){
        int temporary = C[ i ];
        C[ i ] = C[ i + 1 ];
        C[ i  + 1 ] = temporary;
    }
}

__kernel void odd_sort(__global int* C ) {

    // Get the index of the current element to be processed
    int i = get_global_id(0);

    // Do the operation
    if(C[i+1] > C[i+2] ){
        int temporary = C[i+1];
        C[i+1] = C[i+2];
        C[i+2] = temporary;
    }
}
