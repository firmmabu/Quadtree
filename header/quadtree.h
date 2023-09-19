#ifndef QUADTREE_H
#define QUADTREE_H

#include <string>
#include <vector>

using std::string;
using std::vector;

typedef std::pair<double, double> point2d;

enum location { LEFTTOP, LEFTBOT, RIGHTTOP, RIGHTBOT };

class GPSdata {
   public:
    int id;
    string time;
    double longitude;
    double latitude;
    GPSdata();
    GPSdata(int _id, string _time, double _lon, double _lat);

    void print();
};

class Rectangle {
   public:
    point2d top_right;
    point2d bottom_left;

    Rectangle();
    Rectangle(point2d _top_right, point2d _bottom_left);
    Rectangle(double top_right_x,
              double top_right_y,
              double bottom_left_x,
              double bottom_left_y);
    Rectangle(point2d _top_right, double bottom_left_x, double bottom_left_y);
    Rectangle(double top_right_x, double top_right_y, point2d _bottom_left);

    bool Inside(point2d p);
    bool Inside(double x, double y);
};

class QuadNode {
   private:
    void findPointLeaf(point2d p, vector<GPSdata*>& leaf);

   public:
    vector<GPSdata*> data;

    Rectangle* range;
    // 左上 左下 右上 右下
    vector<QuadNode*> child = vector<QuadNode*>(4, nullptr);
    QuadNode* father = nullptr;

    QuadNode();
    QuadNode(Rectangle* bounding);
    void InsertNode(GPSdata* data, int depth);
    QuadNode* Search(point2d p);
    bool isLeaf();

    vector<GPSdata*> PointSearch(point2d p);
    vector<GPSdata*> AreaSearch(Rectangle* rect);
    GPSdata* AdjacentSearch(Rectangle* rect,
                            vector<int> time1,
                            vector<int> time2,
                            int idx);
    bool PointChange(int id, point2d p, double lon, double lat);

    bool PointDelete(int id, double lon, double lat);
    bool PointDelete(int id, string time);
};

#endif