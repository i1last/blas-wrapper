#ifndef BLAS_WRAPPER_VECTOR_HPP
#define BLAS_WRAPPER_VECTOR_HPP

#include <algorithm>
#include <type_traits>

#include "detail/fblas_l1.hpp"

namespace fvec {

template <typename T>
class Vector {
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
            Vector(other).swap(*this);
        }

        return *this;
    }

    T& operator[](size_t index) const {
        return data_[index];
    }
    
    void swap(Vector& other) {
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
        if (size_ == 0 || size_ != x.size()) return;

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
        else return;
    }
private:
    T* data_;
    size_t size_;
}; // class Vector

} // namespace vec

#endif // BLAS_WRAPPER_VECTOR_HPP