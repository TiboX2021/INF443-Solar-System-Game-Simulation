#include "scene.hpp"

#include "background/galaxy.hpp"
#include "cgp/geometry/shape/mesh/primitive/mesh_primitive.hpp"
#include "planet/ring_planet.hpp"
using namespace cgp;

void scene_structure::initialize()
{
    camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
    camera_control.set_rotation_axis_z();
    camera_control.look_at({15.0f, 6.0f, 6.0f}, {0, 0, 0});
    global_frame.initialize_data_on_gpu(mesh_primitive_frame());




    /*
    int N_terrain_samples = 100;
    float terrain_length = 20;

    // Initialize background galaxy
    galaxy.initialize();

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

    */


    //************************************
    //     nessais sur le navion
    //************************************

    un_vaisseau.initialize();



}

void scene_structure::display_frame()
{
    // Set the light to the current position of the camera
    environment.light = camera_control.camera_model.position();

    if (gui.display_frame)
        draw(global_frame, environment);

    /*

    draw(terrain, environment);
    // draw(test_tree, environment);
    // draw(test_cone, environment);

    // Affichage de l'arbre pour toutes les positions
    for (int i = 0; i < tree_positions.size(); ++i)
    {
        tree.model.translation = tree_positions[i];
        draw(tree, environment);
    }

    // Affichage des champignons
    for (int i = 0; i < mushroom_positions.size(); ++i)
    {
        mushroom.model.translation = mushroom_positions[i];
        draw(mushroom, environment);
    }

    if (gui.display_wireframe)
    {
        draw_wireframe(terrain, environment);
        // draw_wireframe(test_tree, environment);
        // draw_wireframe(test_tree, environment);
        // draw_wireframe(tree);
    }
    display_semiTransparent();

    */

    un_vaisseau.set_angle_aile(gui.angle_aile_vaisseau * Pi / 180) ;
    un_vaisseau.draw(environment);

}

void scene_structure::display_gui()
{
    ImGui::Checkbox("Frame", &gui.display_frame);
    ImGui::Checkbox("Wireframe", &gui.display_wireframe);

    ImGui::SliderFloat("Angle Aile", &gui.angle_aile_vaisseau, 0, 80);
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
    // Enable use of alpha component as color blending for transparent elements
    //  alpha = current_color.alpha
    //  new color = previous_color * alpha + current_color * (1-alpha)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Disable depth buffer writing
    //  - Transparent elements cannot use depth buffer
    //  - They are supposed to be display from furest to nearest elements
    glDepthMask(false);

    // Draw ring planet billboard
    ring_planet.draw_ring_billboard(environment, camera_control, gui.display_wireframe);

    // Don't forget to re-activate the depth-buffer write
    glDepthMask(true);
    glDisable(GL_BLEND);
}