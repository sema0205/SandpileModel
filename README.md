# Sandpile Model

Implementation of a simplified [Abelian Sandpile Model](https://en.wikipedia.org/wiki/Abelian_sandpile_model), which can save its states in a [BMP format](https://en.wikipedia.org/wiki/BMP_file_format) image.

The initial state and the size of the grid is specified.

Console application, which supports the following command-line arguments:

**-l, --length**   - the length of the grid (guaranteed to fit in uint16_t)

**-w, --width**    - the width of the grid (guaranteed to fit in uint16_t)

**-i, --input**    - tsv-file (tab-separated values) with initial data

**-o, --output**   - the path to the directory for saving images

**-m, --max-iter** - the maximum number of model iterations

**-f, --freq**     - the frequency at which images should be saved (if 0, only the final state will be saved)

## Initial state

The initial state is determined by the grid size, passed as program arguments and a file with the initial number of sand grains in each cell, except for empty ones.

File format:
Each line contains information about one cell, in the form (x-coordinates, y-coordinates, number of sand grains), separated by a tab character. The number of sand grains guaranteed to fit in uint64_t.

## Notes on the model

1. New grains are added only at initialization.

2. The state of the next generation of the cell depends only on the previous state of the grid

## Program result

The program recalculates the model state according to the initial data, as well as saves intermediate states with the specified frequency (if 0, only the final state is saved).
