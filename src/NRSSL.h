#pragma once

#include <cstdint>
#include <jni.h>

enum Type { POSIT32, POSIT64 };

class NRSSL {

    static JavaVM *jvm;
    JNIEnv *env = nullptr;

    bool shouldDetach = false;

  public:
    NRSSL();
    ~NRSSL();

    static void shutdown();

    uint64_t convertDoubleTo64Type(double value, Type type);
    uint32_t convertFloatTo32Type(float value, Type type);

    double convert32TypeToDouble(uint32_t value, Type type);
    double convert64TypeToDouble(uint64_t value, Type type);

    char *convertBinaryStringToString(const char *binaryString, Type type);

    uint64_t binaryStringToUint64(const char *binaryString);
    uint32_t binaryStringToUint32(const char *binaryString);

    void uint32ToBinaryString(uint32_t value, char *binaryString);
    void uint64ToBinaryString(uint64_t value, char *binaryString);

  private:
    jclass initializeJClass(const char *class_name);
    jmethodID getJMethod(jclass clazz, const char *method_name, const char *signature,
                         bool is_static = false);
};

namespace NRSSL_JNI_STRINGS {

namespace POSIT {
    const char *const value = "ro/upb/nrs/sl/Posit";
    namespace APPLY {
        const char *const value = "apply";
        namespace DOUBLE_R_POSITB {
            const char *const value = "(D)Lro/upb/nrs/sl/Posit_B;";
        }
        namespace STRING_INT_INT_ROUNDINGTYPE_R_POSIT_B {
            const char *const value =
                "(Ljava/lang/String;IILro/upb/nrs/sl/RoundingType;)Lro/upb/nrs/sl/Posit_B;";
        }
        namespace DOUBLE_INT_INT_ROUNDINGTYPE_R_POSIT_B {
            const char *const value = "(DILro/upb/nrs/sl/RoundingType;)Lro/upb/nrs/sl/Posit_B;";
        }
        namespace FLOAT_R_POSITB {
            const char *const value = "(F)Lro/upb/nrs/sl/Posit_B;";
        }
    } // namespace APPLY
    namespace DEFAULTROUNDING {
        const char *const value = "default_rounding";
        namespace R_ROUNDINGTYPE {
            const char *const value = "()Lro/upb/nrs/sl/RoundingType;";
        }
    } // namespace DEFAULTROUNDING
    namespace DEFAULTSIZE {
        const char *const value = "default_size";
        namespace R_INT {
            const char *const value = "()I";
        }
    } // namespace DEFAULTSIZE
    namespace DEFAULTEXPONENTSIZE {
        const char *const value = "default_exponent_size";
        namespace R_INT {
            const char *const value = "()I";
        }
    } // namespace DEFAULTEXPONENTSIZE
} // namespace POSIT

namespace POSITB {
    const char *const value = "ro/upb/nrs/sl/Posit_B";
    namespace TOBINARYSTRING {
        const char *const value = "toBinaryString";
        namespace R_STRING {
            const char *const value = "()Ljava/lang/String;";
        }
    } // namespace TOBINARYSTRING
    namespace TODOUBLE {
        const char *const value = "toDouble";
        namespace R_DOUBLE {
            const char *const value = "()D";
        }
    } // namespace TODOUBLE
} // namespace POSITB
} // namespace NRSSL_JNI_STRINGS
