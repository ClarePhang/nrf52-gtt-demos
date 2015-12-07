/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#ifndef NRF_TWI_H__
#define NRF_TWI_H__
 
/**
 * @defgroup nrf_twi_hal TWI HAL
 * @{
 * @ingroup nrf_twi_master
 *
 * @brief Hardware access layer for managing the TWI peripheral.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "nrf.h"

/**
 * @brief TWI tasks.
 */
typedef enum
{
    /*lint -save -e30*/
    NRF_TWI_TASK_STARTRX = offsetof(NRF_TWI_Type, TASKS_STARTRX), ///< Start TWI receive sequence.
    NRF_TWI_TASK_STARTTX = offsetof(NRF_TWI_Type, TASKS_STARTTX), ///< Start TWI transmit sequence.
    NRF_TWI_TASK_STOP    = offsetof(NRF_TWI_Type, TASKS_STOP),    ///< Stop TWI transaction.
    NRF_TWI_TASK_SUSPEND = offsetof(NRF_TWI_Type, TASKS_SUSPEND), ///< Suspend TWI transaction.
    NRF_TWI_TASK_RESUME  = offsetof(NRF_TWI_Type, TASKS_RESUME)   ///< Resume TWI transaction.
    /*lint -restore*/
} nrf_twi_task_t;

/**
 * @brief TWI events.
 */
typedef enum
{
    /*lint -save -e30*/
    NRF_TWI_EVENT_STOPPED   = offsetof(NRF_TWI_Type, EVENTS_STOPPED),  ///< TWI stopped.
    NRF_TWI_EVENT_RXDREADY  = offsetof(NRF_TWI_Type, EVENTS_RXDREADY), ///< TWI RXD byte received.
    NRF_TWI_EVENT_TXDSENT   = offsetof(NRF_TWI_Type, EVENTS_TXDSENT),  ///< TWI TXD byte sent.
    NRF_TWI_EVENT_ERROR     = offsetof(NRF_TWI_Type, EVENTS_ERROR),    ///< TWI error.
    NRF_TWI_EVENT_BB        = offsetof(NRF_TWI_Type, EVENTS_BB),       ///< TWI byte boundary, generated before each byte that is sent or received.
    NRF_TWI_EVENT_SUSPENDED = offsetof(NRF_TWI_Type, EVENTS_SUSPENDED) ///< TWI entered the suspended state.
    /*lint -restore*/
} nrf_twi_event_t;

/**
 * @brief TWI shortcuts.
 */
typedef enum
{
    NRF_TWI_SHORT_BB_SUSPEND_MASK = TWI_SHORTS_BB_SUSPEND_Msk, ///< Shortcut between BB event and SUSPEND task.
    NRF_TWI_SHORT_BB_STOP_MASK    = TWI_SHORTS_BB_STOP_Msk,    ///< Shortcut between BB event and STOP task.
} nrf_twi_short_mask_t;

/**
 * @brief TWI interrupts.
 */
typedef enum
{
    NRF_TWI_INT_STOPPED_MASK    = TWI_INTENSET_STOPPED_Msk,  ///< Interrupt on STOPPED event.
    NRF_TWI_INT_RXDREADY_MASK   = TWI_INTENSET_RXDREADY_Msk, ///< Interrupt on RXDREADY event.
    NRF_TWI_INT_TXDSENT_MASK    = TWI_INTENSET_TXDSENT_Msk,  ///< Interrupt on TXDSENT event.
    NRF_TWI_INT_ERROR_MASK      = TWI_INTENSET_ERROR_Msk,    ///< Interrupt on ERROR event.
    NRF_TWI_INT_BB_MASK         = TWI_INTENSET_BB_Msk,       ///< Interrupt on BB event.
    NRF_TWI_INT_SUSPENDED_MASK  = TWI_INTENSET_SUSPENDED_Msk ///< Interrupt on SUSPENDED event.
} nrf_twi_int_mask_t;

/**
 * @brief TWI error source.
 */
typedef enum
{
    NRF_TWI_ERROR_ADDRESS_NACK = TWI_ERRORSRC_ANACK_Msk,  ///< NACK received after sending the address.
    NRF_TWI_ERROR_DATA_NACK    = TWI_ERRORSRC_DNACK_Msk,  ///< NACK received after sending a data byte.
    NRF_TWI_ERROR_OVERRUN      = TWI_ERRORSRC_OVERRUN_Msk ///< Overrun error.
                                                          /**< A new byte was received before previous byte got read by software
                                                           *   from the RXD register (previous data is lost). */
} nrf_twi_error_t;

/**
 * @brief TWI master clock frequency.
 */
typedef enum
{
    NRF_TWI_FREQ_100K = TWI_FREQUENCY_FREQUENCY_K100, ///< 100 kbps.
    NRF_TWI_FREQ_250K = TWI_FREQUENCY_FREQUENCY_K250, ///< 250 kbps.
    NRF_TWI_FREQ_400K = TWI_FREQUENCY_FREQUENCY_K400  ///< 400 kbps.
} nrf_twi_frequency_t;


