#include "cgp/graphics/drawable/hierarchy_mesh_drawable/hierarchy_mesh_drawable.hpp"
#include "cgp/graphics/drawable/triangles_drawable/triangles_drawable.hpp"
#include "cgp/core/base/basic_types/basic_types.hpp"
#include <cmath>
#include "navion.hpp"
#include "environment.hpp"

void Navion::initialize() {
	// Initialize the temporary mesh_drawable that will be inserted in the hierarchy
	mesh_drawable corps;
	
	mesh_drawable aile_d;
	mesh_drawable aile_g;
	mesh_drawable reacteur;
	mesh_drawable lance_missile;

	// pour faire le cockpit :
	mesh_drawable cocpit;
	mesh_drawable vitre_cocpit;
	mesh_drawable bord1_cocpit;
	mesh_drawable bord2_cocpit;
	mesh_drawable bord3_cocpit;
	mesh_drawable bord4_cocpit;
	mesh_drawable bord5_cocpit;
	mesh_drawable milieu1_cocpit;
	mesh_drawable milieu2_cocpit;
	mesh_drawable milieu3_cocpit;
	mesh_drawable milieu4_cocpit;


	


	// Create the geometry of the meshes
	//   Note: this geometry must be set in their local coordinates with respect to their position in the hierarchy (and with respect to their animation)
	corps.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(0.35, { -0.5,0,0 }, { 0.5,0,0 }));
	aile_d.initialize_data_on_gpu(cgp::mesh_primitive_quadrangle({ 0.2,0,0 }, { 0.1, 1.5,0 }, { -0.1, 1.5,0 }, { -0.2, 0,0 }));
	aile_g.initialize_data_on_gpu(cgp::mesh_primitive_quadrangle({ -0.2, 0, 0 }, { -0.1, -1.5,0 }, { 0.1, -1.5,0 }, { 0.2,0,0 }));
	reacteur.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(0.05, { -0.2,0,0 }, { 0.1,0,0 }));
	
	lance_missile.initialize_data_on_gpu(cgp::mesh_primitive_cone(0.05, 0.07, { 0,0,0 }, { 1,0,0 }));


	//pour le cocpit :
	//cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cone(0.35, 0.5,{0,0,0}, {1,0,0}));
	cocpit.initialize_data_on_gpu(create_cocpit_coque(0.35, 0.5));
	vitre_cocpit.initialize_data_on_gpu(pseudo_cone(0.35, 0.5, 5));
	bord1_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(0.02, {0,0.35, 0}, {0, 0.35* cos(Pi/5),0.35 * sin(Pi / 5) }));
	bord2_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(0.02, { 0, 0.35 * cos(Pi / 5),0.35 * sin(Pi / 5) }, { 0, 0.35 * cos(2*Pi / 5),0.35 * sin(2 * Pi / 5) }));
	bord3_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(0.02, { 0, 0.35 * cos(2*Pi / 5),0.35 * sin(2 * Pi / 5) }, {0 , 0.35 * cos(3*Pi / 5),0.35 * sin(3 * Pi / 5) }));
	bord4_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(0.02, { 0, 0.35 * cos(3*Pi / 5),0.35 * sin(3 * Pi / 5) }, { 0 , 0.35 * cos(4*Pi / 5),0.35 * sin(4 * Pi / 5) }));
	bord5_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(0.02, { 0, 0.35 * cos(4 * Pi / 5),0.35 * sin(4 * Pi / 5) }, { 0 , -0.35 ,0 }));
	milieu1_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(0.02, { 0, 0.35 * cos( Pi / 5) +0.01 , 0.35 * sin( Pi / 5) +0.01}, { 0.5 , 0 ,0 }));
	milieu2_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(0.02, { 0, 0.35 * cos(2 * Pi / 5)+0.01 , 0.35 * sin(2 * Pi / 5) +0.01}, { 0.5 , 0 ,0 }));
	milieu3_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(0.02, { 0, 0.35 * cos(3 * Pi / 5)+0.01 , 0.35 * sin(3 * Pi / 5) +0.01}, { 0.5 , 0 ,0 }));
	milieu4_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(0.02, { 0, 0.35 * cos(4 * Pi / 5)+0.01 , 0.35 * sin(4 * Pi / 5) +0.01}, { 0.5 , 0 ,0 }));




	// Set the color of some elements
	vec3 gris = { 0.5, 0.5, 0.5 };
	//corps.material.color = gris;
	//aile_d.material.color = gris;
	//aile_g.material.color = gris;
	reacteur.material.color = gris;
	vitre_cocpit.material.color = { 0.1,0.1,0.1 };
	lance_missile.material.color = gris;

	bord1_cocpit.material.color = gris;
	bord2_cocpit.material.color = gris;
	bord3_cocpit.material.color = gris;
	bord4_cocpit.material.color = gris;
	bord5_cocpit.material.color = gris;
	milieu1_cocpit.material.color = gris;
	milieu2_cocpit.material.color = gris;
	milieu3_cocpit.material.color = gris;
	milieu4_cocpit.material.color = gris;


	corps.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);

	cocpit.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);

	aile_d.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	aile_g.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	



	// Add the elements in the hierarchy
	//   The syntax is hierarchy.add(mesh_drawable, "name of the parent element", [optional: local translation in the hierarchy])
	//   Notes: 
	//     - An element must necessarily be added after its parent
	//     - The first element (without explicit name of its parent) is assumed to be the root.

	hierarchie.add(corps, "Corps");


	hierarchie.add(cocpit, "Cocpit", "Corps", { 0.5,0,0 });
	hierarchie.add(vitre_cocpit, "Cocpit_Vitre", "Corps", { 0.5,0,0 });
	hierarchie.add(bord1_cocpit, "Bord1", "Cocpit");
	hierarchie.add(bord2_cocpit, "Bord2", "Cocpit");
	hierarchie.add(bord3_cocpit, "Bord3", "Cocpit");
	hierarchie.add(bord4_cocpit, "Bord4", "Cocpit");
	hierarchie.add(bord5_cocpit, "Bord5", "Cocpit");
	hierarchie.add(milieu1_cocpit, "Milieu1", "Cocpit");
	hierarchie.add(milieu2_cocpit, "Milieu2", "Cocpit");
	hierarchie.add(milieu3_cocpit, "Milieu3", "Cocpit");
	hierarchie.add(milieu4_cocpit, "Milieu4", "Cocpit");



	hierarchie.add(aile_d, "AileDH", "Corps", {0,0.3,0.2});
	hierarchie.add(aile_d, "AileDB", "Corps", { 0,0.3,-0.2 });
	hierarchie.add(aile_g, "AileGH", "Corps", { 0,-0.3,0.2 });
	hierarchie.add(aile_g, "AileGB", "Corps", { 0,-0.3,-0.2 });
	hierarchie.add(reacteur, "reacteurDH", "AileDH", { 0, 1.5,0 });
	hierarchie.add(reacteur, "reacteurGH", "AileGH", { 0, -1.5,0 });
	hierarchie.add(reacteur, "reacteurDB", "AileDB", { 0, 1.5,0 });
	hierarchie.add(reacteur, "reacteurGB", "AileGB", { 0, -1.5,0 });
	hierarchie.add(lance_missile, "LM_DH", "reacteurDH", { 0.1,0,0 });
	hierarchie.add(lance_missile, "LM_GH", "reacteurGH", { 0.1,0,0 });
	hierarchie.add(lance_missile, "LM_DB", "reacteurDB", { 0.1,0,0 });
	hierarchie.add(lance_missile, "LM_GB", "reacteurGB", { 0.1,0,0 });

	

	//return hierarchie;
}


