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

/**@file
 *
 * @defgroup lib_driver_swi SWI driver
 * @{
 * @ingroup  nrf_drivers
 *
 * @brief    Driver for software interrupts (SWI).
 * @details  The SWI driver allows the user to allocate SWIs and pass extra flags to interrupt handler functions.
 */

#ifndef NRF_DRV_SWI_H__
#define NRF_DRV_SWI_H__

#include <stdbool.h>
#include <stdint.h>
#include "app_util.h"
#include "app_util_platform.h"
#include "nrf_drv_config.h"
#include "sdk_errors.h"


typedef uint32_t nrf_swi_t;         ///< @brief SWI channel (unsigned integer).

/**  @brief   SWI user flags (unsigned integer).
 *   @details User flags are set during the SWI trigger and passed to the callback function as an argument.
 */
typedef uint16_t nrf_swi_flags_t;

#define NRF_SWI_UNALLOCATED ((nrf_swi_t) 0xFFFFFFFFuL)  ///< @brief Unallocated channel value.

/** @brief SWI handler. Takes two arguments: SWI number (nrf_swi_t) and flags (nrf_swi_flags_t). */
typedef void (* nrf_swi_handler_t)(nrf_swi_t, nrf_swi_flags_t);

/**@brief Maximum numbers of SWIs. This number is fixed for a specific chip. */
#define SWI_MAX              6

/**@brief Number of flags per SWI (fixed number). */
#define SWI_MAX_FLAGS        16

#ifndef SWI_COUNT
/**@brief Number of software interrupts available. This number can be set in the range from 1 to SWI_MAX. */
#define SWI_COUNT 4
#endif

#ifdef SOFTDEVICE_PRESENT
    #if SWI_COUNT > 2
        #undef SWI_COUNT
        #define SWI_COUNT 2
    #endif
#else
    #ifdef SVCALL_AS_NORMAL_FUNCTION
    // Serialization is enabled.
        #if SWI_COUNT > 2
            #undef SWI_COUNT
            #define SWI_COUNT 2
        #endif
    #endif
#endif

/**@brief Default SWI priority. */
#define SWI_DEFAULT_PRIORITY APP_IRQ_PRIORITY_LOW


/**@brief Function for initializing the SWI module.
 *
 * @retval     NRF_SUCCESS                If the module was successfully initialized.
 * @retval     MODULE_ALREADY_INITIALIZED If the module has already been initialized.
 */
ret_code_t nrf_drv_swi_init(void);


/**@brief Function for uninitializing the SWI module.
 *
 * This function also disables all SWIs.
 *
 * @retval     NRF_SUCCESS             If the module was successfully uninitialized.
 * @retval     NRF_ERROR_INVALID_STATE If the module has not been initialized yet.
 */
void nrf_drv_swi_uninit(void);


/**@brief Function for allocating a first unused SWI instance and setting a handler.
 * @details The event handler function returns void and takes one uint32_t argument (SWI number).
 *
 * @param[out] p_swi                   Pointer to the SWI that has been allocated.
 * @param[in]  event_handler           Event handler function (must not be NULL).
 * @param[in]  priority                Interrupt priority.
 *
 * @retval     NRF_SUCCESS             If the SWI was successfully allocated.
 * @retval     NRF_ERROR_NO_MEM        If there is no available SWI to be used.
 */
ret_code_t nrf_drv_swi_alloc(nrf_swi_t * p_swi, nrf_swi_handler_t event_handler, uint32_t priority);


/**@brief Function for freeing a previously allocated SWI.
 *
 * @param[in]  p_swi                 SWI to free.
 */
void nrf_drv_swi_free(nrf_swi_t * p_swi);


/**@brief Function for triggering the SWI.
 *
 * @param[in]  swi           SWI to trigger.
 * @param[in]  flag_number   Number of user flag to trigger.
 */
void nrf_drv_swi_trigger(nrf_swi_t swi, uint8_t flag_number);

#endif

/** @} */
