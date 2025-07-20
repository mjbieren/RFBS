#include "GlobalHelper.h"

long CheckReturnValue(long lReturn, std::vector<CFastaFile*>* pvecFastaFiles, std::vector<std::vector<char*>*>* pvecHeaders)
{
    if (lReturn != ERR_NOERROR)
    {    
        ClearVector<CFastaFile>(pvecFastaFiles);

        size_t sizeVecHeaders = pvecHeaders->size();
        std::vector<char*>* pRow = nullptr;
        for (size_t idx = 0; idx < sizeVecHeaders; idx++)
        {
            pRow = pvecHeaders->at(idx);
            ClearVector<char>(pRow);
        }

        pvecHeaders->clear();

    }

    return lReturn;

}