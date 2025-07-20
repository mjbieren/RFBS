#include "GlobalHelper.h"

long MatchFastaBlocksToHeaders(std::vector<CFastaFile*>* pvecFastaFiles, std::vector<std::vector<char*>*> *pvecHeaders)
{
	/*
	This function matches the headers that have to be changed.

	//STEP 1) Loop over the FastaFiles vector
	//STEP 2) Loop over the pvecHeaders vector
	//STEP 3) Per Header idx item, find the range for the Fasta Files in the Fasta blocks. But check based on the pvecHeaders if it's a strain you have to look or a single fasta block
	//STEP 4) Check if the range matches the header, if yes set the Fasta block to write as false.
	//STEP 5) Also do the last item
	//STEP 6) Done
	
	*/
	long lReturn = ERR_NOERROR;

	std::vector<CFastaBlock*>::iterator iterLowPos;
	std::vector<CFastaBlock*>::iterator iterHighPos;
	std::vector<CFastaBlock*>::iterator iterBegin;
	std::vector<CFastaBlock*>::iterator iterEnd;

	size_t sizeVecHeaders = pvecHeaders->size();
	size_t sizeFastaFiles = pvecFastaFiles->size();

	CFastaFile* pFastaFile = nullptr;
	char* pHeader = nullptr;
	std::vector<char*> * pHeaderToMatchRow = nullptr;
	char* pHeaderToMatch = nullptr;

	CFastaBlock FastaBlockToMatch;
	CFastaBlock* pFastaBlockFromSource = nullptr;

	std::string strHeaderToMatch;
	std::string strHeaderFromFastaBlock;
	size_t sizeMatchedHeadersFlaggedRemoved = 0;

	//Step one
	for (size_t idx = 0; idx < sizeFastaFiles; idx++)
	{
		pFastaFile = pvecFastaFiles->at(idx);
		if (!pFastaFile) //if not exist
			return MATCHING_FASTAFILE_DOES_NOT_EXIST_RFBS;

		iterBegin = pFastaFile->GetIteratorFirstItem();
		iterEnd = pFastaFile->GetIteratorLastItem();

		//Step 2
		for (size_t idx2 = 0; idx2 < sizeVecHeaders; idx2++)
		{
			pHeaderToMatchRow = pvecHeaders->at(idx2);
			pHeader = pHeaderToMatchRow->at(0);
			if (!pHeader)
				return MATCHING_HEADERNAME_DOES_NOT_EXIST_RFBS;

			pHeaderToMatch = pHeaderToMatchRow->at(1);
			if (!pHeaderToMatch)
				return MATCHING_HEADERNAME_HEADERORSTRAIN_DOES_NOT_EXIST_RFBS;

			//Step 3
			if (*pHeaderToMatch == '+')
			{
				lReturn = pFastaFile->GetFirstAndLastMatchingBlockWithStrain(pHeader, &iterLowPos, &iterHighPos);
				if (lReturn != ERR_NOERROR)
					return lReturn;
			}
			else if (*pHeaderToMatch == '-')
			{
				lReturn = pFastaFile->GetFirstAndLastMatchingBlockWithCompleteHeader(pHeader, &iterLowPos, &iterHighPos);
				if (lReturn != ERR_NOERROR)
					return lReturn;
			}
			else
			{
				return MATCHING_HEADERNAME_WRONG_FORMAT;
			}
			//Step 4
			//Can have 2 situation Min and Max are equal or not. In both situation the last one has to be done

			if (iterLowPos != iterHighPos) //They are not equal
			{
				
				while (iterLowPos != iterHighPos && iterLowPos != iterEnd)
				{
					pFastaBlockFromSource = *iterLowPos;

					//The reason for the StringToUpper is because sometimes user write strain names or species names with a highercase, lowercase or a mix of that. Now we are 100% sure that they are equal in matching!
					strHeaderToMatch = CFastaFile::StringToUpper(pHeader); 
					strHeaderFromFastaBlock = CFastaFile::StringToUpper(pFastaBlockFromSource->GetFastaHeader());

					if (memcmp(strHeaderToMatch.c_str(), strHeaderFromFastaBlock.c_str(), strHeaderToMatch.length()) == 0) //Matched
					{
						//They are equal, so we have to set the FastaBlock to false.
						pFastaBlockFromSource->SetBoolFastaBlockWrite(false);
						sizeMatchedHeadersFlaggedRemoved++;

					}

					//Have to increase the iterator
					iterLowPos++;

				}
			}

			//Step 5
			if (iterLowPos == iterHighPos) //If they are the same. will happen for both situations
			{
				pFastaBlockFromSource = *iterLowPos;

				//The reason for the StringToUpper is because sometimes user write strain names or species names with a highercase, lowercase or a mix of that. Now we are 100% sure that they are equal in matching!
				strHeaderToMatch = CFastaFile::StringToUpper(pHeader); 
				strHeaderFromFastaBlock = CFastaFile::StringToUpper(pFastaBlockFromSource->GetFastaHeader());

				if (memcmp(strHeaderToMatch.c_str(), strHeaderFromFastaBlock.c_str(), strHeaderToMatch.length()) == 0) //Matched
				{
					//They are equal, so we have to set the FastaBlock to false.
					pFastaBlockFromSource->SetBoolFastaBlockWrite(false);
					sizeMatchedHeadersFlaggedRemoved++;

				}
			}
			
		}
	}



	//Step 6
	printf_s("Removed %zu Fasta Blocks from %zu Fasta Files %s", sizeMatchedHeadersFlaggedRemoved, sizeFastaFiles, EOL);

	return lReturn;
}
