#include <pebble.h>
#include "storage.h"
#include "stringbuffer.h"
#include "logging_helper.h"

// Indices to storage and data to access copies of stored values.
enum {
    SELECTED_VERSION = 0x0,
    BATTERY_ESTIMATE = 0x10001,
    LAST_FULL_TIMESTAMP = 0x10002,
    BATTERY_DISPLAY = 0x10003,
    MAXIMUM_EVER_USED = 0x10003,
};
storage_t storage;

static void app_log_storage_log(unsigned int how) {
    battery_estimate_data* be = &(storage.battery_estimate);

    LOG_EXT(how, "storage.selectedVersion: %s", storage.selectedVersion);
    LOG_EXT(how, "storage.battery_estimate.previous_state_timestamp: %ld", be->previous_state_timestamp);
    LOG_EXT(how, "storage.battery_estimate.previous_state.charge_percent: %d", be->previous_state.charge_percent);
    LOG_EXT(how, "storage.battery_estimate.previous_state.is_charging: %d", be->previous_state.is_charging);
    LOG_EXT(how, "storage.battery_estimate.previous_state.is_plugged: %d", be->previous_state.is_plugged);
    LOG_EXT(how, "storage.battery_estimate.average_data_write_head: %d", be->average_data_write_head);
    for (int i = 0; i < battery_estimate_data_average_data_num; i++) {
        LOG_EXT(how, "storage.battery_estimate.averate_data[%d]: %ld", i, be->averate_data[i]);
    }
    LOG_EXT(how, "storage.last_full_timestamp: %ld", storage.last_full_timestamp);
    LOG_EXT(how, "storage.battery_display: %d", storage.battery_display);
}

// Makes sure storage is populated (by checking and, on absence, writing
// defaults) and then reading values.
void storage_init(void) {
    LOG_FUNC();

    // THIS MUST NEVER BE ENABLED */for (unsigned int i = 0; i <= MAXIMUM_EVER_USED; i++) {
    // THIS MUST NEVER BE ENABLED */    if ((i % 10000) == 0) {
    // THIS MUST NEVER BE ENABLED */        LOG_EXT(LOG_STORAGE, "storage_init(): working key %d", i);
    // THIS MUST NEVER BE ENABLED */    }
    // THIS MUST NEVER BE ENABLED */    if (persist_exists(i)) {
    // THIS MUST NEVER BE ENABLED */        LOG_EXT(LOG_STORAGE, "storage_init(): deleting key %d", i);
    // THIS MUST NEVER BE ENABLED */        persist_delete(i);
    // THIS MUST NEVER BE ENABLED */    }
    // THIS MUST NEVER BE ENABLED */}

    if (!persist_exists(SELECTED_VERSION)) {
        persist_write_string(SELECTED_VERSION, "Regular");
        LOG_EXT(LOG_STORAGE, "storage_init(): init %d with data %s", SELECTED_VERSION, "Regular");
    }
    persist_read_string(SELECTED_VERSION, storage.selectedVersion, 64);
    if (!persist_exists(BATTERY_ESTIMATE)) {
        persist_write_data(BATTERY_ESTIMATE, (void*)&battery_estimate_data_init, sizeof(battery_estimate_data));
        LOG_EXT(LOG_STORAGE, "storage_init(): init %d", BATTERY_ESTIMATE);
    }
    persist_read_data(BATTERY_ESTIMATE, (void*)&(storage.battery_estimate), sizeof(battery_estimate_data));
    if (!persist_exists(LAST_FULL_TIMESTAMP)) {
        persist_write_int(LAST_FULL_TIMESTAMP, (time_t)-1);
        LOG_EXT(LOG_STORAGE, "storage_init(): init %d with data %ld", LAST_FULL_TIMESTAMP, (time_t)-1);
    }
    storage.last_full_timestamp = persist_read_int(LAST_FULL_TIMESTAMP);
    if (!persist_exists(BATTERY_DISPLAY)) {
        persist_write_int(BATTERY_DISPLAY, 1);
        LOG_EXT(LOG_STORAGE, "storage_init(): init %d with data %d", BATTERY_DISPLAY, 1);
    }
    storage.battery_display = persist_read_int(BATTERY_DISPLAY);

    app_log_storage_log(LOG_STORAGE_SU);
}
    
// Write back values.
void storage_persist(void) {
    LOG_FUNC();

    persist_write_string(SELECTED_VERSION, storage.selectedVersion);
    persist_write_data(BATTERY_ESTIMATE, (void*)&(storage.battery_estimate), sizeof(battery_estimate_data));
    persist_write_int(LAST_FULL_TIMESTAMP, storage.last_full_timestamp);
    persist_write_int(BATTERY_DISPLAY, storage.battery_display);

    app_log_storage_log(LOG_STORAGE);
}

// Deinitalization, currently just writes back values but might do more in the
// future.
void storage_deinit(void) {
    LOG_FUNC();

    storage_persist();
}
