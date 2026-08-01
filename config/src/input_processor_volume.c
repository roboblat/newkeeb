#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <drivers/input_processor.h>
#include <zephyr/dt-bindings/input/input-event-codes.h>
#include <dt-bindings/zmk/hid_usage_pages.h>
#include <dt-bindings/zmk/keys.h>
#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>

#define DT_DRV_COMPAT zmk_input_processor_volume

static int ip_vol_handle_event(const struct device *dev, struct input_event *event,
                              uint32_t param1, uint32_t param2,
                              struct zmk_input_processor_state *state) {
    if (event->type != INPUT_EV_REL || event->code != INPUT_REL_WHEEL) {
        return 0;
    }

    if (event->value != 0) {
        uint32_t keycode = (event->value > 0) ? C_VOL_UP : C_VOL_DN;

        ZMK_EVENT_RAISE(zmk_keycode_state_changed_create((struct zmk_keycode_state_changed){
            .usage_page = USAGE_CONSUMER,
            .keycode = keycode,
            .state = true,
            .timestamp = k_uptime_get(),
        }));

        ZMK_EVENT_RAISE(zmk_keycode_state_changed_create((struct zmk_keycode_state_changed){
            .usage_page = USAGE_CONSUMER,
            .keycode = keycode,
            .state = false,
            .timestamp = k_uptime_get(),
        }));
    }

    return 1;
}

static const struct zmk_input_processor_driver_api vol_api = {
    .handle_event = ip_vol_handle_event,
};

#define VOL_INST(n) \
    DEVICE_DT_INST_DEFINE(n, NULL, NULL, NULL, NULL, POST_KERNEL, \
                          CONFIG_APPLICATION_INIT_PRIORITY, &vol_api);

DT_INST_FOREACH_STATUS_OKAY(VOL_INST)
