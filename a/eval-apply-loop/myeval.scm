(define restart #f)

(define (myerror msg . args)
  (display msg)
  (for-each (lambda (a)
              (display " ")
              (display a))
            args)
  (newline)
  (restart #t))

(define (myeval exp env)
  (if (not (pair? exp))
      (cond ((or (number? exp) (boolean? exp) (string? exp))
             exp)
            ((symbol? exp)
             (lookup-variable-value exp env)))
      (case (car exp)
        ((quote) (cadr exp))
        ((lambda)
         (if (not (null? (cdddr exp)))
             (myerror "The body has too many expressions -- LAMBDA" (cddr exp))
             (list 'function   ; tag
                   (cadr exp)  ; parameters
                   (caddr exp) ; body（式１個）
                   env)))
        ((if)
         (if (true? (myeval (cadr exp) env))
             (myeval (caddr exp) env) 
             (if (null? (cdddr exp))
                 #f   ; else節がない場合の値
                 (myeval (cadddr exp) env))))
        ((define) 
         (cond ((not (symbol? (cadr exp)))
                (myerror "Not a variable -- DEFINE" (cadr exp)))
               (else
                (define-variable!
                  (cadr exp)               ; 変数名
                  (myeval (caddr exp) env) ; 式の値
                  env)
                'ok)))
        ((set!)
         (set-variable-value!
          (cadr exp)                ; 変数名
          (myeval (caddr exp) env)  ; 式の値
          env)
         'ok)
        (else
         ; 特殊形式でなければ関数呼出し
         (myapply (myeval (car exp) env)
                  (map (lambda (x) (myeval x env))
                       (cdr exp)))))))

(define (myapply func arguments)
  (case (car func)
    ((primitive)  ; （ソース言語上の）組込み関数
     (apply-primitive-function func arguments))
    ((function)   ; （ソース言語上の）lambda式による関数の呼出し
     (myeval (caddr func)        ; body
             (extend-environment ; 環境を拡張
              (cadr func)        ; parameters
              arguments          ; 実引数
              (cadddr func))))   ; env
    (else
     (myerror "Unknown procedure type -- APPLY" func))))

(define (true? x) (not (eq? x #f)))

(define (extend-environment vars vals base-env)
  (if (= (length vars) (length vals))
      (cons (cons vars vals) base-env)
      (if (< (length vars) (length vals))
          (myerror "Too many arguments supplied" vars vals)
          (myerror "Too few arguments supplied" vars vals))))

(define (lookup-variable-value var env)
  (define (env-loop env)     ; envの先頭フレームから順にscanで探す
    (define (scan vars vals) ; 1つのフレーム中の束縛を走査
      (cond ((null? vars)
             ; 外側の環境を探す
             (env-loop (cdr env)))  ; 親のフレームへ
            ((eq? var (car vars)) (car vals))
            (else (scan (cdr vars) (cdr vals)))))
    (if (null? env)
        (myerror "Unbound variable" var)
        (let ((frame (car env)))
          (scan (car frame) (cdr frame)))))
  (env-loop env))

(define (define-variable! var val env)
  (let ((frame (car env)))
    (define (scan vars vals)
      (cond ((null? vars)
             ; 最初のフレームに新しい束縛を追加
             (set-car! frame (cons var (car frame)))
             (set-cdr! frame (cons val (cdr frame))))
            ((eq? var (car vars))
             ; 束縛が存在すれば値の変更
             (set-car! vals val))
            (else (scan (cdr vars) (cdr vals)))))
    (scan (car frame) (cdr frame))))

(define (set-variable-value! var val env)
  (define (env-loop env)
    (define (scan vars vals)
      (cond ((null? vars)
             (env-loop (cdr env)))
            ((eq? var (car vars))
             (set-car! vals val))
            (else (scan (cdr vars) (cdr vals)))))
    (if (null? env)
        (myerror "Unbound variable -- SET!" var)
        (let ((frame (car env)))
          (scan (car frame) (cdr frame)))))
  (env-loop env))

(define (setup-environment)
  (extend-environment '(car cdr cddr cdddr cadr caddr cadddr
                        cons list null? = < > <= >= + - * / not
                        write display newline)
                      (map (lambda (f) (cons 'primitive f))
                           (list car cdr cddr cdddr cadr caddr cadddr
                                 cons list null? = < > <= >= + - * / not
                                 write display newline))
                      '()))  ;; the empty environment

(define the-global-environment (setup-environment))

; ソース言語の組込み関数を呼び出す
(define (apply-primitive-function func args)
  (apply (cdr func) args)) ; このapplyは記述言語のapply

(define (driver-loop)
  (newline) (display "=> ")
  (call-with-current-continuation
   (lambda (k)
     (set! restart k)
     (let ((input (read)))
       (if (eof-object? input)
           (exit 0)
           (let ((output (myeval input the-global-environment)))
             (if (and (pair? output) (eq? (car output) 'function))
                 (display (list 'compound-function
                                (cadr output)   ; parameters
                                (caddr output)  ; body
                                '<function-env>))
                 (display output)))))))
  (driver-loop))