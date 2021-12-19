#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <omp.h>

int main(int argc, char *argv[]) {
    unsigned int num_threads = std::stoul(argv[1]);
    char *buffer;
    try {
        std::ifstream f(argv[2], std::ios::binary | std::ios::ate);
        int length = f.tellg();
        buffer = new char[length];
        f.seekg(0, std::ios::beg);
        f.read(buffer, length);
        f.close();
    }
    catch(std::exception e) {
        std::cout << "No such file";
        return 0;
    }

    char *bitmap = strchr(strchr(strchr(buffer, '\n') + 1, '\n') + 1, '\n') + 1;

    char *p = strtok(buffer, "\n");
    std::string pnm_type(p);
    p = strtok(nullptr, " ");
    unsigned int width = std::stoul(p);
    p = strtok(nullptr, "\n");
    unsigned int height = std::stoul(p);
    p = strtok(nullptr, "\n");
    unsigned int max_val = std::stoul(p);

    if(pnm_type == "P5") {
        int beg = clock();
        uint8_t ming = max_val, maxg = 0;
        #pragma omp parallel for reduction(min:ming) reduction(max:maxg) shared(bitmap) schedule(static) num_threads(num_threads)
        for (int i = 0; i < height * width; ++i) {
            uint8_t g;
            memcpy(&g, bitmap + i, 1);
            ming = std::min(ming, g);
            maxg = std::max(maxg, g);
        }
        #pragma omp parallel for shared(ming, maxg, bitmap) schedule(static) num_threads(num_threads)
        for (int i = 0; i < height * width; ++i) {
            uint8_t g;
            memcpy(&g, bitmap + i, 1);
            g = (g - ming) * max_val / (maxg - ming);
            memcpy(bitmap + i, &g, 1);
        }
        int end = clock();
        std::ofstream o(argv[3], std::ios::binary);
        o.write("P5\n", 3);
        o.write(std::to_string(width).c_str(), std::to_string(width).size());
        o.write(" ", 1);
        o.write(std::to_string(height).c_str(), std::to_string(height).size());
        o.write("\n", 1);
        o.write(std::to_string(max_val).c_str(), std::to_string(max_val).size());
        o.write("\n", 1);
        o.write(bitmap, width * height);
        o.close();
        std::cout << "Time (" << num_threads << " thread(s)): " << (double)(end - beg) / CLOCKS_PER_SEC * 1000 << " ms\n";
    }
    else if(pnm_type == "P6") {
        int beg = clock();
        uint8_t minr = max_val, ming = max_val, minb = max_val, maxr = 0, maxg = 0, maxb = 0;
        #pragma omp parallel for reduction(min:minr,ming,minb) reduction(max:maxr,maxg,maxb) shared(bitmap) schedule(static) num_threads(num_threads)
        for (int i = 0; i < height * width; ++i) {
            uint8_t r, g, b;
            memcpy(&r, bitmap + i * 3, 1);
            memcpy(&g, bitmap + i * 3 + 1, 1);
            memcpy(&b, bitmap + i * 3 + 2, 1);
            minr = std::min(minr, r);
            maxr = std::max(maxr, r);
            ming = std::min(ming, g);
            maxg = std::max(maxg, g);
            minb = std::min(minb, b);
            maxb = std::max(maxb, b);
        }
        #pragma omp parallel for shared(minr, ming, minb, maxr, maxg, maxb, bitmap) schedule(static) num_threads(num_threads)
        for (int i = 0; i < height * width; ++i) {
            uint8_t r, g, b;
            memcpy(&r, bitmap + i * 3, 1);
            memcpy(&g, bitmap + i * 3 + 1, 1);
            memcpy(&b, bitmap + i * 3 + 2, 1);
            r = (r - minr) * max_val / (maxr - minr);
            g = (g - ming) * max_val / (maxg - ming);
            b = (b - minb) * max_val / (maxb - minb);
            memcpy(bitmap + i * 3, &r, 1);
            memcpy(bitmap + i * 3 + 1, &g, 1);
            memcpy(bitmap + i * 3 + 2, &b, 1);
        }
        int end = clock();
        std::ofstream o(argv[3], std::ios::binary);
        o.write("P6\n", 3);
        o.write(std::to_string(width).c_str(), std::to_string(width).size());
        o.write(" ", 1);
        o.write(std::to_string(height).c_str(), std::to_string(height).size());
        o.write("\n", 1);
        o.write(std::to_string(max_val).c_str(), std::to_string(max_val).size());
        o.write("\n", 1);
        o.write(bitmap, width * height * 3);
        o.close();
        std::cout << "Time (" << num_threads << " thread(s)): " << (double)(end - beg) / CLOCKS_PER_SEC * 1000 << " ms\n";
    }
    else std::cout << "Unknown image type";
    return 0;
}
