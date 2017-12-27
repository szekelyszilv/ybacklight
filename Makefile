$(VERBOSE).SILENT:

BUILD_DIR:=build

ifeq "$(BUILD_TYPE)" ""
	BUILD_TYPE="release"
endif

.PHONY: all
all: $(BUILD_DIR)/build.ninja
	ninja -C $(BUILD_DIR)

$(BUILD_DIR)/build.ninja:
	$(MAKE) configure

.PHONY: configure
configure:
	@meson --buildtype=$(BUILD_TYPE) $(BUILD_DIR)

.PHONY: clean
clean:
	-if [ -d $(BUILD_DIR) ]; then ninja -C $(BUILD_DIR) clean; fi

.PHONY: install
install: $(BUILD_DIR)/build.ninja
	ninja -C $(BUILD_DIR) install
