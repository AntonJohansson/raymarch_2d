#pragma once

#include <stdint.h>

void perlin_seed(uint32_t seed);
double perlin_noise(double x, double y, double z);
