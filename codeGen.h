#define _CRT_SECURE_NO_WARNINGS
#ifndef __CODEGEN__
#define __CODEGEN__

#include "parser.h"

// Evaluate the syntax tree
extern int evaluateTree(BTNode *root);

// Print the syntax tree in prefix
extern void printPrefix(BTNode *root);

extern void printAssembly(BTNode* root);
int cur;
#endif // __CODEGEN__
