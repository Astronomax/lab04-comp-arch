#include <iostream>
#include "include/P6_pnm_img.h"
#include <cstring>
#include <omp.h>

int main(int argc, char *argv[]) {
    int num_threads = std::stoul(argv[1]);
    P6_pnm_img image(argv[2]);

    unsigned int width = image.get_width();
    unsigned int height = image.get_height();
    char *bitmap = image.get_pixels();

    uint8_t minr = 255, ming = 255, minb = 255, maxr = 0, maxg = 0, maxb = 0;

    #pragma omp parallel for shared(minr, ming, minb, maxr, maxg, maxb, bitmap) schedule(static) num_threads(num_threads)
    for (int i = 0; i < height * width; ++i) {
        P6_pnm_pixel p{};
        memcpy(&p, bitmap + i * 3, 3);
        minr = std::min(minr, p.r);
        maxr = std::max(maxr, p.r);
        ming = std::min(ming, p.g);
        maxg = std::max(maxg, p.g);
        minb = std::min(minb, p.b);
        maxb = std::max(maxb, p.b);
    }
    #pragma omp parallel for shared(minr, ming, minb, maxr, maxg, maxb, bitmap) schedule(static) num_threads(num_threads)
    for (int i = 0; i < height * width; ++i) {
        P6_pnm_pixel p{};
        memcpy(&p, bitmap + i * 3, 3);
        p.r = (p.r - minr) * 255 / (maxr - minr);
        p.g = (p.g - ming) * 255 / (maxg - ming);
        p.b = (p.b - minb) * 255 / (maxb - minb);
        memcpy(bitmap + i * 3, &p, 3);
    }

    image.save(argv[3]);
    std::cout << "Time (" << num_threads << " thread(s)): " << (double) clock() / CLOCKS_PER_SEC << " ms\n";
    return 0;
}