void Navion::draw(environment_structure const& environment) {

	// Update position/angle

	/*
	hierarchie["AileDH"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, nangle_aile);
	hierarchie["AileGH"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, - nangle_aile);
	hierarchie["AileDB"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, - nangle_aile);
	hierarchie["AileGB"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 },  nangle_aile);
	*/


	// This function must be called before the drawing in order to propagate the deformations through the hierarchy
	hierarchie.update_local_to_global_coordinates();

	//Draw the hierarchy as a single mesh
	cgp::draw(hierarchie, environment);
}



void Navion::set_position(vec3 const& position) {
	hierarchie["global_frame"].transform_local.translation = position;
}

void Navion::set_direction(vec3 const& direction) {
	//******************** A FAIRE ***********************
}

void Navion::set_angle_aile(float const angle) {
	nangle_aile = angle;
}





mesh Navion::create_cocpit_coque(float const& radius, float const& length) {
	
	mesh semi_cone;
	int N = 20;

	for (int k = 0; k < N; ++k)
	{
		float u = k / (N - 1.0f)/2;
		vec3 p = radius * vec3(0.0f, - std::cos(2 * Pi * u), - std::sin(2 * Pi * u));
		semi_cone.position.push_back(p);
		semi_cone.uv.push_back({ 0.5 + std::cos(2 * Pi * u) * 0.43, 0.5 + std::sin(2 * Pi * u) * 0.43 });

	}
	// middle point
	semi_cone.position.push_back({ length, 0,0 });
	semi_cone.uv.push_back({ 0.5,0.5 });

	for (int k = 0; k < N - 1; ++k)
		semi_cone.connectivity.push_back(uint3{ N, k, k + 1 });

	semi_cone.fill_empty_field();
	return semi_cone;
}

