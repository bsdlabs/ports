--- chrome/browser/memory_details.cc.orig	2021-07-15 19:13:33 UTC
+++ chrome/browser/memory_details.cc
@@ -39,7 +39,7 @@
 #include "services/resource_coordinator/public/cpp/memory_instrumentation/memory_instrumentation.h"
 #include "ui/base/l10n/l10n_util.h"
 
-#if defined(OS_POSIX) && !defined(OS_MAC) && !defined(OS_ANDROID)
+#if defined(OS_POSIX) && !defined(OS_MAC) && !defined(OS_ANDROID) && !defined(OS_BSD)
 #include "content/public/browser/zygote_host/zygote_host_linux.h"
 #endif
 
@@ -347,7 +347,7 @@ void MemoryDetails::CollectChildInfoOnUIThread() {
       process.titles.push_back(title);
     }
 
-#if defined(OS_POSIX) && !defined(OS_MAC) && !defined(OS_ANDROID)
+#if defined(OS_POSIX) && !defined(OS_MAC) && !defined(OS_ANDROID) && !defined(OS_BSD)
     if (content::ZygoteHost::GetInstance()->IsZygotePid(process.pid)) {
       process.process_type = content::PROCESS_TYPE_ZYGOTE;
     }
