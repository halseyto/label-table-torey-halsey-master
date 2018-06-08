 /*
 * Test Driver to test the functions to access and manipulate a label table.
 * It includes the following tests:
 *
 *              DESCRIBE TEST CASES HERE
 *
 * Author:  Your_Name(s)
 *        with assistance from:
 *
 * Creation Date:  Creation_Date
 *        modified: Modification_Date        reason
 *        modified: Modification_Date        reason
 * 
 */

#include "assembler.h"

const int SAME = 0;		/* useful for making strcmp readable */
                                /* e.g., if (strcmp (str1, str2) == SAME) */

static int process_debug_choice(int argc, char * argv[]);
static void testSearch(LabelTable * table, char * searchLabel);

int main(int argc, char * argv[])
{
    /* Process command-line argument (if provided) for
     *    debugging indicator (1 = on; 0 = off).
     */
    (void) process_arguments(argc, argv);

    /* Can turn debugging on or off here (debug_on() or debug_off())
     * if not specified on the command line.
     */

    /* Create 2 tables, one static and one dynamic, for testing purposes */
    LabelEntry staticEntries[5];
    staticEntries[0].label = "Label1";
    staticEntries[0].address = 1000;
    LabelTable testTable1;      /* will be a table with static entries */
    LabelTable testTable2;      /* will be a table with dynamic entries */

    /* Initialize testTable1 with a static array of a given size. */
    testTable1.capacity = 5;
    testTable1.nbrLabels = 1;
    testTable1.entries = staticEntries;

    /* Test printLabels and findLabel with static testTable1.
     * Use testSearch to test findLabel, for example:
     *      testSearch(&testTable, "Label1");
     * Examples of basic boundary testing:
     *     Set nbrLabels to 0 and test.
     *     Put four more entries in staticEntries, set nbrLabels to 5,
     *     and test.
     */

	/* Print all label entries in the static label table. */
	printLabels(&testTable1);

	/* Use testSearch to test findLabel */
	testSearch(&testTable1, "Label1");

	/* Basic boundary testing. */

	/* Set nbrLabels to 0 and test. */
	testTable1.nbrLabels = 0;
	/* Print all label entries in the static label table. */
	printLabels(&testTable1);
	testSearch(&testTable1, "Label1");

	/* Put four more entries in staticEntries. */
	staticEntries[0].label = "Label1";
	staticEntries[0].address = 1000;
	staticEntries[1].label = "Label2";
	staticEntries[1].address = 1004;
	staticEntries[2].label = "Label3";
	staticEntries[2].address = 1008;
	staticEntries[3].label = "Label4";
	staticEntries[3].address = 1012;
	staticEntries[4].label = "Label5";
	staticEntries[4].address = 1016;

	/* Add all the static label entries into the static label table. */
	testTable1.entries = staticEntries;

	/* Set the number of labels in the static label table to 5. */
	testTable1.nbrLabels = 5;

	/* Print all the label entries in the static label table. */
	printLabels(&testTable1);

	/* Use testSearch to test findLabel.
	 *      Search for "Label3" ("Label3" should exist).
	 */
	testSearch(&testTable1, "Label3");
	/* Use testSearch to test findLabel.
	 *      Search for "Label10" ("Label10" should not exist).
	 */
	testSearch(&testTable1, "Label10");
	/* Print the capacity of the static label table to the standard output.
	 *      The capacity should be 5.
	 */
	printf("Capacity of the static label table is: %d\n", testTable1.capacity);

	printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

    /* Initialize testTable2 as a dynamic (changeable size) table. */
    tableInit(&testTable2);

	/* Print all label entries in the empty dynamic label table. */
	printLabels(&testTable2);

	/* Add label entries to the label table. */

	addLabel(&testTable2, "DynamicLabel1", 3000);
	addLabel(&testTable2, "DynamicLabel2", 3004);
	addLabel(&testTable2, "DynamicLabel3", 3008);
	addLabel(&testTable2, "DynamicLabel4", 3012);

    /* Test printLabels and findLabel with dynamic testTable2. */

	/* Print all label entries in the dynamic label table. */
	printLabels(&testTable2);
	/* Use testSearch to test findLabel.
	 *      Search for "DynamicLabel1" ("DynamicLabel1" should exist).
	 */
	testSearch(&testTable2, "DynamicLabel1");
	/* Use testSearch to test findLabel.
	*      Search for "DynamicLabel0" ("DynamicLabel0" should not exist).
	*/
	testSearch(&testTable2, "DynamicLabel3");

	/* Print the capacity of the dynamic label table to the standard output. */
	printf("Capacity of the dynamic label table: %d\n", testTable2.capacity);

	/* Print the number of labels in the dynamic label table to the standard output. */
	printf("Number of label entries in the dynamic label table: %d\n", testTable2.nbrLabels);
}


/*
 * testSearch tests the findLabel function, printing out the label being
 * searched for and either the address where it was found or, if it was
 * not found, an address of -1 and a message that the label was not in
 * the table.  
 *  @param  table        a pointer to the table through which to search
 *  @param  searchLabel  the label to search for
 */
static void testSearch(LabelTable * table, char * searchLabel)
{
    /* Declare an int variable to store the address of a label being searched for. */
	int address;

	/* Print to the standard output a message stating that the test search has begun. */
    printf("Looking for %s...\n", searchLabel);

    /* Call the findLabel function and save the address returned. */
	address = findLabel(table, searchLabel);

	/* Print the address that is returned from the findLabel function. */
    printf("\tthe address is %d.\n", address);

	/* Check for an address that does not exist. */
	if ( address == -1 )
	{
		/* Print, to the standard output, a message stating that the label did not exist. */
		printf("\tThe label you're looking for does not exist.\n");
	}
}
