#include "../include/P6_pnm_img.h"
#include <fstream>
#include <cstring>
#include "../include/str_switch.h"
#include <iostream>
#include <cassert>

P6_pnm_img::P6_pnm_img(const std::string &path) {
    std::ifstream f(path,  std::ios::binary|std::ios::ate);
    int length = f.tellg();
    char *buffer = new char[length];
    f.seekg(0, std::ios::beg);
    f.read(buffer, length);
    f.close();

    pixel_array = strchr(strchr(strchr(buffer, '\n') + 1, '\n') + 1, '\n') + 1;

    char *p = strtok(buffer, "\n");
    std::string pnm_type(p);
    p = strtok(nullptr, " ");
    width = std::stoul(p);
    p = strtok(nullptr, "\n");
    height = std::stoul(p);
    p = strtok(nullptr, "\n");
    max_val = std::stoul(p);
    assert(pnm_type == "P6");
}

void P6_pnm_img::save(const std::string &path) const {
    std::ofstream f(path, std::ios::binary);
    f.write("P6\n", 3);
    f.write(std::to_string(width).c_str(), std::to_string(width).size());
    f.write(" ", 1);
    f.write(std::to_string(height).c_str(), std::to_string(height).size());
    f.write("\n", 1);
    f.write(std::to_string(max_val).c_str(), std::to_string(max_val).size());
    f.write("\n", 1);
    f.write(pixel_array, width * height * 3);
    f.close();
}

char *P6_pnm_img::get_pixels() {
    return pixel_array;
}

unsigned int P6_pnm_img::get_height() const {
    return height;
}

unsigned int P6_pnm_img::get_width() const {
    return width;
}
