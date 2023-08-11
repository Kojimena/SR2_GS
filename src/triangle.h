#pragma once
#include <glm/glm.hpp>
#include "fragment.h"


std::vector<Fragment> line(Vertex A, Vertex B) {
    int x1 = A.position.x;
    int y1 = A.position.y;
    int x2 = B.position.x;
    int y2 = B.position.y;
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */


    std::vector<Fragment> lineFragments;

    while (true) { /* loop */
        lineFragments.push_back(Fragment{glm::ivec2(x1, y1), A.color});
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { /* e_xy + e_x > 0 */
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx) { /* e_xy + e_y < 0 */
            err += dx;
            y1 += sy;
        }
    }

    return lineFragments;
}


std::vector<Fragment> triangle(Vertex a, Vertex b, Vertex c) {
    glm::vec3 A = a.position;
    glm::vec3 B = b.position;
    glm::vec3 C = c.position;

    std::vector<Fragment> triangleFragments;

    std::vector<Fragment> line1 = line(a, b);
    std::vector<Fragment> line2 = line(b, c);
    std::vector<Fragment> line3 = line(c, a);

    triangleFragments.insert(
            triangleFragments.end(),
            line1.begin(),
            line1.end()
    );

    triangleFragments.insert(
            triangleFragments.end(),
            line2.begin(),
            line2.end()
    );

    triangleFragments.insert(
            triangleFragments.end(),
            line3.begin(),
            line3.end()
    );

    return triangleFragments;
}