/**
 * @brief Function for activating a specific TWI task.
 *
 * @param[in] p_twi TWI instance.
 * @param     task  Task to activate.
 */
__STATIC_INLINE void nrf_twi_task_trigger(NRF_TWI_Type * p_twi,
                                          nrf_twi_task_t task);

/**
 * @brief Function for getting the address of a specific TWI task register.
 *
 * @param[in] p_twi TWI instance.
 * @param     task  Desired task.
 *
 * @return Address of specified task register.
 */
__STATIC_INLINE uint32_t * nrf_twi_task_address_get(NRF_TWI_Type * p_twi,
                                                    nrf_twi_task_t task);

/**
 * @brief Function for clearing a specific TWI event.
 *
 * @param[in] p_twi TWI instance.
 * @param     event Event to clear.
 */
__STATIC_INLINE void nrf_twi_event_clear(NRF_TWI_Type * p_twi,
                                         nrf_twi_event_t event);

/**
 * @brief Function for returning the state of a specific event.
 *
 * @param[in] p_twi TWI instance.
 * @param[in] event Event.
 *
 * @retval true If the event is set.
 * @retval false If the event is not set.
 */
__STATIC_INLINE bool nrf_twi_event_check(NRF_TWI_Type  * p_twi,
                                         nrf_twi_event_t event);

/**
 * @brief Function for getting the address of a specific TWI event register.
 *
 * @param[in] p_twi TWI instance.
 * @param     event Desired event.
 *
 * @return Address of specified event register.
 */
__STATIC_INLINE uint32_t * nrf_twi_event_address_get(NRF_TWI_Type  * p_twi,
                                                     nrf_twi_event_t event);

/**
 * @brief Function for enabling specified shortcuts.
 *
 * @param[in] p_twi       TWI instance.
 * @param     shorts_mask Shortcuts to enable.
 */
__STATIC_INLINE void nrf_twi_shorts_enable(NRF_TWI_Type * p_twi,
                                           uint32_t shorts_mask);

/**
 * @brief Function for disabling specified shortcuts.
 *
 * @param[in] p_twi       TWI instance.
 * @param     shorts_mask Shortcuts to disable.
 */
__STATIC_INLINE void nrf_twi_shorts_disable(NRF_TWI_Type * p_twi,
                                            uint32_t shorts_mask);

/**
 * @brief Function for enabling specified interrupts.
 *
 * @param[in] p_twi    TWI instance.
 * @param     int_mask Interrupts to enable.
 */
__STATIC_INLINE void nrf_twi_int_enable(NRF_TWI_Type * p_twi,
                                        uint32_t int_mask);

/**
 * @brief Function for disabling specified interrupts.
 *
 * @param[in] p_twi   TWI instance.
 * @param     int_mask Interrupts to disable.
 */
__STATIC_INLINE void nrf_twi_int_disable(NRF_TWI_Type * p_twi,
                                         uint32_t int_mask);

/**
 * @brief Function for retrieving the state of a given interrupt.
 *
 * @param[in] p_twi    TWI instance.
 * @param     int_mask Interrupt to check.
 *
 * @retval true  if the interrupt is enabled,
 * @retval false otherwise.
 */
__STATIC_INLINE bool nrf_twi_int_enable_check(NRF_TWI_Type * p_twi,
                                              nrf_twi_int_mask_t int_mask);

/**
 * @brief Function for enabling the TWI peripheral.
 *
 * @param[in] p_twi TWI instance.
 */
__STATIC_INLINE void nrf_twi_enable(NRF_TWI_Type * p_twi);

/**
 * @brief Function for disabling the TWI peripheral.
 *
 * @param[in] p_twi TWI instance.
 */
__STATIC_INLINE void nrf_twi_disable(NRF_TWI_Type * p_twi);

/**
 * @brief Function for configuring TWI pins.
 *
 *
 * @param[in] p_twi   TWI instance.
 * @param     scl_pin SCL pin number.
 * @param     sda_pin SDA pin number.
 */
__STATIC_INLINE void nrf_twi_pins_set(NRF_TWI_Type * p_twi,
                                      uint32_t scl_pin,
                                      uint32_t sda_pin);

/**
 * @brief Function for setting the TWI master clock frequency.
 *
 * @param[in] p_twi     TWI instance.
 * @param     frequency TWI frequency.
 */
__STATIC_INLINE void nrf_twi_frequency_set(NRF_TWI_Type * p_twi,
                                           nrf_twi_frequency_t frequency);

/**
 * @brief Function for checking the TWI error source.
 *        The error flags are cleared after reading.
 *
 * @param[in] p_twi TWI instance.
 *
 * @return Mask with error source flags.
 */
__STATIC_INLINE uint32_t nrf_twi_errorsrc_get_and_clear(NRF_TWI_Type * p_twi);

/**
 * @brief Function for setting the address to be used in TWI transfers.
 *
 * @param[in] p_twi   TWI instance.
 * @param[in] address Address to be used in transfers.
 */
__STATIC_INLINE void nrf_twi_address_set(NRF_TWI_Type * p_twi, uint8_t address);

/**
 * @brief Function for reading data received by TWI.
 *
 * @param[in] p_twi TWI instance.
 *
 * @return Received data.
 */
