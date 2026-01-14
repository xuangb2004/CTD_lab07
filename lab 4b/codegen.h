/* codegen.h */
#ifndef __CODEGEN_H__
#define __CODEGEN_H__

#include "symtab.h"
#include "instructions.h"

#define DC_VALUE 0

typedef int CodeAddress;

void initCodeBuffer(void);
void printCodeBuffer(void);
void cleanCodeBuffer(void);

CodeAddress getCurrentCodeAddress(void);
void updateJ(Instruction* jmp, CodeAddress label);
void updateFJ(Instruction* jmp, CodeAddress label);

// [QUAN TRỌNG] Cập nhật kiểu trả về là Instruction*
Instruction* genCode(OpCode op, int p, int q);
Instruction* genLA(int p, int q);
Instruction* genLV(int p, int q);
Instruction* genLC(int q);
Instruction* genLI(void);
Instruction* genINT(int q);
Instruction* genDCT(int q);
Instruction* genJ(CodeAddress label);
Instruction* genFJ(CodeAddress label);
Instruction* genHL(void);
Instruction* genST(void);
Instruction* genCALL(int p, CodeAddress label);
Instruction* genEP(void);
Instruction* genEF(void);
Instruction* genRC(void);
Instruction* genRI(void);
Instruction* genWRC(void);
Instruction* genWRI(void);
Instruction* genWLN(void);
Instruction* genAD(void);
Instruction* genSB(void);
Instruction* genML(void);
Instruction* genDV(void);
Instruction* genNEG(void);
Instruction* genCV(void);
Instruction* genEQ(void);
Instruction* genNE(void);
Instruction* genGT(void);
Instruction* genLT(void);
Instruction* genGE(void);
Instruction* genLE(void);

void genVariableAddress(Object* var);
void genVariableValue(Object* var);
void genPredefinedProcedureCall(Object* proc);
void genPredefinedFunctionCall(Object* func);

int isPredefinedProcedure(Object* proc);
int isPredefinedFunction(Object* func);

int serialize(char* fileName);

#endif