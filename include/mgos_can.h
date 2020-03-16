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

/**********************************************************************!
 * @fn          mgos_can_general_config(void)
 * @brief       CAN general configurations (Mode, CAN_TX pin and CAN_RX pin)
 * @param[in]   void
 * @return      No return value
 **********************************************************************/
void mgos_can_general_config();

/**********************************************************************!
 * @fn          mgos_can_timing_config(void)
 * @brief       CAN timing configurations (Baudrate)
 * @param[in]   void
 * @return      No return value
 **********************************************************************/
void mgos_can_timing_config();

/**********************************************************************!
 * @fn          mgos_can_filter_config(void)
 * @brief       CAN filter configurations to accept all ID's
 * @param[in]   void
 * @return      No return value
 **********************************************************************/
void mgos_can_filter_config();

/**********************************************************************!
 * @fn          mgos_can_driver_install(void)
 * @brief       CAN driver installation
 * @param[in]   void
 * @return      bool (status of CAN driver installation)
 **********************************************************************/
bool mgos_can_driver_install(void);

/**********************************************************************!
 * @fn          mgos_can_start(void)
 * @brief       Start CAN driver, put the CAN driver in running mode
 * @param[in]   void
 * @return      bool (status of CAN driver start)
 **********************************************************************/
bool mgos_can_start(void);

/**********************************************************************!
 * @fn          mgos_can_stop(void)
 * @brief       Stop CAN driver, preventing any further message from being
 * 		transmitted or received until can_start() is called
 * @param[in]   void
 * @return      bool (status of CAN driver start)
 **********************************************************************/
bool mgos_can_stop(void);

/**********************************************************************!
 * @fn          mgos_can_driver_uninstall(void)
 * @brief       Uninstall CAN driver,  freeing the memory utilized by the driver. 
 *      	This function can only be called when the driver is in the stopped
 *              state or the bus-off state.
 * @param[in]   void
 * @return      bool (status of CAN driver start)
 **********************************************************************/
bool mgos_can_driver_uninstall(void);

/**********************************************************************!
 * @fn          mgos_can_transmit(void)
 * @brief       Transmit a CAN message.
 * @param[in]   can_message_t *can_data, uint64_t ms
 * @return      bool (status of CAN driver start)
 **********************************************************************/
bool mgos_can_transmit(can_message_t *data, uint64_t ms);

#ifdef __cplusplus
}
#endif

#endif
