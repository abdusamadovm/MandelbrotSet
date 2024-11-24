#include "ComplexPlane.h"


ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
    : m_pixel_size{ pixelWidth, pixelHeight },
    m_plane_center{ 0.0, 0.0 },
    m_zoomCount(0),
    m_state(State::CALCULATING)
{
    m_aspectRatio = static_cast<float>(pixelHeight) / static_cast<float>(pixelWidth);
    m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
    m_vArray.setPrimitiveType(Points);
    m_vArray.resize(pixelWidth * pixelHeight);
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
    target.draw(m_vArray);
}

void ComplexPlane::updateRender()
{
    if(m_state == State::CALCULATING) {
        for (int i = 0; i < m_pixel_size.y; ++i) {
            for (int j = 0; j < m_pixel_size.x; ++j) {
                int index = j + i * m_pixel_size.x;
                m_vArray[index].position = { static_cast<float>(j), static_cast<float>(i) };

                Vector2f complexCoord = mapPixelToCoords({ j, i });
                size_t iterations = countIterations(complexCoord);
                Uint8 r, g, b;

                iterationsToRGB(iterations, r, g, b);
                m_vArray[index].color = { r, g, b };
            }
        }
        m_state = State::DISPLAYING;
    }
}

void ComplexPlane::zoomIn()
{
    m_zoomCount++;
    double x = BASE_WIDTH * (pow(BASE_ZOOM, m_zoomCount));
    double y = BASE_HEIGHT * m_aspectRatio * (pow(BASE_ZOOM, m_zoomCount));

    Vector2f mouseCoord = mapPixelToCoords(m_mouseLocation);
    m_plane_center = mouseCoord;

    m_plane_size = { static_cast<float>(x), static_cast<float>(y) };
    m_state = State::CALCULATING;
}

void ComplexPlane::zoomOut()
{
    m_zoomCount--;
    double x = BASE_WIDTH * (pow(BASE_ZOOM, m_zoomCount));
    double y = BASE_HEIGHT * m_aspectRatio * (pow(BASE_ZOOM, m_zoomCount));

    Vector2f mouseCoord = mapPixelToCoords(m_mouseLocation);
    m_plane_center = mouseCoord;

    m_plane_size = { static_cast<float>(x), static_cast<float>(y) };
    m_state = State::CALCULATING;
}

void ComplexPlane::setCenter(const Vector2i& mousePixel)
{
    Vector2f complexCoord = mapPixelToCoords(mousePixel);
    m_plane_center = complexCoord;
    m_state = State::CALCULATING;
}


void ComplexPlane::setMouseLocation(const Vector2i& mousePixel)
{
    m_mouseLocation = mousePixel;
}


void ComplexPlane::loadText(Text& text)
{
    std::ostringstream oss;
    oss << "Mandelbrot Set\n";
    oss << "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")\n";

    Vector2f cursorCoords = mapPixelToCoords(m_mouseLocation);
    oss << "Cursor: (" << cursorCoords.x << ", " << cursorCoords.y << ")\n";
    oss << "Left-click to Zoom in\nRight-click to Zoom out";

    text.setString(oss.str());
}

size_t ComplexPlane::countIterations(const Vector2f& coord)
{
    float z_real = 0.0;
    float z_imag = 0.0;

    float c_real = coord.x;
    float c_imag = coord.y;

    size_t iterations = 0;

    while (z_real * z_real + z_imag * z_imag < 4.0 && iterations < MAX_ITER) {
        float z_real_new = z_real * z_real - z_imag * z_imag + c_real;
        float z_imag_new = 2.0f * z_real * z_imag + c_imag;

        z_real = z_real_new;
        z_imag = z_imag_new;

        ++iterations;
    }
    return iterations;
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
    if (count == MAX_ITER) {
        r = 0;
        g = 0;
        b = 0;
        return;
    }

    float t = static_cast<float>(count) / static_cast<float>(MAX_ITER);

    if (t < 0.2) {
        r = static_cast<Uint8>(255 * t);
        g = static_cast<Uint8>(0);
        b = static_cast<Uint8>(255 * (1 - t));
    }
    else if (t < 0.4) {
        r = static_cast<Uint8>(0);
        g = static_cast<Uint8>(255 * (t - 0.2) * 2);
        b = static_cast<Uint8>(255);
    }

    else if (t < 0.6) {
        r = static_cast<Uint8>(0);
        g = static_cast<Uint8>(255);
        b = static_cast<Uint8>(255 * (1 - (t - 0.4) * 2));
    }
    else if (t < 0.8) {
        r = static_cast<Uint8>(255);
        g = static_cast<Uint8>(255);
        b = static_cast<Uint8>(255 * (1 - (t - 0.6) * 5));
    }
    else {
        r = static_cast<Uint8>(255);
        g = static_cast<Uint8>(255 * (1 - (t - 0.8) * 5));
        b = static_cast<Uint8>(0);
    }
}


Vector2f ComplexPlane::mapPixelToCoords(const Vector2i& mousePixel)
{
    float real = ((float)mousePixel.x / (float)m_pixel_size.x) * m_plane_size.x + (m_plane_center.x - m_plane_size.x / 2.0 );
    float imaginary = ((float)(m_pixel_size.y - mousePixel.y) / (float)m_pixel_size.y) * m_plane_size.y + (m_plane_center.y - m_plane_size.y / 2.0);

    return Vector2f(real, imaginary);
}
