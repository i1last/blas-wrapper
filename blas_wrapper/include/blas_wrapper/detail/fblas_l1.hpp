#ifndef BLAS_WRAPPER_DETAIL_FBLAS_L1_HPP 
#define BLAS_WRAPPER_DETAIL_FBLAS_L1_HPP 

#include <complex>

using blas_int = int;
using blas_complex_double = std::complex<double>;

extern "C" {
    // --------------------- Level 1 DOUBLE ---------------------

    // Level 1 - DOUBLE - axpy
    //
    // Update vector y with x:
    // --> y := alpha*x + y
    void daxpy_(
        const blas_int* n,
        const double* alpha,
        const double* x,
        const blas_int* incx,
        double* y,
        const blas_int* incy);

    // Level 1 - DOUBLE - scal
    //
    // Scale vector x by a constant:
    // --> x := alpha*x
    void dscal_(
        const blas_int* n,
        const double* alpha,
        double* x,
        const blas_int* incx);

    // Level 1 - DOUBLE - copy
    //
    // Copy vector x to vector y:
    // --> y := x
    void dcopy_(
        const blas_int* n,
        const double* x,
        const blas_int* incx,
        double* y,
        const blas_int* incy);

    // Level 1 - DOUBLE - swap
    //
    // Swap vectors x and y:
    // --> x := y,
    // --> y := x
    void dswap_(
        const blas_int* n,
        double* x,
        const blas_int* incx,
        double* y,
        const blas_int* incy);

    // Level 1 - DOUBLE - dot
    //
    // Dot product:
    // --> double result := x^T * y
    double ddot_(
        const blas_int* n,
        const double* x,
        const blas_int* incx,
        const double* y,
        const blas_int* incy);

    // Level 1 - DOUBLE - nrm2
    //
    // Get 2-norm of vector x:
    // --> double result := ||x||_2
    double dnrm2_(
        const blas_int* n,
        const double* x,
        const blas_int* incx);

    // Level 1 - DOUBLE - asum
    //
    // Get 1-norm of vector x:
    // --> double result := ||Re(x)||_1 + ||Im(x)||_1
    double dasum_(
        const blas_int* n,
        const double* x,
        const blas_int* incx);

    // Level 1 - DOUBLE - i_amax
    //
    // Get infinity-norm of vector x:
    // --> blas_int result := argmax_i(|Re(x_i)| + |Im(x_i)|)
    // IMPORTANT: Returns 1-based index (1, 2, ..., n)
    blas_int idamax_(
        const blas_int* n,
        const double* x,
        const blas_int* incx);

    // Level 1 - DOUBLE - rotg
    //
    // Generate plane rotation parameters (Givens rotation)
    // --> Given scalars a and b, computes scalars c and s such that:
    //     [ c  s ] [ a ] = [ r ]
    //     [-s  c ] [ b ] = [ 0 ]
    // --> On output:
    //     *da contains r = sqrt(a^2 + b^2) (or similar, depending on scaling)
    //     *db is overwritten (often contains info needed to reconstruct rotation)
    //     *c contains the cosine parameter
    //     *s contains the sine parameter
    void drotg_(
        double* da,
        double* db,
        double* c,
        double* s
    );

    // Level 1 - DOUBLE - rot
    //
    // Apply plane rotation (Givens rotation)
    // --> x := c*x + s*y
    // --> y := -s*x + c*y
    void drot_(
        const blas_int* n,
        double* x,
        const blas_int* incx,
        double* y,
        const blas_int* incy,
        const double* c,
        const double* s
    );

    // Level 1 - DOUBLE - rotmg
    //
    // Generate modified plane rotation parameters (for stability)
    // --> Computes parameters for a modified Givens rotation matrix H.
    // --> Input scalars d1, d2, x1, y1.
    // --> Output: Updated d1, d2, x1, and the 5-element param array.
    //     param[0] = flag determining the form of H
    //     param[1..4] = h11, h21, h12, h22 (elements of H)
    void drotmg_(
        double* d1,
        double* d2,
        double* x1,
        const double* y1,
        double param[5]
    );

    // Level 1 - DOUBLE - rotm
    //
    // Apply modified plane rotation
    // --> Applies the modified rotation H computed by drotmg_ to vectors x and y.
    // --> The specific operation depends on param[0] (flag).
    // --> [x] = H [x]
    //     [y]     [y]
    void drotm_(
        const blas_int* n,
        double* x,
        const blas_int* incx,
        double* y,
        const blas_int* incy,
        const double param[5]
    );
    

    // --------------------- Level 1 COMPLEX ---------------------

    // Level 1 - COMPLEX - axpy
    //
    // Update vector y with x:
    // --> y := alpha*x + y
    void zaxpy_(
        const blas_int* n,
        const blas_complex_double* alpha,
        const blas_complex_double* x,
        const blas_int* incx,
        blas_complex_double* y,
        const blas_int* incy);

    // Level 1 - COMPLEX - scal
    // 
    // Scale vector x by a constant:
    // --> x := alpha*x
    void zscal_(
        const blas_int* n,
        const blas_complex_double* alpha,
        blas_complex_double* x,
        const blas_int* incx);
    
    // Level 1 - COMPLEX - copy
    //
    // Copy vector x to vector y:
    // --> y := x
    void zcopy_(
        const blas_int* n,
        const blas_complex_double* x,
        const blas_int* incx,
        blas_complex_double* y,
        const blas_int* incy);

    // Level 1 - COMPLEX - swap
    //
    // Swap vectors x and y:
    // --> x := y,
    // --> y := x
    void zswap_(
        const blas_int* n,
        blas_complex_double* x,
        const blas_int* incx,
        blas_complex_double* y,
        const blas_int* incy);

    // Level 1 - COMPLEX - dotu
    //
    // Complex dot product (unconjugated):
    // --> blas_complex_double result := x^T * y
    blas_complex_double zdotu_(
        const blas_int* n,
        const blas_complex_double* x,
        const blas_int* incx,
        const blas_complex_double* y,
        const blas_int* incy);
    
    // Level 1 - COMPLEX - dotc
    //
    // Complex dot product (conjugated):
    // --> blas_complex_double result := x^H * y
    blas_complex_double zdotc_(
        const blas_int* n,
        const blas_complex_double* x,
        const blas_int* incx,
        const blas_complex_double* y,
        const blas_int* incy);

    // Level 1 - COMPLEX - nrm2
    //
    // Get 2-norm of vector x:
    // --> double result := ||x||_2    
    double dznrm2_(
        const blas_int* n,
        const blas_complex_double* x,
        const blas_int* incx);

    // Level 1 - COMPLEX - asum
    //
    // Get 1-norm of vector x:
    // --> double result := ||Re(x)||_1 + ||Im(x)||_1
    double dzasum_(
        const blas_int* n,
        const blas_complex_double* x,
        const blas_int* incx);
        
    // Level 1 - COMPLEX - i_amax
    //
    // Get infinity-norm of vector x:
    // --> blas_int result := argmax_i(|Re(x_i)| + |Im(x_i)|)
    // IMPORTANT: Returns 1-based index (1, 2, ..., n)
    blas_int izamax_(
        const blas_int* n,
        const blas_complex_double* x,
        const blas_int* incx);

    // Level 1 - COMPLEX - rotg
    //
    // Generate complex plane rotation parameters (Givens rotation)
    // --> Given complex scalars ca and cb, computes real scalar c and complex scalar s
    //     such that application yields specific properties (e.g., making an element real).
    // --> The exact transformation depends on the BLAS implementation details.
    // --> On output:
    //     *ca contains rotated value
    //     *cb is overwritten
    //     *c contains the real cosine-like parameter
    //     *s contains the complex sine-like parameter
    void zrotg_(
        blas_complex_double* ca,
        blas_complex_double* cb,
        double* c,
        blas_complex_double* s
    );

    // Level 1 - COMPLEX - rot
    //
    // Apply complex plane rotation (using parameters from zrotg_)
    // --> Note: This applies a rotation defined by a REAL c and a COMPLEX s.
    // --> MKL Definition:
    //     x := c*x + s*y
    //     y := -conj(s)*x + c*y
    // --> Standard BLAS might not have zrot exactly; csrot/zdrot apply real c, real s.
    //     This assumes availability of an MKL-like zrot extension.
    void zrot_(
        const blas_int* n,
        blas_complex_double* x,
        const blas_int* incx,
        blas_complex_double* y,
        const blas_int* incy,
        const double* c,
        const blas_complex_double* s
    );
} // extern "C"

#endif // BLAS_WRAPPER_DETAIL_FBLAS_L1_HPP 