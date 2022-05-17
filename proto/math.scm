
(define (x-point p)
    (car p))

(define (y-point p)
    (car (cdr p)))

(define (angle p a b)
    (let ((s (list (- (x-point a) (x-point p)) (- (y-point a) (y-point p)))) 
          (t (list (- (x-point b) (x-point p)) (- (y-point b) (y-point p)))))
        (acos
            (/
                (+ (* (x-point s) (x-point t)) (* (y-point s) (y-point t)))
                (* 
                    (sqrt (+ (expt (x-point s) 2) (expt (y-point s) 2)))
                    (sqrt (+ (expt (x-point t) 2) (expt (y-point t) 2))))))))

(define (circumcenter p1 p2 p3)
    (let ((A (angle p1 p2 p3)) (B (angle p2 p1 p3)) (C (angle p3 p2 p1)))
        (list
            (/
                (+ 
                    (* (x-point p1) (sin (* 2 A))) 
                    (* (x-point p2) (sin (* 2 B)))
                    (* (x-point p3) (sin (* 2 C))))
                (+ 
                    (sin (* 2 A)) 
                    (sin (* 2 B))
                    (sin (* 2 C))))
            (/
                (+ 
                    (* (y-point p1) (sin (* 2 A))) 
                    (* (y-point p2) (sin (* 2 B)))
                    (* (y-point p3) (sin (* 2 C))))
                (+ 
                    (sin (* 2 A)) 
                    (sin (* 2 B))
                    (sin (* 2 C)))))))

(define (square n)
    (* n n))

(define (x-midpoint p1 p2)
    (if (< (x-point p1) (x-point p2))
        (+ (x-point p1) (/ (- (x-point p2) (x-point p1)) 2))
        (+ (x-point p2) (/ (- (x-point p1) (x-point p2)) 2))))

(define (break-point p1 p2 s)

    (if (eqv? (y-point p1) (y-point p2))
        
        (let* ( (h 
                    (x-point p1))
                (p 
                    (/ (- (y-point p1) s) 2))
                (k  
                    (- (y-point p1) p))
                (x
                    (x-midpoint p1 p2))
                (y
                    (+
                        (/
                            (square (- x h))
                            (* 4 p))
                        k)))
            (list x y))

        (let* ( (m 
                    (/
                        (* -1 (- (x-point p2) (x-point p1)))
                        (- (y-point p2) (y-point p1))))
                (b 
                    (/
                        (+
                            (- (square (x-point p2)) (square (x-point p1)))
                            (- (square (y-point p2)) (square (y-point p1))))
                        (*
                            2
                            (- (y-point p2) (y-point p1)))))
                (h 
                    (x-point p1))
                (p 
                    (/ (- (y-point p1) s) 2))
                (k  
                    (- (y-point p1) p))
                (x (lambda (f)   ;; -2 or 2                      
                        (+
                            (*
                                (* f (sqrt p))
                                (sqrt
                                    (+
                                        b 
                                        (* h m)
                                        (- 0 k)
                                        (* m m p))))
                            h
                            (* 2 m p))))
                (x1 
                    (x 2))
                (x2 
                    (x -2)))

            (if (< (y-point p1) (y-point p2))
                (list (max x1 x2) (+ (* m (max x1 x2)) b))
                (list (min x1 x2) (+ (* m (min x1 x2)) b))))))

(define (point-distance p1 p2) 
    (sqrt
        (+
            (square (- (x-point p2) (x-point p1)))
            (square (- (y-point p2) (y-point p1))))))

(define (top-intersect p1 p2 y) 
    (let*   ((m 
                (/
                    (* -1 (- (x-point p2) (x-point p1)))
                    (- (y-point p2) (y-point p1))))
            (b 
                (/
                    (+
                        (- (square (x-point p2)) (square (x-point p1)))
                        (- (square (y-point p2)) (square (y-point p1))))
                    (*
                        2
                        (- (y-point p2) (y-point p1)))))
            
            (point-x
                (/ (- y b) m))

            (d1
                (point-distance p1 (list point-x y)))
            
            (d2
                (point-distance p2 (list point-x y))))
                        
            (begin
                (display point-x)
                (newline)
                (display d1)
                (newline)
                (display d2))))