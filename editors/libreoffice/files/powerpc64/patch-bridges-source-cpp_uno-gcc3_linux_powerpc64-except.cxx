--- bridges/source/cpp_uno/gcc3_linux_powerpc64/except.cxx.orig	2022-03-23 13:32:00 UTC
+++ bridges/source/cpp_uno/gcc3_linux_powerpc64/except.cxx
@@ -24,6 +24,7 @@
 #include <cxxabi.h>
 #include <rtl/strbuf.hxx>
 #include <rtl/ustrbuf.hxx>
+#include <sal/log.hxx>
 #include <osl/mutex.hxx>
 
 #include <com/sun/star/uno/genfunc.hxx>
@@ -136,7 +137,7 @@ type_info * RTTI::getRTTI( typelib_CompoundTypeDescrip
         buf.append( 'E' );
 
         OString symName( buf.makeStringAndClear() );
-        rtti = (type_info *)dlsym( m_hApp, symName.getStr() );
+        rtti = static_cast<type_info *>(dlsym( m_hApp, symName.getStr() ));
 
         if (rtti)
         {
@@ -161,9 +162,9 @@ type_info * RTTI::getRTTI( typelib_CompoundTypeDescrip
                 {
                     // ensure availability of base
                     type_info * base_rtti = getRTTI(
-                        (typelib_CompoundTypeDescription *)pTypeDescr->pBaseTypeDescription );
+                        pTypeDescr->pBaseTypeDescription );
                     rtti = new __si_class_type_info(
-                        strdup( rttiName ), (__class_type_info *)base_rtti );
+                        strdup( rttiName ), static_cast<__class_type_info *>(base_rtti ));
                 }
                 else
                 {
@@ -192,8 +193,9 @@ static void deleteException( void * pExc )
 
 static void deleteException( void * pExc )
 {
-    __cxa_exception const * header = ((__cxa_exception const *)pExc - 1);
-    typelib_TypeDescription * pTD = 0;
+    __cxxabiv1::__cxa_exception * header =
+        reinterpret_cast<__cxxabiv1::__cxa_exception *>(pExc) - 1;
+    typelib_TypeDescription * pTD = nullptr;
     OUString unoName( toUNOname( header->exceptionType->name() ) );
     ::typelib_typedescription_getByName( &pTD, unoName.pData );
     assert(pTD && "### unknown exception type! leaving out destruction => leaking!!!");
@@ -216,39 +218,59 @@ void raiseException( uno_Any * pUnoExc, uno_Mapping * 
     if (! pTypeDescr)
         terminate();
 
-    pCppExc = __cxa_allocate_exception( pTypeDescr->nSize );
+    pCppExc = __cxxabiv1::__cxa_allocate_exception( pTypeDescr->nSize );
     ::uno_copyAndConvertData( pCppExc, pUnoExc->pData, pTypeDescr, pUno2Cpp );
 
     // destruct uno exception
-    ::uno_any_destruct( pUnoExc, 0 );
+    ::uno_any_destruct( pUnoExc, nullptr );
     // avoiding locked counts
     static RTTI rtti_data;
-    rtti = (type_info*)rtti_data.getRTTI((typelib_CompoundTypeDescription*)pTypeDescr);
+    rtti = rtti_data.getRTTI(reinterpret_cast<typelib_CompoundTypeDescription*>(pTypeDescr));
     TYPELIB_DANGER_RELEASE( pTypeDescr );
     if (! rtti)
-        terminate();
+    {
+        throw RuntimeException(
+            "no rtti for type " +
+            OUString::unacquired( &pUnoExc->pType->pTypeName ) );
     }
+    }
 
-    __cxa_throw( pCppExc, rtti, deleteException );
+    __cxxabiv1::__cxa_throw( pCppExc, rtti, deleteException );
 }
 
-void fillUnoException(uno_Any * pExc, uno_Mapping * pCpp2Uno)
+void fillUnoException(uno_Any * pUnoExc, uno_Mapping * pCpp2Uno)
 {
-    __cxa_exception * header = __cxa_get_globals()->caughtExceptions;
+    __cxxabiv1::__cxa_exception * header =
+        reinterpret_cast<__cxxabiv1::__cxa_exception *>(
+             __cxxabiv1::__cxa_current_primary_exception());
     if (! header)
-        terminate();
+    {
+        RuntimeException aRE( "no exception header!" );
+        Type const & rType = cppu::UnoType<decltype(aRE)>::get();
+        uno_type_any_constructAndConvert( pUnoExc, &aRE, rType.getTypeLibType(), pCpp2Uno );
+        SAL_WARN("bridges", aRE.Message);
+        return;
+    }
+    __cxxabiv1::__cxa_decrement_exception_refcount(header);
 
-    std::type_info *exceptionType = __cxa_current_exception_type();
+    std::type_info *exceptionType = header[-1].exceptionType;
 
-    typelib_TypeDescription * pExcTypeDescr = 0;
+    typelib_TypeDescription * pExcTypeDescr = nullptr;
     OUString unoName( toUNOname( exceptionType->name() ) );
-    ::typelib_typedescription_getByName( &pExcTypeDescr, unoName.pData );
-    if (! pExcTypeDescr)
-        terminate();
-
-    // construct uno exception any
-    ::uno_any_constructAndConvert( pExc, header->adjustedPtr, pExcTypeDescr, pCpp2Uno );
-    ::typelib_typedescription_release( pExcTypeDescr );
+    typelib_typedescription_getByName( &pExcTypeDescr, unoName.pData );
+    if (pExcTypeDescr == nullptr)
+    {
+        RuntimeException aRE( "exception type not found: " + unoName );
+        Type const & rType = cppu::UnoType<decltype(aRE)>::get();
+        uno_type_any_constructAndConvert( pUnoExc, &aRE, rType.getTypeLibType(), pCpp2Uno );
+        SAL_WARN("bridges", aRE.Message);
+    }
+    else
+    {
+        // construct uno exception any
+        uno_any_constructAndConvert( pUnoExc, header[-1].adjustedPtr, pExcTypeDescr, pCpp2Uno );
+        typelib_typedescription_release( pExcTypeDescr );
+    }
 }
 
 }
