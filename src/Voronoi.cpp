#include "Voronoi.h"

// Voro++2D
#include "config.h"
#include "common.h"
#include "cell_2d.h"
#include "v_base_2d.h"
#include "rad_option.h"
#include "container_2d.h"
#include "v_compute_2d.h"
#include "c_loops_2d.h"
#include "wall_2d.h"
#include "cell_nc_2d.h"
#include "ctr_boundary_2d.h"

void Voronoi::clear()
{
    cells.clear();
    points.clear();
}

//--------------------------------------------------------------
void Voronoi::generate(bool ordered)
{
    voro::container_2d* con = new voro::container_2d(bounds.x1, bounds.x1 + bounds.getWidth(), bounds.y1, bounds.y1 + bounds.getHeight(), 10, 10, false, false, 16);
    voro::c_loop_all_2d* vl = new voro::c_loop_all_2d(*con);
    voro::voronoicell_2d conCell;
        
    for(int i=0; i<points.size(); i++) {
        con->put(i, points[i].x, points[i].y);
    }
    
    if(vl->start()) {
        do {
            con->compute_cell(conCell, *vl);
            int k = 0;
            
            if(conCell.p) {
                VoronoiCell newCell = VoronoiCell();
                
                // Get the current point of the cell
                double* currentPoint = con->p[vl->ij]+con->ps*vl->q;
                newCell.pt = vec2(currentPoint[0], currentPoint[1]);
                
                // Get the edgepoints of the cell
                do {
                    float x = currentPoint[0] + 0.5 * conCell.pts[2*k];
                    float y = currentPoint[1] + 0.5 * conCell.pts[2*k+1];
                    
                    vec2 pt(x, y);
                    newCell.pts.push_back(pt);
                    
                    k = conCell.ed[2*k];
                } while(k!=0);
                
                cells.push_back(newCell);
            }
        } while(vl->inc());
    }
    
    // free up the memory
    delete con, vl;
    
    if(ordered) {
        vector<VoronoiCell> orderedCells;
        for(auto& pt : points) {
//            ofLog() << pt;
            orderedCells.push_back(getCell(pt));
        }
        cells = orderedCells;
    }
}

//--------------------------------------------------------------
void Voronoi::draw()
{
    // TODO
//    ofSetLineWidth(0);
//    ofNoFill();
//    
//    // Draw bounds
//    ofSetColor(220);
//    ofDrawRectangle(bounds);
//    
//    ofSetColor(180, 0, 0);
//    
//    for(int i=0; i<cells.size(); i++) {
//        // Draw cell borders
//        ofSetColor(120);
//        for(int j=0; j<cells[i].pts.size(); j++) {
//            ofPoint lastPt = cells[i].pts[cells[i].pts.size()-1];
//            if(j > 0) {
//                lastPt = cells[i].pts[j-1];
//            }
//            ofPoint thisPt = cells[i].pts[j];
//            
//            if(!isBorder(lastPt) || !isBorder(thisPt)) {
//                ofDrawLine(lastPt, thisPt);
//            }
//        }
//        
//        ofFill();
//        // Draw cell points
//        ofSetColor(180, 0, 0);
//        ofFill();
//        ofDrawCircle(cells[i].pt, 2);
//    }
}

bool Voronoi::isBorder(vec2 _pt)
{
    return (_pt.x == bounds.x1 || _pt.x == bounds.x2
            || _pt.y == bounds.y1 || _pt.y == bounds.y2);
}

void Voronoi::setBounds(Rectf _bounds)
{
    bounds = _bounds;
}

void Voronoi::setPoints(std::vector<vec2> _points)
{
    clear();
    
    points = _points;
}

void Voronoi::addPoint(vec2 _point)
{
    points.push_back(_point);
}

void Voronoi::addPoints(std::vector<vec2> _points)
{
    for(std::vector<vec2>::iterator it=_points.begin(); it!=_points.end(); ++it) {
        addPoint(*it);
    }
}

Rectf Voronoi::getBounds()
{
    return bounds;
}

std::vector<vec2>& Voronoi::getPoints()
{
    return points;
}

vector <VoronoiCell>& Voronoi::getCells()
{
    return cells;
}


//https://en.wikipedia.org/wiki/Lloyd%27s_algorithm
void Voronoi::relax()
{
    // TODO
//    std::vector<vec2> relaxPts;
//    for(int i=0; i<cells.size(); i++) {
//        ofPolyline p;
//        p.addVertices(cells[i].pts);
//        p.close();
//        ofPoint centroid = p.getCentroid2D();
//        relaxPts.push_back(centroid);
//    }
//    clear();
//    points = relaxPts;
//    generate();
}

VoronoiCell& Voronoi::getCell(vec2 _point, bool approximate)
{
    if(approximate) {
        VoronoiCell& nearestCell = cells[0];
        float nearestDistance = numeric_limits<float>::infinity();
        for(VoronoiCell& cell : cells) {
            float distance = distance2(_point, cell.pt);
            if(distance < nearestDistance) {
                nearestDistance = distance;
                nearestCell = cell;
            }
        }
        return nearestCell;
    }
    else {
        for(VoronoiCell& cell : cells) {
            if(_point == cell.pt) {
                return cell;
            }
        }
        app::console() << "ofxVoronoi getCell could not find exact match for " << _point << std::endl;
        return cells[0];
    }
}
