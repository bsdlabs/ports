--- chrome/utility/services.cc.orig	2021-07-15 19:13:36 UTC
+++ chrome/utility/services.cc
@@ -81,7 +81,7 @@
 #endif
 
 #if BUILDFLAG(ENABLE_PRINTING)
-#if defined(OS_WIN) || defined(OS_MAC) || defined(OS_LINUX) || \
+#if defined(OS_WIN) || defined(OS_MAC) || defined(OS_LINUX) || defined(OS_BSD) || \
     defined(OS_CHROMEOS)
 #include "chrome/services/printing/print_backend_service_impl.h"
 #include "chrome/services/printing/public/mojom/print_backend_service.mojom.h"
@@ -241,7 +241,7 @@ auto RunPaintPreviewCompositor(
 #endif  // BUILDFLAG(ENABLE_PAINT_PREVIEW)
 
 #if BUILDFLAG(ENABLE_PRINTING)
-#if defined(OS_WIN) || defined(OS_MAC) || defined(OS_LINUX) || \
+#if defined(OS_WIN) || defined(OS_MAC) || defined(OS_LINUX) || defined(OS_BSD) || \
     defined(OS_CHROMEOS)
 auto RunPrintBackendService(
     mojo::PendingReceiver<printing::mojom::PrintBackendService> receiver) {
@@ -355,7 +355,7 @@ void RegisterMainThreadServices(mojo::ServiceFactory& 
 #endif
 
 #if BUILDFLAG(ENABLE_PRINTING)
-#if defined(OS_WIN) || defined(OS_MAC) || defined(OS_LINUX) || \
+#if defined(OS_WIN) || defined(OS_MAC) || defined(OS_LINUX) || defined(OS_BSD) || \
     defined(OS_CHROMEOS)
   services.Add(RunPrintBackendService);
 #endif
