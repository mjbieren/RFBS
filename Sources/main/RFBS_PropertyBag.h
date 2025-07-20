#pragma once
#include "stdafx.h"
#include "Management.h"

class CRFBSPropertyBag :public CPropertyBag
{
protected:
	char m_szGetOrthoGroupSequenceGrabberErrMessage[255];
public:
	CRFBSPropertyBag() {};
	~CRFBSPropertyBag() {};

	/*
	Fasta Files Path (req)
	Header file Path (req)
	OutputFilePath (req)
	Bool PPP Format. (optional)
	*/

	long SetFastaFilesPath(const char* szFastaFilesPath)
	{
		return SetValueString("Paths", "Fasta", szFastaFilesPath);
	}

	long GetFastaFilesPath(std::string* pstrFastaFilesPath)
	{
		return GetValueString("Paths", "Fasta", pstrFastaFilesPath);
	}

	long SetHeaderFilePath(const char* szHeaderFilePath)
	{
		return SetValueString("Paths", "Header", szHeaderFilePath);
	}

	long GetHeaderFilePath(std::string* pstrHeaderFilePath)
	{
		return GetValueString("Paths", "Header", pstrHeaderFilePath);
	}

	long SetOutputFolderPath(const char* szOutputFolderPath)
	{
		return SetValueString("Paths", "Output", szOutputFolderPath);
	}

	long GetOutputFolderPath(std::string* pstrOutputFolderPath)
	{
		return GetValueString("Paths", "Output", pstrOutputFolderPath);
	}

	long SetPPPFormat(bool bPPPFormat = true)
	{
		return SetValueBool("Bool", "PhylopyPruner", bPPPFormat);
	}

	long GetPhyloPyPrunerFormat(bool* pbPPPFormat)
	{
		long lReturn = GetValueBool("Bool", "PhylopyPruner", pbPPPFormat, false);
		if (lReturn == PROP_ERR_SECTION_NOT_FOUND_DEFAULT_USED)
		{
			printf_s("%s %s","PhyloPyPrunerFormat Bool Not Found: default used", EOL);
			return ERR_NOERROR;
		}
		return lReturn;
	}

};