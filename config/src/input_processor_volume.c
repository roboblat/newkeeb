#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <drivers/input_processor.h>
#include <zephyr/dt-bindings/input/input-event-codes.h>
#include <dt-bindings/zmk/keys.h>
#include <zmk/events/keycode_state_changed.h>

#define DT_DRV_COMPAT zmk_input_processor_volume

static int ip_vol_handle_event(const struct device *dev, struct input_event *event,
                              uint32_t param1, uint32_t param2,
                              struct zmk_input_processor_state *state) {
    if (event->type != INPUT_EV_REL) {
        return ZMK_INPUT_PROC_CONTINUE;
    }

    if (event->code == INPUT_REL_WHEEL && event->value != 0) {
        uint32_t keycode = (event->value > 0) ? C_VOL_UP : C_VOL_DN;
        int64_t now = k_uptime_get();

        raise_zmk_keycode_state_changed_from_encoded(keycode, true, now);
        raise_zmk_keycode_state_changed_from_encoded(keycode, false, now);
    }

    return ZMK_INPUT_PROC_STOP;
}

static const struct zmk_input_processor_driver_api vol_api = {
    .handle_event = ip_vol_handle_event,
};

#define VOL_INST(n) \
    DEVICE_DT_INST_DEFINE(n, NULL, NULL, NULL, NULL, POST_KERNEL, \
                          CONFIG_APPLICATION_INIT_PRIORITY, &vol_api);

DT_INST_FOREACH_STATUS_OKAY(VOL_INST)
