#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "../headers/bit_utils.h"

#define _SET_BIT(BF,N) BF |= (0x1 << N)
#define _CLR_BIT(BF,N) BF &= ~(0x1 << N)
#define _GET_BITS(BF,N,S) ((BF >> N) & ~(0x1 << S))
#define _IS_BIT_SET(BF, N) (GET_BITS(BF,N,1) > 0)

static intptr_t get_offset_address(intptr_t address, int offset) {
    intptr_t byteOffset = floor(offset/BITS_IN_BYTE);
    address += byteOffset;
    return address;
}

static uint8_t* get_offset_byte_address(intptr_t address, int offset) {
    intptr_t newAddress = get_offset_address(address, offset);
    uint8_t* bit = (uint8_t *)newAddress;
    return bit;
}

static uint8_t get_offset_bit(int offset) {
    uint8_t bitOffset = offset%BITS_IN_BYTE;
    return (uint8_t)0b1 << bitOffset;
}

void set_bit(void* data, int offset) {
    *get_offset_byte_address((intptr_t)data, offset) |= get_offset_bit(offset);
}

void clr_bit(void* data, int offset) {
    *get_offset_byte_address((intptr_t)data, offset) &= ~get_offset_bit(offset);
}

uint8_t is_bit_set(const void* data, int offset) {
    return (*get_offset_byte_address((intptr_t)data, offset) & get_offset_bit(offset)) > 0;
}

uint64_t get_bits(void* data, int offset, uint8_t size) {
    uint64_t start = offset*size;
    uint64_t cellValue = 0b0;
    for (uint8_t i = 0; i < size; i++) {
        uint64_t pos = start + i;
        if (is_bit_set(data, pos)) {
            set_bit(&cellValue, i);
        } else {
            clr_bit(&cellValue, i);
        }
    }
    return cellValue;
}

void set_bits(void* board, int offset, int value, uint8_t size) {
    int start = offset*size;
    for (int i = 0; i < size; i++) {
        int pos = start + i;
        if (is_bit_set(&value, i)) {
            set_bit(board, pos);
        } else {
            clr_bit(board, pos);
        }
    }
}

void print_bits(const void* data, size_t bits) {
    for (int i = bits-1; i >= 0; i--) {
        if (is_bit_set(data, i)) {
            printf("1");
        } else {
            printf("0");
        }
    }
    printf("\n");
}