#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

/*
 * Rotation is now handled in software by LVGL (see custom_status_screen.c), not
 * by the panel's MADCTL. Hardware display_set_orientation() had no visible effect
 * on this display, so this init is intentionally a no-op. The panel is left in its
 * native NORMAL orientation so it does not compound with the LVGL rotation.
 */
int disp_set_orientation(void)
{
	return 0;
}

SYS_INIT(disp_set_orientation, APPLICATION, 60);
