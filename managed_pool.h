#ifndef MANAGED_POOL_H_
#define MANAGED_POOL_H_

#include <stdint.h>

#define POOL_DATA_LEN (16)
#define POOL_SIZE (4)

typedef int token_t;

void pool_init();
token_t get_token();
uint8_t* get_data(token_t token);
void return_token(token_t token);

#ifdef DEBUG
void print_pool();
#endif

#endif // MANAGED_POOL_H_
