//  Name: Jacob Villarreal
//  Email: Jacob.Villarreal2@utdallas.edu
//  Course #: CS 3377.002

#include <iostream>
#include <stdint.h>
#include <string>
#include <fstream>
#include <sstream>
#include "cdk.h"

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 25
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;


// This describes the header of the file we're reading from
struct BinaryFileHeader
{
	uint32_t magicNumber;
	uint32_t versionNumber;
	uint64_t numRecords;
};

// Describes the records contained in the file we're reading from
const int maxRecordStringLength = 25;
struct BinaryFileRecord
{
	uint8_t strLength;
	char stringBuffer[maxRecordStringLength];
};


// All of our work gets done in this function
void setMatrixContents(CDKMATRIX *matrix)
{
	// Read in the header
	ifstream binFile("cs3377.bin", ios::in | ios::binary);
	BinaryFileHeader *header = new BinaryFileHeader;
	binFile.read((char *) header, sizeof(BinaryFileHeader));

	// Write the contents of the header to a stringstream and then to the matrix cells
	stringstream ss;
	ss << "Magic: 0x" << hex << uppercase << header->magicNumber << dec;
	setCDKMatrixCell(matrix, 1, 1, ss.str().c_str()); 	
	ss.str(string());
	ss << "Version: " << header->versionNumber;
	setCDKMatrixCell(matrix, 1, 2, ss.str().c_str());
	ss.str(string());
	ss << "NumRecords: " << header->numRecords;
	setCDKMatrixCell(matrix, 1, 3, ss.str().c_str());
	ss.str(string());

	// Read in the first four records and put them in the matrix
	for (unsigned int row = 2; row <= header->numRecords + 1 && row <= 5; ++row)
	{
		BinaryFileRecord *record = new BinaryFileRecord;
		binFile.read((char *) record, sizeof(BinaryFileRecord));
		ss << "strlen: " << int(record->strLength);
		setCDKMatrixCell(matrix, row, 1, ss.str().c_str());
		setCDKMatrixCell(matrix, row, 2, record->stringBuffer);
		ss.str(string());
		delete record;
	}
	delete header;
}

int main()
{
  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix

  // Remember that matrix starts out at 1,1.
  // Since arrays start out at 0, the first entries
  // below ("R0", and "C0") are just placeholders
  // 
  // Finally... make sure your arrays have enough entries given the
  // values you choose to set for MATRIX_WIDTH and MATRIX_HEIGHT
  // above.

  const char 		*rowTitles[] = {"R0", "a", "b", "c", "d", "e"};
  const char 		*columnTitles[] = {"C0", "a", "b", "c", "d", "e"};
  int		boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED,  vMIXED,  vMIXED};

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
  */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);

  /*
   * Dipslay a message
   */
  setCDKMatrixCell(myMatrix, 2, 2, "Test Message");
  setMatrixContents(myMatrix);
  drawCDKMatrix(myMatrix, true);    /* required  */

  /* So we can see results, pause until a key is pressed. */
  unsigned char x;
  cin >> x;

  // Cleanup screen
  endCDK();
}
