#ifndef _MGOS_CAN_H_
#define _MGOS_CAN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <stdbool.h>

#include "mgos.h"
#include "mgos_init.h"
#include "mgos_features.h"
#include "mgos_mongoose.h"
#include "mgos_sys_config.h"

#include "driver/can.h"

void mgos_can_general_config();
void mgos_can_timing_config();
void mgos_can_filter_config();
bool mgos_can_driver_install(void);
bool mgos_can_start(void);
bool mgos_can_stop(void);
bool mgos_can_driver_uninstall(void);
//bool mgos_can_transmit(uint8_t *data, uint64_t ms);
bool mgos_can_transmit(can_message_t *data, uint64_t ms);

#ifdef __cplusplus
}
#endif

#endif
