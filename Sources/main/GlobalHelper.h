#pragma once
#include "stdafx.h"
#include "Management.h"
#include "Fasta.h"
#include "RFBS_PropertyBag.h"

class CGlobal
{
public:
	CGlobal() {};
	~CGlobal() {};

	CRFBSPropertyBag propertyBag;
	CManagement management;


};

extern CGlobal glb;


#define CHECK_RETURNVALUE(value)        \
if(value!=ERR_NOERROR)                  \
{                                       \
   glb.management.ErrorChecker(value);  \
   return (int) value;                        \
}                              

#define CHECK_RETURNVALUE_REL_PTR(value, ptr)        \
if(value!=ERR_NOERROR)                  \
{                                       \
    delete ptr;                         \
     glb.management.ErrorChecker(value); \
   return (int) value;                  \
}
