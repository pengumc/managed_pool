#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "managed_pool.h"

int main() {
  pool_init();
  printf("fresh ");
  print_pool();

  token_t tokens[POOL_SIZE + 1];
  int i;
  // Grab all items
  for (i = 0; i < POOL_SIZE; ++i) {
    tokens[i] = get_token();
    assert(tokens[i] != 0);
    uint8_t* data = get_data(tokens[i]);
    assert(data != NULL);
    data[0] = i;
    data[1] = i + 1;
  }
  tokens[POOL_SIZE] = get_token();
  assert(tokens[POOL_SIZE] == 0);
  printf("grabbed ");
  print_pool();

  // release all items
  for (i = POOL_SIZE - 1; i >= 0; --i) {
    uint8_t* data = get_data(tokens[i]);
    assert(data != NULL);
    assert(data[0] == i);
    assert(data[1] == i + 1);
    return_token(tokens[i]);
  }
  assert(get_data(tokens[POOL_SIZE]) == NULL);
  printf("released ");
  print_pool();

  // set and release in random order
  for (i = 0; i < POOL_SIZE; ++i) {
    tokens[i] = get_token();
    assert(tokens[i] != 0);
    get_data(tokens[i])[0] = (uint8_t)tokens[i];
  }
  printf("grabbed again ");
  print_pool();

  // Return a token from the middle somewhere
  return_token(tokens[2]);
  printf("returned token %i ", tokens[2]);
  print_pool();
  tokens[2] = get_token();
  assert(tokens[2] != 0);
  printf("got token %i ", tokens[2]);
  print_pool();

  // Return token at the start of the list
  return_token(tokens[0]);
  printf("returned token %i ", tokens[0]);
  print_pool();

  // Return token at the end of the list
  tokens[4] = get_token();
  printf("got token %i ", tokens[4]);
  print_pool();

  return_token(tokens[4]);
  printf("returned token %i ", tokens[4]);
  print_pool();



  return 0;
}