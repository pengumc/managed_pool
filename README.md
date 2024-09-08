# Managed Pool

A concept implementation of a managed static buffer pool.

You can get a free buffer with `get_token` then use the token to get the actual buffer with `get_data`. When you're done with the buffer, release it with `return_token`.

```c
#include "managed_pool.h"

init_pool();
token_t my_token = get_token();
uint8_t* my_buffer = get_data(my_token);
// do stuff with your buffer
// ...
return_token(my_token);
```

You'll get `0` from `get_token` if there are no free buffers and `NULL` from `get_data` if the token was already returned.
