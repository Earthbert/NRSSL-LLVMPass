#include "NRSSL.h"
#include <cstdlib>
#include <cstring>
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
        snprintf(options[0].optionString, 1024,
                 "-Djava.class.path=%s/scala-library-2.13.8.jar"
                 ":%s/nrssl.jar",
                 jarsPath, jarsPath);

        vm_args.version = JNI_VERSION_10;
        vm_args.nOptions = 1;
        vm_args.options = options;
        vm_args.ignoreUnrecognized = false;

        auto ret = JNI_CreateJavaVM(&jvm, (void **)&env, &vm_args);
        if (ret != JNI_OK)
            std::cerr << "Failed to create Java VM with error code: " << ret << std::endl;

        delete[] options[0].optionString;
        delete[] options;
    } else {
        if (jvm->GetEnv((void **)&env, JNI_VERSION_10) != JNI_OK) {
            if (jvm->AttachCurrentThread((void **)&env, nullptr) != JNI_OK) {
                std::cerr << "Failed to attach current thread to Java VM" << std::endl;
                jvm->DestroyJavaVM();
                exit(1);
            }
            shouldDetach = true;
        }
    }
}

NRSSL::~NRSSL() {
    if (shouldDetach) {
        jvm->DetachCurrentThread();
    }
}

void NRSSL::shutdown() {
    std::cout << "Shutting down NRSSL" << std::endl;
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

jmethodID NRSSL::getJMethod(jclass clazz, const char *method_name, const char *signature,
                            bool is_static) {
    jmethodID method = is_static ? env->GetStaticMethodID(clazz, method_name, signature)
                                 : env->GetMethodID(clazz, method_name, signature);
    if (method == nullptr) {
        std::cerr << "Failed to find \"" << method_name << "\" sig: " << signature << std::endl;
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

uint32_t NRSSL::convertFloatTo32Type(float value, Type type) {

    jclass positClass = initializeJClass(NRSSL_JNI_STRINGS::POSIT::value);
    jclass positBClass = initializeJClass(NRSSL_JNI_STRINGS::POSITB::value);

    jmethodID applyMethod =
        getJMethod(positClass, NRSSL_JNI_STRINGS::POSIT::APPLY::value,
                   NRSSL_JNI_STRINGS::POSIT::APPLY::FLOAT_R_POSITB::value, true);

    jobject positB = env->CallObjectMethod(positClass, applyMethod, value);

    jmethodID getBitsMethod =
        getJMethod(positBClass, NRSSL_JNI_STRINGS::POSITB::TOBINARYSTRING::value,
                   NRSSL_JNI_STRINGS::POSITB::TOBINARYSTRING::R_STRING::value);

    jstring bits = (jstring)env->CallObjectMethod(positB, getBitsMethod);

    const char *bitsStr = env->GetStringUTFChars(bits, nullptr);

    std::cout << "Bits: " << bitsStr << std::endl;

    return binaryStringToUint32(bitsStr);
}

double NRSSL::convert32TypeToDouble(uint32_t value, Type type) {

    jclass positClass = initializeJClass(NRSSL_JNI_STRINGS::POSIT::value);
    jclass positBClass = initializeJClass(NRSSL_JNI_STRINGS::POSITB::value);

    jmethodID defaultRoundingMethod =
        getJMethod(positClass, NRSSL_JNI_STRINGS::POSIT::DEFAULTROUNDING::value,
                   NRSSL_JNI_STRINGS::POSIT::DEFAULTROUNDING::R_ROUNDINGTYPE::value, true);

    jmethodID defaultSizeMethod =
        getJMethod(positClass, NRSSL_JNI_STRINGS::POSIT::DEFAULTSIZE::value,
                   NRSSL_JNI_STRINGS::POSIT::DEFAULTSIZE::R_INT::value, true);

    jmethodID defaultExponentSizeMethod =
        getJMethod(positClass, NRSSL_JNI_STRINGS::POSIT::DEFAULTEXPONENTSIZE::value,
                   NRSSL_JNI_STRINGS::POSIT::DEFAULTEXPONENTSIZE::R_INT::value, true);

    jmethodID applyMethod = getJMethod(
        positClass, NRSSL_JNI_STRINGS::POSIT::APPLY::value,
        NRSSL_JNI_STRINGS::POSIT::APPLY::STRING_INT_INT_ROUNDINGTYPE_R_POSIT_B::value, true);

    jmethodID toDoubleMethod = getJMethod(positBClass, NRSSL_JNI_STRINGS::POSITB::TODOUBLE::value,
                                          NRSSL_JNI_STRINGS::POSITB::TODOUBLE::R_DOUBLE::value);

    jobject roundingType = env->CallStaticObjectMethod(positClass, defaultRoundingMethod);
    jint size = env->CallStaticIntMethod(positClass, defaultSizeMethod);
    jint exponentSize = env->CallStaticIntMethod(positClass, defaultExponentSizeMethod);

    char binaryString[33];
    uint32ToBinaryString(value, binaryString);

    jobject posit_1 = env->CallStaticObjectMethod(
        positClass, applyMethod, env->NewStringUTF(binaryString), exponentSize, size, roundingType);

    jdouble doubleValue = env->CallDoubleMethod(posit_1, toDoubleMethod);

    return doubleValue;
}

double NRSSL::convert64TypeToDouble(uint64_t value, Type type) { return 0; }

uint64_t NRSSL::binaryStringToUint64(const char *binaryString) {
    if (strlen(binaryString) > 64) {
        std::cerr << "Binary string is longer than 64 bits" << std::endl;
        exit(1);
    }

    uint64_t value = 0;
    for (int i = 0; i < strlen(binaryString); i++) {
        value = (value << 1) | (binaryString[i] - '0');
    }

    return value;
}

uint32_t NRSSL::binaryStringToUint32(const char *binaryString) {
    if (strlen(binaryString) > 32) {
        std::cerr << "Binary string is longer than 32 bits" << std::endl;
        exit(1);
    }

    uint32_t value = binaryStringToUint64(binaryString);
    return value;
}

void NRSSL::uint32ToBinaryString(uint32_t value, char *binaryString) {
    for (int i = 31; i >= 0; i--) {
        binaryString[31 - i] = ((value >> i) & 1) + '0';
    }
    binaryString[32] = '\0';
}

void NRSSL::uint64ToBinaryString(uint64_t value, char *binaryString) {
    for (int i = 63; i >= 0; i--) {
        binaryString[63 - i] = ((value >> i) & 1) + '0';
    }
    binaryString[64] = '\0';
}
