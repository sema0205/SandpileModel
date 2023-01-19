#include "../headers/image.h"

Color::Color() {
    r = 0;
    g = 0;
    b = 0;
}

Color::Color(float e, float f, float j) {
    r = e;
    g = f;
    b = j;
}


Image::Image(int width, int length) {
    image_width = width;
    image_length = length;
    image_colors = new Color[width*length];
}

Color Image::GetColor(int x, int y) const {

    // y shows how many rows we up and image.width is an amount of pixels for each row, x refers to specific pixel
    return image_colors[y*image_width+x];
}

void Image::SetColor(const Color& color, int x, int y) const {
    image_colors[y*image_width+x].r = color.r;
    image_colors[y*image_width+x].g = color.g;
    image_colors[y*image_width+x].b = color.b;
}



void Image::Export(const std::string& file_path) const {

    std::ofstream output_file;

    // opens a file to write something in it in binary format
    output_file.open(file_path, std::ios::out | std::ios::binary);

    // each row's bytes size should be divided on chunks with 4 bytes size each
    // 3 bytes is a size of one pixel
    unsigned char bmp_pad[3] = {0,0,0};
    const int padding_amount = (4  - (image_width * 3) % 4) % 4;

    // header file contains general file information about generated .bmp
    const int file_header_size = 14;

    // information header file contains detailed information about generated .bmp
    const int info_header_size = 40;

    // 3 * image_length * image_width is a size for all pixels (3 possible colors for each one)
    const int filesize = file_header_size + info_header_size + 3 * image_length * image_width + padding_amount * image_length;

    unsigned char bmp_file_header[file_header_size];

    // B and M indicates that we generate a .bmp file
    bmp_file_header[0] = 'B';
    bmp_file_header[1] = 'M';

    // file size
    bmp_file_header[2] = filesize;
    bmp_file_header[3] = filesize >> 8;
    bmp_file_header[4] = filesize >> 16;
    bmp_file_header[5] = filesize >> 24;

    bmp_file_header[6] = 0;
    bmp_file_header[7] = 0;

    bmp_file_header[8] = 0;
    bmp_file_header[9] = 0;

    // pixel data
    bmp_file_header[10] = file_header_size + info_header_size;
    bmp_file_header[11] = 0;
    bmp_file_header[12] = 0;
    bmp_file_header[13] = 0;

    unsigned char bmp_info_header[info_header_size];

    // header info size
    bmp_info_header[0] = info_header_size;
    bmp_info_header[1] = 0;
    bmp_info_header[2] = 0;
    bmp_info_header[3] = 0;

    // image width
    bmp_info_header[4] = image_width;
    bmp_info_header[5] = image_width >> 8;
    bmp_info_header[6] = image_width >> 16;
    bmp_info_header[7] = image_width >> 24;

    bmp_info_header[8] = image_length;
    bmp_info_header[9] = image_length >> 8;
    bmp_info_header[10] = image_length >> 16;
    bmp_info_header[11] = image_length >> 24;

    // planes
    bmp_info_header[12] = 1;
    bmp_info_header[13] = 0;

    // bits per pixel
    bmp_info_header[14] = 24;
    bmp_info_header[15] = 0;

    bmp_info_header[16] = 0;
    bmp_info_header[17] = 0;
    bmp_info_header[18] = 0;
    bmp_info_header[19] = 0;

    bmp_info_header[20] = 0;
    bmp_info_header[21] = 0;
    bmp_info_header[22] = 0;
    bmp_info_header[23] = 0;

    bmp_info_header[24] = 0;
    bmp_info_header[25] = 0;
    bmp_info_header[26] = 0;
    bmp_info_header[27] = 0;

    bmp_info_header[28] = 0;
    bmp_info_header[29] = 0;
    bmp_info_header[30] = 0;
    bmp_info_header[31] = 0;

    bmp_info_header[32] = 0;
    bmp_info_header[33] = 0;
    bmp_info_header[34] = 0;
    bmp_info_header[35] = 0;

    bmp_info_header[36] = 0;
    bmp_info_header[37] = 0;
    bmp_info_header[38] = 0;
    bmp_info_header[39] = 0;

    // we write arrays to a file
    output_file.write(reinterpret_cast<const char*>(bmp_file_header), file_header_size);
    output_file.write(reinterpret_cast<const char*>(bmp_info_header), info_header_size);

    for (int y = 0; y < image_length; y++) {
        for (int x = 0; x < image_width; x++) {

            // color channels
            unsigned char r = static_cast<unsigned char>(GetColor(x, y).r);
            unsigned char g = static_cast<unsigned char>(GetColor(x, y).g);
            unsigned char b = static_cast<unsigned char>(GetColor(x, y).b);

            // for .bmp we start with blue channel
            unsigned char color[] = {b, g, r};

            // we add a color (size of which is 3 bytes)
            output_file.write(reinterpret_cast<char*>(color), 3);
        }

        // we write into a file padding after each row
        output_file.write(reinterpret_cast<const char*>(bmp_pad), padding_amount);
    }

    output_file.close();
    delete [] image_colors;
    std::cout << "file created";
}
