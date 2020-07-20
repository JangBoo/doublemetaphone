/**
 * proc.cpp
 * 
 * Implementation of the SQL Server Extended stored procedure, xp_metaphone.
 * Provides SQL Server queries, stored procedures, triggers, etc, access to the
 * Double Metaphone phonetic matching algorithm.
 * 
 * For the latest version, implemenatations for other languages, and links to articles
 * I've written on how to use this and all my other Double Metaphone implementations, go to
 * http://www.nullpointer.net/anelson/
 * 
 * Implementation by Adam J. Nelson (anelson@nullpointer.net)
 * Copyright (C) 2003 Adam J. Nelson, All Rights Reserved
 * 
 * The Double Metaphone algorithm was written by Lawrence Phillips, and is 
 * Copyright (c) 1998, 1999 by Lawrence Philips.
 **/
#include <stdafx.h>
#include "..\MetaphoneLib\ShortDoubleMetaphone.h"

#define XP_NOERROR              0
#define XP_ERROR                1
#define MAXCOLNAME				25
#define MAXNAME					25
#define MAXTEXT					255

#define ERROR_WRONG_PARAM_COUNT			1
#define ERROR_WRONG_WORD_TYPE			2
#define ERROR_WRONG_KEY_TYPE			3
#define ERROR_FAILED_GET_WORD			4
#define ERROR_METAPHONE_FAILED			5
#define ERROR_WORD_NULL					6
#define ERROR_OUTPUT_FAILED				7
#define ERROR_KEY_NOT_OUTPUT			8
#define ERROR_FAILED_GET_PARAMINFO		9
#define ERROR_MEM						10

#define	PARAM_INDEX_WORD				1
#define PARAM_INDEX_KEY1				2
#define PARAM_INDEX_KEY2				3

#ifdef __cplusplus
extern "C" {
#endif

	RETCODE __declspec(dllexport) xp_metaphone(SRV_PROC *srvproc);
	ULONG __declspec(dllexport) __GetXpVersion();
	void OutputError(SRV_PROC* srvproc, int nErrNum, LPCTSTR lpszError);

#ifdef __cplusplus
}
#endif

ULONG __declspec(dllexport) __GetXpVersion() {
	return ODS_VERSION;
}

