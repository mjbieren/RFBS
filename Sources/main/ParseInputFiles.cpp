#include "GlobalHelper.h"

//External Functions
long SetFileBuffer(char** pPos, std::string strFileName);
long SortFastaFilesAlphabeticallyMatchingName(std::vector<CFastaFile*> *pvecFastaFiles);


//Own Functions

long ParseHeaderFile(std::vector<std::vector<char*>*>* pvecHeadersToMatch)
{
	/*
	This functions translates the header file into headers to match zero terminated string objects.
	The format is as follows:
	SAG24.12\n
	SAG33\n
	SAG128-1\n


	Meaning you can either remove complete species (Based on the strain SAG24.12) or specific headers (based on the strain + gene identification SAG128-1)
	
	*/

	long lReturn = ERR_NOERROR;
	std::string strHeaderFilePath;

	lReturn = glb.propertyBag.GetHeaderFilePath(&strHeaderFilePath);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_GETHEADERFILE_RFBS;

	//Get the file buffer;
	char* pBuffer;
	lReturn = SetFileBuffer(&pBuffer, strHeaderFilePath);
	if (lReturn != ERR_NOERROR)
		return PARSE_INPUT_FILE_HEADERFILE_SETFILEBUFFER_RFBS;

	//Set the smart buffer so we do not have to worry about a delete will happen automatically after the function finish
	CSmartPtr<char> pAutoDelete = pBuffer;

	//Set needed variables;
	char* pStart = pBuffer;
	char* pPos = pBuffer;
	char* pEnd = pBuffer;
	char* szHeaderBuffer = nullptr;
	char* szBoolHeaderOrNot = nullptr;
	size_t sizeHeaderBuffer = 0;

	std::vector<char*>* pRow = nullptr;


	while (*pEnd)
	{
		MOVE_PTR_TO_EOL_REAL(pEnd); //It also stops at an EOF :)
		MOVE_PTR_TO_TAB(pPos); //To find the separator
		if (pEnd - pStart < pEnd - pPos) //Meaning the gap is bigger. It means the format was incorrect.
			return PARSE_INPUT_FILE_HEADERFILE_WRONG_FORMAT_RFBS;
		sizeHeaderBuffer = pEnd - pStart + 10;

		//A row looks like : 
		/*
		Aagrestis_BONN	+
		Aagrestis_OXF@17242	-

		The first one being an indicator to remove all the Fasta Blocks with the strain name Aagrestis_BONN, regardless of the header number
		The second one being an indicator to remove all FastaBlocks with the complete header Aagrestis_OXF@17242 (which should translate to only one Fasta block per file)
		
		*/
		pRow = new std::vector<char*>;
		szHeaderBuffer = new char[sizeHeaderBuffer];
		memset(szHeaderBuffer, '\0', sizeHeaderBuffer); //Making sure that the end is zero terminated.
		
		//Copy the header into the sz object.
		memcpy(szHeaderBuffer, pStart, pPos-pStart ); 
		pRow->push_back(szHeaderBuffer);

		pPos++;
		szBoolHeaderOrNot = new char[sizeHeaderBuffer];
		memset(szBoolHeaderOrNot, '\0', sizeHeaderBuffer); //Making sure that the end is zero terminated.

		//Copy the bBoolHeaderOrNot into the sz object.
		memcpy(szBoolHeaderOrNot, pPos, pEnd-pPos ); 
		pRow->push_back(szBoolHeaderOrNot);

		pvecHeadersToMatch->push_back(pRow);

		//Move the pointers.
		MOVE_PTR_TO_BOL(pEnd);
		pPos = pEnd;
		pStart = pEnd;

	}

	//Done
	return lReturn;

}


