#include "../header/quadtree.h"
#include <float.h>
#include <cmath>
#include <iomanip>
#include <iostream>
#include "../header/common.h"

GPSdata::GPSdata() {}

GPSdata::GPSdata(int _id, string _time, double _lon, double _lat)
    : id(_id), time(_time), longitude(_lon), latitude(_lat) {}

void GPSdata::print() {
    std::cout << "id:" << this->id << '\n';
    std::cout << "time:" << this->time << '\n';
    std::cout << "coordinate:" << std::fixed << std::setprecision(5)
              << this->longitude << ',' << std::fixed << std::setprecision(5)
              << this->latitude << '\n';
}

Rectangle::Rectangle() {}

Rectangle::Rectangle(point2d _top_right, point2d _bottom_left)
    : top_right(_top_right), bottom_left(_bottom_left) {}

Rectangle::Rectangle(double top_right_x,
                     double top_right_y,
                     double bottom_left_x,
                     double bottom_left_y) {
    top_right = std::make_pair(top_right_x, top_right_y);
    bottom_left = std::make_pair(bottom_left_x, bottom_left_y);
}

Rectangle::Rectangle(point2d _top_right,
                     double bottom_left_x,
                     double bottom_left_y)
    : top_right(_top_right) {
    bottom_left = std::make_pair(bottom_left_x, bottom_left_y);
}
Rectangle::Rectangle(double top_right_x,
                     double top_right_y,
                     point2d _bottom_left)
    : bottom_left(_bottom_left) {
    top_right = std::make_pair(top_right_x, top_right_y);
}

bool Rectangle::Inside(point2d p) {
    auto x = p.first;
    auto y = p.second;
    return x >= bottom_left.first && x <= top_right.first &&
           y <= top_right.second && y >= bottom_left.second;
}

bool Rectangle::Inside(double x, double y) {
    return (x >= bottom_left.first || equal(x, bottom_left.first)) &&
           (x <= top_right.first || equal(x, top_right.first)) &&
           (y <= top_right.second || equal(y, top_right.second)) &&
           (y >= bottom_left.second || equal(y, bottom_left.second));
}

QuadNode::QuadNode() {}

QuadNode::QuadNode(Rectangle* bounding) : range(bounding) {}

void QuadNode::InsertNode(GPSdata* gpsdata, int depth) {
    // 空节点不用插入
    if (gpsdata == nullptr) {
        return;
    }

    double node_x = gpsdata->longitude;
    double node_y = gpsdata->latitude;

    // 要插入的节点不在范围内
    if (!this->range->Inside(node_x, node_y)) {
        return;
    }

    double root_x1 = this->range->top_right.first;
    double root_y1 = this->range->top_right.second;
    double root_x2 = this->range->bottom_left.first;
    double root_y2 = this->range->bottom_left.second;
    double x_center = (root_x1 + root_x2) / 2;
    double y_center = (root_y1 + root_y2) / 2;

    if (depth == 0) {
        this->data.push_back(gpsdata);
        return;
    }

    // 在根节点范围的左半部分
    if (node_x <= x_center) {
        // 在左上角
        if (node_y >= y_center) {
            if (this->child[LEFTTOP] == nullptr) {
                Rectangle* rect =
                    new Rectangle(x_center, root_y1, root_x2, y_center);
                this->child[LEFTTOP] = new QuadNode(rect);
                this->child[LEFTTOP]->father = this;
            }
            this->child[LEFTTOP]->InsertNode(gpsdata, depth - 1);
        }
        // 在左下角
        else {
            if (this->child[LEFTBOT] == nullptr) {
                Rectangle* rect =
                    new Rectangle(x_center, y_center, root_x2, root_y2);
                this->child[LEFTBOT] = new QuadNode(rect);
                this->child[LEFTBOT]->father = this;
            }
            this->child[LEFTBOT]->InsertNode(gpsdata, depth - 1);
        }
    }
    // 在根节点范围的右半部分
    else {
        // 在右上角
        if (node_y >= y_center) {
            if (this->child[RIGHTTOP] == nullptr) {
                Rectangle* rect =
                    new Rectangle(root_x1, root_y1, x_center, y_center);
                this->child[RIGHTTOP] = new QuadNode(rect);
                this->child[RIGHTTOP]->father = this;
            }
            this->child[RIGHTTOP]->InsertNode(gpsdata, depth - 1);
        }
        // 在右下角
        else {
            if (this->child[RIGHTBOT] == nullptr) {
                Rectangle* rect =
                    new Rectangle(root_x1, y_center, x_center, root_y2);
                this->child[RIGHTBOT] = new QuadNode(rect);
                this->child[RIGHTBOT]->father = this;
            }
            this->child[RIGHTBOT]->InsertNode(gpsdata, depth - 1);
        }
    }
}

