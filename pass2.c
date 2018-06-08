/**
 * LabelTable pass2 (FILE * fp, LabelTable table)
 *      @param  fp  pointer to an open file (stdin or other file pointer)
 *                  from which to read lines of assembly source code
 *      @param  table  an existing Label Table
 *      @return a newly-created table containing labels found in the
 *              input file, each with the address of the instruction
 *              containing it (assuming the first line of input
 *              corresponds to address 0)
 *
 * DESCRIPTION MISSING !
 *
 * Author: <author>
 * Date:   <date>
 *
 * Modified by: Torey Halsey 6/5/2018 
 *      Editted comments.
 *
 */

#include "assembler.h"

/* Declaration of stub function defined later in this file. */
void processInstruction(char * instName, char * restOfInstruction,
                        int lineNum, int PC);

void pass2 (FILE * fp, LabelTable table)
  /* Returns a copy of the label table that was constructed */
{
    int    lineNum;                /* Line number. */
    int    PC;                     /* Program counter (PC). */
    char * tokBegin, * tokEnd;     /* Used to step through instruction. */
    char   inst[BUFSIZ];           /* Will hold instruction; BUFSIZ is max size of I/O buffer (defined in stdio.h). */
    char * instrName;              /* Instruction name (e.g., "add"). */

    /* Continuously read next line of input until EOF is encountered.*/
    for (lineNum = 1, PC = 0; fgets (inst, BUFSIZ, fp); lineNum++, PC += 4)
    {
        /* If the line starts with a comment, move on to next line.
         * If there's a comment later in the line, strip it off (replace the '#' with a null byte).
         */
        if ( *inst == '#' ) continue;
        (void) strtok (inst, "#");

        /* Read the first token, skipping any leading whitespace. */
        tokBegin = inst;
        getToken (&tokBegin, &tokEnd);
            /* tokBegin now points to 1st non-whitespace character in the token;
			 * tokEnd points to 1st punctuation mark or whitespace after the end of the token.
             */

        /* Skip label, if any. */
        if ( *(tokEnd) == ':' )
        {
            /* Line has a label.  Adjust beginning pointer of new token. */
            tokBegin = tokEnd + 1;
			/* Get new token. */
            getToken (&tokBegin, &tokEnd);
        }

        /* If empty line or line containing only a label, get next line. */
        if ( *tokBegin == '\0' )
            continue;

        /* We have a valid token.  Set the token end pointer to the null byte. */
        *tokEnd = '\0';
		/* Store the token as a string. */
        instrName = tokBegin;
		/* Set tokBegin to point to the character after the end. */
        tokBegin = tokEnd + 1;

		/* Debug printing of the instruction name. */
        printDebug ("first non-label token is: %s\n", instrName);

        /* CALL STUB CODE TO PROCESS INSTRUCTION !!! */
        processInstruction(instrName, tokBegin, lineNum, PC);

    }

    return;
}

/* STUB CODE !!!
* For now, assume we don't care what type of instruction it is,
* instead just assume all instructions have 3 arguments.
*/
void processInstruction(char * instName, char * restOfInstruction,
                        int lineNum, int PC)
{
    char * arguments[3];           /* Registers or values after name. */

    /* Get the 3 arguments.  (Bad assumption, but this is just a stub.) */
    if ( ! getNTokens(restOfInstruction, 3, arguments) )
    {
        /* When getNTokens encounters an error, it puts a pointer to the error message in arguments[0]. */
        printError("Error on line %d: %s\n", lineNum, arguments[0]);
        return;
    }

    /* Print the instruction name and the 3 arguments. */
    printDebug("Line %d: %s %s, %s, %s\n", lineNum, instName,
                        arguments[0], arguments[1], arguments[2]);

    return;
}
