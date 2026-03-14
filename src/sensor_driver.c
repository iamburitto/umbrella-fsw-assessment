/**
 * sensor_driver.c
 *
 * Implementation of the flight software sensor driver.
 *
 * In a real embedded target this file would talk to hardware registers
 * or a HAL.  Here the "hardware" is simulated so that the driver logic
 * can be built and tested on any host machine.
 */

#include "sensor_driver.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Private types                                                       */
/* ------------------------------------------------------------------ */

struct sensor_handle {
    int      is_open;       /**< Non-zero when the driver is open     */
    uint32_t read_count;    /**< Number of successful reads performed */
};

/* ------------------------------------------------------------------ */
/* Simulated hardware helpers                                         */
/* ------------------------------------------------------------------ */

/**
 * hw_init() – would configure hardware registers on a real target.
 * Returns 0 on success, non-zero on failure.
 */
static int hw_init(void)
{
    /* Simulated hardware always initialises successfully. */
    return 0;
}

/**
 * hw_read_temperature() – returns a simulated temperature value in
 * centi-degrees Celsius (e.g. 2350 = 23.50 °C).
 */
static int16_t hw_read_temperature(void)
{
    return 2350; /* 23.50 °C */
}

/**
 * hw_read_pressure() – returns a simulated pressure in hPa.
 */
static uint16_t hw_read_pressure(void)
{
    return 1013; /* standard atmosphere */
}

/* ------------------------------------------------------------------ */
/* Public interface                                                    */
/* ------------------------------------------------------------------ */

sensor_status_t sensor_open(sensor_handle_t **handle)
{
    if (handle == NULL) {
        return SENSOR_ERR_PARAM;
    }

    sensor_handle_t *h = (sensor_handle_t *)malloc(sizeof(sensor_handle_t));
    if (h == NULL) {
        return SENSOR_ERR_INIT;
    }

    if (hw_init() != 0) {
        free(h);
        return SENSOR_ERR_INIT;
    }

    h->is_open    = 1;
    h->read_count = 0U;

    *handle = h;
    return SENSOR_OK;
}

sensor_status_t sensor_read(sensor_handle_t *handle, sensor_data_t *data)
{
    if (handle == NULL || data == NULL) {
        return SENSOR_ERR_PARAM;
    }

    if (!handle->is_open) {
        return SENSOR_ERR_CLOSED;
    }

    data->temperature_cdegC = hw_read_temperature();
    data->pressure_hPa      = hw_read_pressure();
    data->timestamp_ms      = handle->read_count * 100U; /* 100 ms per tick */

    handle->read_count++;
    return SENSOR_OK;
}

sensor_status_t sensor_close(sensor_handle_t *handle)
{
    if (handle == NULL) {
        return SENSOR_ERR_PARAM;
    }

    if (!handle->is_open) {
        return SENSOR_ERR_CLOSED;
    }

    handle->is_open = 0;
    free(handle);
    return SENSOR_OK;
}