RETCODE __declspec(dllexport) xp_metaphone(SRV_PROC *srvproc) {
	int nNumParams;
	LPSTR lpszWord = NULL;
	LPWSTR lpwszWord = NULL;
	BYTE *pData = NULL;
	BYTE bType = 0;
	ULONG uMaxLen;
	ULONG uActualLen;
	USHORT uKey1, uKey2;
	BOOL bNull;
	BOOL bUnicode;
	TCHAR szError[100];

	//Check the parameter count
	nNumParams = srv_rpcparams(srvproc);

	if (nNumParams != 3) {
		//Not the right parameters
		OutputError(srvproc, ERROR_WRONG_PARAM_COUNT, _T("xp_metaphone requires three parameters: @word varchar(255), @primaryKey smallint output, @alternateKey smallint output"));

		return XP_ERROR;
	}

	//Get the word
	if (srv_paraminfo(srvproc, PARAM_INDEX_WORD, &bType, &uMaxLen, &uActualLen, NULL, &bNull) == FAIL) {
		OutputError(srvproc, ERROR_FAILED_GET_PARAMINFO, NULL);

		return XP_ERROR;
	}

	if (bType == SRVVARCHAR || bType == SRVCHAR || bType == SRVBIGVARCHAR || bType == SRVBIGCHAR)
		bUnicode = FALSE;
	else if (bType == SRVNVARCHAR || bType == SRVNCHAR)
		bUnicode = TRUE;
	else {
		//Invalid type
		wsprintf(szError, _T("The first parameter to xp_metaphone must be a char, varchar, nchar, or nvarchar, not %d"), bType);

		OutputError(srvproc, ERROR_WRONG_WORD_TYPE, szError);
		return XP_ERROR;
	}

	if (bNull) {
		//No no 
		OutputError(srvproc, ERROR_WORD_NULL, _T("The first parameter to xp_metaphone cannot be NULL"));
		return XP_ERROR;
	}

	//Allocate space
	lpszWord = new char[uActualLen+1];

	if (!lpszWord) {
		OutputError(srvproc, ERROR_MEM, NULL);
		return XP_ERROR;
	}

	memset(lpszWord, 0, uActualLen+1);

	if (bUnicode) {
		lpwszWord = new wchar_t[uActualLen+1];

		if (!lpwszWord) {
			OutputError(srvproc, ERROR_MEM, NULL);
			delete[] lpszWord;
			return XP_ERROR;
		}

		memset(lpwszWord, 0, sizeof(wchar_t) * (uActualLen+1));

		pData = reinterpret_cast<BYTE*>(lpwszWord);
	} else
		pData = reinterpret_cast<BYTE*>(lpszWord);

	//Make sure the other two params are int2
	srv_paraminfo(srvproc, PARAM_INDEX_KEY1, &bType, &uMaxLen, &uActualLen, NULL, &bNull);

	if ((bType == SRVINTN && uMaxLen != sizeof(short)) || (bType != SRVINT2 && bType != SRVINTN)) {
		OutputError(srvproc, ERROR_WRONG_KEY_TYPE, _T("The second parameter to xp_metaphone must be a smallint"));

		delete[] lpszWord;
		if(lpwszWord) delete[] lpwszWord;
		return XP_ERROR;
	}

	srv_paraminfo (srvproc, PARAM_INDEX_KEY2, &bType, &uMaxLen, &uActualLen, NULL, &bNull);

	if ((bType == SRVINTN && uMaxLen != sizeof(short)) || (bType != SRVINT2 && bType != SRVINTN)) {
		OutputError(srvproc, ERROR_WRONG_KEY_TYPE, _T("The third parameter to xp_metaphone must be a smallint"));

		delete[] lpszWord;
		if(lpwszWord) delete[] lpwszWord;

		return XP_ERROR;
	}

	// Make sure two key params are output
	if (((srv_paramstatus(srvproc, 2) & SRV_PARAMRETURN) == FAIL) || ((srv_paramstatus(srvproc, 3) & SRV_PARAMRETURN) == FAIL)) {
		OutputError(srvproc, ERROR_KEY_NOT_OUTPUT, NULL);

		delete[] lpszWord;
		if(lpwszWord) delete[] lpwszWord;

		return XP_ERROR;
	}

	//Get the input string
	if (srv_paraminfo(srvproc, 1, &bType, &uMaxLen, &uActualLen, pData, &bNull)==FAIL) {
		OutputError(srvproc, ERROR_FAILED_GET_WORD, NULL);

		delete[] lpszWord;
		if(lpwszWord) delete[] lpwszWord;

		return XP_ERROR;
	}

	if (bUnicode) {
		//Convert from unicode
		WideCharToMultiByte(CP_ACP, 0, lpwszWord, uActualLen, lpszWord, uActualLen+1, NULL, NULL);
		delete[] lpwszWord;
		lpwszWord = NULL;
	}

	//Perform the metaphone building
	ShortDoubleMetaphone mphone(lpszWord);
	uKey1 = mphone.getPrimaryShortKey();
	uKey2 = mphone.getAlternateShortKey();

	//Free the string buffer
	delete[] lpszWord;

	//Populate the return parameters
	if (srv_paramsetoutput(srvproc, PARAM_INDEX_KEY1, reinterpret_cast<BYTE*>(&uKey1), sizeof(USHORT), FALSE) == FAIL) {
		OutputError(srvproc, ERROR_OUTPUT_FAILED, NULL);

		return XP_ERROR;
	}

	//If the alternate key was also computed, set it, else set the second key param to null
	if (uKey2 != METAPHONE_INVALID_KEY) {
		srv_paramsetoutput(srvproc, PARAM_INDEX_KEY2, reinterpret_cast<BYTE*>(&uKey2), sizeof(USHORT), FALSE);
	} else {
		srv_paramsetoutput(srvproc, PARAM_INDEX_KEY2, NULL, 0, TRUE);
	}

	return XP_NOERROR ;
}

void OutputError(SRV_PROC *srvproc, int nErrNum, LPCTSTR lpszError) {
	srv_sendmsg(srvproc, 
				SRV_MSG_ERROR,
				nErrNum,
				15,
				1,
				NULL,
				NULL,
				0,
				(lpszError?const_cast<LPTSTR>(lpszError):_T("Other error (see error number)")),
				SRV_NULLTERM);
}
