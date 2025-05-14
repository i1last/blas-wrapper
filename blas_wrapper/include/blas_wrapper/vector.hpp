#ifndef BLAS_WRAPPER_VECTOR_HPP
#define BLAS_WRAPPER_VECTOR_HPP

#include <cstddef>
#include <algorithm>
#include <type_traits>
#include <complex>
#include <cassert>

#include "detail/fblas_l1.hpp"

namespace blas_wrapper {

template <typename T>
class Vector {
    static_assert(
        std::is_same_v<T, double> || std::is_same_v<T, std::complex<double>>,
        "Vector<T> only supports T = double or std::complex<double>"
    );
private:
    T* data_;
    size_t size_;
public:
    Vector() : data_(nullptr), size_(0) { }
    
    Vector(size_t n) : size_(n) {
        if (n == 0) data_ = nullptr;
        else data_ = new T[size_];
    }
    
    Vector(const Vector& other) : data_(nullptr), size_(other.size_) {
        if (size_ > 0) data_ = new T[size_];
        
        for (size_t i = 0; i < size_; i++) {
            this->data_[i] = other.data_[i];
        }
    }
    
    ~Vector() {
        delete[] data_;
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            Vector(other).swap_cv(*this);
        }

        return *this;
    }

    T& operator[](size_t index) const {
        assert(index < this->size_ && "Index out of range access");
        return data_[index];
    }
    
    void swap_cv(Vector& other) {
        std::swap(size_, other.size_);
        std::swap(data_, other.data_);
    }

    T* data() const {
        return data_;
    }

    size_t size() const {
        return size_;
    }

    // ---------- ОБЕРТКИ ----------
    
    // Update vector y with x:
    // --> y := alpha * x + y
    void axpy(T alpha, const Vector<T>& x) {
        assert(size_ != 0 && size_ == x.size() &&
                "Vector sizes must match");

        blas_int n = static_cast<blas_int>(size_);
        blas_int inc = 1;
        
        if constexpr (std::is_same_v<T, double>) {
            daxpy_(
                &n,
                &alpha,
                x.data(),
                &inc,
                this->data(),
                &inc);
        }
        else if constexpr (std::is_same_v<T, std::complex<double>>) {
            zaxpy_(
                &n,
                static_cast<blas_complex_double*>(&alpha),
                static_cast<blas_complex_double*>(x.data()),
                &inc,
                static_cast<blas_complex_double*>(this->data()),
                &inc);
        }
    }

    // Scale vector x by a constant:
    // --> x := alpha * x
    void scal(T alpha) {
        blas_int n = static_cast<blas_int>(size_);
        blas_int inc = 1;
        
        if constexpr (std::is_same_v<T, double>) {
            dscal_(
                &n,
                &alpha,
                this->data(),
                &inc);
        }
        else if constexpr (std::is_same_v<T, std::complex<double>>) {
            zscal_(
                &n,
                static_cast<blas_complex_double*>(&alpha),
                static_cast<blas_complex_double*>(this->data()),
                &inc);
        }
    }

    // Copy vector x to vector y:
    // --> y := x
    void copy(const Vector<T>& x) {
        assert(size_ == x.size() && "Vector sizes must match");

        blas_int n = static_cast<blas_int>(size_);
        blas_int inc = 1;

        if constexpr (std::is_same_v<T, double>) {
            dcopy_(
                &n,
                x.data(),
                &inc,
                this->data(),
                &inc);
        }
        else if constexpr (std::is_same_v<T, std::complex<double>>) {
            zcopy_(
                &n,
                static_cast<blas_complex_double*>(x.data()),
                &inc,
                static_cast<blas_complex_double*>(this->data()),
                &inc);
        }
    }

    // Swap vectors x and y:
    // --> x := y,
    // --> y := x
    void swap(Vector<T>& x) {
        assert(size_ == x.size() && "Vector sizes must match");

        blas_int n = static_cast<blas_int>(size_);
        blas_int inc = 1;

        if constexpr (std::is_same_v<T, double>) {
            dswap_(
                &n,
                x.data(),
                &inc,
                this->data(),
                &inc);
        }
        else if constexpr (std::is_same_v<T, std::complex<double>>) {
            zswap_(
                &n,
                static_cast<blas_complex_double*>(x.data()),
                &inc,
                static_cast<blas_complex_double*>(this->data()),
                &inc);
        }
    }

    // Dot product:
    // --> double result := x^T * y
    double dot(const Vector<double>& x) {
        static_assert(std::is_same_v<T, double>, "Vector::dot is only supported for double");
        assert(size_ == x.size() && "Vector sizes must match");

        blas_int n = static_cast<blas_int>(size_);
        blas_int inc = 1;

        return ddot_(
            &n,
            x.data(),
            &inc,
            this->data(),
            &inc
        );
    }

    // Complex dot product (unconjugated):
    // --> blas_complex_double result := x^T * y
    std::complex<double> dotu(const Vector<std::complex<double>>& x) {
        static_assert(std::is_same_v<T, std::complex<double>>, "Vector::dotu is only supported for std::complex<double>");
        assert(size_ == x.size() && "Vector sizes must match");

        blas_int n = static_cast<blas_int>(size_);
        blas_int inc = 1;

        return static_cast<std::complex<double>>(zdotu_(
            &n,
            x.data(),
            &inc,
            this->data(),
            &inc
        ));
    }

