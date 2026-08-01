# zmk-pointing-acceleration includes <drivers/input_processor.h>, which lives in
# ZMK's app-private include dir and is NOT exported to modules. Put it on the
# global include path so the module builds against current ZMK / Zephyr 4.1.
# config/ and zmk/ are siblings in the west workspace, so ../zmk resolves it.
zephyr_include_directories(${CMAKE_CURRENT_LIST_DIR}/../zmk/app/include)
