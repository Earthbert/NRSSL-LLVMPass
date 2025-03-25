BUILD_DIR=build
TEST_DIR=test

NRSSL_PATH:=$(dir $(realpath $(lastword $(MAKEFILE_LIST))))jars

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/Makefile: CMakeLists.txt | $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug

build: $(BUILD_DIR)/Makefile
	$(MAKE) -C $(BUILD_DIR)

run-test: build
	NRSSL_JARS=$(NRSSL_PATH) $(BUILD_DIR)/TestPass

clean:
	rm -rf $(BUILD_DIR)

.PHONY: build run-test clean clean-pass
