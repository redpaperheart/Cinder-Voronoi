#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace std;

class VoronoiCell {
  public:
    std::vector<vec2> pts;
    vec2 pt;
};

class Voronoi {
private:
    Rectf bounds;
    std::vector<vec2> points;
    std::vector<VoronoiCell> cells;
    
public:
    Voronoi() {};
    ~Voronoi() {};
    
    void clear();
    void generate(bool ordered = true);
    void draw();
    
    bool isBorder(vec2 _pt);
    
    void setBounds(Rectf _bounds);
    void setPoints(std::vector<vec2> _points);
    void addPoint(vec2 _point);
    void addPoints(std::vector<vec2> _points);
    
    Rectf getBounds();
    std::vector<vec2>& getPoints();
    std::vector<VoronoiCell>& getCells();
    VoronoiCell& getCell(vec2 _point, bool approximate = false);
    
    //borg
    void relax();
};