long ParseFastaFiles(std::vector<CFastaFile*> *pvecFastaFilesSource, std::vector<std::string> *pvecDirList, size_t sizeDirList, std::string strFolderPath, long lCutOffStrainName, size_t * pIdxFastaFile)
{
	long lReturn = ERR_NOERROR;

	CFastaFile* pFasta = nullptr;
	
	std::string strFileFullPath;
	std::string strFastaFileName;

	for (size_t idx = 0; idx < sizeDirList; idx++)
	{
		pFasta = new CFastaFile;
		strFastaFileName = pvecDirList->at(idx);
		//We want to copy the fasta file name, but we do not want to add .fasta/.fa/etc so we remove that.
		lReturn = pFasta->SetFastaFileName(strFastaFileName.c_str(), (strFastaFileName.c_str() + (strFastaFileName.length() - lCutOffStrainName)));
		lReturn = pFasta->SetMatchingName(strFastaFileName.c_str(), (strFastaFileName.c_str() + (strFastaFileName.length() - lCutOffStrainName)));
		if (lReturn != ERR_NOERROR)
		{
			delete pFasta;
			return lReturn;
		}

		//Set the full path
		strFileFullPath = strFolderPath + strFastaFileName;
		//Parse the fasta file into the object
		printf_s("Working on Fasta File Nr:%zu %s %s", *pIdxFastaFile,strFastaFileName.c_str(), EOL);
		*pIdxFastaFile = *pIdxFastaFile + 1;
		lReturn = pFasta->ParseFastaFile(strFileFullPath.c_str());
		//Check if there is an error;
		if (lReturn != ERR_NOERROR)
		{
			delete pFasta;
			return lReturn;
		}
		//if not order the fasta file now
		lReturn = pFasta->SortFastaFileAlphabetically();
		if (lReturn != ERR_NOERROR)
		{
			delete pFasta;
			return lReturn;
		}

		//It's sorted alphabetically now and there are no errors, we can add the file into the vector.
		pvecFastaFilesSource->push_back(pFasta);
		//NExt one :)
	}
	return lReturn;

}
long ParseFastaFiles(std::vector<CFastaFile*>* pvecFastaFilesSource)
{
	//Parse all fasta files into fasta file object.
	/*
	1) Get the Path of the fasta files
	2) Get a dir list of all files in there, with .fasta, .fna, .ffn, .faa, .frn, .fa, .pep in them (7 in total).
	3) Check if there are some fasta files in them if not return error
	4) Parse the fasta files
	5) Order the Fasta files as well.
	
	*/
	long lReturn = ERR_NOERROR;

	size_t sizeFastaFilesTotal = 0;
	size_t sizeFastaDirList = 0;

	std::string strFastaFileDir;
	lReturn = glb.propertyBag.GetFastaFilesPath(&strFastaFileDir);
	if (lReturn != ERR_NOERROR)
		return PROPERTYBAG_GETFASTADIR_RFBS;


	//.FASTA
	std::vector<std::string> vecFastaFilesPaths;
	sizeFastaDirList = GetFilteredDirList(strFastaFileDir.c_str(), "*.fasta", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;

	size_t idxFasta = 1;

	if (sizeFastaDirList != 0) //There are fasta files wtih the format .fasta
	{
		lReturn = ParseFastaFiles(pvecFastaFilesSource, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 6, &idxFasta);
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}
	//Empty them
	vecFastaFilesPaths.clear();

	//.FNA
	sizeFastaDirList = GetFilteredDirList(strFastaFileDir.c_str(), "*.fna", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;
	if (sizeFastaDirList != 0) //There are fasta files wtih the format .fna
	{
		lReturn = ParseFastaFiles(pvecFastaFilesSource, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 4,&idxFasta);
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}
	//Empty them
	vecFastaFilesPaths.clear();

	//.FFN
	sizeFastaDirList = GetFilteredDirList(strFastaFileDir.c_str(), "*.ffn", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;
	if (sizeFastaDirList != 0) //There are fasta files wtih the format .ffn
	{
		lReturn = ParseFastaFiles(pvecFastaFilesSource, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 4,&idxFasta);
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}
	//Empty them
	vecFastaFilesPaths.clear();


	//.FAA
	sizeFastaDirList = GetFilteredDirList(strFastaFileDir.c_str(), "*.faa", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;
	if (sizeFastaDirList != 0) //There are fasta files wtih the format .faa
	{
		lReturn = ParseFastaFiles(pvecFastaFilesSource, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 4, &idxFasta);
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}
	//Empty them
	vecFastaFilesPaths.clear();
	

	//.FRN
	sizeFastaDirList = GetFilteredDirList(strFastaFileDir.c_str(), "*.frn", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;
	if (sizeFastaDirList != 0) //There are fasta files wtih the format .frn
	{
		lReturn = ParseFastaFiles(pvecFastaFilesSource, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 4, &idxFasta);
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}
	//Empty them
	vecFastaFilesPaths.clear();

	//.FA
	sizeFastaDirList = GetFilteredDirList(strFastaFileDir.c_str(), "*.fa", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;
	if (sizeFastaDirList != 0) //There are fasta files wtih the format .fa
	{
		lReturn = ParseFastaFiles(pvecFastaFilesSource, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 3, &idxFasta);
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}
	//Empty them
	vecFastaFilesPaths.clear();


	//.PEP
	sizeFastaDirList = GetFilteredDirList(strFastaFileDir.c_str(), "*.pep", &vecFastaFilesPaths);
	sizeFastaFilesTotal += sizeFastaDirList;
	if (sizeFastaDirList != 0) //There are fasta files wtih the format .pep
	{
		lReturn = ParseFastaFiles(pvecFastaFilesSource, &vecFastaFilesPaths, sizeFastaDirList, strFastaFileDir, 4, &idxFasta);
		if (lReturn != ERR_NOERROR)
			return lReturn;
	}
	//Empty them
	vecFastaFilesPaths.clear();

	if (sizeFastaFilesTotal == 0)
		return DIRLIST_FAIL;


	//Sort the fasta files alphabetically
	lReturn = SortFastaFilesAlphabeticallyMatchingName(pvecFastaFilesSource);
	return lReturn;
}

long ParseInputFiles(std::vector<CFastaFile*>* pvecFastaFiles, std::vector<std::vector<char*>*>* pvecHeadersToMatch)
{
	/*
	This program parses the fasta files from a folder path and the Header File from a full path
	STEP 1) ParseFastaFile
	STEP 2) ParseHeaderFile
	
	*/

	long lReturn = ERR_NOERROR;

	//Fasta Files
	printf_s("Parsing all the fasta files into the Fasta File Vector %s", EOL);
	lReturn = ParseFastaFiles(pvecFastaFiles);
	if (lReturn != ERR_NOERROR)
		return lReturn;
	printf_s("Done with parsing all the fasta files in to fasta file vector %s", EOL);


	//HeaderFile
	printf_s("Parsing the Header file into the Headers to Match object %s", EOL);
	lReturn = ParseHeaderFile(pvecHeadersToMatch);
	if (lReturn != ERR_NOERROR)
		return lReturn;
	printf_s("Done with parsing the header file into the headers to match object %s", EOL);

	//Done

	return lReturn;


}