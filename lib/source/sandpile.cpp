#include "../headers/sandpile.h"
#include "../headers/image.h"

#include <cstring>

void Sandpile::SetVariables(int arguments_amount, char **arguments_massive) {

    for (int i = 1; i < arguments_amount; i++) {
        if (strcmp(arguments_massive[i], "-l") == 0 || strcmp(arguments_massive[i], "--length") == 0) {
            length = atoi(arguments_massive[i + 1]);
        } else if (strcmp(arguments_massive[i], "-w") == 0 || strcmp(arguments_massive[i], "--width") == 0) {
            width = atoi(arguments_massive[i + 1]);
        } else if (strcmp(arguments_massive[i], "-i") == 0 || strcmp(arguments_massive[i], "--input") == 0) {
            tsv_file_path = arguments_massive[i + 1];
        } else if (strcmp(arguments_massive[i], "-o") == 0 || strcmp(arguments_massive[i], "--output") == 0) {
            output_dir_path = arguments_massive[i + 1];
        } else if (strcmp(arguments_massive[i], "-m") == 0 || strcmp(arguments_massive[i], "--max-iter") == 0) {
            max_iter = atoi(arguments_massive[i + 1]);
        } else if (strcmp(arguments_massive[i], "-f") == 0 || strcmp(arguments_massive[i], "--freq") == 0) {
            freq = atoi(arguments_massive[i + 1]);
        }
    }

    MassiveCurrentInitialize();
}

void Sandpile::MassiveCurrentInitialize() {

    current_massive = new uint64_t* [width];

    for (int i = 0; i < width; i++) {
        current_massive[i] = new uint64_t [length];
    }

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < length; y++) {
            current_massive[x][y] = 0;
        }
    }

    std::ifstream reading_tsv_file(tsv_file_path);
    uint64_t coordinate_x, coordinate_y, sand_amount;

    while (reading_tsv_file >> coordinate_x >> coordinate_y >> sand_amount) {
        current_massive[coordinate_x][coordinate_y] += sand_amount;
    }

}

void Sandpile::MassiveSetInitialize() {

    set_massive = new int[width*length];

    for (int i = 0; i < width*length; i++) {
        set_massive[i] = 0;
    }

}

void Sandpile::MassiveNextInitialize() {

    next_massive = new uint64_t* [width];

    for (int i = 0; i < width; i++) {
        next_massive[i] = new uint64_t [length];
    }

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < length; y++) {
            next_massive[x][y] = 0;
        }
    }

}

void Sandpile::DeleteCurrentMassive() const {

    for (int i = 0; i < width; i++) {
        delete [] current_massive[i];
    }
    delete [] current_massive;

}

void Sandpile::DeleteNextMassive() const {

    for (int i = 0; i < width; i++) {
        delete [] next_massive[i];
    }
    delete [] next_massive;

}

void Sandpile::MakeSandpile() {

    int counter = 1;
    bool stable_check = true;

    while (counter != max_iter) {

        stable_check = false;

        std::cout << counter << "\n";

        MassiveNextInitialize();
        MassiveSetInitialize();

        int index_more_4 = -1;

        for (int x = 0; x < width; x++) {
            for (int y = 0; y < length; y++) {
                if (current_massive[x][y] < 4) {
                    next_massive[x][y] = current_massive[x][y];
                } else {

                    stable_check = true;
                    index_more_4++;
                    set_massive[index_more_4] = x;
                    index_more_4++;
                    set_massive[index_more_4] = y;

                }
            }
        }

        if (stable_check && index_more_4 > 0) {
            SandWork(index_more_4);
        }

        for (int x = 0; x < width; x++) {
            for (int y = 0; y < length; y++) {
                current_massive[x][y] = next_massive[x][y];
            }
        }

        DeleteNextMassive();
        delete [] set_massive;

        if (counter % freq == 0 && freq != 0) {
            ConvertSandpile("/" + std::to_string(counter) + ".bmp");
        }

        counter++;
    }
    ConvertSandpile("/" + std::to_string(counter) + ".bmp");
}

