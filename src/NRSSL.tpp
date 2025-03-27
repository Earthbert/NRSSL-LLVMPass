#ifndef NRSSL_TPP
#define NRSSL_TPP

#include "NRSSL.h"
#include <iostream>
#include <limits>
#include <type_traits>

template <typename T>
typename std::enable_if<std::is_unsigned<T>::value, T>::type NRSSL::convertDoubleToUint(float value,
                                                                                        Type type) {

    jclass positClass = initializeJClass(JNI_TYPES::POSIT);
    jclass positBClass = initializeJClass(JNI_TYPES::POSIT_B);

    jmethodID applyMethod =
        getJMethod(positClass, JNI_METHODS::APPLY,
                   createSignature(JNI_TYPES::POSIT_B, {JNI_TYPES::DOUBLE}), true);

    jobject positB = env->CallObjectMethod(positClass, applyMethod, value);

    jmethodID getBitsMethod = getJMethod(positBClass, JNI_METHODS::TO_BINARY_STRING,
                                         createSignature(JNI_TYPES::STRING, {}), false);

    jstring bits = (jstring)env->CallObjectMethod(positB, getBitsMethod);

    const char *bitsStr = env->GetStringUTFChars(bits, nullptr);

    std::cout << "Bits: " << bitsStr << std::endl;

    return binaryStringToUint<T>(bitsStr);
}

template <typename T, typename std::enable_if<std::is_unsigned<T>::value, bool>::type>
double NRSSL::convertUintToDouble(T value, Type type) {

    jclass positClass = initializeJClass(JNI_TYPES::POSIT);
    jclass positBClass = initializeJClass(JNI_TYPES::POSIT_B);

    jmethodID defaultRoundingMethod =
        getJMethod(positClass, JNI_METHODS::DEFAULTROUNDING,
                   createSignature(JNI_TYPES::ROUNDING_TYPE, {}), true);

    jmethodID defaultSizeMethod =
        getJMethod(positClass, JNI_METHODS::DEFAULTSIZE, createSignature(JNI_TYPES::INT, {}), true);

    jmethodID defaultExponentSizeMethod = getJMethod(positClass, JNI_METHODS::DEFAULTEXPSIZE,
                                                     createSignature(JNI_TYPES::INT, {}), true);

    jmethodID applyMethod =
        getJMethod(positClass, JNI_METHODS::APPLY,
                   createSignature(JNI_TYPES::POSIT_B, {JNI_TYPES::STRING, JNI_TYPES::INT,
                                                        JNI_TYPES::INT, JNI_TYPES::ROUNDING_TYPE}),
                   true);

    jmethodID toDoubleMethod = getJMethod(positBClass, JNI_METHODS::TO_DOUBLE,
                                          createSignature(JNI_TYPES::DOUBLE, {}));

    jobject roundingType = env->CallStaticObjectMethod(positClass, defaultRoundingMethod);
    jint size = env->CallStaticIntMethod(positClass, defaultSizeMethod);
    jint exponentSize = env->CallStaticIntMethod(positClass, defaultExponentSizeMethod);

    auto binaryString = UintToBinaryString(value);

    jobject posit_1 = env->CallStaticObjectMethod(positClass, applyMethod,
                                                  env->NewStringUTF(binaryString.c_str()),
                                                  exponentSize, size, roundingType);

    jdouble doubleValue = env->CallDoubleMethod(posit_1, toDoubleMethod);

    return doubleValue;
}

template <typename T>
typename std::enable_if<std::is_unsigned<T>::value, T>::type
NRSSL::binaryStringToUint(std::string binaryString) {
    if (binaryString.size() > sizeof(T) * 8) {
        std::cerr << "Binary string is longer than " << sizeof(T) * 8 << " bits" << std::endl;
        exit(1);
    }

    T value = 0;
    for (int i = 0; i < binaryString.size(); i++) {
        if (binaryString[i] != '0' && binaryString[i] != '1') {
            std::cerr << "Binary string contains non-binary characters" << std::endl;
            exit(1);
        }
        value = (value << 1) | (binaryString[i] - '0');
    }

    return value;
}

template <typename T, typename std::enable_if<std::is_unsigned<T>::value, bool>::type>
std::string NRSSL::UintToBinaryString(T value) {
    constexpr size_t bitSize = std::numeric_limits<T>::digits;
    std::string binaryString(bitSize, '0');

    for (size_t i = 0; i < bitSize; i++) {
        binaryString[bitSize - 1 - i] = ((value >> i) & 1) + '0';
    }

    return binaryString;
}

#endif // NRSSL_TPP
