#ifndef BLAS_WRAPPER_VECTOR_HPP
#define BLAS_WRAPPER_VECTOR_HPP

#include <cstddef>
#include <algorithm>
#include <type_traits>
#include <complex>
#include <cassert>

#include "detail/fblas_l1.hpp"

namespace fvec {

template <typename T>
class Vector {
    static_assert(
        std::is_same_v<T, double> || std::is_same_v<T, std::complex<double>>,
        "Vector<T> only supports T = double or std::complex<double>"
    );
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

    std::complex<double> dotc(const Vector<std::complex<double>>& x) {
        static_assert(std::is_same_v<T, std::complex<double>>, "Vector::dotc is only supported for std::complex<double>");
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

    void rotg(double& da, double& db, double& c, double& s) {
        drotg_(&da, &db, &c, &s);
    }

    void rotg(std::complex<double>& ca, std::complex<double>& cb, double& c, std::complex<double>& s) {
        zrotg_(&ca, &cb, &c, &s);
    }

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

    void rotmg(double& d1, double& d2, double& x1, const double& y1, double param[5]) {
        drotmg_(&d1, &d2, &x1, &y1, param);
    }

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
private:
    T* data_;
    size_t size_;
}; // class Vector

} // namespace vec

#endif // BLAS_WRAPPER_VECTOR_HPP