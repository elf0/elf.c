source_filename = "String_ParseU64.ll"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i64 @String_ParseU64(i64 noundef %v, ptr nocapture noundef %pp) {
.entry:
  %p = load ptr, ptr %pp, align 8
  br label %.next

.next:
  %p0 = phi ptr [ %p, %.entry ], [ %p1, %.evaluate ], [ %p2, %.not_overflow ]
  %v0 = phi i64 [ %v, %.entry ], [ %v2, %.evaluate ], [ %v3, %.not_overflow ]
  %c = load i8, ptr %p0, align 1
  %r = add i8 %c, -48
  %is_digit = icmp ult i8 %r, 10
  br i1 %is_digit, label %.digit, label %.end

.digit:
  %r0 = zext i8 %r to i64
  %not_overflow = icmp ult i64 %v0, 1844674407370955161
  br i1 %not_overflow, label %.evaluate, label %.maybe_overflow0

.evaluate:
  %v1 = mul nuw i64 %v0, 10
  %v2 = add nuw i64 %v1, %r0
  %p1 = getelementptr inbounds i8, ptr %p0, i64 1
  br label %.next

.not_overflow:
  %v3 = add nuw i64 %r0, 18446744073709551610
  %p2 = getelementptr inbounds i8, ptr %p0, i64 1
  br label %.next

.end:
  store ptr %p0, ptr %pp, align 8
  ret i64 %v0

.maybe_overflow0:
  %overflow0 = icmp eq i64 %v0, 1844674407370955161
  br i1 %overflow0, label %.maybe_overflow1, label %.overflow

.maybe_overflow1:
  %overflow1 = icmp ult i8 %r, 6
  br i1 %overflow1, label %.not_overflow, label %.overflow

.overflow:
  store ptr %p0, ptr %pp, align 8
  ret i64 -1
}
