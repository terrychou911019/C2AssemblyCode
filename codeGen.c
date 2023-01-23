#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codeGen.h"

//int cur = -1;

int evaluateTree(BTNode *root) {
    int retval = 0, lv = 0, rv = 0;

    if (root != NULL) {
        switch (root->data) {
            case ID:
                retval = getval(root->lexeme);
                break;
            case INT:
                retval = atoi(root->lexeme);
                break;
            case ASSIGN:
                rv = evaluateTree(root->right);
                retval = setval(root->left->lexeme, rv);
                break;
            case INCDEC:
                rv = evaluateTree(root->right);
                if (strcmp(root->lexeme, "++") == 0) {
                    retval = rv + 1;
                }
                else if (strcmp(root->lexeme, "--") == 0) {
                    retval = rv - 1;
                }
                break;
            case ADDSUB:
            case MULDIV:
                lv = evaluateTree(root->left);
                rv = evaluateTree(root->right);
                if (strcmp(root->lexeme, "+") == 0) {
                    retval = lv + rv;
                } else if (strcmp(root->lexeme, "-") == 0) {
                    retval = lv - rv;
                } else if (strcmp(root->lexeme, "*") == 0) {
                    retval = lv * rv;
                } else if (strcmp(root->lexeme, "/") == 0) {
                    if (rv == 0) {
                        error(DIVZERO);
                    }
                    else {
                        retval = lv / rv;
                    }
                }
                break;
            case AND:
            case OR:
            case XOR:
                if (strcmp(root->lexeme, "&") == 0) {
                    retval = lv & rv;
                }
                else if (strcmp(root->lexeme, "|") == 0) {
                    retval = lv | rv;
                }
                else if (strcmp(root->lexeme, "^") == 0) {
                    retval = lv ^ rv;
                }
                break;
            default:
                retval = 0;
        }
    }
    return retval;
}

int get_mp(char *str) {
    for (int i = 0; i < TBLSIZE; i++) {
        if (strcmp(str, table[i].name) == 0) {
            return i;
        }
    }
}

void printAssembly(BTNode* root) {
    //printf("in\n");
    //printf("MOV r0, [0]\n");
    //printf("MOV r1, [1]\n");
    //printf("MOV r2, [2]\n");
    if (root == NULL) return;
    switch (root->data) {
        case ID:
            cur += 1;
            printf("MOV r%d [%d]\n", cur, get_mp(root->lexeme) * 4);
            break;
        case INT:
            cur += 1;
            printf("MOV r%d %d\n", cur, atoi(root->lexeme));
            break;
        case ASSIGN:
            printAssembly(root->left);
            printAssembly(root->right);
            printf("MOV [%d] r%d\n", get_mp(root->left->lexeme) * 4, cur);
            break;
        case INCDEC:
            if (strcmp(root->lexeme, "++") == 0) {
                printAssembly(root->left);
                printAssembly(root->right);
                printf("ADD r%d r%d\n", cur - 1, cur);
                cur -= 1;
                printf("MOV [%d] r%d\n", get_mp(root->left->lexeme) * 4, cur);
            }
            else if (strcmp(root->lexeme, "--") == 0) {
                printAssembly(root->left);
                printAssembly(root->right);
                printf("SUB r%d r%d\n", cur - 1, cur);
                cur -= 1;
                printf("MOV [%d] r%d\n", get_mp(root->left->lexeme) * 4, cur);
            }
            break;
        case ADDSUB:
        case MULDIV:
        case AND:
        case OR:
        case XOR:
            if (strcmp(root->lexeme, "+") == 0) {
                printAssembly(root->left);
                printAssembly(root->right);
                printf("ADD r%d r%d\n", cur - 1, cur);
                cur -= 1;
            }
            else if (strcmp(root->lexeme, "-") == 0) {
                printAssembly(root->left);
                printAssembly(root->right);
                printf("SUB r%d r%d\n", cur - 1, cur);
                cur -= 1;
            }
            else if (strcmp(root->lexeme, "*") == 0) {
                printAssembly(root->left);
                printAssembly(root->right);
                printf("MUL r%d r%d\n", cur - 1, cur);
                cur -= 1;
            }
            else if (strcmp(root->lexeme, "/") == 0) {
                printAssembly(root->left);
                printAssembly(root->right);
                printf("DIV r%d r%d\n", cur - 1, cur);
                cur -= 1;
            }
            else if (strcmp(root->lexeme, "&") == 0) {
                printAssembly(root->left);
                printAssembly(root->right);
                printf("AND r%d r%d\n", cur - 1, cur);
                cur -= 1;
            }
            else if (strcmp(root->lexeme, "|") == 0) {
                printAssembly(root->left);
                printAssembly(root->right);
                printf("OR r%d r%d\n", cur - 1, cur);
                cur -= 1;
            }
            else if (strcmp(root->lexeme, "^") == 0) {
                printAssembly(root->left);
                printAssembly(root->right);
                printf("XOR r%d r%d\n", cur - 1, cur);
                cur -= 1;
            }
            break;
        default:
            break;
    }
}

void printPrefix(BTNode *root) {
    if (root != NULL) {
        printf("%s ", root->lexeme);
        printPrefix(root->left);
        printPrefix(root->right);
    }
}
