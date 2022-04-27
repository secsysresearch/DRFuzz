#ifndef DRIVERANALYSIS_H
#define DRIVERANALYSIS_H

#include "Analyzer.h"
#include "Common.h"
#include "DataFlowAnalysis.h"

//common device driver type: pci/isa/scsi/i2c/usb

enum DriverTypeID{
	PCIDRIVER,
	ISADRIVER,
	SCSIDRIVER,
	I2CDRIVER,
	USBDRIVER
};

class DriverDesc{

public:
	Function *entryFunc;
	GlobalVariable * driverStruct;
	Module *M;
	std::list<Value *> coreData;
	Function *BusRegFunc;
	StructType * BusType;
	std::list<Value *> allCriticalFieldVal;
	std::list<Value *> allCritialStructVal;
	DriverTypeID TypeID;
};

class DriverAnalysisPass : public IterativeModulePass {

	typedef std::pair<Instruction *, BasicBlock *> CFGEdge;
	typedef std::pair<CFGEdge, Value *> EdgeValue;
	typedef std::list<BasicBlock *> BList;
	typedef std::list<Function *> FList;
	typedef std::list<Value *> VList;
	typedef std::list<Instruction *>IList;
	typedef std::list<std::pair<Function *, Module *>> DriverList;
	typedef std::list<std::pair<Value *, Value *>> DriverProbeList;
//	typedef std::list<std::pair<Value *, Value *> > DriverCoreList;
	typedef std::set<std::pair<Function *, Module *>> DriverSet;
	typedef std::list<DriverDesc *> DriverDescList;
	typedef std::list<StructType *> DriverTypeList;
	typedef std::map<StructType *, list<Value *>> DriverTypesMap;
	typedef std::vector<Value*> CandPos;
	typedef std::map<const Value*, CandPos> SourceToIOPosMaps;
	typedef std::map<const Value*, std::set<Value *>> SourceToIOValueMaps;
	typedef std::map<Module*, CandPos> IOPosMaps;

	public:
	Function *DriverRegFunc;
	std::list<Function *> BusRegFuncs;
	std::list<Value *> BusDrivers;
	std::list<Value *> PCIDrivers;
	std::list<Value *> ISADrivers;
	std::list<Value *> SCSIDrivers;
	std::list<Value *> I2CDrivers;
	std::list<Value *> USBDrivers;
	StructType * BusType;
	StructType * PCIDriverType;
	StructType * ISADriverType;
	StructType * SCSIDriverType;
	StructType * I2CDriverType;
	StructType * USBDriverType;
	DriverTypesMap TargetDrivers;
//	DriverList DriverEntrys;
//	DriverProbeList DriverProbes;
	DriverDescList Drivers;
	DriverDescList LDMDriver, NonLDMDriver;
	std::set<Function *> DriverRegs;
	int LDMnums = 0;
	int NonLDMnums = 0;
	Function *vmcallF;

	std::map<enum DriverTypeID, int> driverTypeFunOffset;
	Function *entryManually;
	
	SourceToIOPosMaps srcIOPos;
	SourceToIOValueMaps srcIOValue;
	std::map<Value*, std::set<int>> iOValueMask;
	std::vector<Value*> driverIOPos;
	std::vector<Value*> driverIOValue;
	DataFlowAnalysis DFA;
	std::map<Module *, SetVector<Value *>> MCVsetMap;
	std::vector<std::pair<BasicBlock *, Value *>> needInstBB;
	public:

	DriverAnalysisPass(GlobalContext *Ctx_)
			: IterativeModulePass(Ctx_, "DriverAnalysis"), DFA(Ctx_) {
		DriverRegFunc = NULL;
		entryManually = NULL;
		BusType = NULL;
		PCIDriverType = NULL;
		ISADriverType = NULL;
		SCSIDriverType = NULL;
		I2CDriverType = NULL;
		USBDriverType = NULL;
//		TargetDrivers["pci"] = PCIDrivers;
//		TargetDrivers["isa"] = ISADrivers;
//		TargetDrivers["scsi"] = SCSIDrivers;
//		TargetDrivers["i2c"] = I2CDrivers;
//		TargetDrivers["usb"] = USBDrivers;
//
		TargetDrivers[PCIDriverType] = PCIDrivers;
		TargetDrivers[ISADriverType] = ISADrivers;
		TargetDrivers[SCSIDriverType] = SCSIDrivers;
		TargetDrivers[I2CDriverType] = I2CDrivers;
		TargetDrivers[USBDriverType] = USBDrivers;
		driverTypeFunOffset =  { {PCIDRIVER, 3}, {ISADRIVER, 1}, {SCSIDRIVER, 1}, {I2CDRIVER, 2}, {USBDRIVER, 1}, };
	}

	virtual bool doInitialization(llvm::Module *);
	virtual bool doFinalization(llvm::Module *);
	virtual bool doModulePass(llvm::Module *);
	virtual void doOnceForAll();
	bool hasPathToReg(Function *start, Function *end, int deepth);
	void collectCritialStruct(std::list<Value *> &allStructVal, Function *probe);
	void collectCritialField(std::list<Value *> &allCriticalFieldVal, Value *structVal);
	std::list<Value *> getCoreData(Function *driverFun);
	string generateOutput(DriverDescList &DriverModules);
	bool isTargetStruct(GlobalVariable *var, StructType *target);
	void identifyIOPos(Instruction *Inst);
	void updateErrMaps();
	void inferInputLocs(Value *IOValue, vector<unsigned long>&CandLoc);
	void inferOffsetInUnit(Value *IOValue);
	void inferinitialConVal(Value *IOValue, set<Value *> &CVSet, vector<unsigned long>&InitialVal);
	void getConstfromDFA(Value * Src, vector<unsigned long> &CandConst);
	void InstErrFeedback(Function *F);
};

#endif
