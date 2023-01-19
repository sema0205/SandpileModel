#pragma once

#include <cinttypes>
#include <iostream>
#include <vector>
#include <fstream>

struct Color {
    float r, g, b;
    Color();
    Color(float e, float f, float j);
};

struct Image {

    int image_width;
    int image_length;
    Color* image_colors;

    Image(int width, int length);
    Color GetColor(int x, int y) const;
    void SetColor(const Color &color, int x, int y) const;

    // creates a file and sets colors
    void Export(const std::string& file_path) const;
};
