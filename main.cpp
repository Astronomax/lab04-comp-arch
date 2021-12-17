#include <iostream>
#include "include/P6_pnm_img.h"
#include <cstring>
#include <omp.h>

int main(int argc, char *argv[]) {
    int threads_num = std::stoul(argv[1]);
    P6_pnm_img image(argv[2]);

    unsigned int width = image.get_width();
    unsigned int height = image.get_height();
    char *bitmap = image.get_pixels();

    #pragma omp parallel num_threads(4)
    {
        uint8_t minr = 255, ming = 255, minb = 255, maxr = 0, maxg = 0, maxb = 0;

        #pragma omp parallel for schedule(static, 4)
        for (int i = 0; i < height; ++i) {
            std::cout << omp_get_thread_num() << std::endl;
            for (int j = 0; j < width; ++j) {
                P6_pnm_pixel p{};
                memcpy(&p, bitmap + ((i * width) + j) * 3, 3);
                minr = std::min(minr, p.r);
                maxr = std::max(maxr, p.r);
                ming = std::min(ming, p.g);
                maxg = std::max(maxg, p.g);
                minb = std::min(minb, p.b);
                maxb = std::max(maxb, p.b);
            }
        }

        #pragma omp parallel for schedule(static, 4)
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                P6_pnm_pixel p{};
                memcpy(&p, bitmap + ((i * width) + j) * 3, 3);
                p.r = (p.r - minr) * 255 / (maxr - minr);
                p.g = (p.g - ming) * 255 / (maxg - ming);
                p.b = (p.b - minb) * 255 / (maxb - minb);
                memcpy(bitmap + ((i * width) + j) * 3, &p, 3);
            }
        }
    }
    image.save(argv[3]);
    return 0;
}
