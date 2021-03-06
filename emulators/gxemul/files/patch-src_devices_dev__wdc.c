--- src/devices/dev_wdc.c.orig	2021-04-22 18:04:20 UTC
+++ src/devices/dev_wdc.c
@@ -274,12 +274,12 @@ void wdc__read(struct cpu *cpu, struct wdc_data *d)
 	    + (int64_t)d->head * d->sectors_per_track[d->drive] +
 	    (int64_t)d->heads[d->drive] * d->sectors_per_track[d->drive] * cyl);
 
-#if 0
+#if 1
 	/*  LBA:  */
 	if (d->lba)
 		offset = 512 * (((d->head & 0xf) << 24) + (cyl << 8)
 		    + d->sector);
-	printf("WDC read from offset %lli\n", (long long)offset);
+	debug("WDC read from offset %lli\n", (long long)offset);
 #endif
 
 	while (count > 0) {
@@ -320,12 +320,12 @@ void wdc__write(struct cpu *cpu, struct wdc_data *d)
 	uint64_t offset = 512 * (d->sector - 1
 	    + (int64_t)d->head * d->sectors_per_track[d->drive] +
 	    (int64_t)d->heads[d->drive] * d->sectors_per_track[d->drive] * cyl);
-#if 0
+#if 1
 	/*  LBA:  */
 	if (d->lba)
 		offset = 512 * (((d->head & 0xf) << 24) +
 		    (cyl << 8) + d->sector);
-	printf("WDC write to offset %lli\n", (long long)offset);
+	debug("WDC write to offset %lli\n", (long long)offset);
 #endif
 
 	d->write_in_progress = d->cur_command;
