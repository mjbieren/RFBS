#include "GlobalHelper.h"
CGlobal glb;

long ParseCommandLine(int argc, char* argv[], char* envp[]);
long CheckReturnValue(long lReturn, std::vector<CFastaFile*>* pvecFastaFiles, std::vector<std::vector<char*>*>* pvecHeaders);


long ParseInputFiles(std::vector<CFastaFile*>* pvecFastaFiles, std::vector<std::vector<char*>*>* pvecHeadersToMatch);
long MatchFastaBlocksToHeaders(std::vector<CFastaFile*>* pvecFastaFiles, std::vector<std::vector<char*>*>* pvecHeaders);
long WriteOutputFiles(std::vector<CFastaFile*>* pvecFastaFiles);

/*
RFBS (Remove Fasta Blocks Sequences) is a tool that remove certain strains/headers from a fasta file. Based on the user input.
The User creates a txt tile with for every line one strain (without the Gene identification) and uses that to flag the FastaBlocks as false (to write)


STEP 1) Parse Command line
STEP 2) Parse input files (fasta files & Header file)
STEP 3) Find the FastaBlocks and Match (Flag them as false for writing)
STEP 4) Write Output Files
STEP 5) Cleanup
STEP 6) Done
*/

int main(int argc, char* argv[], char* envp[])
{
	 long lReturn = ERR_NOERROR;

    //STEP ONE
    printf_s("Parsing the command line %s", EOL);
    lReturn = ParseCommandLine(argc, argv, envp);
    CHECK_RETURNVALUE(lReturn);
    printf_s("Done with parsing the command line%s", EOL);

    //Set All the variables;
    std::vector<CFastaFile*> vecFastaFiles;
    std::vector<std::vector<char*>*> vecHeadersToMatch;


    //STEP TWO
    printf_s("Parse all the input files %s", EOL);
    lReturn = ParseInputFiles(&vecFastaFiles, &vecHeadersToMatch);
    CheckReturnValue(lReturn, &vecFastaFiles, &vecHeadersToMatch);
    CHECK_RETURNVALUE(lReturn);
    printf_s("Done with parsing all the input files in their appriopriate objects %s", EOL);


    //STEP THREE
    printf_s("Search all the matching fasta blocks corresponding to the Header File and flag them as false for writing the fasta files %s", EOL);
    lReturn = MatchFastaBlocksToHeaders(&vecFastaFiles, &vecHeadersToMatch);
    CheckReturnValue(lReturn, &vecFastaFiles, &vecHeadersToMatch);
    CHECK_RETURNVALUE(lReturn);
    printf_s("Done with Matching %s", EOL);

    //STEP FOUR
    printf_s("Write all the Fasta files to the output folder %s", EOL);
    lReturn = WriteOutputFiles(&vecFastaFiles);
    CheckReturnValue(lReturn, &vecFastaFiles, &vecHeadersToMatch);
    CHECK_RETURNVALUE(lReturn);
    printf_s("Done with creating all the fasta files %s", EOL);

    //STEP FIVE
    printf_s("Cleaning up the program variables. %s", EOL);
    ClearVector<CFastaFile>(&vecFastaFiles);

    size_t sizeVectorHeader = vecHeadersToMatch.size();
    std::vector<char*>* pRow = nullptr;
    for (size_t idx = 0; idx < sizeVectorHeader; idx++)
    {
        pRow = vecHeadersToMatch.at(idx);
        ClearVector<char>(pRow);
    }

    vecHeadersToMatch.clear();

    printf_s("Done with cleaning up%s", EOL);


    //STEP SIX
    printf_s("Done with the program, thank you for using 'Remove Fasta Block Sequences', Have a nice day ;)%s", EOL);


    return 0;
}