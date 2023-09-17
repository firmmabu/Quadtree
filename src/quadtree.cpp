#include "f:/quadtree/header/quadtree.h"
#include <cmath>
#include <iostream>
#include "f:/quadtree/header/common.h"

GPSdata::GPSdata() {}

GPSdata::GPSdata(int _id, string _time, double _lon, double _lat)
    : id(_id), time(_time), longitude(_lon), latitude(_lat) {}

void GPSdata::print() {
    std::cout << "id:" << this->id << '\n';
    std::cout << "time:" << this->time << '\n';
    std::cout << "coordinate:" << this->longitude << ',' << this->latitude
              << '\n';
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
    return x >= bottom_left.first && x <= top_right.first &&
           y <= top_right.second && y >= bottom_left.second;
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
    double x = (rect->bottom_left.first + rect->top_right.first) / 2;
    double y = (rect->bottom_left.second + rect->top_right.second) / 2;

    point2d p = std::make_pair(x, y);
    vector<GPSdata*> leaf;
    this->findPointLeaf(p, leaf);
    vector<GPSdata*> result;
    for(auto dat:leaf){
        if(rect->Inside(dat->longitude,dat->latitude)){
            result.emplace_back(dat);
        }
    }
    return result;
}
vector<GPSdata*> QuadNode::AdjacentSearch(Rectangle* rect,
                                          string time1,
                                          string time2) {
    return vector<GPSdata*>();
}