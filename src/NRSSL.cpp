#include "NRSSL.h"
#include <cstdlib>
#include <iostream>
#include <jni.h>

JavaVM *NRSSL::jvm = nullptr;

NRSSL::NRSSL() {
  if (!jvm) {
    JavaVMInitArgs vm_args;
    JavaVMOption *options = new JavaVMOption[1];

    char *jarsPath = getenv("NRSSL_JARS");
    if (jarsPath == nullptr) {
      std::cerr << "NRSSL_JARS environment variable not set" << std::endl;
      exit(1);
    }

    std::cout << "NRSSL_JARS: " << jarsPath << std::endl;

    options[0].optionString = new char[1024];
    snprintf(options[0].optionString, 1024, "-Djava.class.path=%s", jarsPath);

    vm_args.version = JNI_VERSION_10;
    vm_args.nOptions = 1;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = false;

    auto ret = JNI_CreateJavaVM(&jvm, (void **)&env, &vm_args);
    if (ret != JNI_OK)
      std::cerr << "Failed to create Java VM with error code: " << ret
                << std::endl;

    delete[] options[0].optionString;
    delete[] options;
  } else {
    jvm->AttachCurrentThread((void **)&env, nullptr);
  }
}

NRSSL::~NRSSL() {
  if (env) {
    jvm->DetachCurrentThread();
  }
}

void NRSSL::shutdown() {
  if (jvm) {
    jvm->DestroyJavaVM();
  }
}

jclass NRSSL::initializeJClass(const char *class_name) {
  jclass clazz = env->FindClass(class_name);
  if (clazz == nullptr) {
    std::cerr << "Failed to find " << class_name << std::endl;
    if (env->ExceptionCheck()) {
      env->ExceptionDescribe();
      env->ExceptionClear();
    }
    jvm->DestroyJavaVM();
    exit(1);
  }
  return clazz;
}

jmethodID NRSSL::getJMethod(jclass clazz, const char *method_name,
                            const char *signature, bool is_static) {
  jmethodID method = is_static
                         ? env->GetStaticMethodID(clazz, method_name, signature)
                         : env->GetMethodID(clazz, method_name, signature);
  if (method == nullptr) {
    std::cerr << "Failed to find \"" << method_name << "\" sig: " << signature
              << std::endl;
    if (env->ExceptionCheck()) {
      env->ExceptionDescribe();
      env->ExceptionClear();
    }
    jvm->DestroyJavaVM();
    exit(1);
  }
  return method;
}

uint64_t NRSSL::convertDoubleTo64Type(double value, Type type) { return 0; }
uint32_t NRSSL::convertFloatTo32Type(float value, Type type) { return 0; }

double NRSSL::convert32TypeToDouble(uint32_t value, Type type) { return 0; }
double NRSSL::convert64TypeToDouble(uint64_t value, Type type) { return 0; }