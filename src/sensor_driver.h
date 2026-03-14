/**
 * sensor_driver.h
 *
 * Flight software driver interface for a generic sensor device.
 * Provides open/read/close lifecycle and a simple data structure
 * for sensor readings.
 */

#ifndef SENSOR_DRIVER_H
#define SENSOR_DRIVER_H

#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Status codes                                                        */
/* ------------------------------------------------------------------ */
typedef enum {
    SENSOR_OK          =  0,  /**< Operation succeeded                */
    SENSOR_ERR_INIT    = -1,  /**< Initialisation failed              */
    SENSOR_ERR_READ    = -2,  /**< Read operation failed              */
    SENSOR_ERR_CLOSED  = -3,  /**< Driver is not open                 */
    SENSOR_ERR_PARAM   = -4   /**< Invalid parameter supplied         */
} sensor_status_t;

/* ------------------------------------------------------------------ */
/* Data types                                                          */
/* ------------------------------------------------------------------ */

/** Raw sensor reading returned by sensor_read(). */
typedef struct {
    int16_t  temperature_cdegC; /**< Temperature in centi-degrees C   */
    uint16_t pressure_hPa;      /**< Pressure in hecto-Pascals        */
    uint32_t timestamp_ms;      /**< Milliseconds since driver opened */
} sensor_data_t;

/** Opaque driver handle. Callers must not inspect its contents. */
typedef struct sensor_handle sensor_handle_t;

/* ------------------------------------------------------------------ */
/* Driver interface                                                    */
/* ------------------------------------------------------------------ */

/**
 * Open and initialise the sensor driver.
 *
 * @param[out] handle  Pointer-to-pointer that will receive the handle.
 * @return SENSOR_OK on success, or a negative error code.
 */
sensor_status_t sensor_open(sensor_handle_t **handle);

/**
 * Read the latest sensor data.
 *
 * @param[in]  handle  Handle returned by sensor_open().
 * @param[out] data    Pointer to caller-allocated structure to fill.
 * @return SENSOR_OK on success, or a negative error code.
 */
sensor_status_t sensor_read(sensor_handle_t *handle, sensor_data_t *data);

/**
 * Close the sensor driver and release all resources.
 *
 * After this call, @p handle is invalid and must not be used.
 *
 * @param[in] handle  Handle returned by sensor_open().
 * @return SENSOR_OK on success, or a negative error code.
 */
sensor_status_t sensor_close(sensor_handle_t *handle);

#endif /* SENSOR_DRIVER_H */