    // Complex dot product (conjugated):
    // --> blas_complex_double result := x^H * y
    std::complex<double> dotc(const Vector<std::complex<double>>& x) {
        static_assert(std::is_same_v<T, std::complex<double>>, "Vector::dotc is only supported for std::complex<double>");
        assert(size_ == x.size() && "Vector sizes must match");

        blas_int n = static_cast<blas_int>(size_);
        blas_int inc = 1;

        return static_cast<std::complex<double>>(zdotc_(
            &n,
            x.data(),
            &inc,
            this->data(),
            &inc
        ));
    }

    // Get 2-norm of vector x:
    // --> double result := ||x||_2
    double nrm2() {
        blas_int n = static_cast<blas_int>(size_);
        blas_int inc = 1;

        if constexpr (std::is_same_v<T, double>) {
            return dnrm2_(
                &n,
                this->data(),
                &inc);
        }
        else if constexpr (std::is_same_v<T, std::complex<double>>) {
            return dznrm2_(
                &n,
                static_cast<blas_complex_double*>(this->data()),
                &inc);
        }
    }

    // Get 1-norm of vector x:
    // --> double result := ||Re(x)||_1 + ||Im(x)||_1
    double asum() {
        blas_int n = static_cast<blas_int>(size_);
        blas_int inc = 1;

        if constexpr (std::is_same_v<T, double>) {
            return dasum_(
                &n,
                this->data(),
                &inc);
        }
        else if constexpr (std::is_same_v<T, std::complex<double>>) {
            return dzasum_(
                &n,
                static_cast<blas_complex_double*>(this->data()),
                &inc);
        }
    }

    // Get infinity-norm of vector x:
    // --> blas_int result := argmax_i(|Re(x_i)| + |Im(x_i)|)
    // IMPORTANT: Returns 1-based index (1, 2, ..., n)
    int i_amax() {
        blas_int n = static_cast<blas_int>(size_);
        blas_int inc = 1;

        if constexpr (std::is_same_v<T, double>) {
            return static_cast<int>(idamax_(
                &n,
                this->data(),
                &inc) - 1);  // Returns 1-based index (1, 2, ..., n)
        }
        else if constexpr (std::is_same_v<T, std::complex<double>>) {
            return static_cast<int>(izamax_(
                &n,
                static_cast<blas_complex_double*>(this->data()),
                &inc) - 1);  // Returns 1-based index (1, 2, ..., n)
        }
    }

    // Generate plane rotation parameters (Givens rotation)
    // --> Given scalars a and b, computes scalars c and s such that:
    //     [ c  s ] [ a ] = [ r ]
    //     [-s  c ] [ b ] = [ 0 ]
    // --> On output:
    //     *da contains r = sqrt(a^2 + b^2) (or similar, depending on scaling)
    //     *db is overwritten (often contains info needed to reconstruct rotation)
    //     *c contains the cosine parameter
    //     *s contains the sine parameter
    void rotg(double& da, double& db, double& c, double& s) {
        drotg_(&da, &db, &c, &s);
    }

    // Generate complex plane rotation parameters (Givens rotation)
    // --> Given complex scalars ca and cb, computes real scalar c and complex scalar s
    //     such that application yields specific properties (e.g., making an element real).
    // --> The exact transformation depends on the BLAS implementation details.
    // --> On output:
    //     *ca contains rotated value
    //     *cb is overwritten
    //     *c contains the real cosine-like parameter
    //     *s contains the complex sine-like parameter
    void rotg(std::complex<double>& ca, std::complex<double>& cb, double& c, std::complex<double>& s) {
        zrotg_(&ca, &cb, &c, &s);
    }

    // Apply plane rotation (Givens rotation)
    // --> x := c*x + s*y
    // --> y := -s*x + c*y
    void rot(Vector& x, const double& c, const T& s) {
        blas_int n = static_cast<blas_int>(size_);
        blas_int inc = 1;

        if constexpr (std::is_same_v<T, double>) {
            drot_(
                &n,
                x.data(),
                &inc,
                this->data(),
                &inc,
                &c,
                &s);
        }
        else if constexpr (std::is_same_v<T, std::complex<double>>) {
            zrot_(
                &n,
                static_cast<blas_complex_double*>(x.data()),
                &inc,
                static_cast<blas_complex_double*>(this->data()),
                &inc,
                &c,
                static_cast<blas_complex_double*>(&s));
        }
    }

    // Generate modified plane rotation parameters (for stability)
    // --> Computes parameters for a modified Givens rotation matrix H.
    // --> Input scalars d1, d2, x1, y1.
    // --> Output: Updated d1, d2, x1, and the 5-element param array.
    //     param[0] = flag determining the form of H
    //     param[1..4] = h11, h21, h12, h22 (elements of H)
    void rotmg(double& d1, double& d2, double& x1, const double& y1, double param[5]) {
        drotmg_(&d1, &d2, &x1, &y1, param);
    }

    // Apply modified plane rotation
    // --> Applies the modified rotation H computed by drotmg_ to vectors x and y.
    // --> The specific operation depends on param[0] (flag).
    // --> [x] = H [x]
    //     [y]     [y]
    void rotm(Vector<double>& x, const double param[5]) {
        blas_int n = static_cast<blas_int>(size_);
        blas_int inc = 1;

        drotmg_(
            &n,
            x.data(),
            &inc,
            this->data(),
            &inc,
            param);
    }
}; // class

} // namespace

#endif // BLAS_WRAPPER_VECTOR_HPP