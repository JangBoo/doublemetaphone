// DoubleMetaphoneShort.h : Declaration of the DoubleMetaphoneShort

#pragma once
#include "resource.h"       // main symbols


// IDoubleMetaphoneShort
[
	object,
	uuid("8DDA7DB4-1548-4E71-B9F7-EB6714A5CD07"),
	dual,	helpstring("IDoubleMetaphoneShort Interface"),
	pointer_default(unique)
]
__interface IDoubleMetaphoneShort : IDispatch
{
	HRESULT ComputeMetaphoneKeys([in] BSTR Word, [out] short* PrimaryKey, [out] short* AlternateKey);
	HRESULT CompareMetaphoneKeys([in] short PrimaryKey, [in] short AlternateKey1, [in] short PrimaryKey2, [in] short AlternateKey2, [out, retval] VARIANT_BOOL* Result);

	HRESULT ComputeMetaphoneKeysScr([in] BSTR Word, [out] VARIANT* PrimaryKey, [out] VARIANT* AlternateKey);
}; 



// DoubleMetaphoneShort

[
	coclass,
	threading("both"),
	vi_progid("MetaphoneCOM.DoubleMetaphoneShort"),
	progid("MetaphoneCOM.DoubleMetaphoneShort.1"),
	version(1.0),
	uuid("C762BBEB-9D86-4988-BB7E-3A9963249AB6"),
	helpstring("Adam J. Nelson's Double Metaphone Short Implementation")
]
class ATL_NO_VTABLE DoubleMetaphoneShort : 
	public IDoubleMetaphoneShort
{
public:
	DoubleMetaphoneShort()
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
	STDMETHOD(ComputeMetaphoneKeys)(/*[in]*/ BSTR Word, /*[out]*/ short* PrimaryKey, /*[out]*/ short* AlternateKey);
	STDMETHOD(CompareMetaphoneKeys)(/*[in]*/ short PrimaryKey1, /*[in]*/ short AlternateKey1, /*[in]*/ short PrimaryKey2, /*[in]*/ short AlternateKey2, /*[out, retval]*/ VARIANT_BOOL* Result);

	STDMETHOD(ComputeMetaphoneKeysScr)(/*[in]*/ BSTR Word, /*[out]*/ VARIANT* PrimaryKey, /*[out]*/ VARIANT* AlternateKey);
};

