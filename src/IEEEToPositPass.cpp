#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <jni.h>
#include <llvm-19/llvm/Support/Casting.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/Support/raw_ostream.h>

#include "NRSSL.h"

using namespace llvm;

class IEEEToPositPass : public PassInfoMixin<IEEEToPositPass> {

    NRSSL nrssl;

  public:
    IEEEToPositPass() { errs() << "Creating IEEEToPositPass\n"; }

    ~IEEEToPositPass() { errs() << "Destroying IEEEToPositPass\n"; }

    PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM) {
        errs() << "Running IEEEToPositPass on " << M.getName().str() << "\n";

        bool Modified = false;

        for (auto &Global : M.globals()) {

            if (Global.hasInitializer()) {
                Constant *Initializer = Global.getInitializer();

                if (ConstantFP *FpConst = dyn_cast<ConstantFP>(Initializer)) {
                    Constant *PositConst = convertConstantFP(FpConst, M.getContext());
                    if (PositConst && PositConst != FpConst) {
                        Global.setInitializer(PositConst);
                        Modified = true;
                    }
                }

                else if (ConstantDataArray *ArrayConst = dyn_cast<ConstantDataArray>(Initializer)) {
                    if (ArrayConst->getType()->getElementType()->isFloatingPointTy()) {
                        SmallVector<Constant *, 16> ConvertedElements;
                        bool ArrayModified = false;

                        for (unsigned i = 0; i < ArrayConst->getNumElements(); ++i) {
                            if (ConstantFP *FpElement =
                                    dyn_cast<ConstantFP>(ArrayConst->getElementAsConstant(i))) {
                                Constant *PositElement =
                                    convertConstantFP(FpElement, M.getContext());
                                if (PositElement && PositElement != FpElement) {
                                    ConvertedElements.push_back(PositElement);
                                    ArrayModified = true;
                                } else {
                                    ConvertedElements.push_back(FpElement);
                                }
                            } else {
                                ConvertedElements.push_back(ArrayConst->getElementAsConstant(i));
                            }
                        }

                        if (ArrayModified) {
                            Constant *NewArrayConst =
                                ConstantArray::get(ArrayConst->getType(), ConvertedElements);
                            Global.setInitializer(NewArrayConst);
                            Modified = true;
                        }
                    }
                }
            }
        }

        for (auto &F : M) {
            for (auto &BB : F) {
                for (auto &I : BB) {
                    if (auto *Store = dyn_cast<StoreInst>(&I)) {
                        std::cout << "StoreInst\n";
                        Value *Value = Store->getValueOperand();
                        if (ConstantFP *FpConst = dyn_cast<ConstantFP>(Value)) {
                            Constant *PositConst = convertConstantFP(FpConst, M.getContext());
                            if (PositConst && PositConst != FpConst) {
                                Store->setOperand(0, PositConst);
                                Modified = true;
                            }
                        }
                    }
                }
            }
        }

        return Modified ? PreservedAnalyses::none() : PreservedAnalyses::all();
    }

    static bool isRequired() { return true; }

    static void nrsslShutdown() { NRSSL::shutdown(); }

  private:
    Constant *convertConstantFP(ConstantFP *FpConst, LLVMContext &Context) {
        APFloat OldAPF = FpConst->getValueAPF();
        APInt OldBits = OldAPF.bitcastToAPInt();

        if (FpConst->getType()->isFloatTy()) {
            float IeeeValue;
            memcpy(&IeeeValue, OldBits.getRawData(), sizeof(float));

            uint32_t PositValue = nrssl.convertDoubleToUint<uint32_t>(IeeeValue, NRSSL::POSIT);
            APInt NewBits(32, PositValue);

            APFloat NewAPF(APFloat::IEEEsingle(), NewBits);
            return ConstantFP::get(Context, NewAPF);
        }

        if (FpConst->getType()->isDoubleTy()) {
            double IeeeValue;
            memcpy(&IeeeValue, OldBits.getRawData(), sizeof(double));

            uint64_t PositValue = nrssl.convertDoubleToUint<uint64_t>(IeeeValue, NRSSL::POSIT);
            APInt NewBits(64, PositValue);

            APFloat NewAPF(APFloat::IEEEdouble(), NewBits);
            return ConstantFP::get(Context, NewAPF);
        }

        return nullptr;
    }
};

PassPluginLibraryInfo getIEEEToPositPassPluginInfo() {
    return {LLVM_PLUGIN_API_VERSION, "IEEEToPositPass", LLVM_VERSION_STRING, [](PassBuilder &PB) {
                PB.registerPipelineParsingCallback([](StringRef Name, ModulePassManager &MPM,
                                                      ArrayRef<PassBuilder::PipelineElement>) {
                    if (Name == "ieee-to-posit") {
                        MPM.addPass(IEEEToPositPass());
                        return true;
                    }
                    return false;
                });
                std::atexit(IEEEToPositPass::nrsslShutdown);
            }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return getIEEEToPositPassPluginInfo();
}