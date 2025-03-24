BUILD_DIR=build
TEST_DIR=test

LIBRARY_NAME=libIEEEToPosit

NRSSL_PATH:=$(dir $(realpath $(lastword $(MAKEFILE_LIST))))jars

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/Makefile: CMakeLists.txt | $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug

$(BUILD_DIR)/$(LIBRARY_NAME).so: build-pass

$(TEST_DIR)/test.ll: $(TEST_DIR)/test.c
	clang $< -S -emit-llvm -o $@

.PHONY: cmake-build build-pass build-test run-pass clean clean-pass

cmake-build: $(BUILD_DIR)/Makefile

build-pass:
	cd $(BUILD_DIR) && make

build-test: $(TEST_DIR)/test.ll

run-pass: $(TEST_DIR)/test.ll $(BUILD_DIR)/$(LIBRARY_NAME).so
	NRSSL_JARS=$(NRSSL_PATH) LD_PRELOAD=${JAVA_HOME}/lib/server/libjvm.so opt \
	-load-pass-plugin=$(BUILD_DIR)/$(LIBRARY_NAME).so -passes=ieee-to-posit \
	$(TEST_DIR)/test.ll -S \
	 -o $(TEST_DIR)/test_edited.ll

clean:
	rm -rf $(TEST_DIR)/*.ll *.o

clean-pass:
	rm -rf build/