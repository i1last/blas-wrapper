#include <iostream>
#include <cstdlib> // Для malloc/free, т.к. std::vector запрещен
#include <numeric> // Для iota (опционально, для заполнения)
#include <stdexcept> // Для ошибок

// --- Объявление функции Fortran BLAS ---
// Имя функции и соглашение о вызовах зависят от компилятора Fortran (ifx)
// и настроек сборки BLAS (MKL).
// С Intel Fortran (ifx/ifort) для стандартных имен BLAS часто используется
// соглашение без подчеркивания или полностью в верхнем регистре.
// Самый надежный способ - проверить символы в библиотеке MKL (сложно)
// или начать с наиболее вероятных вариантов.
// Попробуем вариант в нижнем регистре без подчеркивания.
// `extern "C"` предотвращает C++ name mangling.

extern "C" {
    // y := alpha*x + y
    void daxpy_(const int* n,      // Размер векторов (по указателю!)
               const double* alpha, // Скаляр alpha (по указателю!)
               const double* x,     // Вектор x (указатель на первый элемент)
               const int* incx,   // Шаг для x (по указателю!)
               double* y,     // Вектор y (указатель на первый элемент, будет изменен)
               const int* incy);  // Шаг для y (по указателю!)

    // Возможно, потребуется 'DAXPY' или 'daxpy_' - см. Шаг 2
    // Другие функции Level 1 (объявляйте по мере необходимости):
    // double ddot(const int* n, const double* x, const int* incx, const double* y, const int* incy);
    // void dscal(const int* n, const double* alpha, double* x, const int* incx);
    // double dnrm2(const int* n, const double* x, const int* incx);
    // ... и т.д.
}

// Простая функция для печати массива
void print_array(const char* name, const double* arr, int n) {
    std::cout << name << " = [ ";
    for (int i = 0; i < n; ++i) {
        std::cout << arr[i] << (i == n - 1 ? "" : " ");
    }
    std::cout << "]" << std::endl;
}


int main() {
    // Параметры для BLAS
    int n = 5;
    double alpha = 2.0;
    int incx = 1; // Шаг 1 - обычный плотный вектор
    int incy = 1; // Шаг 1

    // Выделяем память вручную (как в C)
    double* x = (double*)malloc(n * sizeof(double));
    double* y = (double*)malloc(n * sizeof(double));

    if (!x || !y) {
        std::cerr << "Memory allocation failed!" << std::endl;
        free(x); // free(nullptr) безопасен
        free(y);
        return 1;
    }

    // Инициализируем массивы
    for (int i = 0; i < n; ++i) {
        x[i] = static_cast<double>(i + 1); // x = [1.0, 2.0, 3.0, 4.0, 5.0]
        y[i] = static_cast<double>(n - i); // y = [5.0, 4.0, 3.0, 2.0, 1.0]
    }

    std::cout << "Before DAXPY:" << std::endl;
    print_array("X", x, n);
    print_array("Y", y, n);
    std::cout << "alpha = " << alpha << std::endl;

    // --- Вызов функции Fortran BLAS ---
    // ВАЖНО: ВСЕ аргументы передаются по указателю!
    daxpy_(&n, &alpha, x, &incx, y, &incy);
    // Если daxpy_ не сработает при линковке, попробуйте DAXPY(...) или daxpy_(...)
    // изменив и объявление в extern "C" блоке

    std::cout << "\nAfter DAXPY (Y = Y + alpha*X):" << std::endl;
    print_array("Y", y, n); // Ожидаемый результат: Y = [7.0, 8.0, 9.0, 10.0, 11.0]

    // Освобождаем память
    free(x);
    free(y);

    return 0;
}