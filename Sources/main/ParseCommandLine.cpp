#include "GlobalHelper.h"

#define TOTALREQUIREMENTS 4

long ParseCommandLine(int argc, char* argv[], char* envp[])
{
	long lRet = ERR_NOERROR;

	int aiRequirements[TOTALREQUIREMENTS] = { 0,0,0,0};
	for (int i = 1; i < argc; i++)
	{

		printf_s("arg %i, %s %s", i, argv[i], EOL);



		if (*argv[i] == '-') //switch detected
		{
			switch (argv[i][1])
			{
			case 'f': //Input FastaFiles Folder
				lRet = glb.propertyBag.SetFastaFilesPath(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PARSECOMMANDLINE_FASTADIR_RFBS;
				aiRequirements[0] = 1;
				break;
			case 'h': //Input Header file path
				lRet = glb.propertyBag.SetHeaderFilePath(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return 	PROPERTYBAG_GETHEADERFILE_RFBS;
				aiRequirements[1] = 1;
				break;
			case 'r': //Output dir Path
				lRet = glb.propertyBag.SetOutputFolderPath(&argv[i][3]);
				if (lRet != ERR_NOERROR)
					return PROPERTYBAG_GETOUTPUTDIR_RFBS;
				aiRequirements[2] = 1;
				break;
			case 'p': //Set PhylopyPruner format to write to yes
				lRet = glb.propertyBag.SetPPPFormat(true);
				if (lRet != ERR_NOERROR)
					return PROPERTYBAG_GETBOOLPHYLOPYPRUNERFORMAT_RFBS;
				aiRequirements[3] = 1;
				break;
			default:
			{
				printf_s("Invalid command line parameter no %i detected: %s %s", i, argv[i], EOL);
				return PARSE_COMMAND_LINE;
			}
			}

			/*

E-value

*/

		}
		else if (*argv[i] == '?')
		{
			//print arguments: ToDO change it
			printf_s(
				"%s"
				"-f <FastFileFolder>\t\t Set the Path to the directory containing your Fasta files: REQUIRED %s"
				"-h <HeaderFilePath> \t\t Set the full path to the file containing the Headers. One line per header/strain name. WITH or WITHOUT Gene identification (in case you want to remove specific entries): REQUIRED%s"
				"-r <OutputFolderPath>\t\t Set the Output Folder Path: REQUIRED %s"
				"-p \t\t Set this value if you want all the headers of your fasta file turn into the PhyloPyPruner format: NOT REQUIRED. Not Setting this will result in the Fasta files having the same headers as the input fasta file.%s"
				"%s"
				, EOL, EOL, EOL, EOL,EOL, EOL);
			return PARSE_COMMAND_LINE_MAN;
		}
		if (lRet != ERR_NOERROR)
			return lRet;
	}

	//Check if -p is set if not give a messsage to the user

	if (aiRequirements[3] == 0)
	{
		printf_s("The option to change your Fasta headers into the PhyloPyPruner format has not been set. And will result in Fasta files having the same header as the input Fasta file. If you want to change the Fasta Files to the PhylopyPruner format, you will have to do this manually %s", EOL);
		aiRequirements[3] = 1;

	}



	//Check if the other requiprements are met, if not sent a message to the user like if they would use ?. And sent back an error message.
	int iSum = 0;
	size_t idx = 0;
	while (idx < TOTALREQUIREMENTS)
	{
		iSum += aiRequirements[idx];
		idx++;
	}

	if (iSum == TOTALREQUIREMENTS)
		return lRet;
	else
	{
		//print arguments
		printf_s(
			"%s"
			"-f <FastFileFolder>\t\t Set the Path to the directory containing your Fasta files: REQUIRED %s"
			"-h <HeaderFilePath> \t\t Set the full path to the file containing the Headers. One line per header/strain name. WITH or WITHOUT Gene identification (in case you want to remove specific entries): REQUIRED%s"
			"-r <OutputFolderPath>\t\t Set the Output Folder Path: REQUIRED %s"
			"-p \t\t Set this value if you want all the headers of your fasta file turn into the PhyloPyPruner format: NOT REQUIRED. Not Setting this will result in the Fasta files having the same headers as the input fasta file.%s"
			"%s"
			, EOL, EOL, EOL, EOL,EOL, EOL);
		return PARSE_COMMAND_LINE_MAN;
	}
	
	return lRet;
}