void Sandpile::ConvertSandpile(const std::string& file_name) const {

    Image image(width, length);

    for (int y = 0; y < length; y++) {
        for (int x = 0; x < width; x++) {

            if (current_massive[x][y] == 0) {
                image.SetColor(Color(255, 255, 255), x, y);
            } else if (current_massive[x][y] == 1) {
                image.SetColor(Color(0, 128, 0), x, y);
            } else if (current_massive[x][y] == 2) {
                image.SetColor(Color(139, 0, 255), x, y);
            } else if (current_massive[x][y] == 3) {
                image.SetColor(Color(255, 255, 0), x, y);
            } else if (current_massive[x][y] > 3) {
                image.SetColor(Color(0, 0, 0), x, y);
            }

            // we draw sides of barriers
            if (barrier_massive_size > 0) {
                for (int k = 0; k < barrier_massive_size; k++) {

                    int w1 = barrier_massive[k].width_first_barrier;
                    int w2 = barrier_massive[k].width_second_barrier;

                    int l1 = barrier_massive[k].length_first_barrier;
                    int l2 = barrier_massive[k].length_second_barrier;

                    if ((x == w1 && (y >= l1 && y <= l2)) || (x == w2 && (y >= l1 && y <= l2)) ||
                        (y == l1 && (x >= w1 && x <= w2)) || (y == l2 && (x >= w1 && x <= w2))) {
                        image.SetColor(Color(255, 0, 0), x, y);
                    }
                }
            }

        }
    }

    std::string file_path = output_dir_path + file_name;
    image.Export(file_path);
}

void Sandpile::SandWork(int index_more_4) {

    for (int i = 0; i < index_more_4 + 1; i += 2) {

        int index_first = set_massive[i];
        int index_second = set_massive[i + 1];

        next_massive[index_first][index_second] += current_massive[index_first][index_second] - 4;

        if (barrier_massive_size > 0) {

            // for barriers, we check if the point is in there, if no we handle it as a default one
            // we have 4 loops for each point that possibly can get sand

            // prevents adding more sand than its needed
            bool sand_added = false;

            for (int k = 0; k < barrier_massive_size; k++) {

                int w1 = barrier_massive[k].width_first_barrier;
                int w2 = barrier_massive[k].width_second_barrier;

                int l1 = barrier_massive[k].length_first_barrier;
                int l2 = barrier_massive[k].length_second_barrier;

                if ((index_first + 1 >= w1 && index_first + 1 <= w2) && (index_second >= l1 && index_second <= l2)) {
                    next_massive[index_first][index_second]++;
                    sand_added = true;
                    break;
                }
            }

            if (!sand_added) {
                if (index_first + 1 < width) {
                    next_massive[index_first + 1][index_second]++;
                }
            }


            sand_added = false;

            for (int k = 0; k < barrier_massive_size; k++) {

                int w1 = barrier_massive[k].width_first_barrier;
                int w2 = barrier_massive[k].width_second_barrier;

                int l1 = barrier_massive[k].length_first_barrier;
                int l2 = barrier_massive[k].length_second_barrier;

                if ((index_second + 1 >= l1 && index_second + 1 <= l2) && (index_first >= w1 && index_first <= w2)) {
                    next_massive[index_first][index_second]++;
                    sand_added = true;
                    break;
                }
            }

            if (!sand_added) {
                if (index_second + 1 < length) {
                    next_massive[index_first][index_second + 1]++;
                }
            }

            sand_added = false;

            for (int k = 0; k < barrier_massive_size; k++) {

                int w1 = barrier_massive[k].width_first_barrier;
                int w2 = barrier_massive[k].width_second_barrier;

                int l1 = barrier_massive[k].length_first_barrier;
                int l2 = barrier_massive[k].length_second_barrier;

                if ((index_first - 1 >= w1 && index_first - 1 <= w2) && (index_second >= l1 && index_second <= l2)) {
                    next_massive[index_first][index_second]++;
                    sand_added = true;
                    break;
                }
            }

            if (!sand_added) {
                if (index_first >= 1) {
                    next_massive[index_first - 1][index_second]++;
                }
            }

            sand_added = false;

            for (int k = 0; k < barrier_massive_size; k++) {

                int w1 = barrier_massive[k].width_first_barrier;
                int w2 = barrier_massive[k].width_second_barrier;

                int l1 = barrier_massive[k].length_first_barrier;
                int l2 = barrier_massive[k].length_second_barrier;

                if ((index_second - 1 >= l1 && index_second - 1 <= l2) && (index_first >= w1 && index_first <= w2)) {
                    next_massive[index_first][index_second]++;
                    sand_added = true;
                    break;
                }
            }

            if (!sand_added) {
                if (index_second - 1 >= 0) {
                    next_massive[index_first][index_second - 1]++;
                }
            }


        } else {

            if (index_first + 1 < width) {
                next_massive[index_first + 1][index_second]++;
            }

            if (index_second + 1 < length) {
                next_massive[index_first][index_second + 1]++;
            }

            if (index_first - 1 >= 0) {
                next_massive[index_first - 1][index_second]++;
            }

            if (index_second - 1 >= 0) {
                next_massive[index_first][index_second - 1]++;
            }

        }

    }
}
