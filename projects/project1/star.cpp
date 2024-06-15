#include "star.h"
#include <cmath>

Star::Star(const glm::vec2& position, float rotation, float radius, float aspect)
    : _position(position), _rotation(rotation), _radius(radius) {
    // TODO: assemble the vertex data of the star
    // write your code here
    // -------------------------------------
    // for (int i = 0; i < 5; ++i) {
    //     _vertices.push_back( ... );
    // }
    // -------------------------------------
    

    glm::vec2 vto, vti;
    float angle = glm::radians(72.0f);
    float rotation_in = rotation + glm::radians(36.0f);
    float radius_in = radius * 0.381966f;

    for (int i = 0; i < 5; i++)
    {
        vto.x = position.x + radius * std::cos(rotation + angle * i);
        vto.y = position.y + radius * std::sin(rotation + angle * i) * aspect;
        _vertices.push_back(vto);

        vto.x = position.x + radius * std::cos(rotation + angle * ((i + 2) % 5));
        vto.y = position.y + radius * std::sin(rotation + angle * ((i + 2) % 5)) * aspect;
        _vertices.push_back(vto);

        vti.x = position.x + radius_in * std::cos(rotation_in + angle * ((i + 2) % 5));
        vti.y = position.y + radius_in * std::sin(rotation_in + angle * ((i + 2) % 5)) * aspect;
        _vertices.push_back(vti);
    }
    
    glGenVertexArrays(1, &_vao);
    glGenBuffers(1, &_vbo);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(glm::vec2) * _vertices.size(), _vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

Star::Star(Star&& rhs) noexcept
    : _position(rhs._position), _rotation(rhs._rotation), _radius(rhs._radius), _vao(rhs._vao),
      _vbo(rhs._vbo) {
    rhs._vao = 0;
    rhs._vbo = 0;
}

Star::~Star() {
    if (_vbo) {
        glDeleteVertexArrays(1, &_vbo);
        _vbo = 0;
    }

    if (_vao) {
        glDeleteVertexArrays(1, &_vao);
        _vao = 0;
    }
}

void Star::draw() const {
    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(_vertices.size()));
}