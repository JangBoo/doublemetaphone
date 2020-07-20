/**
 * DoubleMetaphoneString.cpp
 * 
 * Implementation of the Double Metaphone phonetic matching algorithm, wrapped as a dual-interface
 * COM component callable from Visual Basic and scripting clients.
 * 
 * For the latest version, implemenatations for other languages, and links to articles
 * I've written on how to use this and all my other Double Metaphone implementations, go to
 * http://www.nullpointer.net/anelson/
 * 
 * Implementation by Adam J. Nelson (anelson@nullpointer.net)
 * Copyright (C) 2003 Adam J. Nelson, All Rights Reserved
 * 
 * Current Version: 1.0.0
 * Revision History:
 * 	1.0.0 - ajn - First release
 * 
 * The Double Metaphone algorithm was written by Lawrence Phillips, and is 
 * Copyright (c) 1998, 1999 by Lawrence Philips.
 **/

#include "stdafx.h"
#include "DoubleMetaphoneString.h"

#define		METAPHONE_KEY_LENGTH	4	//Phillips says 4 seems just right, and my tests do as well


/** Identical in semantics to DoubleMetaphone::doubleMetaphone */
STDMETHODIMP DoubleMetaphoneString::ComputeMetaphoneKeys(/*[in]*/ BSTR Word, /*[out]*/ BSTR* PrimaryKey, /*[out]*/ BSTR* AlternateKey) {
	//Simply pass the call along to a DoubleMetaphone instance
	DoubleMetaphone<METAPHONE_KEY_LENGTH> mphone(Word);
	
	*PrimaryKey = ::SysAllocString(mphone.getPrimaryKey());
	if (mphone.getAlternateKey()) {
		*AlternateKey = ::SysAllocString(mphone.getAlternateKey());
	} else {
		*AlternateKey = NULL;
	}
	
	return S_OK;
}

/** Compares the metaphone keys from two words, 1 and 2, testing all 4 possible comparisons for a match.
 * Returns TRUE if any pair of keys from word 1 and word 2 match, else false */
STDMETHODIMP DoubleMetaphoneString::CompareMetaphoneKeys(/*[in]*/ BSTR PrimaryKey1, /*[in]*/ BSTR AlternateKey1, /*[in]*/ BSTR PrimaryKey2, /*[in]*/ BSTR AlternateKey2, /*[out, retval]*/ VARIANT_BOOL* Result) {
	if ( 
			(wcscmp(PrimaryKey1, PrimaryKey2)==0) ||
			(AlternateKey2 != NULL && wcscmp(PrimaryKey1, AlternateKey2)==0) ||
			(AlternateKey1 != NULL && wcscmp(AlternateKey1, PrimaryKey2)==0) ||
			(AlternateKey1 != NULL && AlternateKey2 != NULL && wcscmp(AlternateKey1, AlternateKey2)==0)
		) {
		*Result = VARIANT_TRUE;
	} else {
		*Result = VARIANT_FALSE;
	}
	
	return S_OK;
}

/** ComputeMetaphoneKeys, but using only IDispatch-compatible types for use with VBScript/JScript/etc */
STDMETHODIMP DoubleMetaphoneString::ComputeMetaphoneKeysScr(/*[in]*/ BSTR Word, /*[out]*/ VARIANT* PrimaryKey, /*[out]*/ VARIANT* AlternateKey) {
	BSTR prime = NULL, alt = NULL;
	
	ComputeMetaphoneKeys(Word, &prime, &alt);
	
	::VariantInit(PrimaryKey);
	::VariantInit(AlternateKey);
	
	V_VT(PrimaryKey) = VT_BSTR;
	PrimaryKey->bstrVal = prime;
	
	if (alt != NULL) {
		V_VT(AlternateKey) = VT_BSTR;
		AlternateKey->bstrVal = alt;
	} else {
		V_VT(AlternateKey) = VT_NULL;
	}
	
	return S_OK;
}


