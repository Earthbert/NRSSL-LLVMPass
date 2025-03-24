#pragma once

#include <cstdint>
#include <jni.h>

enum Type { POSIT32, POSIT64 };

class NRSSL {

private:
  static JavaVM *jvm;
  JNIEnv *env = nullptr;

public:
  NRSSL();
  ~NRSSL();

  static void shutdown();

  uint64_t convertDoubleTo64Type(double value, Type type);
  uint32_t convertFloatTo32Type(float value, Type type);

  double convert32TypeToDouble(uint32_t value, Type type);
  double convert64TypeToDouble(uint64_t value, Type type);

private:
  jclass initializeJClass(const char *class_name);
  jmethodID getJMethod(jclass clazz, const char *method_name,
                       const char *signature, bool is_static = false);

  uint64_t binaryStringToUint64(const char *binaryString);
};