__STATIC_INLINE uint8_t nrf_twi_rxd_get(NRF_TWI_Type * p_twi);

/**
 * @brief Function for writing data to be transmitted by TWI.
 *
 * @param[in] p_twi TWI instance.
 * @param[in] data  Data to be transmitted.
 */
__STATIC_INLINE void nrf_twi_txd_set(NRF_TWI_Type * p_twi, uint8_t data);

__STATIC_INLINE void nrf_twi_shorts_set(NRF_TWI_Type * p_twi,
                                        uint32_t shorts_mask);

/**
 * @}
 */


#ifndef SUPPRESS_INLINE_IMPLEMENTATION

__STATIC_INLINE void nrf_twi_task_trigger(NRF_TWI_Type * p_twi,
                                          nrf_twi_task_t task)
{
    *((volatile uint32_t *)((uint8_t *)p_twi + (uint32_t)task)) = 0x1UL;
}

__STATIC_INLINE uint32_t * nrf_twi_task_address_get(NRF_TWI_Type * p_twi,
                                                    nrf_twi_task_t task)
{
    return (uint32_t *)((uint8_t *)p_twi + (uint32_t)task);
}

__STATIC_INLINE void nrf_twi_event_clear(NRF_TWI_Type  * p_twi,
                                         nrf_twi_event_t event)
{
    *((volatile uint32_t *)((uint8_t *)p_twi + (uint32_t)event)) = 0x0UL;
}

__STATIC_INLINE bool nrf_twi_event_check(NRF_TWI_Type  * p_twi,
                                         nrf_twi_event_t event)
{
    return (bool)*(volatile uint32_t *)((uint8_t *)p_twi + (uint32_t)event);
}

__STATIC_INLINE uint32_t * nrf_twi_event_address_get(NRF_TWI_Type  * p_twi,
                                                     nrf_twi_event_t event)
{
    return (uint32_t *)((uint8_t *)p_twi + (uint32_t)event);
}

__STATIC_INLINE void nrf_twi_shorts_enable(NRF_TWI_Type * p_twi,
                                           uint32_t shorts_mask)
{
    p_twi->SHORTS |= shorts_mask;
}

__STATIC_INLINE void nrf_twi_shorts_disable(NRF_TWI_Type * p_twi,
                                            uint32_t shorts_mask)
{
    p_twi->SHORTS &= ~(shorts_mask);
}

__STATIC_INLINE void nrf_twi_int_enable(NRF_TWI_Type * p_twi,
                                        uint32_t int_mask)
{
    p_twi->INTENSET = int_mask;
}

__STATIC_INLINE void nrf_twi_int_disable(NRF_TWI_Type * p_twi,
                                         uint32_t int_mask)
{
    p_twi->INTENCLR = int_mask;
}

__STATIC_INLINE bool nrf_twi_int_enable_check(NRF_TWI_Type * p_twi,
                                              nrf_twi_int_mask_t int_mask)
{
    return (bool)(p_twi->INTENSET & int_mask);
}

__STATIC_INLINE void nrf_twi_enable(NRF_TWI_Type * p_twi)
{
    p_twi->ENABLE = (TWI_ENABLE_ENABLE_Enabled << TWI_ENABLE_ENABLE_Pos);
}

__STATIC_INLINE void nrf_twi_disable(NRF_TWI_Type * p_twi)
{
    p_twi->ENABLE = (TWI_ENABLE_ENABLE_Disabled << TWI_ENABLE_ENABLE_Pos);
}

__STATIC_INLINE void nrf_twi_pins_set(NRF_TWI_Type * p_twi,
                                      uint32_t scl_pin,
                                      uint32_t sda_pin)
{
    p_twi->PSELSCL = scl_pin;
    p_twi->PSELSDA = sda_pin;
}

__STATIC_INLINE void nrf_twi_frequency_set(NRF_TWI_Type * p_twi,
                                           nrf_twi_frequency_t frequency)
{
    p_twi->FREQUENCY = frequency;
}

__STATIC_INLINE uint32_t nrf_twi_errorsrc_get_and_clear(NRF_TWI_Type * p_twi)
{
    uint32_t error_source = p_twi->ERRORSRC;

    // [error flags are cleared by writing '1' on their position]
    p_twi->ERRORSRC = error_source;

    return error_source;
}

__STATIC_INLINE void nrf_twi_address_set(NRF_TWI_Type * p_twi, uint8_t address)
{
    p_twi->ADDRESS = address;
}

__STATIC_INLINE uint8_t nrf_twi_rxd_get(NRF_TWI_Type * p_twi)
{
    return (uint8_t)p_twi->RXD;
}

__STATIC_INLINE void nrf_twi_txd_set(NRF_TWI_Type * p_twi, uint8_t data)
{
    p_twi->TXD = data;
}

__STATIC_INLINE void nrf_twi_shorts_set(NRF_TWI_Type * p_twi,
                                        uint32_t shorts_mask)
{
    p_twi->SHORTS = shorts_mask;
}

#endif // SUPPRESS_INLINE_IMPLEMENTATION

#endif // NRF_TWI_H__
