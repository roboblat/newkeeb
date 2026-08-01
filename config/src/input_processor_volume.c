#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <drivers/input_processor.h>
#include <zephyr/dt-bindings/input/input-event-codes.h>
#include <zmk/hid.h>
#include <zmk/endpoints.h>

#define DT_DRV_COMPAT zmk_input_processor_volume

static int ip_vol_handle_event(const struct device *dev, struct input_event *event,
                              uint32_t param1, uint32_t param2,
                              struct zmk_input_processor_state *state) {
    if (event->type != INPUT_EV_REL || event->code != INPUT_REL_WHEEL) {
        return 0;
    }

    if (event->value > 0) {
        zmk_hid_consumer_press(HID_USAGE_CONSUMER_VOLUME_INCREMENT);
        zmk_endpoints_send_report(ZMK_HID_REPORT_TYPE_CONSUMER);
        zmk_hid_consumer_release(HID_USAGE_CONSUMER_VOLUME_INCREMENT);
        zmk_endpoints_send_report(ZMK_HID_REPORT_TYPE_CONSUMER);
    } else if (event->value < 0) {
        zmk_hid_consumer_press(HID_USAGE_CONSUMER_VOLUME_DECREMENT);
        zmk_endpoints_send_report(ZMK_HID_REPORT_TYPE_CONSUMER);
        zmk_hid_consumer_release(HID_USAGE_CONSUMER_VOLUME_DECREMENT);
        zmk_endpoints_send_report(ZMK_HID_REPORT_TYPE_CONSUMER);
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
