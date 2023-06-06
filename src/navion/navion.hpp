#pragma once

#include "cgp/cgp.hpp"
#include "cgp/geometry/transform/rotation_transform/rotation_transform.hpp"
#include "environment.hpp"
#include "reacteur.hpp"
#include <vector>

using cgp::mesh;
using cgp::mesh_drawable;
using cgp::numarray;
using cgp::vec3;

class Navion
{
public:
    void initialize();
    void draw(environment_structure const &environment);
    void set_position(vec3 const &position);
    void set_orientation(rotation_transform const &orientation);
    void update_hierachy();
    void set_angle_aile(float const angle);
    void create_millennium_falcon(float const &scale = 1);
    void create_vaisseau_vador(float const &scale = 1);
    void create_star_destroyer(float const &scale = 1);

protected:
    cgp::hierarchy_mesh_drawable hierarchie;

private:
    float nangle_aile;
    mesh create_cocpit_coque(float const &radius, float const &length);
    mesh pseudo_cone(float const &radius, float const &length, int const &n);

    mesh create_corps_falcon(float const &radius, float const &heigh, int const &n);
    mesh create_truc_sur_le_falcon(float const &scale, bool const &droite);

    mesh transversale_vador(float const &scale);

    mesh corps_destroyer(float const &scale);
    mesh batiment_destroyer(float const &scale);
    mesh tour_destroyer(float const &scale);
    mesh poste_de_commande_destroyer(float const &scale);

    bool has_wings = false;
    float angle_aile_min = 0;
    float angle_aile_max = 90;

    // Pour ajouter des flammes, on d�finit ces variables globales de la classe.
    // Elles donnent les positions et orientations des flammes des reacteurs, par d�faut il n'y en a pas
    Reacteur feu_sa_mere;
    std::vector<vec3> position_reacteur;
    std::vector<rotation_transform> direction_reacteur;
};

// void initialize_navion();
// void bouge_navion();
