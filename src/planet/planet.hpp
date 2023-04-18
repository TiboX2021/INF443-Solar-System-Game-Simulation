#include "environment.hpp"

// TODO : faire une classe avec des méthodes, genre pour l'affichage, pour les collisions...
// Pas comme terrain qui n'ajoute que des fonctions...

// TODO: faire un fichier pour les fonctions de bruit perlin (dossier utils?)

// TODO : radius, color : on fait ça dans le constructeur ?
// Le mieux c'est de laisser des fonctions d'initialisation
// TODO : héritage pour les objets initialisables on gpu

// Pour l'instant, je propose de faire tout à l'initialisation, et de mettre des setters ensuite? De toutes façons on peut jouer sur tout

/**
 * Planète avec son apparence visuelle et ses propriétés physiques
 */
class Planet
{
public:
    Planet();
    Planet(float radius, vec3 position);

    // Draw function
    void initialize();
    void draw(environment_structure const &environment, bool show_wireframe = true) const;

    // Setters
    void set_radius(float radius);
    void set_position(vec3 position);

private:
    // Planet properties
    float radius;
    vec3 position;

    // CGP elements
    cgp::mesh planet_mesh;
    cgp::mesh_drawable planet_mesh_drawable;
};