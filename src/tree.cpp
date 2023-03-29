#include "tree.hpp"
#include "cgp/core/array/numarray_stack/special_types/special_types.hpp"
#include "cgp/core/base/basic_types/basic_types.hpp"
#include <cmath>
#define CYLINDER_N 20 // Nombre de points utilisés pour faire un cercle de cylindre

using namespace cgp;

float circle_fraction(int i, int max = CYLINDER_N)
{
    return 2 * Pi * (float)i / max;
}

mesh create_cylinder_mesh(float radius, float height)
{
    mesh m; // Buffer de points vec3 (à insérer dans le bon ordre)

    // REMPLISSAGE DES POINTS
    m.position.resize(2 * CYLINDER_N);

    // Remplissage du bas puis du haut
    for (int i = 0; i < CYLINDER_N; ++i)
    {
        // Bas
        m.position[2 * i] = vec3{radius * cos(circle_fraction(i)), radius * sin(circle_fraction(i)), 0}; // z = 0
        // Haut
        m.position[2 * i + 1] = vec3{radius * cos(circle_fraction(i)), radius * sin(circle_fraction(i)), height};
    }

    // REMPLISSAGE DE LA CONNECTIVITE
    // En gros, ce sont les triangles qu'on va tracer à partir de la liste de positions
    // 1 triangle par point
    m.connectivity.resize(CYLINDER_N * 2);
    for (int i = 0; i < CYLINDER_N * 2; ++i)
    {
        // Attention au sens, qui doit être direct
        if (i % 2 == 0)
            m.connectivity[i] = uint3{(i + 2) % (CYLINDER_N * 2), (i + 1) % (CYLINDER_N * 2), i};
        else
            m.connectivity[i] = uint3{i, (i + 1) % (CYLINDER_N * 2), (i + 2) % (CYLINDER_N * 2)};
    }

    // Similar with the triangle connectivity:
    //  m.connectivity.push_back(uint3{index_1, index_2, index_3});

    // Need to call fill_empty_field() before returning the mesh
    //  this function fill all empty buffer with default values (ex. normals, colors, etc).
    m.fill_empty_field();

    return m;
}

mesh create_cone_mesh(float radius, float height, float z_offset)
{
    mesh m;

    m.position.resize(CYLINDER_N + 2);

    m.position[0] = vec3{0, 0, z_offset};                       // Base
    m.position[CYLINDER_N + 1] = vec3{0, 0, height + z_offset}; // Sommet

    // Création des points du cercle
    for (int i = 0; i < CYLINDER_N; ++i)
    {
        m.position[i + 1] = vec3{radius * cos(circle_fraction(i)), radius * sin(circle_fraction(i)), z_offset};
    }

    // Remplissage de la connectivité
    for (int i = 0; i < CYLINDER_N; ++i)
    {
        // Triangles de la base
        // On met la normale vers le bas, tournevis inversé
        m.connectivity.push_back(uint3{0, (i + 1) % CYLINDER_N + 1, i + 1});

        // Triangles du chapeau
        m.connectivity.push_back(uint3{CYLINDER_N + 1, i + 1, (i + 1) % CYLINDER_N + 1});
    }

    m.fill_empty_field();
    return m;
}

mesh create_tree()
{
    float h = 0.7f; // trunk height
    float r = 0.1f; // trunk radius

    // Create a brown trunk
    mesh trunk = create_cylinder_mesh(r, h);
    trunk.color.fill({0.4f, 0.3f, 0.3f});

    // Create a green foliage from 3 cones
    mesh foliage = create_cone_mesh(4 * r, 6 * r, 0.0f);      // base-cone
    foliage.push_back(create_cone_mesh(4 * r, 6 * r, 2 * r)); // middle-cone
    foliage.push_back(create_cone_mesh(4 * r, 6 * r, 4 * r)); // top-cone
    foliage.apply_translation_to_position({0, 0, h});         // place foliage at the top of the trunk
    foliage.color.fill({0.4f, 0.6f, 0.3f});

    // The tree is composed of the trunk and the foliage
    mesh tree = trunk;
    tree.push_back(foliage);

    return tree;
}

mesh create_mushroom()
{
    float h = 0.1f;  // trunk height
    float r = 0.03f; // trunk radius

    mesh stem = create_cylinder_mesh(r, h);
    stem.color.fill({0.6, 0.5, 0.5}); // Marron

    mesh head = create_cone_mesh(r * 3, 4 * r, h);
    head.color.fill({1, 0, 0}); // Rouge

    mesh mushroom = stem;
    mushroom.push_back(head);

    return mushroom;
}