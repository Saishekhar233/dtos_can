#include "mgos_can.h"

#define CAN_FRAME_MAX_DLC 8 /**< Max data bytes allowed in CAN2.0 */

typedef struct
{
  union {
    struct
    {
      //The order of these bits must match deprecated message flags for compatibility reasons
      uint32_t extd : 1;         /**< Extended Frame Format (29bit ID) */
      uint32_t rtr : 1;          /**< Message is a Remote Transmit Request */
      uint32_t ss : 1;           /**< Transmit as a Single Shot Transmission. Unused for received. */
      uint32_t self : 1;         /**< Transmit as a Self Reception Request. Unused for received. */
      uint32_t dlc_non_comp : 1; /**< Message's Data length code is larger than 8. This will break compliance with CAN2.0B. */
      uint32_t reserved : 27;    /**< Reserved bits */
    } info_can_struct;
    //Todo: Deprecate flags
    uint32_t flags; /**< Alternate way to set message flags using message flag macros (see documentation) */
  } info_can_union;
  uint32_t identifier;             /**< 11 or 29 bit identifier */
  uint8_t data_length_code;        /**< Data length code */
  uint8_t data[CAN_FRAME_MAX_DLC]; /**< Data bytes (not relevant in RTR frame) */
} can_message_u;

can_general_config_t g_config = CAN_GENERAL_CONFIG_DEFAULT(GPIO_NUM_21/*mgos_sys_config_get_can_tx_pin()*/, 
                                                           GPIO_NUM_22/*mgos_sys_config_get_can_rx_pin()*/, 
                                                           CAN_MODE_NORMAL/*mgos_sys_config_get_can_mode()*/);
can_timing_config_t t_config = CAN_TIMING_CONFIG_500KBITS(); //CAN_TIMING_CONFIG_500KBITS();
can_filter_config_t f_config =  CAN_FILTER_CONFIG_ACCEPT_ALL(); // CAN_FILTER_CONFIG_ACCEPT_ALL();

void mgos_can_general_config(void)
{
    g_config.mode = mgos_sys_config_get_can_mode();
    g_config.tx_io = mgos_sys_config_get_can_tx_pin();
    g_config.rx_io = mgos_sys_config_get_can_rx_pin();
}

void mgos_can_timing_config(void)
{
  if (mgos_sys_config_get_can_baud() == 500)
  {
    t_config.brp = 8;
    t_config.tseg_1 = 15;
    t_config.tseg_2 = 4;
    t_config.sjw = 3;
    t_config.triple_sampling = false;
  }
  else if (mgos_sys_config_get_can_baud() == 250)
  {
    t_config.brp = 16;
    t_config.tseg_1 = 15;
    t_config.tseg_2 = 4;
    t_config.sjw = 3;
    t_config.triple_sampling = false;
  }
  else
  {
      //Default to 500KBits
    t_config.brp = 8;
    t_config.tseg_1 = 15;
    t_config.tseg_2 = 4;
    t_config.sjw = 3;
    t_config.triple_sampling = false;
  }
  
}

void mgos_can_filter_config(void)
{
    f_config.acceptance_code = 0;
    f_config.acceptance_mask = 0xFFFFFFFF; 
    f_config.single_filter = true;
}

bool mgos_can_driver_install(void)
{
    esp_err_t ret;
    bool status = 0;
    ret = can_driver_install(&g_config, &t_config, &f_config);
    switch(ret)
    {
        case ESP_OK: 
                status = true; break;

        case ESP_ERR_INVALID_ARG: 
                LOG(LL_ERROR, ("FAILED: Arguments are invalid"));
                status = false; break;

        case ESP_ERR_NO_MEM:
                LOG(LL_ERROR, ("FAILED: Insufficient memory"));
                status = false; break;

        case ESP_ERR_INVALID_STATE:
                LOG(LL_ERROR, ("FAILED: Driver is already installed"));
                status = false; break;

        default: break;
    }
    return status;
}

bool mgos_can_start(void)
{
    esp_err_t ret;
    bool status = 0;
    ret = can_start();
    switch(ret)
    {
        case ESP_OK: 
                status = true; break;

        case ESP_ERR_INVALID_STATE:
                LOG(LL_ERROR, ("FAILED: Driver is not in stopped state, or is not installed"));
                status = false; break;

        default: break;
    }
    return status;
}

bool mgos_can_stop(void)
{
    esp_err_t ret;
    bool status = 0;
    ret = can_stop();
    switch(ret)
    {
        case ESP_OK: 
                status = true; break;

        case ESP_ERR_INVALID_STATE:
                LOG(LL_ERROR, ("FAILED: Driver is not in stopped state, or is not installed"));
                status = false; break;

        default: break;
    }
    return status;
}

bool mgos_can_driver_uninstall(void)
{
    esp_err_t ret;
    bool status = 0;
    ret = can_driver_uninstall();
    switch(ret)
    {
        case ESP_OK: 
                status = true; break;

        case ESP_ERR_INVALID_STATE:
                LOG(LL_ERROR, ("FAILED: Driver is not in stopped/bus-off state, or is not installed"));
                status = false; break;

        default: break;
    }
    return status;
}

bool mgos_can_transmit(uint8_t *can_data, uint64_t ms)
{
    can_message_u message; 
    esp_err_t ret;
    bool status = 0;
    uint8_t temp_buf[CAN_FRAME_MAX_DLC] = {0};
    //strcpy((char *)temp_buf, (char *)can_data);
    memcpy(temp_buf, can_data, strlen((char *)can_data));
    //strcpy((char *)message.data, (char *)temp_buf);
    memcpy(message.data, temp_buf, strlen((char *)temp_buf));
    message.identifier = mgos_sys_config_get_can_identifier();//0x18FFFA64;
    message.info_can_union.info_can_struct.extd = mgos_sys_config_get_can_extd();//1;
    message.data_length_code = mgos_sys_config_get_can_dlc();//CAN_FRAME_MAX_DLC; //8;

    ret = can_transmit((can_message_t *)&message, pdMS_TO_TICKS(ms));
    switch(ret)
    {
        case ESP_OK: 
                status = true; break;

        case ESP_ERR_INVALID_ARG: 
                LOG(LL_ERROR, ("FAILED: Arguments are invalid"));
                status = false; break;

        case ESP_ERR_TIMEOUT:
                LOG(LL_ERROR, ("FAILED: Timed out waiting for space on TX queue"));
                status = false; break;

        case ESP_FAIL:
                LOG(LL_ERROR, ("FAILED: TX queue is disabled and another message is currently transmitting"));
                status = false; break;

        case ESP_ERR_INVALID_STATE:
                LOG(LL_ERROR, ("FAILED: Driver is already installed"));
                status = false; break;

        case ESP_ERR_NOT_SUPPORTED:
                LOG(LL_ERROR, ("FAILED: Listen Only Mode does not support transmissions"));
                status = false; break;

        default: break;
    }
    return status;
}

bool mgos_mgos_can_init(void)
{
    return true;
}
