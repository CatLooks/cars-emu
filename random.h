// -- random manager -- //

// RNG
int rint(int min, int max) {
    return rand() % (max - min) + min;
};
int rint(int max) {
    return rand() % max;
};
