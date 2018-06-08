/*
 * Label Table: functions to access and manipulate a label table
 *
 * This file provides the definitions of a set of functions for
 *      creating,
 *      maintaining, and
 *      using
 *  a table of assembly language instruction labels and the memory addresses associated with them.
 *
 * Author: Alyce Brady
 *
 * Creation Date:   2/16/99
 *   Modified:  12/20/2000   Updated postcondition information.
 *   Modified:  4/19/2013    Modernized call to fprintf for errors.
 *   Modified:  5/25/2015    Updated to use printError function.
 *
 *   Torey Halsey
 *   Modified:  6/5/2018     Completed functions printLabels,
 *							      findLabels, tableInit,
 *								  tableResize, and addLabel.
 *                           Formatted comments.
 *
*/

#include "assembler.h"

/* Internal global variables (global to this file only). */
static const char * ERROR0 = "Error: label table is a NULL pointer.\n";
static const char * ERROR1 = "Error: a duplicate label was found.\n";
static const char * ERROR2 = "Error: cannot allocate space in memory.\n";

/* Internal function (visible to this file only). */
static int verifyTableExists(LabelTable * table);

void tableInit (LabelTable * table)
  /* Postcondition: Table is initialized to indicate that there are no label entries in it. */
{
        /* Verify that table exists.
		 * Check for nonexistent label table.
		 */
		if (!verifyTableExists(table))
		{
			/* This is an error, a fatal one.  Report error. */

			/* ERROR0: Error: label table is a NULL pointer. */

			/* Print the ERROR0 message to the standard error. */
			printError("%s", ERROR0);
			return;           /* FATAL ERROR: Table doesn't exist. */
		}
			

        /* Initialize the table to indicate that there are no label entries in it. */
		table->capacity = 0; /* The initial capacity of the table is zero. */
		table->nbrLabels = 0; /* There are no label entries in the table initially. */
		table->entries = NULL; /* Label entries is a pointer to the null byte initially. */

}

void printLabels (LabelTable * table)
  /* Postcondition: All the labels in the table, with their associated addresses, have been printed to the standard output. */
{
        /* Declare an int variable to store an index to the label entries in the table. */
		int i;

        /* Verify that table exists.
		 * Check for nonexistence of table.
		 */
        if ( ! verifyTableExists (table) )
		{ 
			/* This is an error (ERROR0), a fatal one.  Report error. */
			
			/* ERROR0: Error: label table is a NULL pointer. */
			
			/* Print the ERROR0 message to the standard error. */
			printError("%s", ERROR0);
            return;           /* FATAL ERROR: Table doesn't exist. */
		}

		/* Print to the standard output the number of label entries in the table.
		 * Terminate with the newline character.
		 */
        (void) printf ("There are %d labels in the table:\n",
                                table->nbrLabels);

		/* Print a header to display before printing all the labels entries in the table. */
		printf("Label\t\tAddress\n");

		/* Loop through all the label entries in the table. */
		for (i = 0; i < table->nbrLabels; i++)
		{
			/* Print each label and its associated address to the standard output. */
			printf("%s\t\t%d\n", table->entries[i].label, table->entries[i].address);
		}
}

int findLabel (LabelTable * table, char * label)
  /* Returns the address associated with the label;
   *         -1 if label is not in the table or table doesn't exist
   */
{
		/* Verify that table exists.
		 * Check for nonexistence of label table.
		 */
		if ( !verifyTableExists(table) )
		{
			/* This is an error (ERROR0), a fatal one.  Report error. */

			/* ERROR0: Error: label table is a NULL pointer. */

			/* Print the ERROR0 message to the standard error. */
			printError("%s", ERROR0);
			return -1;           /* FATAL ERROR: Table doesn't exist. */
		}

		/* Declare an int to store an index of the label entries in the table. */
		int i;

		/* Loop through each label entry in the table. */
		for ( i = 0; i < table->nbrLabels; i++ )
		{
			/* Check whether the label exists.
			 *      The label exists if strcmp returns a 0 after its comparison.
			 *      The first string parameter (label) is the label we are in search of.
			 *		The second string parameter (table->entries[i].label) is the current label entry in the table.
			 */
			if ( SAME == strcmp(label, table->entries[i].label) )
			{
				/* Return the address associated with the label. */
				return table->entries[i].address;
			}
		}

		/* The label is not in the table. */
        return -1;
}

