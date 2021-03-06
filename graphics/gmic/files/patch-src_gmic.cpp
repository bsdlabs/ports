--- src/gmic.cpp.orig	2022-05-11 08:15:20 UTC
+++ src/gmic.cpp
@@ -2516,7 +2516,7 @@ double gmic::mp_store(const Ts *const ptr, const unsig
 // Manage correspondence between abort pointers and thread ids.
 CImgList<void*> gmic::list_p_is_abort = CImgList<void*>();
 bool *gmic::abort_ptr(bool *const p_is_abort) {
-#if defined(__MACOSX__) || defined(__APPLE__)
+#if defined(__MACOSX__) || defined(__APPLE__) || defined(__FreeBSD__)
   void* tid = (void*)(cimg_ulong)getpid();
 #elif cimg_OS==1
   void* tid = (void*)(cimg_ulong)syscall(SYS_gettid);
@@ -2858,7 +2858,7 @@ gmic::~gmic() {
   cimg::exception_mode(cimg_exception_mode);
   cimg_forX(display_windows,l) delete &display_window(l);
   cimg::mutex(21);
-#if defined(__MACOSX__) || defined(__APPLE__)
+#if defined(__MACOSX__) || defined(__APPLE__) || defined(__FreeBSD__)
   void* tid = (void*)(cimg_ulong)getpid();
 #elif cimg_OS==1
   void* tid = (void*)(cimg_ulong)syscall(SYS_gettid);
