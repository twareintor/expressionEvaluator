// developed in onlinegdb - link here: https://onlinegdb.com/B1I9_poxu
//gcc 7.4.0
/** 
 * Interpreter of expression of basic operations (+,-,*,/) adapted for float etc.
 * Useful for interpreters with macro definitions and other uses as well
 * WARNING! in development, not fully tested and implemented: 
 * WARNING! suboptimal design: adds a "0.0+ " before the epxression to mitigate
 *  the beginning with a multiplication/ division; add blanks too
 * don't use in any project without testing and checking the results
 * Is the input expression not a valid sum of <type>, the result is undefined!!!
 */
// Code by Twareintor - code lost and written again in 2020
// copyright (c) 2020-2021 Claudiu Ciutacu


#include  <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define VER "110.A12"
#define VERSION(X) printf("VER: " X "/" __DATE__ " " __TIME__ " _____________________\n");

const char* szExpressions[] =  {

                                };

#define DEBUG_RULE
#undef DEBUG_RULE

#define PRN_PARTS
#undef PRN_PARTS

#ifdef DEBUG_RULE
unsigned g_pss[100] = {0};

/** 
 * Function printing carets '^' in the position contained in data[] in an string of blanks
 * to indicate some positions in a string: useful only for debug, debuggers, interpreters etc
 */
void prnDebRul(char* szOut, size_t outLen, unsigned* data, size_t dataLen)
{
    int i;
    for(i=0; i<dataLen; i++) if(data[i]<outLen) szOut[data[i]] = '^';
} 

#endif

#ifdef USE_GENERATED_EXPRESSIONS
#include <time.h>  

#endif

float evalPowSimple(const char* szExpr, const size_t exprLen, const char* szOps)
{
    float y = 0.f; // return value
    char* expr = strdup(szExpr);  // modifyable copy of expression
    char* toks[6] = {NULL}; // tokens
    int i = 0;
    
    if(!strstr(expr, szOps))
    {
        sscanf("%G", expr, &y);
    }
    else
    {
        toks[i] = strtok(expr, szOps);
        while(toks[i])
        {
            toks[++i] = strtok(NULL, szOps);
        }
        printf("%d, %s, %s, %s, %s\n", i, toks[0], toks[1], toks[2], toks[3]);
        i--;
        // printf("\n%s\n", toks[i]);
        // sscanf("%G", toks[i], &y);
        y = atof(toks[i]);   // y is exp at the same time, the first one
        // return y;
        while(i--)
        {
            float base = atof(toks[i]);
            y = pow(base, y);
        }
    }
    return y;
}


/** 
 * Function to evaluate a sum of integers: put a single sign before every term
 * Ver. 2
 */
float evalSimple(const char* szExpr, const size_t exprLen, const char* szOps)
{
    float y = 0.; // end result
    float x = 0.; // partial term get with sscanf

    if(!szOps) szOps = "+-"; // function is recursive: "+-" then "*/" 
    // get a copy of the original string for strtok()  a fixed length copy this time
    char* szTest = strdup(szExpr); 
    int testLen = strlen(szTest); // length of this string... needed for debug only?
#ifdef DEBUG_RULE    
    unsigned pss[100] = {0};
    char* szRule = (char*)malloc(testLen+1); // for debug only
    memset(szRule, ' ', testLen); szRule[testLen] = 0; // for debug only
#endif    
    int pos = 0;    // the position of the sign char '+' or '-' ...
    int i = 0;      // for debug only
    sscanf(szTest, "%G", &x); // to automatically get the sign!
    char* szTok = strtok_r(szTest, szOps, &szTest); // strtok
    y+=x; // get the first term, direct from the original string, see below ///< (1.)
    int t = strlen(szTok); 
    pos+=(t+1); // add one to this: assuming a single consecutive separator...
#ifdef DEBUG_RULE    
    pss[i++] = pos-1;   // for debug only
#endif    
    for(;;)             // exits @"if(!szT) break;" 
    {
#ifdef PRN_PARTS            
        if(szTok) printf("%d\t%c\t\"%s\"\n", pos, szExpr[pos-1], szT);
#endif        
        szTok = strtok_r(NULL, szOps, &szTest); 
        if(!szTok) break; // last token already here, break         // else, proceed next:
        if(strchr(szTok, '*')||strchr(szTok, '/'))     // is another operation lower priority
        {   // recursively call with last argument [*][/]
            x = evalSimple(szTok, strlen(szTok), "*/");
        }
        else
        {   // proceed next: evaluate and apply operator
            sscanf(szTok, "%G", &x); // now the token; the sign gets another way!
        }
        switch(szExpr[pos-1]) // the sign must be there...
        {
            case '+': y+=x; break;                                         ///< (2.)
            case '-': y-=x; break;                                         ///< (2.)
            case '*': y*=x; break;                                         ///< (2.)
            case '/': y/=x; break;                                         ///< (2.)
            default: y = 0;
        }
        t = strlen(szTok);
        pos+=(t+1);
#ifdef DEBUG_RULE        
        g_pss[i++] = pos-1;
#endif        
    }
    /// for debug only
#ifdef DEBUG_RULE
    printf("szExpr \"%s\"\n", szExpr);
    prnDebRul(szRule, testLen, pss+1, 100);
    printf("szRule \"%s\"\n", szRule);
    free(szRule); // for debug only
#endif    
    // free(szTest);
    return y;                                                              ///< (3.)
}

/**
 *  This adds an "0.0+" at the beginning of the expression szExpr in order not to begin
 *  with a multiplication nor with a sign or something like this: remember: "0.0 + + 1.2" is valid
 *  "0.0 + - 1.2" is valid, "0.0 ++ 1.2" is not valid, 0.0 + 
 */
float evalExpr(char const* szExpr)
{
    size_t exprLen = strlen(szExpr);
    char* szTest = (char*)malloc(exprLen + 9);
    memset(szTest, '\0', exprLen + 9);
    strcat(szTest, " 0.0 ");
    strcat(szTest, "+ ");
    strcat(szTest, szExpr);
    return evalSimple(szTest, strlen(szTest), NULL);
}

int TEST__EvalExpr()
{
    int i = 0;          // ?? no needed
    float y;        // end result
    float y0;       // presumed result
    char* szTok;    // token: expression or presumed result

    // const char* szExpressions = EXPRESSIONS;
    FILE* f = fopen("testExpr.txt", "r");
    char szLine[0x1000];
    while(fgets(szLine, 0x1000, f))
    {
        i = i + 1;
        szTok = strtok(szLine, ";");
        y = evalExpr(szTok);
        if(1 || 59==i)
        {
            szTok = strtok(NULL, ";");
            y0 = atof(szTok);
            printf("(%d) %G - (%G) = %G", i, y, y0, y-y0);
            if(y-y0 > 0.001) printf("        <== WRONG!!!");
            printf("\n");
        } // // // to check the results
    }
    fclose(f);
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    VERSION(VER)

    TEST__EvalExpr();
    printf("Hello, world!\n");
    return 0;
    
}