int addLabel (LabelTable * table, char * label, int PC)
  /* Postcondition: If label was already in table,
   *                     the table is unchanged;
   *                otherwise
   *                     a new entry has been added to the table with the specified label name and instruction address (memory location) and
   *                     the table has been resized if necessary.
   *
   * Returns 1 if no fatal errors occurred;
   *         0 if memory allocation error or table doesn't exist.
   */
{
	/* Declare a char pointer variable to store a duplicate label. */    
	char * labelDuplicate;

		/* Verify that table exists.
		 * Check for nonexistence of label table.
		 */
		if ( ! verifyTableExists(table) )
		{
			/* This is an error (ERROR0), a fatal one.  Report error. */

			/* ERROR0: Error: label table is a NULL pointer. */

			/* Print the ERROR0 message to the standard error output. */
			printError("%s", ERROR0);
				return 0;           /* FATAL ERROR: Table doesn't exist. */
		}

        /* Was the label already in the table? */

		/* Check whether the label entry that is to be added to the label table is already exists.
		 *      If the result from the call to findLabel is a -1, then the label entry already exists.
		 */
		if (findLabel(table, label) != -1)
		{
			/* This is an error (ERROR1), but not a fatal one.
			 * Report error; don't add the label to the table again.
			 */

			/* ERROR1: Error: a duplicate label was found. */

			/* Print the ERROR1 message to the standard error output. */
			printError("%s", ERROR1);
			return 1; /* The error was not fatal, and the label was not added. */
		}

        /* Create a dynamically allocated version of label that will persist. */
        /*   NOTE: On some machines you may need to make this _strdup !  */
		/* Check for NULL in the duplicated label. */
        if ( ( labelDuplicate = strdup(label) ) == NULL )
        {
			/* This is an error (ERROR2), a fatal one.  Report error. */

			/* Print the ERROR2 message to the standard error output. */
			printError("%s", ERROR2);
            return 0;           /* FATAL ERROR: Couldn't allocate memory. */
        }

        /* Resize the table if necessary. */
        if ( table->nbrLabels >= table->capacity )
        {
			/* Resize the table and update the capacity with the new capacity.
			 * The new capacity will be double the size of the current capacity, plus one
			 *  (so when capacity equals 0, tableResize functions properly).
			 */
			
			/* Declare an int variable to store the result of a computation. */
			int result;

			/* Initialize result with the return value of a call to tableResize. */
			result = tableResize(table, (2 * table->capacity) + 1 );

			/* Check whether there was a memory allocation error. */
			if (result == 0)
			{
				/* There was a memory allocation error that has been reported within the tableResize function. */
				return 0;           /* FATAL ERROR: Couldn't allocate memory. */
			}
        }

        /* Add the label as an entry to the label table. */
		table->entries[table->nbrLabels].label = labelDuplicate;
		/* Add the address associated with the label. */
		table->entries[table->nbrLabels].address = PC;
		/* Increment, by one, the number of label entries in the label table. */
		table->nbrLabels = table->nbrLabels + 1;

        return 1;               /* Everything worked. */
}

int tableResize (LabelTable * table, int newSize)
  /* Postcondition: Table now has the capacity to hold newSize label entries.
   *                If the new size is smaller than the old size,
   *                 the table is truncated after the first newSize entries.
   *
   * Returns 1 if everything went OK;
   *         0 if memory allocation error or table doesn't exist.
   */
{
		/* Declare a Label Entry pointer variable to point to a new entry list. */    
		LabelEntry * newEntryList;
		/* Declare an int variable to store the smaller size number of label entries.  */
        int          smaller;

        /* Verify that table exists.
		 * Check for nonexistant label table.
		 */
		if ( ! verifyTableExists(table) )
		{
			/* This is an error (ERROR0), a fatal one.  Report error. */

			/* ERROR0: Error: label table is a NULL pointer.*/

			/* Print the ERROR0 message to the standard output. */
			printError("%s", ERROR0);
			return 0;           /* FATAL ERROR: Table doesn't exist. */
		}

        /* Create a new internal table of the specified size. */
        if ((newEntryList = malloc (newSize * sizeof(LabelEntry))) == NULL)
        {
            /* This is an error (ERROR2), a fatal one.  Report error. */

			/* ERROR2: Error: cannot allocate space in memory. */
			
			/* Print the ERROR2 message to the standard error. */
			printError ("%s", ERROR2);
            return 0;           /* FATAL ERROR: Couldn't allocate memory. */
        }

        /* Check whether there are label entries in the table. */
		if ( table->entries )           /* If there were entries. */
        {
            /* Move contents of internal table to new internal table. */
            smaller = table->nbrLabels < newSize ? table->nbrLabels : newSize;
            (void) memcpy (newEntryList, table->entries,
                           smaller * sizeof(LabelEntry));

            /* Free the space taken up by the old internal table. */
            free (table->entries);
			/* The number of labels in the table is now equal to the smaller size of labels. */
            table->nbrLabels = smaller;
        }

        /* Place the entry list back into the resized table. */
		table->entries = newEntryList;

		/* Assign the capacity of the label table to its new size. */
        table->capacity = newSize;

        return 1; /* Everything worked. */
}

static int verifyTableExists(LabelTable * table)
 /* Returns TRUE (1) if table exists (pointer is non-null);
  *         prints an error and returns FALSE (0) otherwise.
  */
{
        /* Verify that table exists.
		 * Check for nonexistent label table.
		 */
        if ( ! table )
        {
            /* This is an error, a fatal one. Report error. */

			/* ERROR0: Error: label table is a NULL pointer. */

			/* Print the ERROR0 message to the standard error output. */
			printError ("%s", ERROR0);
            return 0;
        }

        return 1; /* Table exists (pointer is non-null).*/
}
