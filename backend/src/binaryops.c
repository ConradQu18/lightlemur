#include "../include/ops.h"
#include "../include/tensor.h"

FORWARD_FUNC_DEF(b_op_add_forward){
    if ((is_contiguous(k0) == true) && (is_contiguous(k1)==true)){
        BINARY_CONTIGUOUS_ELEMENTWISE_OP_SIMD(kr,k0,k1,+);
    }
    else{
        KERNEL_TENSOR_5D_LOOP_START(kr){
            size_t offset_k0 = KERNEL_TENSOR_GET_OFFSET(k0);
            size_t offset_k1 = KERNEL_TENSOR_GET_OFFSET(k1);
            size_t offset_kr = KERNEL_TENSOR_GET_OFFSET(kr);
            kr->array[offset_kr] = k0->array[offset_k0] + k1->array[offset_k1];
        }
    }
}

BACKWARD_FUNC_DEF(b_op_add_backward){
    (void) kr; (void) k0; (void) k1; 
    kernel_tensor *next_seed;
    if (idx == 0){
        next_seed = seed;
    }
    else{
        next_seed = contiguous_deepcopy_kernel_tensor(seed);
    }
    return next_seed;
}

FORWARD_FUNC_DEF(b_op_mul_forward){
    KERNEL_TENSOR_5D_LOOP_START(kr){
        size_t offset_k0 = KERNEL_TENSOR_GET_OFFSET(k0);
        size_t offset_k1 = KERNEL_TENSOR_GET_OFFSET(k1);
        size_t offset_kr = KERNEL_TENSOR_GET_OFFSET(kr);
        kr->array[offset_kr] = k0->array[offset_k0] * k1->array[offset_k1];
    }
}

BACKWARD_FUNC_DEF(b_op_mul_backward){
    (void) kr;
    kernel_tensor *next_seed;
    kernel_tensor *k;
    if (idx == 0){
        next_seed = seed;
        k = k1;
    }
    else{
        next_seed = contiguous_deepcopy_kernel_tensor(seed);
        k = k0;
    }
    KERNEL_TENSOR_5D_LOOP_START(next_seed){
        size_t offset_next_seed = KERNEL_TENSOR_GET_OFFSET(next_seed);
        size_t offset_seed = KERNEL_TENSOR_GET_OFFSET(seed);
        size_t offset_k = KERNEL_TENSOR_GET_OFFSET(k);
        next_seed->array[offset_next_seed] = seed->array[offset_seed] * k->array[offset_k];
    }

    return next_seed;
}

FORWARD_FUNC_DEF(b_op_division_forward){
    KERNEL_TENSOR_5D_LOOP_START(kr){
        size_t offset_k0 = KERNEL_TENSOR_GET_OFFSET(k0);
        size_t offset_k1 = KERNEL_TENSOR_GET_OFFSET(k1);
        size_t offset_kr = KERNEL_TENSOR_GET_OFFSET(kr);
        kr->array[offset_kr] = k0->array[offset_k0] / k1->array[offset_k1];
    }
}

BACKWARD_FUNC_DEF(b_op_division_backward){
    (void) k0;
    kernel_tensor *next_seed;
    if (idx == 0) {
        next_seed = seed;
    }
    else {
        next_seed = contiguous_deepcopy_kernel_tensor(seed);
    }
    KERNEL_TENSOR_5D_LOOP_START(next_seed){
        size_t offset_next_seed = KERNEL_TENSOR_GET_OFFSET(next_seed);
        size_t offset_seed = KERNEL_TENSOR_GET_OFFSET(seed);
        size_t offset_kr = KERNEL_TENSOR_GET_OFFSET(kr);
        size_t offset_k1 = KERNEL_TENSOR_GET_OFFSET(k1);
        if (idx == 0) {
            next_seed->array[offset_next_seed] = seed->array[offset_seed] / k1->array[offset_k1];
        }
        else {
            next_seed->array[offset_next_seed] = -1.0 * seed->array[offset_seed] * (kr->array[offset_kr] / (k1->array[offset_k1]));
        }
    }
    return next_seed;
}