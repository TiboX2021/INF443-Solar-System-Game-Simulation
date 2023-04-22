#include "scene.hpp"

#include "background/galaxy.hpp"
#include "cgp/geometry/shape/mesh/primitive/mesh_primitive.hpp"
#include "cgp/graphics/drawable/hierarchy_mesh_drawable/hierarchy_mesh_drawable.hpp"
#include "cgp/graphics/drawable/triangles_drawable/triangles_drawable.hpp"
#include "terrain.hpp"
#include "tree.hpp"

using namespace cgp;

void scene_structure::initialize()
{
    camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
    camera_control.set_rotation_axis_z();
    camera_control.look_at({15.0f, 6.0f, 6.0f}, {0, 0, 0});
    global_frame.initialize_data_on_gpu(mesh_primitive_frame());

    int N_terrain_samples = 100;
    float terrain_length = 20;

    parameters.persistency = 0.55;
    parameters.frequency_gain = 2.3;
    parameters.octave = 4;
    parameters.terrain_height = 0.5; // unused

    terrain_mesh = create_terrain_mesh(N_terrain_samples, terrain_length, parameters);
    terrain.initialize_data_on_gpu(terrain_mesh);
    terrain.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/texture_grass.jpg",
                                                          GL_REPEAT,
                                                          GL_REPEAT);
    terrain.material.color = {0.6f, 0.85f, 0.5f};
    terrain.material.phong.specular = 0.0f; // non-specular terrain material

    // test_tree.initialize_data_on_gpu(create_cylinder_mesh(2, 10));
    // test_cone.initialize_data_on_gpu(create_cone_mesh(4, 4, 10));
    tree.initialize_data_on_gpu(create_tree());

    // Génération des positions des arbres
    tree_positions = generate_positions_on_terrain(100, terrain_length, parameters);

    mushroom.initialize_data_on_gpu(create_mushroom());
    mushroom_positions = generate_positions_on_terrain(300, terrain_length, parameters);

    // Generate grass position
    grass_positions = generate_positions_on_terrain(200, terrain_length, parameters);

    // Grass mesh
    mesh quad_mesh = mesh_primitive_quadrangle({-0.5f, 0, 0}, {0.5f, 0, 0}, {0.5f, 0, 1}, {-0.5f, 0, 1});
    grass.initialize_data_on_gpu(quad_mesh);

    // Grass texture
    grass.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/grass.png");
    grass.model.scaling = 0.5; // Reduce size

    // Initialize planet
    planet.initialize();

    // TODO : essayer de mettre un e image de background pour l'environnement ? Ou mettre un objet qui fasse le tour
    // + voir comment correctement transformer l'image en qqch de sphérique, qui bouge avec la vue
    // (un objet, en fait... Sauf qu'on est à l'intérieur, donc il faut inverser la connectivité)
    galaxy.initialize();
}

void scene_structure::display_frame()
{
    // Set the light to the current position of the camera
    environment.light = camera_control.camera_model.position();

    // if (gui.display_frame)
    //     draw(global_frame, environment);

    // draw(terrain, environment);
    // // draw(test_tree, environment);
    // // draw(test_cone, environment);

    // // Affichage de l'arbre pour toutes les positions
    // for (int i = 0; i < tree_positions.size(); ++i)
    // {
    //     tree.model.translation = tree_positions[i];
    //     draw(tree, environment);
    // }

    // // Affichage des champignons
    // for (int i = 0; i < mushroom_positions.size(); ++i)
    // {
    //     mushroom.model.translation = mushroom_positions[i];
    //     draw(mushroom, environment);
    // }

    // if (gui.display_wireframe)
    // {
    //     draw_wireframe(terrain, environment);
    //     // draw_wireframe(test_tree, environment);
    //     // draw_wireframe(test_tree, environment);
    //     // draw_wireframe(tree);
    // }

    // DEBUG : draw sphere
    planet.draw(environment, gui.display_wireframe);
    // Drow galaxy (use very big raduis !)
    galaxy.draw(environment, gui.display_wireframe);

    // display_semiTransparent();
}

void scene_structure::display_gui()
{
    ImGui::Checkbox("Frame", &gui.display_frame);
    ImGui::Checkbox("Wireframe", &gui.display_wireframe);

    bool update = false;
    update |= ImGui::SliderFloat("Persistance", &parameters.persistency, 0.1f, 0.6f);
    update |= ImGui::SliderFloat("Frequency gain", &parameters.frequency_gain, 1.5f, 2.5f);
    update |= ImGui::SliderInt("Octave", &parameters.octave, 1, 8);
    update |= ImGui::SliderFloat("Height", &parameters.terrain_height, 0.1f, 1.5f);

    if (update) // if any slider has been changed - then update the terrain
        update_terrain(terrain_mesh, terrain, parameters, 20);
}

void scene_structure::mouse_move_event()
{
    if (!inputs.keyboard.shift)
        camera_control.action_mouse_move(environment.camera_view);
}
void scene_structure::mouse_click_event()
{
    camera_control.action_mouse_click(environment.camera_view);
}
void scene_structure::keyboard_event()
{
    camera_control.action_keyboard(environment.camera_view);
}
void scene_structure::idle_frame()
{
    camera_control.idle_frame(environment.camera_view);
}

void scene_structure::display_semiTransparent()
{
    // TODO : affichage des billboards

    // Enable use of alpha component as color blending for transparent elements
    //  alpha = current_color.alpha
    //  new color = previous_color * alpha + current_color * (1-alpha)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Disable depth buffer writing
    //  - Transparent elements cannot use depth buffer
    //  - They are supposed to be display from furest to nearest elements
    glDepthMask(false);

    // TODO : faire les trucs ici
    // Calculs avec la caméra pour trier les billboards dans leur ordre d'affichage
    auto const &camera = camera_control.camera_model;

    // Re-orient the grass shape to always face the camera direction
    vec3 const right = camera.right();
    rotation_transform R = rotation_transform::from_frame_transform({1, 0, 0}, {0, 0, 1}, right, {0, 0, 1});

    // Rotate and draw all
    for (int i = 0; i < grass_positions.size(); ++i)
    {
        grass.model.rotation = R;                     // Apliqquer la rotation à l'herbe
        grass.model.translation = grass_positions[i]; // Translate grass

        // Draw the result?
        draw(grass, environment);
    }

    // XXX SKIP THIS STEP
    // Sort transparent shapes by depth to camera
    //   This step can be skipped, but it will be associated to visual artifacts

    // Transform matrix (the same matrix which is applied in the vertices in the shader: T = Projection x View)
    // mat4 T = camera_projection.matrix() * camera.matrix_view();
    // // Projected vertices (center of quads) in homogeneous coordinates
    // vec4 p1 = T * vec4{0, -0.5f, 0, 1};
    // vec4 p2 = T * vec4{0, +0.5f, 0, 1};

    // Depth to camera
    // float z1 = p1.z / p1.w;
    // float z2 = p2.z / p2.w;

    // // Affichage des billboards : il faut les avoir instanciés, etc
    // // Display the quads relative to their depth
    // if (z1 <= z2)
    // {
    //     draw(quad_2, environment);
    //     draw(quad_1, environment);
    // }
    // else
    // {
    //     draw(quad_1, environment);
    //     draw(quad_2, environment);
    // }

    // Don't forget to re-activate the depth-buffer write
    glDepthMask(true);
    glDisable(GL_BLEND);
}