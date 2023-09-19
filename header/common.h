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

vector<int> time2date(string time);

string date2time(vector<int> date);

// time1 is earlier than time2
bool earlier(string time1, vector<int> time2);

// time1 is later than times
bool later(string time1, vector<int> time2);

double distance(GPSdata* d1,GPSdata* d2);
#endif