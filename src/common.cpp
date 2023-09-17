#include "f:/quadtree/header/common.h"

GPSdata* str2data(std::string s) {
    std::vector<std::string> data(4, "");
    int index = 0;
    for (char ch : s) {
        if (ch != ',') {
            data[index] += ch;
        } else {
            ++index;
        }
    }
    GPSdata* gps = new GPSdata();
    gps->id = stoi(data[0]);
    gps->time = data[1];
    gps->longitude = stof(data[2]);
    gps->latitude = stof(data[3]);

    return gps;
}

void printVector(vector<GPSdata*> v) {
    for (auto data : v) {
        data->print();
    }
}

void printVector(vector<GPSdata*> v, string path) {
    std::ofstream fout(path, std::ios::out);
    for (auto data : v) {
        fout << "id:" << data->id << '\n';
        fout << "time:" << data->time << '\n';
        fout << "coordinate:" << std::fixed << std::setprecision(5)
             << data->longitude << ',' << std::fixed << std::setprecision(5)
             << data->latitude << '\n';
    }
}

bool equal(double a, double b) {
    const double epsilon = 0.00001;
    if (fabs(a - b) <= epsilon)
        return true;

    return false;
}