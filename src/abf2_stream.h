#include "deserialize.h"
#include "stream.h"
#include "abf2_struct.h"

StreamError abf2_stream_read_guid(stream_dt *stream, struct guid *guid, bool to_swap);
