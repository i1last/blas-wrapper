#include <blas_wrapper/vector.hpp>
#include <iostream>
#include <vector>

template <typename T>
void print_vector(const std::string& name, const fvec::Vector<T>& v) {
    std::cout << name << " (" << v.size() << ") = [ ";
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i] << (i == v.size() - 1 ? "" : " ");
    }
    std::cout << " ]" << std::endl;
}

int main() {
    using fvec::Vector;

    size_t n = 5;
    double alpha = 2.0;

    Vector<double> x(n);
    Vector<double> y(n);

    for (size_t i = 0; i < n; ++i) {
        x[i] = static_cast<double>(i + 1); // x = [1.0, 2.0, 3.0, 4.0, 5.0]
        y[i] = static_cast<double>(n - i); // y = [5.0, 4.0, 3.0, 2.0, 1.0]
    }

    std::cout << "--- Testing DAXPY ---" << std::endl;
    std::cout << "Before DAXPY:" << std::endl;
    print_vector("X", x);
    print_vector("Y", y);
    std::cout << "alpha = " << alpha << std::endl;

    // y := alpha * x + y
    y.axpy(alpha, x);

    std::cout << "\nAfter DAXPY ( y := alpha * x + y ):" << std::endl;
    print_vector("Y", y);

    return 0;
}