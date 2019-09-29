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
	int c;
	int sum = 0;

  if(origin == NULL || destination == NULL) return -1;

	while((c = getc(origin)) != EOF){
		putc((unsigned char) c, destination);
		sum++;
	}

  return sum;
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
	while(getc(file) != (int)'\0') tam++;
	if (tam != 1) {
		string = malloc(tam);
		fseek(file, -tam, SEEK_CUR);

		//copy the string
		for(int i = 0; tam; i++){
			string[i] = (char)getc(file);
			tam--;

		}
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
	int numFiles = 0;

	fread(&numFiles, sizeof(int), 1, tarFile);
	header = malloc(sizeof(stHeaderEntry) * numFiles);

	for(int i = 0; i < numFiles; ++i){
		header[i].name = loadstr(tarFile);
			fread(&header[i].size, sizeof(int), 1, tarFile);
	}

	(*nFiles) = numFiles;
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
	FILE * dataFile = NULL;
	FILE * tarFile = NULL;
	stHeaderEntry * headers = NULL;
	int jumpBytes = 0;

	tarFile = fopen(tarName, "wb");
	if (tarFile == NULL) return EXIT_FAILURE;

	//Calculate the space needed for the headers
	headers = malloc(sizeof(stHeaderEntry) * nFiles);

	//Bytes that the program must jump to write headers content
	//numFiles + headers + headerSize
	jumpBytes = sizeof(int) + (nFiles + 1) + nFiles * sizeof(unsigned int);

	//Copies the each header into the array
	for(int i = 0; i < nFiles; ++i){
		headers[i].name = malloc(strlen(fileNames[i]) + 1); //Create space
		strcpy(headers[i].name, fileNames[i]); 									//Copy into the space created
		jumpBytes += strlen(fileNames[i]) + 1; 							//Add space needed for each header
	}

	//Move the pointer as needed
	fseek(tarFile, jumpBytes, SEEK_SET);

	//Copy the data from the files
	for(int i = 0; i < nFiles; ++i){
		dataFile = fopen(fileNames[i], "rb");
		if (dataFile == NULL) return EXIT_FAILURE; 					//Open

		jumpBytes = copynFile(dataFile, tarFile, INT_MAX); //Copy
		headers[i].size = jumpBytes;

		fclose(dataFile);																	//Close
	}

	//Move the pointer to the first btye of tarFile
	rewind(tarFile);

	//Copy the headerof the mtar
	fwrite(&nFiles, sizeof(int), 1, tarFile);
	fwrite("\n", sizeof(char), 1, tarFile);

	//Writes the filename and size for each of the header and free unneeded space
	for(int i = 0; i < nFiles; i++){
		fwrite(headers[i].name, strlen(fileNames[i]), 1, tarFile);
		fwrite("\n", sizeof(char), 1, tarFile);

		fwrite(&headers[i].size, sizeof(unsigned int), 1, tarFile);
		fwrite("\n", sizeof(char), 1, tarFile);

		free(headers[i].name);
	}

	printf("Fichero Mytar creado con exito\n");
	free(headers);
	fclose(tarFile);

	return EXIT_SUCCESS;
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
	FILE * tarFile = NULL;
	FILE * outFile = NULL;

	stHeaderEntry * ent = NULL;
	int numFiles = 0;
	int nCop = 0;

	//Validate the file / load tarball header into memory
	tarFile = fopen(tarName, "rb");

	if(tarFile == NULL) return EXIT_FAILURE;
	ent = readHeader(tarFile, &numFiles);

	//Extract files stored in the data section of the tarball
	for(int i = 0; i < numFiles; i++){
		outFile = fopen(ent[i].name, "wb");
	  copynFile(tarFile, outFile, ent[i].size);
		printf("[]%d]: Creando fichero %s, tamano %d Bytes...Ok\n", i, ent[i].name, nCop);
	}

	free(ent);
	fclose(tarFile);

	return EXIT_SUCCESS;
}
