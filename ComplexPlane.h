#pragma once
#include <iostream>
#include <complex>
#include <SFML/Graphics.hpp>
#include <sstream>

using namespace sf;
using namespace std;

const int MAX_ITER = 64;
const float BASE_WIDTH = 4.0;
const float BASE_HEIGHT = 4.0;
const float BASE_ZOOM = 0.5;

enum class State {
    CALCULATING,
    DISPLAYING
};

class ComplexPlane : public Drawable {
private:
    Vector2i m_pixel_size;
    Vector2f m_plane_center;
    Vector2f m_plane_size;
    int m_zoomCount;
    float m_aspectRatio;
    VertexArray m_vArray;
    State m_state;
    Vector2i m_mouseLocation;
    size_t countIterations(const Vector2f& coord);
    void iterationsToRGB(size_t iter, uint8_t& r, uint8_t& g, uint8_t& b);
    Vector2f mapPixelToCoords(const Vector2i& pixel);
    virtual void draw(RenderTarget& target, RenderStates states) const override;
public:
    void updateRender();
    ComplexPlane(int pixelWidth, int pixelHeight);
    void zoomIn();
    void zoomOut();
    void setCenter(const Vector2i& newCenter);
    void setMouseLocation(const Vector2i& mousePixel);
    void loadText(Text& text);
};