QuadNode* QuadNode::Search(point2d p) {
    return nullptr;
}

bool QuadNode::isLeaf() {
    return child[0] == nullptr && child[1] == nullptr && child[2] == nullptr &&
           child[3] == nullptr;
}

void QuadNode::findPointLeaf(point2d p, vector<GPSdata*>& leaf) {
    if (this->isLeaf()) {
        leaf = this->data;
    } else {
        if (this->child[LEFTTOP] != nullptr &&
            this->child[LEFTTOP]->range->Inside(p)) {
            this->child[LEFTTOP]->findPointLeaf(p, leaf);
        } else if (this->child[LEFTBOT] != nullptr &&
                   this->child[LEFTBOT]->range->Inside(p)) {
            this->child[LEFTBOT]->findPointLeaf(p, leaf);
        } else if (this->child[RIGHTTOP] != nullptr &&
                   this->child[RIGHTTOP]->range->Inside(p)) {
            this->child[RIGHTTOP]->findPointLeaf(p, leaf);
        } else if (this->child[RIGHTBOT] != nullptr &&
                   this->child[RIGHTBOT]->range->Inside(p)) {
            this->child[RIGHTBOT]->findPointLeaf(p, leaf);
        }
    }
}

vector<GPSdata*> QuadNode::PointSearch(point2d p) {
    vector<GPSdata*> leaf;
    this->findPointLeaf(p, leaf);
    vector<GPSdata*> result;
    // std::cout << leaf.size();
    // printVector(leaf, "F:/quadtree/1.txt");
    for (auto dat : leaf) {
        if (equal(dat->longitude, p.first) && equal(dat->latitude, p.second)) {
            result.emplace_back(dat);
        }
    }

    return result;
}

vector<GPSdata*> QuadNode::AreaSearch(Rectangle* rect) {
    point2d p1 = rect->top_right;
    point2d p2 = rect->bottom_left;
    point2d p3 =
        std::make_pair(rect->top_right.first, rect->bottom_left.second);
    point2d p4 =
        std::make_pair(rect->bottom_left.first, rect->top_right.second);

    vector<vector<GPSdata*>> leafs;
    vector<GPSdata*> leaf1, leaf2, leaf3, leaf4;
    this->findPointLeaf(p1, leaf1);
    this->findPointLeaf(p2, leaf2);
    this->findPointLeaf(p3, leaf3);
    this->findPointLeaf(p4, leaf4);
    leafs.emplace_back(leaf1);
    leafs.emplace_back(leaf2);
    leafs.emplace_back(leaf3);
    leafs.emplace_back(leaf4);

    vector<GPSdata*> temp = {leafs[0].empty() ? nullptr : leafs[0][0],
                             leafs[1].empty() ? nullptr : leafs[1][0],
                             leafs[2].empty() ? nullptr : leafs[2][0],
                             leafs[3].empty() ? nullptr : leafs[3][0]};
    vector<int> same(4, 0);
    int k = 1;
    for (int i = 0; i < 4; ++i) {
        if (same[i] == 0) {
            same[i] = k;
            ++k;
        }

        for (int j = 0; j < 4; ++j) {
            // 这块是空的就设置为5，因为如果不是空的最大也就是4
            if (temp[i] == nullptr) {
                same[i] = 5;
            } else {
                if (same[j] == 0 && temp[i]->time == temp[j]->time) {
                    same[j] = same[i];
                }
            }
        }
    }

    k = 1;
    vector<GPSdata*> result;
    for (int i = 0; i < 4; ++i) {
        if (same[i] == k) {
            ++k;
            for (auto dat : leafs[i]) {
                if (rect->Inside(dat->longitude, dat->latitude)) {
                    result.emplace_back(dat);
                }
            }
        }
    }

    return result;
}

GPSdata* QuadNode::AdjacentSearch(Rectangle* rect,
                                  vector<int> time1,
                                  vector<int> time2,
                                  int idx) {
    vector<GPSdata*> area = AreaSearch(rect);
    vector<GPSdata*> taxi_this;
    vector<GPSdata*> taxi_other;
    GPSdata* result;
    double min_dis = DBL_MAX;
    for (auto gps : area) {
        if (later(gps->time, time1) && earlier(gps->time, time2)) {
            if (gps->id == idx) {
                taxi_this.emplace_back(gps);
            } else {
                taxi_other.emplace_back(gps);
            }
        }
    }

    for (auto other_taxi : taxi_other) {
        for (auto this_taxi : taxi_this) {
            double d = distance(other_taxi, this_taxi);

            if (d < min_dis) {
                min_dis = d;
                result = other_taxi;
            }
        }
    }
    return result;
}

bool QuadNode::PointChange(int id, point2d p, double lon, double lat) {}

bool QuadNode::PointDelete(int id, double lon, double lat) {}

bool QuadNode::PointDelete(int id, string time) {}