#include <stdint.h>
#include <stdlib.h>

#include "managed_pool.h"

typedef struct PoolItem pool_item_t;

// Items with tokens are always in the front
struct PoolItem {
  uint8_t data[POOL_DATA_LEN];
  pool_item_t* next; // NULL for last in list
  pool_item_t* prev; // NULL for first in list
  pool_item_t* last; // Only guaranteed to be valid if first
  token_t token;
};

static pool_item_t pool[POOL_SIZE];
static pool_item_t* first;
static token_t next_token = 1;

void pool_init() {
  // Make a linked list out of pool[], starting at pool[0]
  int i;
  for (i = 0; i < POOL_SIZE; ++i) {
    pool[i].next = &pool[i + 1];
    pool[i].prev = &pool[i - 1];
    pool[i].last = NULL;
    pool[i].token = 0;
  }
  pool[POOL_SIZE - 1].next = NULL;
  pool[0].last = &pool[POOL_SIZE - 1];
  pool[0].prev = NULL;
  first = &pool[0];
}

// Returns 0 if no free pool item was available
token_t get_token() {
  token_t return_token = next_token;
  // Find the first tokenless pool item
  // and assign next_token there
  pool_item_t* item = first;
  while (item != NULL && item->token) {
    item = item->next;
  }
  if (item != NULL) {
    item->token = next_token++;
  } else {
    return_token = 0;
  }
  return return_token;
}

uint8_t* get_data(token_t token) {
  pool_item_t* item = NULL;
  if (token != 0) {
    item = first;
  }
  // Find the pool item with this token
  while (item != NULL) {
    if (item->token == token) {
      break;
    } else if (item->token == 0) {
      item = NULL;
    } else {
      item = item->next;
    }
  }
  return (item != NULL) ? item->data : NULL;
}

static void move_to_last(pool_item_t* item) {
  if (item->next != NULL && item->prev != NULL) {
    // first -->.. item .. --> last
    item->prev->next = item->next;
    item->next->prev = item->prev;
    first->last->next = item;
    item->prev = first->last;
    item->next = NULL;
    first->last = item;
  } else if (item->next != NULL && item->prev == NULL) {
    // item --> A --> last
    item->next->prev = NULL;
    first = item->next;
    item->last->next = item;
    item->prev = item->last;
    item->next = NULL;
    first->last = item;
  } else if (item->next == NULL && item->prev != NULL) {
    // first --> A --> item
    // We're already last. do nothing
  } else {
    // Very invalid
  }
}

void return_token(token_t token) {
  pool_item_t* item = NULL;
  if (token != 0) {
    item = first;
  }
  // Find the item with this token
  while (item != NULL) {
    if (item->token == token) {
      break;
    } else if (item->token == 0) {
      item = NULL;
    } else {
      item = item->next;
    }
  }
  if (item != NULL) {
    item->token = 0;
    // If the item after us has a token we need to move to the end of the list
    if (item->next != NULL && item->next->token) {
      move_to_last(item);
    }
  }
}

#ifdef DEBUG
#include <stdio.h>
void print_pool() {
  pool_item_t* item = first;
  printf("pool:\n");
  while (item != NULL) {
    printf("item %p: token %i\n", item, item->token);
    item = item->next;
  }
}
#endif