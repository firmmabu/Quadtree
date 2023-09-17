#include <algorithm>
#include <fstream>
#include <iostream>
#include "f:/quadtree/header/common.h"

int main() {
    int DATANUM = 10000;

    QuadNode* root = new QuadNode();
    Rectangle* bounding_box = new Rectangle();
    double min_lon = 181, max_lon = 0, min_lat = 181, max_lat = 0;
    // 找到Bounding Box
    std::cout << "正在构建包围盒\n";
    for (int i = 1; i <= DATANUM; ++i) {
        std::string buffer;
        std::string path = "F:/quadtree/data/" + std::to_string(i) + ".txt";
        std::ifstream file(path);
        if (!file.is_open()) {
            // std::cerr << "cant open file\t" + path << '\n';
            continue;
        }
        while (std::getline(file, buffer)) {
            auto gps = str2data(buffer);
            min_lon = std::min(min_lon, gps->longitude);
            min_lat = std::min(min_lat, gps->latitude);
            max_lon = std::max(max_lon, gps->longitude);
            max_lat = std::max(max_lat, gps->latitude);
            delete gps;
        }
        file.close();
    }

    bounding_box->bottom_left = std::make_pair(min_lon, min_lat);
    bounding_box->top_right = std::make_pair(max_lon, max_lat);
    root->range = bounding_box;

    std::cout << "正在构建四叉树\n";
    for (int i = 1; i <= DATANUM; ++i) {
        std::string buffer;
        std::string path = "F:/quadtree/data/" + std::to_string(i) + ".txt";
        std::ifstream file(path);
        if (!file.is_open()) {
            // std::cerr << "cant open file\t" + path << '\n';
            continue;
        }
        while (std::getline(file, buffer)) {
            auto gps = str2data(buffer);
            root->InsertNode(gps, 8);
        }
        file.close();
    }

    while (true) {
        std::cout << "-------------------------------------------\n"
                  << "请输入要执行的操作:\n"
                  << "1:点查询\n"
                  << "2:区域查询(矩形区域)\n"
                  << "3:邻接查询\n"
                  << "4.轨迹查询\n"
                  << "5.轨迹修改\n"
                  << "6.轨迹删除\n"
                  << "0:退出\n"
                  << "-------------------------------------------\n";
        int choice;
        std::cout << "请输入想要执行的操作:";
        std::cin >> choice;
        if (choice == 0) {
            break;
        } else if (choice == 1) {
            double lon, lat;
            std::cout << "[点查询输入]请输入查询点的经度Longitude=";
            std::cin >> lon;
            std::cout << "[点查询输入]请输入查询点的纬度Latitude=";
            std::cin >> lat;
            std::cout << "正在查询\n";
            std::cout << "[点查询结果]查询结果：\n";
            auto result = root->PointSearch(std::make_pair(lon, lat));
            if (result.empty()) {
                std::cout << "未查询到\n";
            } else {
                printVector(result);
            }
        } else if (choice == 2) {
            double lon_max, lat_max, lon_min, lat_min;
            std::cout << "[区域查询输入]请输入最大经度Longitude=";
            std::cin >> lon_max;
            std::cout << "[区域查询输入]请输入最小经度Longitude=";
            std::cin >> lon_min;
            std::cout << "[区域查询输入]请输入最大纬度Latitude=";
            std::cin >> lat_max;
            std::cout << "[区域查询输入]请输入最小纬度Latitude=";
            std::cin >> lat_min;

            Rectangle* rect = new Rectangle(lon_max, lat_max, lon_min, lat_min);
            auto result = root->AreaSearch(rect);
            if (result.empty()) {
                std::cout << "未查询到\n";
            } else {
                printVector(result);
            }
            delete rect;
        } else if (choice == 3) {
        } else if (choice == 4) {
        } else if (choice == 5) {
        } else if (choice == 6) {
        }

        else {
            std::cout << "输入错误！\n";
        }
    }
    return 0;
}