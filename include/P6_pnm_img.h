#ifndef LAB04_ACS_P6_PNM_IMG_H
#define LAB04_ACS_P6_PNM_IMG_H

#include <string>
#include <vector>

struct P6_pnm_pixel {
    uint8_t r, g, b;
};

struct P6_pnm_bitmap {
    unsigned int width, height;
    std::vector<std::vector<P6_pnm_pixel>> arr;
};

class P6_pnm_img {
public:
    explicit P6_pnm_img(const std::string &path);
    unsigned int get_height() const;
    unsigned int get_width() const;
    char *get_pixels();
    void save(const std::string &path) const;
private:
    unsigned int width, height, max_val;
    char *pixel_array;
};

#endif //LAB04_ACS_P6_PNM_IMG_H
