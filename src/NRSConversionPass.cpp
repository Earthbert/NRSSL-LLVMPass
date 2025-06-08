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

class NRSConversionPass : public PassInfoMixin<NRSConversionPass> {

    NRSSL nrssl;

  public:
    NRSConversionPass() { std::cout << "Creating NRSConversionPass\n"; }

    ~NRSConversionPass() { std::cout << "Destroying NRSConversionPass\n"; }

    PreservedAnalyses run(Module &M, ModuleAnalysisManager &MAM) {
        std::cout << "Running NRSConversionPass on " << M.getName().str() << "\n";

        bool Modified = false;

        for (auto &Global : M.globals()) {
            if (Global.hasInitializer()) {
                Constant *Initializer = Global.getInitializer();
                std::cout << "Processing global variable: " << Global.getName().str() << "\n";

                Constant *NewInitializer = processConstant(Initializer, M.getContext());
                if (NewInitializer != Initializer) {
                    Global.setInitializer(NewInitializer);
                    Modified = true;
                }
            }
        }

        for (auto &F : M) {
            for (auto &BB : F) {
                for (auto &I : BB) {
                    if (auto *Store = dyn_cast<StoreInst>(&I)) {
                        Value *Value = Store->getValueOperand();
                        if (auto *Const = dyn_cast<Constant>(Value)) {
                            Constant *NewConst = processConstant(Const, M.getContext());
                            if (NewConst != Const) {
                                Store->setOperand(0, NewConst);
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

    Constant *processConstant(Constant *C, LLVMContext &Context) {

        std::cout << "Processing constant" << "\n";
        
        if (ConstantFP *FpConst = dyn_cast<ConstantFP>(C)) {
            return convertConstantFP(FpConst, Context);
        }

        if (ConstantDataArray *DataArrayConst = dyn_cast<ConstantDataArray>(C)) {
            std::cout << "Processing ConstantDataArray with " << DataArrayConst->getNumElements() << " elements\n";
            std::vector<Constant *> NewElements;
            for (int i = 0; i < DataArrayConst->getNumElements(); ++i) {
                Constant *Elem = DataArrayConst->getElementAsConstant(i);
                Constant *NewElem = processConstant(Elem, Context);
                if (NewElem != Elem) {
                    NewElements.push_back(NewElem);
                } else {
                    NewElements.push_back(Elem);
                }
            }
            return ConstantArray::get(DataArrayConst->getType(), NewElements);
        }
        
        if (ConstantArray *ArrConst = dyn_cast<ConstantArray>(C)) {
            std::cout << "Processing ConstantArray with " << ArrConst->getNumOperands() << " elements\n";
            std::vector<Constant *> NewElements;
            for (int i = 0; i < ArrConst->getNumOperands(); ++i) {
                Constant *Elem = ArrConst->getOperand(i);
                Constant *NewElem = processConstant(Elem, Context);
                if (NewElem != Elem) {
                    NewElements.push_back(NewElem);
                } else {
                    NewElements.push_back(Elem);
                }
            }
            return ConstantArray::get(ArrConst->getType(), NewElements);
        }

        return C;
    }
};

PassPluginLibraryInfo getNRSConversionPassPluginInfo() {
    return {LLVM_PLUGIN_API_VERSION, "NRSConversionPass", LLVM_VERSION_STRING, [](PassBuilder &PB) {
                // For clang integration - adds the pass at the start of the pipeline
                PB.registerPipelineStartEPCallback(
                    [](ModulePassManager &MPM, llvm::OptimizationLevel Level) {
                        std::cout << "Starting pipeline for NRSConversionPass\n";
                        MPM.addPass(NRSConversionPass());
                    });

                // For opt integration - allows the pass to be found by name
                PB.registerPipelineParsingCallback([](StringRef Name, ModulePassManager &MPM,
                                                      ArrayRef<PassBuilder::PipelineElement>) {
                    if (Name == "ieee-to-posit") {
                        std::cout << "Adding NRSConversionPass to pipeline\n";
                        MPM.addPass(NRSConversionPass());
                        return true;
                    }
                    return false;
                });

                std::atexit(NRSConversionPass::nrsslShutdown);
            }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return getNRSConversionPassPluginInfo();
}