mesh Navion::pseudo_cone(float const& radius, float const& length, int const& n=5) {
	mesh semi_cone;

	int N = n + 1;

	for (int k = 0; k < 2*N; ++k)
	{
		float u = k / (N - 1.0f) / 2;
		vec3 p = radius * vec3(0.0f, std::cos(2 * Pi * u), std::sin(2 * Pi * u));
		semi_cone.position.push_back(p);
		semi_cone.uv.push_back({ 0.5 + std::cos(2 * Pi * u) * 0.43, 0.5 + std::sin(2 * Pi * u) * 0.43 });

	}
	// middle point
	semi_cone.position.push_back({ length, 0,0 });
	semi_cone.uv.push_back({ 0.5,0.5 });

	for (int k = 0; k < 2*N - 1; ++k)
		semi_cone.connectivity.push_back(uint3{ 2*N, k, k + 1 });

	semi_cone.fill_empty_field();
	return semi_cone;
}

mesh Navion::create_bande(float const& radius, float const& heigh, int const& n) {
	mesh bande;
	for (int k = 0; k < n+1; k++) {
		float u = k / (n + 0.0f);
		vec3 p1 = radius * vec3(std::cos(2 * Pi * u), std::sin(2 * Pi * u), heigh / 2);
		vec3 p2 = radius * vec3(std::cos(2 * Pi * u), std::sin(2 * Pi * u), -heigh / 2);
		bande.position.push_back(p1);
		bande.position.push_back(p2);
	}

	for (int k = 0; k < n; k++) {
		bande.connectivity.push_back(uint3(2 * k, 2 * k + 1, 2 * k + 2));
		bande.connectivity.push_back(uint3(2 * k + 2, 2 * k + 1, 2 * k + 3));

	}

	bande.fill_empty_field();
	return bande;

}



mesh Navion::create_truc_sur_le_falcon(float const& scale, bool const& droite) {

	mesh aile;

	// d'abord les point dont on aura besoin :  en haut les numeros impaires, en bas les paires :
	//      0--------------------------------1
	//      |                                |
	//      4                                2
	//      |                       ---------
	//      |       ---------------
	//      3------
	
	int d = droite * 2 - 1;

	aile.position.push_back(scale*vec3(0,0,0.2));
	aile.position.push_back(scale*vec3(0, 0, -0.2));

	aile.position.push_back(scale*vec3(2, 0, 0.2));
	aile.position.push_back(scale*vec3(2, 0, -0.2));
	
	aile.position.push_back(scale*vec3(2, d*0.3, 0.2));
	aile.position.push_back(scale*vec3(2, d*0.3, -0.2));

	aile.position.push_back(scale*vec3(0, d*1, 0.2));
	aile.position.push_back(scale*vec3(0, d*1, -0.2));
	
	aile.position.push_back(scale*vec3(0, d*0.3, 0.2));
	aile.position.push_back(scale*vec3(0, d*0.3, -0.2));

	
	//On ajoute un à un les triangles : 
	
	//ensuite le dessous : 
	aile.connectivity.push_back(uint3(1, 3, 5));
	aile.connectivity.push_back(uint3(1, 5, 9));
	aile.connectivity.push_back(uint3(9, 5, 7));

	//et enfin les côtés :
	
	aile.connectivity.push_back(uint3(0, 1, 2));
	aile.connectivity.push_back(uint3(2, 3, 4));
	aile.connectivity.push_back(uint3(4, 5, 6));
	aile.connectivity.push_back(uint3(6, 7, 0));

	aile.connectivity.push_back(uint3(3, 2, 1));
	aile.connectivity.push_back(uint3(5, 4, 3));
	aile.connectivity.push_back(uint3(7, 6, 5));
	aile.connectivity.push_back(uint3(1, 0, 7));

	//d'abord le dessus :
	aile.connectivity.push_back(uint3(8, 4, 6));
	aile.connectivity.push_back(uint3(0, 2, 4));
	aile.connectivity.push_back(uint3(0, 4, 8));

	

	

	

	

	aile.fill_empty_field();
	return aile;
}




