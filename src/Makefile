BUILD_DIR=build

NRSSL_PATH:=$(dir $(realpath $(lastword $(MAKEFILE_LIST))))jars

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/Makefile: CMakeLists.txt | $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -DLLVM_DIR=$(shell llvm-config-19 --cmakedir)

build_all: $(BUILD_DIR)/Makefile
	$(MAKE) -C $(BUILD_DIR)

run-test: build_all
	NRSSL_JARS=$(NRSSL_PATH) $(BUILD_DIR)/TestPass

clean:
	rm -rf $(BUILD_DIR)

.PHONY: build_all run-test clean clean-pass
