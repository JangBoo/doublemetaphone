// DoubleMetaphoneString.h : Declaration of the DoubleMetaphoneString

#pragma once
#include "resource.h"       // main symbols


// IDoubleMetaphoneString
[
	object,
	uuid("CF4FA34B-FDB2-42A5-8DD0-38338FC46320"),
	dual,	helpstring("IDoubleMetaphoneString Interface"),
	pointer_default(unique)
]
__interface IDoubleMetaphoneString : IDispatch
{
	HRESULT ComputeMetaphoneKeys([in] BSTR Word, [out] BSTR* PrimaryKey, [out] BSTR* AlternateKey);
	HRESULT CompareMetaphoneKeys([in] BSTR PrimaryKey, [in] BSTR AlternateKey1, [in] BSTR PrimaryKey2, [in] BSTR AlternateKey2, [out, retval] VARIANT_BOOL* Result);
	
	HRESULT ComputeMetaphoneKeysScr([in] BSTR Word, [out] VARIANT* PrimaryKey, [out] VARIANT* AlternateKey);
};



// DoubleMetaphoneString

[
	coclass,
	threading("both"),
	vi_progid("MetaphoneCOM.DoubleMetaphoneString"),
	progid("MetaphoneCOM.DoubleMetaphoneString.1"),
	version(1.0),
	uuid("EDDED454-8E77-472D-9AAC-D690AFDF2BCB"),
	helpstring("Adam J. Nelson's Double Metaphone String Implementation")
]
class ATL_NO_VTABLE DoubleMetaphoneString : 
	public IDoubleMetaphoneString
{
public:
	DoubleMetaphoneString()
	{
	}


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:
	STDMETHOD(ComputeMetaphoneKeys)(/*[in]*/ BSTR Word, /*[out]*/ BSTR* PrimaryKey, /*[out]*/ BSTR* AlternateKey);
	STDMETHOD(CompareMetaphoneKeys)(/*[in]*/ BSTR PrimaryKey1, /*[in]*/ BSTR AlternateKey1, /*[in]*/ BSTR PrimaryKey2, /*[in]*/ BSTR AlternateKey2, /*[out, retval]*/ VARIANT_BOOL* Result);

	STDMETHOD(ComputeMetaphoneKeysScr)(/*[in]*/ BSTR Word, /*[out]*/ VARIANT* PrimaryKey, /*[out]*/ VARIANT* AlternateKey);
};

