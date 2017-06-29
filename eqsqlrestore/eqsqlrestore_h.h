

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for eqsqlrestore.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __eqsqlrestore_h_h__
#define __eqsqlrestore_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __Ieqsqlrestore_FWD_DEFINED__
#define __Ieqsqlrestore_FWD_DEFINED__
typedef interface Ieqsqlrestore Ieqsqlrestore;

#endif 	/* __Ieqsqlrestore_FWD_DEFINED__ */


#ifndef __eqsqlrestore_FWD_DEFINED__
#define __eqsqlrestore_FWD_DEFINED__

#ifdef __cplusplus
typedef class eqsqlrestore eqsqlrestore;
#else
typedef struct eqsqlrestore eqsqlrestore;
#endif /* __cplusplus */

#endif 	/* __eqsqlrestore_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __eqsqlrestore_LIBRARY_DEFINED__
#define __eqsqlrestore_LIBRARY_DEFINED__

/* library eqsqlrestore */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_eqsqlrestore;

#ifndef __Ieqsqlrestore_DISPINTERFACE_DEFINED__
#define __Ieqsqlrestore_DISPINTERFACE_DEFINED__

/* dispinterface Ieqsqlrestore */
/* [uuid] */ 


EXTERN_C const IID DIID_Ieqsqlrestore;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("F85E9902-0D2E-4863-BBB5-6D49F8F6EA6D")
    Ieqsqlrestore : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IeqsqlrestoreVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            Ieqsqlrestore * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            Ieqsqlrestore * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            Ieqsqlrestore * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            Ieqsqlrestore * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            Ieqsqlrestore * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            Ieqsqlrestore * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            Ieqsqlrestore * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } IeqsqlrestoreVtbl;

    interface Ieqsqlrestore
    {
        CONST_VTBL struct IeqsqlrestoreVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define Ieqsqlrestore_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define Ieqsqlrestore_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define Ieqsqlrestore_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define Ieqsqlrestore_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define Ieqsqlrestore_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define Ieqsqlrestore_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define Ieqsqlrestore_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __Ieqsqlrestore_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_eqsqlrestore;

#ifdef __cplusplus

class DECLSPEC_UUID("9B7DD6F1-6C66-4861-9E73-200B6A3EC4F9")
eqsqlrestore;
#endif
#endif /* __eqsqlrestore_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


