#include "battery_estimate_data.h"
#include "stringbuffer.h"
#include "logging_helper.h"

const battery_estimate_data battery_estimate_data_init = {
    0,
    { 255, false, false },
    battery_estimate_data_average_data_num-1,
    { 43200, 43200, 43200, 43200, 43200, 43200, 43200, 43200, 43200, 43200 }
};

void battery_estimate_data_log(time_t* data, size_t size, unsigned int how) {
    for (unsigned int i = 0; i < size; i++) {
        stringbuffer time;
        stringbuffer_init(&time);
        stringbuffer_append_ti(&time, data[i]);
        LOG_EXT(how, "storage.battery_estimate.averate_data[%d]: %s", i, time.retval);
    }
}
