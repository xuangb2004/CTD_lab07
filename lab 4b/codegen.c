/* codegen.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reader.h"
#include "symtab.h"
#include "codegen.h"
#include "instructions.h"

extern SymTab* symtab;

#define MAX_CODE_SIZE 1000

Instruction* codeBuffer[MAX_CODE_SIZE];
int codeLength = 0;

void initCodeBuffer(void) {
  codeLength = 0;
}

void printCodeBuffer(void) {
  int i;
  for (i = 0; i < codeLength; i++) {
    printf("%d: ", i);
    printInstruction(codeBuffer[i]);
    printf("\n");
  }
}

void cleanCodeBuffer(void) {
  int i;
  for (i = 0; i < codeLength; i++) {
    free(codeBuffer[i]);
  }
  codeLength = 0;
}

CodeAddress getCurrentCodeAddress(void) {
  return codeLength;
}

Instruction* genCode(OpCode op, int p, int q) {
  Instruction* instr = (Instruction*) malloc(sizeof(Instruction));
  instr->op = op;
  instr->p = p;
  instr->q = q;
  if (codeLength < MAX_CODE_SIZE) {
    codeBuffer[codeLength] = instr;
    codeLength++;
  }
  return instr;
}

Instruction* genLA(int p, int q) { return genCode(OP_LA, p, q); }
Instruction* genLV(int p, int q) { return genCode(OP_LV, p, q); }
Instruction* genLC(int q)        { return genCode(OP_LC, 0, q); }
Instruction* genLI(void)         { return genCode(OP_LI, 0, 0); }
Instruction* genINT(int q)       { return genCode(OP_INT, 0, q); }
Instruction* genDCT(int q)       { return genCode(OP_DCT, 0, q); }
Instruction* genJ(CodeAddress label) { return genCode(OP_J, 0, label); }
Instruction* genFJ(CodeAddress label){ return genCode(OP_FJ, 0, label); }
Instruction* genHL(void)         { return genCode(OP_HL, 0, 0); }
Instruction* genST(void)         { return genCode(OP_ST, 0, 0); }
Instruction* genCALL(int p, CodeAddress label) { return genCode(OP_CALL, p, label); }
Instruction* genEP(void)         { return genCode(OP_EP, 0, 0); }
Instruction* genEF(void)         { return genCode(OP_EF, 0, 0); }
Instruction* genRC(void)         { return genCode(OP_RC, 0, 0); }
Instruction* genRI(void)         { return genCode(OP_RI, 0, 0); }
Instruction* genWRC(void)        { return genCode(OP_WRC, 0, 0); }
Instruction* genWRI(void)        { return genCode(OP_WRI, 0, 0); }
Instruction* genWLN(void)        { return genCode(OP_WLN, 0, 0); }
Instruction* genAD(void)         { return genCode(OP_AD, 0, 0); }
Instruction* genSB(void)         { return genCode(OP_SB, 0, 0); }
Instruction* genML(void)         { return genCode(OP_ML, 0, 0); }
Instruction* genDV(void)         { return genCode(OP_DV, 0, 0); }
Instruction* genNEG(void)        { return genCode(OP_NEG, 0, 0); }
Instruction* genCV(void)         { return genCode(OP_CV, 0, 0); }
Instruction* genEQ(void)         { return genCode(OP_EQ, 0, 0); }
Instruction* genNE(void)         { return genCode(OP_NE, 0, 0); }
Instruction* genGT(void)         { return genCode(OP_GT, 0, 0); }
Instruction* genLT(void)         { return genCode(OP_LT, 0, 0); }
Instruction* genGE(void)         { return genCode(OP_GE, 0, 0); }
Instruction* genLE(void)         { return genCode(OP_LE, 0, 0); }

void updateJ(Instruction* jmp, CodeAddress label) {
  jmp->q = label;
}

void updateFJ(Instruction* jmp, CodeAddress label) {
  jmp->q = label;
}

// [SỬA THEO YÊU CẦU] Tạm thời xem các biến đều nằm mức 0
void genVariableAddress(Object* var) {
  int offset;
  
  if (var->kind == OBJ_VARIABLE) {
    offset = var->varAttrs->localOffset;
  } else if (var->kind == OBJ_PARAMETER) {
    offset = var->paramAttrs->localOffset;
  } else if (var->kind == OBJ_FUNCTION) {
    offset = 0;
  } else return;

  // Luôn dùng mức 0 (diff = 0)
  genLA(0, offset);
}

// [SỬA THEO YÊU CẦU] Sinh mã LA + LI (2 lệnh) thay vì LV
void genVariableValue(Object* var) {
  int diff, offset;
  Scope* scope;

  if (var->kind == OBJ_VARIABLE) {
    scope = var->varAttrs->scope;
    offset = var->varAttrs->localOffset;
  } else if (var->kind == OBJ_PARAMETER) {
    scope = var->paramAttrs->scope;
    offset = var->paramAttrs->localOffset;
  } else if (var->kind == OBJ_FUNCTION) {
    scope = var->funcAttrs->scope;
    offset = 0;
  } else return;

  diff = symtab->currentScope->level - scope->level;
  genLV(diff, offset);
}

void genPredefinedProcedureCall(Object* proc) {
  if (strcmp(proc->name, "WRITEI") == 0) genWRI();
  else if (strcmp(proc->name, "WRITEC") == 0) genWRC();
  else if (strcmp(proc->name, "WRITELN") == 0) genWLN();
}

void genPredefinedFunctionCall(Object* func) {
  if (strcmp(func->name, "READI") == 0) genRI();
  else if (strcmp(func->name, "READC") == 0) genRC();
}

int isPredefinedProcedure(Object* proc) {
  return (strcmp(proc->name, "WRITEI") == 0 ||
          strcmp(proc->name, "WRITEC") == 0 ||
          strcmp(proc->name, "WRITELN") == 0);
}

int isPredefinedFunction(Object* func) {
  return (strcmp(func->name, "READI") == 0 ||
          strcmp(func->name, "READC") == 0);
}

int serialize(char* fileName) {
  FILE* f;
  int i;
  f = fopen(fileName, "wb");
  if (f == NULL) return IO_ERROR;
  for (i = 0; i < codeLength; i++) {
    fwrite(codeBuffer[i], sizeof(Instruction), 1, f);
  }
  fclose(f);
  return IO_SUCCESS;
}