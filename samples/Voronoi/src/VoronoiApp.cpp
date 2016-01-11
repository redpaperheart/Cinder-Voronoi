#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "Voronoi.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class VoronoiApp : public App {
  public:
	void setup() override;
	void update() override;
    void draw() override;
	void resize() override;
    void mouseDown( MouseEvent event ) override;
    void keyDown( KeyEvent event ) override;
    void addPoints(int numPoints);
    
    vector<vec2> mPoints;
    vector<Color> mColors;
    Rectf mBounds;
    Voronoi mVoronoi;
};

void VoronoiApp::setup()
{
    Rand::randomize();
    mBounds = Rectf(getWindowBounds()).inflated(vec2(-10));
    addPoints(25);
}

void VoronoiApp::update()
{
}

void VoronoiApp::draw()
{
    gl::clear(Color::white());
    
    std::vector<VoronoiCell> &cells = mVoronoi.getCells();
    
    for (int i = 0; i < cells.size(); i++) {
        // draw cell
        gl::ScopedColor cellCol(mColors[i]);
        gl::drawSolid(PolyLine2f(cells[i].pts));
        
        // draw cell point
        gl::ScopedColor cellPointCol(Color::white());
        gl::drawSolidCircle(cells[i].pt, 1.5);
    }
}

void VoronoiApp::addPoints(int numPoints)
{
    int newSize = glm::clamp(int(numPoints + mPoints.size()), 1, 1000);
    
    while (mPoints.size() > newSize) {
        mPoints.pop_back();
    }
    
    while (mPoints.size() < newSize) {
        vec2 point(randFloat(mBounds.x1, mBounds.x2), randFloat(mBounds.y1, mBounds.y2));
        mPoints.push_back(point);
    }
    
    while (mPoints.size() > mColors.size()) {
        mColors.push_back(Color(CM_HSV, randFloat(1.0), randFloat(0.5, 0.8), 1.0));
    }
    
    mVoronoi.clear();
    mVoronoi.setBounds(mBounds);
    mVoronoi.setPoints(mPoints);
    mVoronoi.generate();
}

void VoronoiApp::resize()
{
    mBounds = Rectf(getWindowBounds()).inflated(vec2(-10));
}

void VoronoiApp::mouseDown( MouseEvent event )
{
}

void VoronoiApp::keyDown( KeyEvent event )
{
    switch (event.getChar()) {
        case ' ': {
            int numPoints = mPoints.size();
            mPoints.clear();
            mColors.clear();
            addPoints(numPoints);
        }
            break;
        case '+':
            addPoints(5);
            break;
        case '-':
            addPoints(-5);
            break;
        case 'r':
            mVoronoi.relax();
        default:
            break;
    }
}

CINDER_APP( VoronoiApp, RendererGl(RendererGl::Options().msaa(16)) )
