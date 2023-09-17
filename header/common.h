#ifndef COMMON_H
#define COMMON_H

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "f:/quadtree/header/quadtree.h"

GPSdata* str2data(std::string s);

void printVector(vector<GPSdata*> v);

bool equal(double a, double b);

void printVector(vector<GPSdata*> v, string path);
#endif