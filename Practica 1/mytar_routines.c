#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytar.h"

extern char *use;

/** Copy nBytes bytes from the origin file to the destination file.
 *
 * origin: pointer to the FILE descriptor associated with the origin file
 * destination:  pointer to the FILE descriptor associated with the destination file
 * nBytes: number of bytes to copy
 *
 * Returns the number of bytes actually copied or -1 if an error occured.
 */
int copynFile(FILE * origin, FILE * destination, int nBytes) {
	int ret, c;
	int sum = 0;

	while((c = getc(origin)) != EOF){
		ret = putc((unsigned char) c, destination);
		if(ret == EOF) return -1;
		else sum++;		
	}	
	if(c == EOF) return -1;
	else return sum;
}

/** Loads a string from a file.
 *
 * file: pointer to the FILE descriptor 
 * 
 * The loadstr() function must allocate memory from the heap to store 
 * the contents of the string read from the FILE. 
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc()) 
 * 
 * Returns: !=NULL if success, NULL if error
 */
char* loadstr(FILE * file) {
	char* string = NULL;
	int tam = 1; //Por el '\0'

	//get tam of the array
	while(getc(file)) != (int)'\0') tam++;
	if (tam != 1) {
		string = malloc(tam);
		fseek(file, -tam, SEEK_CUR);

		//copy the string
		for(int i = 0; tam; i++){
			string[i] = (char)getc(file);	
			tam--;

	}
	return string;
}

/** Read tarball header and store it in memory.
 *
 * tarFile: pointer to the tarball's FILE descriptor 
 * nFiles: output parameter. Used to return the number 
 * of files stored in the tarball archive (first 4 bytes of the header).
 *
 * On success it returns the starting memory address of an array that stores 
 * the (name,size) pairs read from the tar file. Upon failure, the function returns NULL.
 */
stHeaderEntry* readHeader(FILE * tarFile, int *nFiles) {
	stHeaderEntry * header = NULL;

	if(fread(nFiles, sizeof(int), 1, tarFile) == 1) {
		header = malloc(sizeof(stHeaderEntry * nFiles));
		for(int i = 0; i < nFiles; ++i){
			header[i].name = loadstr(tarFile);
 			fread(header[i].size, sizeof(int), 1, tarFile);
		}
	}
	return header;
}

/** Creates a tarball archive 
 *
 * nfiles: number of files to be stored in the tarball
 * filenames: array with the path names of the files to be included in the tarball
 * tarname: name of the tarball archive
 * 
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First reserve room in the file to store the tarball header.
 * Move the file's position indicator to the data section (skip the header)
 * and dump the contents of the source files (one by one) in the tarball archive. 
 * At the same time, build the representation of the tarball header in memory.
 * Finally, rewind the file's position indicator, write the number of files as well as 
 * the (file name,file size) pairs in the tar archive.
 *
 * Important reminder: to calculate the room needed for the header, a simple sizeof 
 * of stHeaderEntry will not work. Bear in mind that, on disk, file names found in (name,size) 
 * pairs occupy strlen(name)+1 bytes.
 *
 */
int createTar(int nFiles, char *fileNames[], char tarName[]) {
	int sizeHeader = sizeof(int);

	//Calc the room needed for the header
	for(int i = 0; i < nFiles; ++i) sizeHeader += strlen(fileNames[i] + 1);

	//Move the file's position indicator to the data section
	FILE * tarFile = fopen(tarName, 'w');
	fseek(tarFile,sizeHeader,SEEK_SET);
	//Dump the contest of the source files (one by one)
	FILE * file; char buff;
	for(int i = 0; i < nFiles; ++i){
		file = fopen(fileNames[i]);
		while(feof(file)== 0){
			fread(buff,1,1,file);
			fwrite(buff,1,1,tarFile);
		}
		fclose(file);
	}
	
	return EXIT_FAILURE;
}

/** Extract files stored in a tarball archive
 *
 * tarName: tarball's pathname
 *
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First load the tarball's header into memory.
 * After reading the header, the file position indicator will be located at the 
 * tarball's data section. By using information from the 
 * header --number of files and (file name, file size) pairs--, extract files 
 * stored in the data section of the tarball.
 *
 */
int extractTar(char tarName[]) {
	// Complete the function
	return EXIT_FAILURE;
}
