--- third_party/perfetto/include/perfetto/ext/base/event_fd.h.orig	2022-03-28 18:11:04 UTC
+++ third_party/perfetto/include/perfetto/ext/base/event_fd.h
@@ -55,6 +55,8 @@ class EventFd {
   // On Mac and other non-Linux UNIX platforms a pipe-based fallback is used.
   // The write end of the wakeup pipe.
   ScopedFile write_fd_;
+#else
+  ScopedFile write_fd_;
 #endif
 };
 
