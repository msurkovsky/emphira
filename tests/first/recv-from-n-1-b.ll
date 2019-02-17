; ModuleID = 'recv-from-n-1-b.cpp'
source_filename = "recv-from-n-1-b.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"class.std::ios_base::Init" = type { i8 }
%struct.MPI_Status = type { i32, i32, i32, i32, i32 }

@_ZStL8__ioinit = internal global %"class.std::ios_base::Init" zeroinitializer, align 1
@__dso_handle = external global i8
@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 65535, void ()* @_GLOBAL__sub_I_recv_from_n_1_b.cpp, i8* null }]

declare void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"*) unnamed_addr #0

declare void @_ZNSt8ios_base4InitD1Ev(%"class.std::ios_base::Init"*) unnamed_addr #0

; Function Attrs: nounwind
declare i32 @__cxa_atexit(void (i8*)*, i8*, i8*) local_unnamed_addr #1

; Function Attrs: norecurse uwtable
define i32 @main(i32, i8**) local_unnamed_addr #2 {
  %3 = alloca i32, align 4
  %4 = alloca i8**, align 8
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  store i32 %0, i32* %3, align 4, !tbaa !1
  store i8** %1, i8*** %4, align 8, !tbaa !5
  %8 = call i32 @MPI_Init(i32* nonnull %3, i8*** nonnull %4)
  %9 = bitcast i32* %5 to i8*
  call void @llvm.lifetime.start(i64 4, i8* nonnull %9) #1
  %10 = bitcast i32* %6 to i8*
  call void @llvm.lifetime.start(i64 4, i8* nonnull %10) #1
  %11 = call i32 @MPI_Comm_rank(i32 1140850688, i32* nonnull %5)
  %12 = call i32 @MPI_Comm_size(i32 1140850688, i32* nonnull %6)
  %13 = load i32, i32* %5, align 4, !tbaa !1
  %14 = icmp eq i32 %13, 0
  br i1 %14, label %15, label %52

; <label>:15:                                     ; preds = %2
  %16 = load i32, i32* %6, align 4, !tbaa !1
  %17 = add nsw i32 %16, -1
  %18 = sext i32 %17 to i64
  %19 = call { i64, i1 } @llvm.umul.with.overflow.i64(i64 %18, i64 4)
  %20 = extractvalue { i64, i1 } %19, 1
  %21 = extractvalue { i64, i1 } %19, 0
  %22 = select i1 %20, i64 -1, i64 %21
  %23 = call i8* @_Znam(i64 %22) #7
  %24 = bitcast i8* %23 to i32*
  %25 = call i8* @_Znam(i64 %22) #7
  %26 = bitcast i8* %25 to i32*
  %27 = call { i64, i1 } @llvm.umul.with.overflow.i64(i64 %18, i64 20)
  %28 = extractvalue { i64, i1 } %27, 1
  %29 = extractvalue { i64, i1 } %27, 0
  %30 = select i1 %28, i64 -1, i64 %29
  %31 = call i8* @_Znam(i64 %30) #7
  %32 = bitcast i8* %31 to %struct.MPI_Status*
  %33 = icmp sgt i32 %16, 1
  br i1 %33, label %34, label %36

; <label>:34:                                     ; preds = %15
  br label %40

; <label>:35:                                     ; preds = %40
  br label %36

; <label>:36:                                     ; preds = %35, %15
  %37 = phi i32 [ %16, %15 ], [ %49, %35 ]
  %38 = add nsw i32 %37, -1
  %39 = call i32 @MPI_Waitall(i32 %38, i32* nonnull %26, %struct.MPI_Status* nonnull %32)
  br label %55

; <label>:40:                                     ; preds = %34, %40
  %41 = phi i64 [ %48, %40 ], [ 1, %34 ]
  %42 = add nsw i64 %41, -1
  %43 = getelementptr inbounds i32, i32* %24, i64 %42
  %44 = bitcast i32* %43 to i8*
  %45 = getelementptr inbounds i32, i32* %26, i64 %42
  %46 = trunc i64 %41 to i32
  %47 = call i32 @MPI_Irecv(i8* %44, i32 1, i32 1275069445, i32 %46, i32 0, i32 1140850688, i32* %45)
  %48 = add nuw i64 %41, 1
  %49 = load i32, i32* %6, align 4, !tbaa !1
  %50 = sext i32 %49 to i64
  %51 = icmp slt i64 %48, %50
  br i1 %51, label %40, label %35

; <label>:52:                                     ; preds = %2
  %53 = bitcast i32* %7 to i8*
  call void @llvm.lifetime.start(i64 4, i8* nonnull %53) #1
  store i32 %13, i32* %7, align 4, !tbaa !1
  %54 = call i32 @MPI_Send(i8* nonnull %53, i32 1, i32 1275069445, i32 0, i32 0, i32 1140850688)
  call void @llvm.lifetime.end(i64 4, i8* nonnull %53) #1
  br label %55

; <label>:55:                                     ; preds = %52, %36
  %56 = call i32 @MPI_Finalize()
  call void @llvm.lifetime.end(i64 4, i8* nonnull %10) #1
  call void @llvm.lifetime.end(i64 4, i8* nonnull %9) #1
  ret i32 0
}

declare i32 @MPI_Init(i32*, i8***) local_unnamed_addr #0

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start(i64, i8* nocapture) #3

declare i32 @MPI_Comm_rank(i32, i32*) local_unnamed_addr #0

declare i32 @MPI_Comm_size(i32, i32*) local_unnamed_addr #0

; Function Attrs: nounwind readnone
declare { i64, i1 } @llvm.umul.with.overflow.i64(i64, i64) #4

; Function Attrs: nobuiltin
declare noalias nonnull i8* @_Znam(i64) local_unnamed_addr #5

declare i32 @MPI_Irecv(i8*, i32, i32, i32, i32, i32, i32*) local_unnamed_addr #0

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end(i64, i8* nocapture) #3

declare i32 @MPI_Waitall(i32, i32*, %struct.MPI_Status*) local_unnamed_addr #0

declare i32 @MPI_Send(i8*, i32, i32, i32, i32, i32) local_unnamed_addr #0

declare i32 @MPI_Finalize() local_unnamed_addr #0

; Function Attrs: uwtable
define internal void @_GLOBAL__sub_I_recv_from_n_1_b.cpp() #6 section ".text.startup" {
  tail call void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"* nonnull @_ZStL8__ioinit)
  %1 = tail call i32 @__cxa_atexit(void (i8*)* bitcast (void (%"class.std::ios_base::Init"*)* @_ZNSt8ios_base4InitD1Ev to void (i8*)*), i8* getelementptr inbounds (%"class.std::ios_base::Init", %"class.std::ios_base::Init"* @_ZStL8__ioinit, i64 0, i32 0), i8* nonnull @__dso_handle) #1
  ret void
}

attributes #0 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }
attributes #2 = { norecurse uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { argmemonly nounwind }
attributes #4 = { nounwind readnone }
attributes #5 = { nobuiltin "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #6 = { uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #7 = { builtin }

!llvm.ident = !{!0}

!0 = !{!"clang version 4.0.0 (tags/RELEASE_400/final)"}
!1 = !{!2, !2, i64 0}
!2 = !{!"int", !3, i64 0}
!3 = !{!"omnipotent char", !4, i64 0}
!4 = !{!"Simple C++ TBAA"}
!5 = !{!6, !6, i64 0}
!6 = !{!"any pointer", !3, i64 0}
