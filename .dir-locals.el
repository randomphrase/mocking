;;; Directory Local Variables
;;; For more information see (info "(emacs) Directory Variables")

((c++-mode
  (helm-make-list-target-method . ninja)
  (helm-make-build-dir . "build.gcc.dbg")
  (helm-make-executable . "ninja")
  (helm-make-arguments . "-j4")
  ))