void Navion::create_millennium_falcon() {
	// Initialize the temporary mesh_drawable that will be inserted in the hierarchy
	mesh_drawable haut;
	mesh_drawable bas;
	mesh_drawable centre;
	mesh_drawable contour;
	mesh_drawable tunnel_interne;
	mesh_drawable bords;


	haut.initialize_data_on_gpu(pseudo_cone(2, 0.6,6));
	bas.initialize_data_on_gpu(pseudo_cone(2, 0.6,6));
	centre.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(0.4, {0,0, -0.8}, {0,0,0.8}, 10, 20, true));
	contour.initialize_data_on_gpu(create_bande(2, 0.25, 12));

	//pour le cocpit :
	//cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cone(0.35, 0.5,{0,0,0}, {1,0,0}));
	mesh_drawable corps_cocpit;
	mesh_drawable cocpit;
	mesh_drawable vitre_cocpit;
	mesh_drawable bord1_cocpit;
	mesh_drawable bord2_cocpit;
	mesh_drawable bord3_cocpit;
	mesh_drawable bord4_cocpit;
	mesh_drawable bord5_cocpit;
	mesh_drawable milieu1_cocpit;
	mesh_drawable milieu2_cocpit;
	mesh_drawable milieu3_cocpit;
	mesh_drawable milieu4_cocpit;

	corps_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(0.35, { -1,0,0 }, { 0,0,0 }));
	cocpit.initialize_data_on_gpu(create_cocpit_coque(0.35, 0.5));
	vitre_cocpit.initialize_data_on_gpu(pseudo_cone(0.35, 0.5, 5));
	bord1_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(0.02, { 0,0.35, 0 }, { 0, 0.35 * cos(Pi / 5),0.35 * sin(Pi / 5) }));
	bord2_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(0.02, { 0, 0.35 * cos(Pi / 5),0.35 * sin(Pi / 5) }, { 0, 0.35 * cos(2 * Pi / 5),0.35 * sin(2 * Pi / 5) }));
	bord3_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(0.02, { 0, 0.35 * cos(2 * Pi / 5),0.35 * sin(2 * Pi / 5) }, { 0 , 0.35 * cos(3 * Pi / 5),0.35 * sin(3 * Pi / 5) }));
	bord4_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(0.02, { 0, 0.35 * cos(3 * Pi / 5),0.35 * sin(3 * Pi / 5) }, { 0 , 0.35 * cos(4 * Pi / 5),0.35 * sin(4 * Pi / 5) }));
	bord5_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(0.02, { 0, 0.35 * cos(4 * Pi / 5),0.35 * sin(4 * Pi / 5) }, { 0 , -0.35 ,0 }));
	milieu1_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(0.02, { 0, 0.35 * cos(Pi / 5) + 0.01 , 0.35 * sin(Pi / 5) + 0.01 }, { 0.5 , 0 ,0 }));
	milieu2_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(0.02, { 0, 0.35 * cos(2 * Pi / 5) + 0.01 , 0.35 * sin(2 * Pi / 5) + 0.01 }, { 0.5 , 0 ,0 }));
	milieu3_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(0.02, { 0, 0.35 * cos(3 * Pi / 5) + 0.01 , 0.35 * sin(3 * Pi / 5) + 0.01 }, { 0.5 , 0 ,0 }));
	milieu4_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(0.02, { 0, 0.35 * cos(4 * Pi / 5) + 0.01 , 0.35 * sin(4 * Pi / 5) + 0.01 }, { 0.5 , 0 ,0 }));






	// Set the color of some elements




	// Add the elements in the hierarchy
	//   The syntax is hierarchy.add(mesh_drawable, "name of the parent element", [optional: local translation in the hierarchy])
	//   Notes: 
	//     - An element must necessarily be added after its parent
	//     - The first element (without explicit name of its parent) is assumed to be the root.

	hierarchie.add(centre, "Centre");
	hierarchie.add(haut, "Haut", "Centre", {0,0,0.25});
	hierarchie.add(bas, "Bas", "Centre", {0,0,-0.25});

	hierarchie.add(contour, "Contour", "Centre");

	hierarchie["Haut"].transform_local.rotation = rotation_transform::from_axis_angle({ 0,1,0 }, -Pi / 2);
	hierarchie["Bas"].transform_local.rotation = rotation_transform::from_axis_angle({ 0,1,0 },  Pi / 2);


	hierarchie.add(corps_cocpit, "Corps_cocpit", "Centre", { 1.7,1.4,0 });
	hierarchie.add(cocpit, "Cocpit", "Corps_cocpit");
	hierarchie.add(vitre_cocpit, "Cocpit_Vitre", "Corps_cocpit");
	hierarchie.add(bord1_cocpit, "Bord1", "Cocpit");
	hierarchie.add(bord2_cocpit, "Bord2", "Cocpit");
	hierarchie.add(bord3_cocpit, "Bord3", "Cocpit");
	hierarchie.add(bord4_cocpit, "Bord4", "Cocpit");
	hierarchie.add(bord5_cocpit, "Bord5", "Cocpit");
	hierarchie.add(milieu1_cocpit, "Milieu1", "Cocpit");
	hierarchie.add(milieu2_cocpit, "Milieu2", "Cocpit");
	hierarchie.add(milieu3_cocpit, "Milieu3", "Cocpit");
	hierarchie.add(milieu4_cocpit, "Milieu4", "Cocpit");



}
