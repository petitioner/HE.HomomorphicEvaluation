#include <iostream>
#include <omp.h>

int main() {
    const int N = 10;

    #pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < N; ++i) {
            std::cout << "Thread " << omp_get_thread_num() << "i = " << i << std::endl;
            #pragma omp critical
            {
                std::cout << "Thread " << omp_get_thread_num() << ": i = " << i << std::endl;
            }
        }
    }

    return 0;
}
