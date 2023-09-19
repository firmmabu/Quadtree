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
                  << "4.轨迹修改\n"
                  << "5.轨迹删除(按坐标)\n"
                  << "6.轨迹删除(按时间)\n"
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
            std::cout << "[区域查询结果]查询结果：\n";
            if (result.empty()) {
                std::cout << "未查询到\n";
            } else {
                printVector(result);
            }
            delete rect;
        } else if (choice == 3) {
            double lon_max, lat_max, lon_min, lat_min;
            int idx = 7856;
            vector<int> time1(6);
            vector<int> time2(6);

            std::cout << "[邻接查询输入]请输入最大经度Longitude=";
            std::cin >> lon_max;
            std::cout << "[邻接查询输入]请输入最小经度Longitude=";
            std::cin >> lon_min;
            std::cout << "[邻接查询输入]请输入最大纬度Latitude=";
            std::cin >> lat_max;
            std::cout << "[邻接查询输入]请输入最小纬度Latitude=";
            std::cin >> lat_min;
            std::cout
                << "[邻接查询输入]请输入最早时间(格式:年 月 日 时 分 秒):";
            std::cin >> time1[0] >> time1[1] >> time1[2] >> time1[3] >>
                time1[4] >> time1[5];
            std::cout
                << "[邻接查询输入]请输入最晚时间(格式:年 月 日 时 分 秒):";
            std::cin >> time2[0] >> time2[1] >> time2[2] >> time2[3] >>
                time2[4] >> time2[5];
            std::cout << "[邻接查询输入]请输入要进行邻接查询的出租车ID:";
            std::cin >> idx;
            Rectangle* rect = new Rectangle(lon_max, lat_max, lon_min, lat_min);
            auto result = root->AdjacentSearch(rect, time1, time2, idx);
            std::cout << "[邻接查询结果]查询结果：\n";
            if (result == nullptr) {
                std::cout << "未查询到\n";
            } else {
                result->print();
            }
            delete rect;
        } else if (choice == 4) {
            int id;
            double lon1, lat1, lon2, lat2;
            std::cout << "[轨迹修改输入]请输入出租车ID:";
            std::cin >> id;
            std::cout << "[轨迹修改输入]请输入经度Longitude=";
            std::cin >> lon1;
            std::cout << "[轨迹修改输入]请输入纬度Latitude=";
            std::cin >> lat1;
            std::cout << "[轨迹修改输入]请输入修改后的经度Longitude=";
            std::cin >> lon2;
            std::cout << "[轨迹修改输入]请输入修改后的纬度Longitude=";
            std::cin >> lat2;
            bool b =
                root->PointChange(id, std::make_pair(lon1, lat1), lon2, lat2);
            if (b) {
                std::cout << "修改成功\n";
            } else {
                std::cout << "修改失败\n";
            }

        } else if (choice == 5) {
            int id;
            double lon, lat;
            std::cout << "[轨迹删除输入]请输入出租车ID:";
            std::cin >> id;
            std::cout << "[轨迹删除输入]请输入经度Longitude=";
            std::cin >> lon;
            std::cout << "[轨迹删除输入]请输入纬度Latitude=";
            std::cin >> lat;
            bool b = root->PointDelete(id, lon, lat);
            if (b) {
                std::cout << "删除成功\n";
            } else {
                std::cout << "删除失败\n";
            }
        } else if (choice == 6) {
            int id;
            vector<int> time(6);
            std::cout << "[轨迹删除输入]请输入出租车ID:";
            std::cin >> id;
            std::cout << "[轨迹删除输入]请输入经度Longitude=";
            std::cin >> time[0] >> time[1] >> time[2] >> time[3] >> time[4] >>
                time[5];
            bool b = root->PointDelete(id, date2time(time));
            if (b) {
                std::cout << "删除成功\n";
            } else {
                std::cout << "删除失败\n";
            }
        }

        else {
            std::cout << "输入错误！\n";
        }
    }
    return 0;
}