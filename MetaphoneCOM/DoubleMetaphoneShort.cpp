/**
 * DoubleMetaphoneShort.cpp
 * 
 * Implementation of the Double Metaphone phonetic matching algorithm, wrapped as a dual-interface
 * COM component callable from Visual Basic and scripting clients.  This implementation exposes
 * the metaphone keys as shorts instead of strings, for even more efficient storage and retrieval.
 * 
 * For the latest version, implemenatations for other languages, and links to articles
 * I've written on how to use this and all my other Double Metaphone implementations, go to
 * http://www.nullpointer.net/anelson/
 * 
 * Current Version: 1.0.0
 * Revision History:
 * 	1.0.0 - ajn - First release
 * 
 * Implementation by Adam J. Nelson (anelson@nullpointer.net)
 * Copyright (C) 2003 Adam J. Nelson, All Rights Reserved
 * 
 * The Double Metaphone algorithm was written by Lawrence Phillips, and is 
 * Copyright (c) 1998, 1999 by Lawrence Philips.
 **/

#include "stdafx.h"
#include "DoubleMetaphoneShort.h"

STDMETHODIMP DoubleMetaphoneShort::ComputeMetaphoneKeys(/*[in]*/ BSTR Word, /*[out]*/ short* PrimaryKey, /*[out]*/ short* AlternateKey) {
	//Simply pass the call along to a ShortDoubleMetaphone instance
	ShortDoubleMetaphone mphone(Word);
	
	*PrimaryKey = mphone.getPrimaryShortKey();
	if (mphone.getAlternateKey()) {
		*AlternateKey = mphone.getAlternateShortKey();
	} else {
		*AlternateKey = static_cast<short>(METAPHONE_INVALID_KEY);
	}
	
	return S_OK;
}

STDMETHODIMP DoubleMetaphoneShort::CompareMetaphoneKeys(/*[in]*/ short PrimaryKey1, /*[in]*/ short AlternateKey1, /*[in]*/ short PrimaryKey2, /*[in]*/ short AlternateKey2, /*[out, retval]*/ VARIANT_BOOL* Result) {
	if ( 
			(PrimaryKey1 == PrimaryKey2) ||
			(AlternateKey2 != METAPHONE_INVALID_KEY && PrimaryKey1 == AlternateKey2) ||
			(AlternateKey1 != METAPHONE_INVALID_KEY && AlternateKey1 == PrimaryKey2) ||
			(AlternateKey1 != METAPHONE_INVALID_KEY && AlternateKey2 != METAPHONE_INVALID_KEY && AlternateKey1 == AlternateKey2)
		) {
		*Result = VARIANT_TRUE;
	} else {
		*Result = VARIANT_FALSE;
	}
	
	return S_OK;
} 

/** ComputeMetaphoneKeys, but using only IDispatch-compatible types for use with VBScript/JScript/etc */
STDMETHODIMP DoubleMetaphoneShort::ComputeMetaphoneKeysScr(/*[in]*/ BSTR Word, /*[out]*/ VARIANT* PrimaryKey, /*[out]*/ VARIANT* AlternateKey) {
	short prime, alt;
	
	ComputeMetaphoneKeys(Word, &prime, &alt);
	
	::VariantInit(PrimaryKey);
	::VariantInit(AlternateKey);
	
	V_VT(PrimaryKey) = VT_I2;
	PrimaryKey->iVal = prime;
	
	if (alt != METAPHONE_INVALID_KEY) {
		V_VT(AlternateKey) = VT_I2;
		AlternateKey->iVal = alt;
	} else {
		V_VT(AlternateKey) = VT_NULL;
	}
	
	return S_OK;
}
