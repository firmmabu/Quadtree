#include "f:/quadtree/header/common.h"
using std::to_string;

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

vector<int> time2date(string time) {
    string temp = "";
    vector<int> date;
    for (char ch : time) {
        if (ch == '-' || ch == ':' || ch == ' ') {
            date.emplace_back(std::stoi(temp));
            temp = "";
        } else {
            temp += ch;
        }
    }
    date.emplace_back(std::stoi(temp));
    return date;
}

string date2time(vector<int> date) {
    return to_string(date[0]) + "-" + to_string(date[1]) + "-" +
           to_string(date[2]) + " " + to_string(date[3]) + ":" +
           to_string(date[4]) + ":" + to_string(date[5]);
}

bool earlier(string time1, vector<int> time2) {
    auto time1_int = time2date(time1);
    for (int i = 0; i < time1_int.size(); ++i) {
        if (time1_int[i] < time2[i]) {
            return true;
        }

        if (time1_int[i] > time2[i]) {
            return false;
        }
    }

    return true;
}

bool later(string time1, vector<int> time2) {
    return !earlier(time1, time2);
}

double distance(GPSdata* d1, GPSdata* d2) {
    return (d1->longitude * 1000 - d2->longitude * 1000) *
               (d1->longitude * 1000 - d2->longitude * 1000) +
           (d1->latitude * 1000 - d2->latitude * 1000) *
               (d1->latitude * 1000 - d2->latitude * 1000);
}