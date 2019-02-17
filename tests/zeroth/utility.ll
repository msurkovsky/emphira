
declare double @foo()

declare double @bar()

define double @baz(double %x) {
entry:
  %ifcond = fcmp one double %x, 0.000000e+00
  br i1 %ifcond, label %then, label %else

then:                                         ; preds = %entry
  %calltmp = call double @foo()
  ; %abc = add nsw i32 1, 3
  br label %ifcont

else:                                         ; preds = %entry
  %calltmp1 = call double @bar()
  br label %ifcont

ifcont:                                       ; preds = %else, %then
  %iftmp = phi double [ %calltmp, %then ], [ %calltmp1, %else ]
  ret double %iftmp
}