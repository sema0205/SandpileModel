#pragma once

#include <iostream>

struct Barrier {

    // (width_second_barrier - width_second_barrier) is a width size in coordinates, same for length
    int width_first_barrier;
    int width_second_barrier;

    int length_first_barrier;
    int length_second_barrier;

};

struct Sandpile {

    uint16_t length;
    uint16_t width;
    std::string tsv_file_path;
    std::string output_dir_path;
    int max_iter;
    int freq;

    uint64_t** current_massive;
    uint64_t** next_massive;

    // set massive contains coordinates of a point which has >= 4 pieces of sand in a way [i] = x, [i+1] = y
    int* set_massive;

    Barrier* barrier_massive;
    int barrier_massive_size;

    void SetVariables(int arguments_amount, char* arguments_massive[]);

    // initialization and delete functions added because I use dynamic massives
    void MassiveCurrentInitialize();
    void MassiveNextInitialize();
    void MassiveSetInitialize();

    void DeleteCurrentMassive() const;
    void DeleteNextMassive() const;

    void MakeSandpile();

    // convertSandpile is a function that creates an .bmp file for specific iteration of our sandpile
    void ConvertSandpile(const std::string& file_name) const;

    // SandWork implement abelian sandpile model
    void SandWork(int index_more_4);

};
