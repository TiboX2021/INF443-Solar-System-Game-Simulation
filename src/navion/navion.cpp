#include "cgp/graphics/drawable/hierarchy_mesh_drawable/hierarchy_mesh_drawable.hpp"
#include "cgp/graphics/drawable/triangles_drawable/triangles_drawable.hpp"
#include "cgp/core/base/basic_types/basic_types.hpp"
#include <cmath>
#include "navion.hpp"
#include "environment.hpp"

void Navion::initialize() {

	has_wings = true;
	angle_aile_min = 0;
	angle_aile_max = 80;
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

	if (has_wings) {
		float langle = Pi/180 * angle_aile_min + Pi / 180 * nangle_aile * (angle_aile_max - angle_aile_min) / 100;
		hierarchie["AileDH"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, langle);
		hierarchie["AileGH"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, -langle);
		hierarchie["AileDB"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, -langle);
		hierarchie["AileGB"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 },  langle);
	}


	
	// This function must be called before the drawing in order to propagate the deformations through the hierarchy
	hierarchie.update_local_to_global_coordinates();

	//Draw the hierarchy as a single mesh
	cgp::draw(hierarchie, environment);


	// On ajoute ensuite le feu des réacteurs par l'appel à la classe
	// à noter : si le vaisseau n'a pas de réacteur, la liste est vide et rien n'est affiché
	feu_sa_mere.display_reacteur(position_reacteur, direction_reacteur, environment);

}



void Navion::set_position(vec3 const& position) {
	hierarchie["Corps"].transform_local.translation = position;
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

	for (int k = 0; k < N; ++k)
	{
		float u = k / (N - 1.0f) / 2;
		vec3 p = radius * vec3(0.0f, std::cos(2 * Pi * u), std::sin(2 * Pi * u));
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



//*************************************************************************
//*************************************************************************
//					Le falcon
//*************************************************************************
//*************************************************************************


mesh Navion::create_corps_falcon(float const& radius, float const& heigh, int const& n) {
	mesh bande;
	for (int k = 0; k < n+1; k++) {
		float u = k / (n + 0.0f);
		vec3 p1 =  vec3(radius * std::cos(2 * Pi * u), radius * std::sin(2 * Pi * u), heigh );
		vec3 p2 =  vec3(radius * std::cos(2 * Pi * u), radius * std::sin(2 * Pi * u), -heigh);
		bande.position.push_back(p1);
		bande.position.push_back(p2);
		bande.uv.push_back(vec2(0.49, 0.535) + vec2(std::cos(2 * Pi * u + Pi) / 4, std::sin(2 * Pi * u + Pi) / 3));
		bande.uv.push_back(vec2(0.49,0.535) + vec2(std::cos(2 * Pi * u)/4, std::sin(2 * Pi * u)/3 ));
		

	}
	bande.position.push_back(vec3(0,0, 2.9*heigh));
	bande.uv.push_back({ 0.49,0.535 });
	bande.position.push_back(vec3(0, 0, -2.9 * heigh));
	bande.uv.push_back({ 0.49,0.535 });

	for (int k = 0; k < n; k++) {
		bande.connectivity.push_back(uint3(2 * n + 2, 2 * k, 2 * k + 2));
		bande.connectivity.push_back(uint3(2 * n + 3, 2 * k + 3, 2 * k + 1));
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
	vec2 centre_texture = { 0.43,0.4 };
	float echelle_texture = -0.15;

	aile.position.push_back(scale*vec3(0,0,0.2));
	aile.uv.push_back(centre_texture);
	aile.position.push_back(scale*vec3(0, 0, -0.2));
	aile.uv.push_back(centre_texture+ echelle_texture*vec2(0.3,0.3));

	aile.position.push_back(scale*vec3(2, 0, 0.2));
	aile.uv.push_back(centre_texture + echelle_texture* vec2(2,0));
	aile.position.push_back(scale*vec3(2, 0, -0.2));
	aile.uv.push_back(centre_texture + echelle_texture * vec2(2, 0) + echelle_texture * vec2(0.3, 0.3));
	
	aile.position.push_back(scale*vec3(2, d*0.3, 0.2));
	aile.uv.push_back(centre_texture + echelle_texture * vec2(2, 0.3));
	aile.position.push_back(scale*vec3(2, d*0.3, -0.2));
	aile.uv.push_back(centre_texture + echelle_texture * vec2(2, 0.3) + echelle_texture * vec2(0.3, 0.3));

	aile.position.push_back(scale*vec3(0, d*1, 0.2));
	aile.uv.push_back(centre_texture + echelle_texture * vec2(0, 1));
	aile.position.push_back(scale*vec3(0, d*1, -0.2));
	aile.uv.push_back(centre_texture + echelle_texture * vec2(0, 1) + echelle_texture * vec2(0.3, 0.3));

	aile.position.push_back(scale*vec3(0, d*0.3, 0.2));
	aile.uv.push_back(centre_texture + echelle_texture * vec2(0, 0.3));
	aile.position.push_back(scale*vec3(0, d*0.3, -0.2));
	aile.uv.push_back(centre_texture + echelle_texture * vec2(0, 0.3) + echelle_texture * vec2(0.3, 0.3));
	
	//On ajoute un à un les triangles : 
	
	//ensuite le dessous : 
	aile.connectivity.push_back(uint3(1, 5, 3));
	aile.connectivity.push_back(uint3(1, 9, 5));
	aile.connectivity.push_back(uint3(9, 7, 5));

	

	//d'abord le dessus :
	aile.connectivity.push_back(uint3(8, 4, 6));
	aile.connectivity.push_back(uint3(0, 2, 4));
	aile.connectivity.push_back(uint3(0, 4, 8));

	
	//et enfin les côtés :

	aile.connectivity.push_back(uint3(0, 1, 2));
	aile.connectivity.push_back(uint3(2, 3, 4));
	aile.connectivity.push_back(uint3(4, 5, 6));
	aile.connectivity.push_back(uint3(6, 7, 0));

	aile.connectivity.push_back(uint3(3, 2, 1));
	aile.connectivity.push_back(uint3(5, 4, 3));
	aile.connectivity.push_back(uint3(7, 6, 5));
	aile.connectivity.push_back(uint3(1, 0, 7));
	

	

	

	aile.fill_empty_field();
	return aile;
}



void Navion::create_millennium_falcon(float const& scale ) {
	has_wings = false;
	// Initialize the temporary mesh_drawable that will be inserted in the hierarchy
	
	mesh_drawable centre;
	mesh_drawable corps;
	mesh_drawable tunnel_interne;  // A faire
	mesh_drawable bords;         // A faire

	mesh_drawable aile_droite;
	mesh_drawable aile_gauche;


	
	centre.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(scale * 0.4, {0,0, -0.8 * scale}, {0,0,0.8* scale}, 10, 20, true));
	corps.initialize_data_on_gpu(create_corps_falcon(scale * 2, scale * 0.25, 12));

	aile_droite.initialize_data_on_gpu(create_truc_sur_le_falcon(scale, true));
	aile_gauche.initialize_data_on_gpu(create_truc_sur_le_falcon(scale, true));

	tunnel_interne.initialize_data_on_gpu(cgp::mesh_primitive_cone(0.35, 2.5, { 0,0,-2.5 }));


	// ******************************************
	//pour le cocpit :
	//cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cone(0.35, 0.5,{0,0,0}, {1,0,0}));
	// ************************************************
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

	corps_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(scale * 0.35, { -1 * scale,0,0 }, { 0,0,0 }));
	cocpit.initialize_data_on_gpu(create_cocpit_coque(0.35 * scale, scale * 0.5));
	vitre_cocpit.initialize_data_on_gpu(pseudo_cone(scale * 0.35, scale * 0.5, 5));
	bord1_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(scale * 0.02,  { 0,scale * 0.35, 0 }, { 0, scale * 0.35 * cos(Pi / 5),scale * 0.35 * sin(Pi / 5) }));
	bord2_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(scale * 0.02, { 0, scale * 0.35 * cos(Pi / 5),scale * 0.35 * sin(Pi / 5) }, { 0, scale * 0.35 * cos(2 * Pi / 5),scale * 0.35 * sin(2 * Pi / 5) }));
	bord3_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(scale * 0.02, { 0, scale * 0.35 * cos(2 * Pi / 5),scale * 0.35 * sin(2 * Pi / 5) }, { 0 , scale * 0.35 * cos(3 * Pi / 5),scale * 0.35 * sin(3 * Pi / 5) }));
	bord4_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(scale * 0.02, { 0,scale * 0.35 * cos(3 * Pi / 5),scale * 0.35 * sin(3 * Pi / 5) }, { 0 , scale * 0.35 * cos(4 * Pi / 5),scale * 0.35 * sin(4 * Pi / 5) }));
	bord5_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(scale * 0.02, { 0, scale * 0.35 * cos(4 * Pi / 5),scale * 0.35 * sin(4 * Pi / 5) }, { 0 , -0.35 * scale ,0 }));
	milieu1_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(scale * 0.02, { 0, scale * 0.35 * cos(Pi / 5) + 0.01* scale , scale * 0.35 * sin(Pi / 5) + scale *0.01 }, { scale * 0.5 , 0 ,0 }));
	milieu2_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(scale * 0.02, { 0, scale * 0.35 * cos(2 * Pi / 5) + scale * 0.01 , scale * 0.35 * sin(2 * Pi / 5) + scale * 0.01 }, { scale * 0.5 , 0 ,0 }));
	milieu3_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(scale * 0.02, { 0, scale * 0.35 * cos(3 * Pi / 5) + scale * 0.01 , scale * 0.35 * sin(3 * Pi / 5) + scale * 0.01 }, { scale * 0.5 , 0 ,0 }));
	milieu4_cocpit.initialize_data_on_gpu(cgp::mesh_primitive_cylinder(scale * 0.02, { 0, scale * 0.35 * cos(4 * Pi / 5) + scale * 0.01 , scale * 0.35 * sin(4 * Pi / 5) + scale * 0.01 }, { scale * 0.5 , 0 ,0 }));

	// *********************************************************************




	// Set the color of some elements


	centre.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	corps.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/Star_Wars_Millenium_Falcon_FSX_P3D_1.jpg",
		GL_REPEAT,
		GL_REPEAT);

	aile_droite.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/Star_Wars_Millenium_Falcon_FSX_P3D_1.jpg",
		GL_REPEAT,
		GL_REPEAT);
	aile_gauche.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/Star_Wars_Millenium_Falcon_FSX_P3D_1.jpg",
		GL_REPEAT,
		GL_REPEAT);

	

	centre.material.phong.specular = 0.0;
	corps.material.phong.specular = 0.0;
	aile_droite.material.phong.specular = 0.0;
	aile_gauche.material.phong.specular = 0.0;
	corps_cocpit.material.phong.specular = 0.0;
	cocpit.material.phong.specular = 0.0;
	vitre_cocpit.material.phong.specular = 0.9;


	// Add the elements in the hierarchy
	//   The syntax is hierarchy.add(mesh_drawable, "name of the parent element", [optional: local translation in the hierarchy])
	//   Notes: 
	//     - An element must necessarily be added after its parent
	//     - The first element (without explicit name of its parent) is assumed to be the root.

	
	
	hierarchie.add(corps, "Corps");

	hierarchie.add(centre, "Centre", "Corps");
	

	hierarchie.add(aile_droite, "Aile_droite", "Centre", { scale * 1.5, scale * 0.2,0 });
	hierarchie.add(aile_gauche, "Aile_gauche", "Centre", { scale * 1.5, -0.2 * scale,0 });
	hierarchie["Aile_gauche"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, Pi);



	// **************************************
	//        re le cocpit 
	// *************************************
	corps_cocpit.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	cocpit.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	bord1_cocpit.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	bord2_cocpit.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	bord3_cocpit.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	bord4_cocpit.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	bord5_cocpit.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	milieu1_cocpit.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	milieu2_cocpit.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	milieu3_cocpit.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	milieu4_cocpit.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	vitre_cocpit.material.color = { 0,0,0 };
	
	hierarchie.add(corps_cocpit, "Corps_cocpit", "Centre", { scale * 1.7, -1.4 * scale,0});
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



//***************************************************************************
//*************************************************************************
//					Le T-wing chelou 


void Navion::create_vaisseau_vador(float const& scale) {
	has_wings = true;
	angle_aile_min = -20;
	angle_aile_max = -50;


	mesh_drawable corps;
	mesh_drawable barre_transversale;
	mesh_drawable aile_droite;
	mesh_drawable aile_gauche;
	mesh_drawable aile_HD;
	mesh_drawable aile_HG;
	mesh_drawable aile_BD;
	mesh_drawable aile_BG;
	mesh_drawable avant;
	mesh_drawable arriere;

	



	corps.initialize_data_on_gpu(mesh_primitive_cylinder(0.5 * scale, scale * vec3(0.3, 0, 0), scale * vec3(-0.3, 0, 0), 10, 20, true));
	barre_transversale.initialize_data_on_gpu(transversale_vador(scale));
	avant.initialize_data_on_gpu(mesh_primitive_sphere(scale*0.5));
	arriere.initialize_data_on_gpu(mesh_primitive_sphere(scale *0.5));
	aile_droite.initialize_data_on_gpu(mesh_primitive_quadrangle(
		{-1.6*scale, 0,0.15 * scale }, { -1.6 * scale,0, -0.15 * scale }, { 1.6 * scale,0, -0.15 * scale }, { 1.6 * scale,0,0.15 * scale }));
	aile_gauche.initialize_data_on_gpu(mesh_primitive_quadrangle(
		{ 1.6 * scale,0,0.15 * scale }, { 1.6 * scale,0, -0.15 * scale }, { -1.6 * scale,0, -0.15 * scale }, { -1.6 * scale, 0,0.15 * scale }));
	aile_BD.initialize_data_on_gpu(mesh_primitive_quadrangle(
		{ 1.6 * scale , 0, 0}, { -1.6 * scale, 0, 0 }, { -0.45 * scale, 0, -1.6 * scale }, { 0.45 * scale, 0, -1.6 * scale }));
	aile_BG.initialize_data_on_gpu(mesh_primitive_quadrangle(
		{ 0.45 * scale, 0, -1.6 * scale } , { -0.45 * scale, 0, -1.6 * scale }, { -1.6 * scale, 0, 0 }, { 1.6 * scale , 0, 0 }));
	aile_HG.initialize_data_on_gpu(mesh_primitive_quadrangle(
		{ 1.6 * scale , 0, 0 }, { -1.6 * scale, 0, 0 }, { -0.45 * scale, 0, 1.6 * scale }, { 0.45 * scale, 0, 1.6 * scale }));
	aile_HD.initialize_data_on_gpu(mesh_primitive_quadrangle(
		{ 0.45 * scale, 0, 1.6 * scale }, { -0.45 * scale, 0, 1.6 * scale }, { -1.6 * scale, 0, 0 }, { 1.6 * scale , 0, 0 }));


	// set the color and texture of the elements :
	corps.material.phong.specular = 0.0;
	barre_transversale.material.phong.specular = 0.0;
	arriere.material.phong.specular = 0.0;
	aile_droite.material.phong.specular = 0.0;
	aile_gauche.material.phong.specular = 0.0;
	aile_BD.material.phong.specular = 0.0;
	aile_BG.material.phong.specular = 0.0;
	aile_HD.material.phong.specular = 0.0;
	aile_HG.material.phong.specular = 0.0;

	corps.material.color = { 0.3,0.3,0.3 };
	barre_transversale.material.color = { 0.3,0.3,0.3 };
	arriere.material.color = { 0.3,0.3,0.3 };
	aile_droite.material.color = { 0.3,0.3,0.3 };
	aile_gauche.material.color = { 0.3,0.3,0.3 };
	aile_BD.material.color = { 0.3,0.3,0.3 };
	aile_BG.material.color = { 0.3,0.3,0.3 };
	aile_HD.material.color = { 0.3,0.3,0.3 };
	aile_HG.material.color = { 0.3,0.3,0.3 };


	corps.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	barre_transversale.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	aile_droite.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	aile_gauche.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	aile_HD.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	aile_HG.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	aile_BD.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	aile_BG.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	arriere.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	avant.material.color = { 0.1,0.1,0.1 };

	



	// add elements to hierarchie

	hierarchie.add(corps, "Corps");
	hierarchie.add(barre_transversale, "Barre", "Corps");
	hierarchie.add(avant, "Avant", "Corps", {scale*0.3,0,0});
	hierarchie.add(arriere, "Arriere", "Corps", {scale* -0.3,0,0 });
	hierarchie.add(aile_droite, "AileDroite", "Barre", { 0,-2 * scale,0 });
	hierarchie.add(aile_droite, "AileGauche", "Barre", { 0,2 * scale,0 });
	hierarchie.add(aile_BD, "AileDB", "AileDroite", { 0,0, -0.15 * scale });
	hierarchie.add(aile_BG, "AileGB", "AileGauche", { 0,0,-0.15 * scale });
	hierarchie.add(aile_HD, "AileDH", "AileDroite", { 0,0, 0.15 * scale });
	hierarchie.add(aile_HG, "AileGH", "AileGauche", { 0,0,0.15 * scale });


	// Pour le cocpit : une demi-sphère, 24 tuyaux
	float radius = scale * 0.5;
	mesh_drawable contours_cocpit;
	mesh_drawable transversal_cocpit;
	mesh_drawable hublot;

	contours_cocpit.initialize_data_on_gpu(mesh_primitive_cylinder(0.03*scale, 
		{0.02*scale, radius*cos(Pi/8) +0.02*scale, -sin(Pi/8)*radius - 0.02 * scale }, 
		{0.02*scale,radius*cos(Pi/8) + 0.02 * scale, radius*sin(Pi/8) + 0.02 * scale }));
	hublot.initialize_data_on_gpu(mesh_primitive_cylinder(0.02*scale, 
		{0.02*scale + 0.8*radius, 0.6*radius*cos(Pi/8) , -0.6*radius* sin(Pi/8)}, 
		{0.02 * scale + 0.8 * radius, 0.6 * radius*cos(Pi/8), 0.6*radius*sin(Pi/8)}));
	transversal_cocpit.initialize_data_on_gpu(mesh_primitive_cylinder(0.02*scale, 
		{ 0.02 * scale, radius * cos(Pi / 8) + 0.02 * scale, -sin(Pi / 8) * radius - 0.02 * scale },
		{ 0.02 * scale + 0.8 * radius, 0.6 * radius * cos(Pi / 8) , -0.6 * radius * sin(Pi / 8) }));

	contours_cocpit.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	hublot.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);
	transversal_cocpit.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture vaisseau.jpg",
		GL_REPEAT,
		GL_REPEAT);

	hierarchie.add(contours_cocpit, "Contour1", "Avant");
	hierarchie.add(contours_cocpit, "Contour2", "Avant");
	hierarchie["Contour2"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, Pi / 4);
	hierarchie.add(contours_cocpit, "Contour3", "Avant");
	hierarchie["Contour3"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, 2*Pi / 4);
	hierarchie.add(contours_cocpit, "Contour4", "Avant");
	hierarchie["Contour4"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, 3*Pi / 4);
	hierarchie.add(contours_cocpit, "Contour5", "Avant");
	hierarchie["Contour5"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, Pi);
	hierarchie.add(contours_cocpit, "Contour6", "Avant");
	hierarchie["Contour6"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, 5*Pi / 4);
	hierarchie.add(contours_cocpit, "Contour7", "Avant");
	hierarchie["Contour7"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, 6*Pi / 4);
	hierarchie.add(contours_cocpit, "Contour8", "Avant");
	hierarchie["Contour8"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, 7*Pi / 4);

	hierarchie.add(hublot, "Hublot1", "Avant");
	hierarchie.add(hublot, "Hublot2", "Avant");
	hierarchie["Hublot2"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, Pi / 4);
	hierarchie.add(hublot, "Hublot3", "Avant");
	hierarchie["Hublot3"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, 2*Pi / 4);
	hierarchie.add(hublot, "Hublot4", "Avant");
	hierarchie["Hublot4"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, 3*Pi / 4);
	hierarchie.add(hublot, "Hublot5", "Avant");
	hierarchie["Hublot5"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, Pi);
	hierarchie.add(hublot, "Hublot6", "Avant");
	hierarchie["Hublot6"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, 5*Pi / 4);
	hierarchie.add(hublot, "Hublot7", "Avant");
	hierarchie["Hublot7"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, 6*Pi / 4);
	hierarchie.add(hublot, "Hublot8", "Avant");
	hierarchie["Hublot8"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, 7*Pi / 4);

	hierarchie.add(transversal_cocpit, "Trans1", "Avant");
	hierarchie.add(transversal_cocpit, "Trans2", "Avant");
	hierarchie["Trans2"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, Pi / 4);
	hierarchie.add(transversal_cocpit, "Trans3", "Avant");
	hierarchie["Trans3"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, 2*Pi / 4);
	hierarchie.add(transversal_cocpit, "Trans4", "Avant");
	hierarchie["Trans4"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, 3*Pi / 4);
	hierarchie.add(transversal_cocpit, "Trans5", "Avant");
	hierarchie["Trans5"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, Pi);
	hierarchie.add(transversal_cocpit, "Trans6", "Avant");
	hierarchie["Trans6"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, 5*Pi / 4);
	hierarchie.add(transversal_cocpit, "Trans7", "Avant");
	hierarchie["Trans7"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, 6*Pi / 4);
	hierarchie.add(transversal_cocpit, "Trans8", "Avant");
	hierarchie["Trans8"].transform_local.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, 7*Pi / 4);
}


mesh Navion::transversale_vador(float const& scale) {
	mesh truc;
	// face avant :
	//                        2 
	//              ---------- ----------
	//   -----------                     -----------
	//  1                     5                     3
	//   -----------                     -----------
	//              ---------- ----------
	//                        4

	truc.position.resize(11);

	truc.position[0] = { 0,0,0 }; // en vrai, inutile mais je me suis chibré dans la numérotation
	truc.position[1] = scale *vec3( 0.2, -2, 0 );
	truc.position[2] = scale * vec3( 0.3, 0, 0.3 );
	truc.position[3] = scale * vec3( 0.2, 2 , 0 );
	truc.position[4] = scale * vec3( 0.3, 0, -0.3 );
	truc.position[5] = scale * vec3( 0.4, 0, 0 );


	// Arrière : la même chose +5

	truc.position[6] = scale * vec3( -0.2, -2, 0 );
	truc.position[7] = scale * vec3( -0.3, 0, 0.3 );
	truc.position[8] = scale * vec3( -0.2,2 , 0 );
	truc.position[9] = scale * vec3( -0.3, 0, -0.3 );
	truc.position[10] = scale * vec3( -0.4, 0, 0);

	// ensuite la connectivité (aled)
	truc.connectivity.push_back(uint3(1, 4,5));
	truc.connectivity.push_back(uint3(1, 5, 2));
	truc.connectivity.push_back(uint3(3, 2, 5));
	truc.connectivity.push_back(uint3(3, 5, 4));

	truc.connectivity.push_back(uint3(6, 10, 9));
	truc.connectivity.push_back(uint3(6, 7, 10));
	truc.connectivity.push_back(uint3(8, 10, 7));
	truc.connectivity.push_back(uint3(8, 9, 10));

	truc.connectivity.push_back(uint3(1, 2, 6));
	truc.connectivity.push_back(uint3(7, 6, 2));
	truc.connectivity.push_back(uint3(1, 4, 6));
	truc.connectivity.push_back(uint3(9, 6, 4));

	truc.connectivity.push_back(uint3(2, 3, 8));
	truc.connectivity.push_back(uint3(8, 7, 2));
	truc.connectivity.push_back(uint3(4, 3, 8));
	truc.connectivity.push_back(uint3(8, 9, 4));

	truc.fill_empty_field();

	return truc;
}




//*********************************************************************
//********************************************************************
//            le star destroyer
//********************************************************************
//********************************************************************



void Navion::create_star_destroyer(float const& scale) {
	mesh_drawable corps;
	mesh_drawable batiment1;
	mesh_drawable batiment2;
	mesh_drawable tour;
	mesh_drawable command;
	mesh_drawable reacteur;


	has_wings = false;
	 
	corps.initialize_data_on_gpu(corps_destroyer(scale));
	batiment1.initialize_data_on_gpu(batiment_destroyer(scale));
	batiment2.initialize_data_on_gpu(batiment_destroyer(0.7*scale));
	tour.initialize_data_on_gpu(tour_destroyer(scale));
	command.initialize_data_on_gpu(poste_de_commande_destroyer(scale));
	reacteur.initialize_data_on_gpu(mesh_primitive_cylinder(0.4, {0,0,0}, {-0.65,0,0}));

	
	

	corps.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture_vaisseau_mieux.jpg",
		GL_REPEAT,
		GL_REPEAT);

	batiment1.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture_vaisseau_mieux.jpg",
		GL_REPEAT,
		GL_REPEAT);
	
	batiment2.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture_vaisseau_mieux.jpg",
		GL_REPEAT,
		GL_REPEAT);

	tour.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture_vaisseau_mieux.jpg",
		GL_REPEAT,
		GL_REPEAT);

	command.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture_vaisseau_mieux.jpg",
		GL_REPEAT,
		GL_REPEAT);

	reacteur.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/navion/texture_vaisseau_mieux.jpg",
		GL_REPEAT,
		GL_REPEAT);

	hierarchie.add(corps, "Corps");
	hierarchie.add(batiment1, "Bat1", "Corps", scale * vec3(-0.75, 0,0));
	hierarchie.add(batiment2, "Bat2", "Bat1", scale* vec3( - 0.3, 0, 0.5 ));
	hierarchie.add(tour, "Tour", "Corps", scale * vec3(-2, 0, 1));
	hierarchie.add(command, "Command", "Tour", scale * vec3(0,0, 1.05));
	hierarchie.add(reacteur, "Reacteur1", "Corps", scale * vec3(-3, 0, 0));
	hierarchie.add(reacteur, "Reacteur2", "Corps", scale * vec3(-3, -2, 0));
	hierarchie.add(reacteur, "Reacteur3", "Corps", scale * vec3(-3, 2, 0));



	// Pour les flammes des reacteurs, on utilise la classe reacteur : il suffit d'ajouter les positions et directions à la liste
	feu_sa_mere.initialize();

	rotation_transform angle1 = rotation_transform::from_axis_angle({ 0,1,0 }, -Pi / 2);
	position_reacteur.push_back(scale * vec3(-3, 0, 0));
	direction_reacteur.push_back(angle1);
	position_reacteur.push_back(scale * vec3(-3, -2, 0));
	direction_reacteur.push_back(angle1);
	position_reacteur.push_back(scale * vec3(-3, 2, 0));
	direction_reacteur.push_back(angle1);

	rotation_transform angle2 = rotation_transform::from_axis_angle({ 1,0,0 }, -Pi / 2) * rotation_transform::from_axis_angle({ 0,1,0 }, -Pi / 2) ;
	position_reacteur.push_back(scale * vec3(-3, 0, 0));
	direction_reacteur.push_back(angle2);
	position_reacteur.push_back(scale * vec3(-3, -2, 0));
	direction_reacteur.push_back(angle2);
	position_reacteur.push_back(scale * vec3(-3, 2, 0));
	direction_reacteur.push_back(angle2);

	rotation_transform angle3 = rotation_transform::from_axis_angle({ 1,0,0 }, Pi / 4) * rotation_transform::from_axis_angle({ 0,1,0 }, -Pi / 2);
	position_reacteur.push_back(scale * vec3(-3, 0, 0));
	direction_reacteur.push_back(angle3);
	position_reacteur.push_back(scale * vec3(-3, -2, 0));
	direction_reacteur.push_back(angle3);
	position_reacteur.push_back(scale * vec3(-3, 2, 0));
	direction_reacteur.push_back(angle3);

	rotation_transform angle4 = rotation_transform::from_axis_angle({ 1,0,0 }, -Pi / 4) * rotation_transform::from_axis_angle({ 0,1,0 }, -Pi / 2);
	position_reacteur.push_back(scale * vec3(-3, 0, 0));
	direction_reacteur.push_back(angle4);
	position_reacteur.push_back(scale * vec3(-3, -2, 0));
	direction_reacteur.push_back(angle4);
	position_reacteur.push_back(scale * vec3(-3, 2, 0));
	direction_reacteur.push_back(angle4);

	// On ajoute ensuite trois disques pour que les réacteurs ressemblent à qqch :
	mesh_drawable disque_feu;
	disque_feu.initialize_data_on_gpu(mesh_primitive_disc(0.4, { -0.01,0,0 }, {1,0,0}));

	// TODO : revoir la couleur
	disque_feu.material.color = { 0.5,0.5,1 };
	disque_feu.material.phong.ambient = 1;
	disque_feu.material.phong.diffuse = 1;
	disque_feu.material.phong.specular = 0;

	hierarchie.add(disque_feu, "Disque1", "Reacteur1");
	hierarchie.add(disque_feu, "Disque2", "Reacteur2");
	hierarchie.add(disque_feu, "Disque3", "Reacteur3");
}


mesh Navion::corps_destroyer(float const& scale) {
	mesh corps;


	// le plan est le suivant :
	//   2-------0-------4
	//    \             /
	//     \           /
	//      \         /
	//       \       /
	//        \     /
	//         \   /
	//          \ /
	//           6
	//pour le dessus

	float length = scale * 8.0;
	float back_length = scale * -3;
	float boarder_heigth = scale * 0.2;
	float heigth = scale * 1;
	float depth = scale * -1.75;
	float large = scale * 4.5;


	corps.position.push_back({back_length, 0, heigth});
	corps.uv.push_back({ 0,0 });
	corps.position.push_back({back_length, 0, depth});
	corps.uv.push_back({ 2, 2});
	corps.position.push_back({back_length, -large, boarder_heigth});
	corps.uv.push_back({ 0,-1 });
	corps.position.push_back({ back_length, -large, -boarder_heigth });
	corps.uv.push_back({ 2,1 });
	corps.position.push_back({back_length, large, boarder_heigth});
	corps.uv.push_back({ 0,1 });
	corps.position.push_back({ back_length, large, -boarder_heigth });
	corps.uv.push_back({ 2,3 });
	corps.position.push_back({length, 0, boarder_heigth});
	corps.uv.push_back({ 5,0 });
	corps.position.push_back({ length, 0, -boarder_heigth });
	corps.uv.push_back({ 7,2 });

	// la connectivité entre les triangles : 
	// l'arriere:
	corps.connectivity.push_back(uint3(0, 4, 2));
	corps.connectivity.push_back(uint3(1, 5, 3));
	
	//le dessus :
	corps.connectivity.push_back(uint3(0, 6, 4));
	corps.connectivity.push_back(uint3(0, 2, 6));

	//le dessous: 
	corps.connectivity.push_back(uint3(1, 7, 5 ));
	corps.connectivity.push_back(uint3(1, 3, 7));



	// les côtés :
	corps.connectivity.push_back(uint3(2, 3, 4));
	corps.connectivity.push_back(uint3(5, 4, 3));

	corps.connectivity.push_back(uint3(4, 6, 5));
	corps.connectivity.push_back(uint3(7, 5, 6));

	corps.connectivity.push_back(uint3(2, 3, 6));
	corps.connectivity.push_back(uint3(7, 6, 3));

	corps.fill_empty_field();

	return corps;

}

mesh Navion::batiment_destroyer(float const& scale) {
	mesh batiment;

	
	//
	//       _____---0---_____
	//   2---                 ---8
	//   |                       |
	//   |                       |
	//   |                       | 
	//   4-----------------------6
	//
	batiment.position.push_back(scale* vec3(0, 0, 1.2));
	batiment.uv.push_back({ 0,0 });
	batiment.position.push_back(scale * vec3(-2, 0, 1.2)); 
	batiment.uv.push_back({ 0.5,0 });

	batiment.position.push_back(scale * vec3(0, -1, 0.9)); 
	batiment.uv.push_back({ 0,-0.5 });
	batiment.position.push_back(scale * vec3(-2, -1, 0.9));
	batiment.uv.push_back({ 0.5,-0.5 });

	batiment.position.push_back(scale * vec3(0, -1, 0)); 
	batiment.uv.push_back({ -1, -1 });
	batiment.position.push_back(scale * vec3(-2, -1, 0));
	batiment.uv.push_back({ 1.5 ,-1 });

	batiment.position.push_back(scale * vec3(0, 1, 0)); 
	batiment.uv.push_back({ -1, 1 });
	batiment.position.push_back(scale * vec3(-2, 1, 0));
	batiment.uv.push_back({ 1.5, 1 });

	batiment.position.push_back(scale * vec3(0, 1, 0.9));
	batiment.uv.push_back({ 0, 0.5 });
	batiment.position.push_back(scale * vec3(-2, 1, 0.9));
	batiment.uv.push_back({ 0.5, 0.5 });


	// puis la connectivité :

	//devant:
	batiment.connectivity.push_back(uint3(0,2,8));
	batiment.connectivity.push_back(uint3(8,2,4));
	batiment.connectivity.push_back(uint3(8, 4, 6));

	//derriere: 
	batiment.connectivity.push_back(uint3(1, 3, 9));
	batiment.connectivity.push_back(uint3(9, 3, 5));
	batiment.connectivity.push_back(uint3(9, 5, 7));

	// le toit:
	batiment.connectivity.push_back(uint3(0, 1, 2));
	batiment.connectivity.push_back(uint3(2, 1, 3));

	batiment.connectivity.push_back(uint3(1, 9, 0));
	batiment.connectivity.push_back(uint3(0, 8, 9));

	// les cotes :
	batiment.connectivity.push_back(uint3(2, 3, 5));
	batiment.connectivity.push_back(uint3(2, 5, 4));

	batiment.connectivity.push_back(uint3(7, 9, 8));
	batiment.connectivity.push_back(uint3(8, 6, 7));

	batiment.fill_empty_field();

	return batiment;
}


mesh Navion::tour_destroyer(float const& scale) {
	mesh tour;

	//  base de la tour:     coté:     face avant: 
	//   2---------3           4       4----5
	//   |         |         / |       |    |
	//   |         |        /  |       |    |
	//   0---------1       2---0       0----1
	
	tour.position.push_back(scale * vec3(0, -0.5, 0));
	tour.uv.push_back({ 0,0 });
	tour.position.push_back(scale * vec3(0, 0.5, 0));
	tour.uv.push_back({ 0,1 });
	tour.position.push_back(scale * vec3(-0.6, -0.5, 0));
	tour.uv.push_back({ 2,0 }); 
	tour.position.push_back(scale * vec3(-0.6, 0.5, 0));
	tour.uv.push_back({ 2,1 });
	tour.position.push_back(scale * vec3(0, -0.5, 1.3));
	tour.uv.push_back({ 1,0.25 });
	tour.position.push_back(scale * vec3(0, 0.5, 1.3));
	tour.uv.push_back({ 1,0.75 });

	//face avant :
	tour.connectivity.push_back(uint3(0, 1, 4));
	tour.connectivity.push_back(uint3(5, 4, 1));

	//face arrière :
	tour.connectivity.push_back(uint3(2, 4, 5));
	tour.connectivity.push_back(uint3(5, 2, 3));

	//cotes :
	tour.connectivity.push_back(uint3(1, 5, 3));
	tour.connectivity.push_back(uint3(0, 4, 2));



	tour.fill_empty_field();
	return tour;
}


mesh Navion::poste_de_commande_destroyer(float const& scale) {
	mesh commande;

	// les points et la connectivité
	//           ____----0----____
	//    10-----                  -----2
	//    |                             |
	//    |                             |
	//    8------____         ____------4
	//               ----6----
	// et idem de l'autre côté
	// 
	// Problème : la texture s'applique de manière bizarre
	// Solution : doubler les points pour avoir une texture cohérente (Problèmes de shaders après ?)


	float heigh = scale * 0.6;
	float length = scale * 2.0;
	float depth = scale * 0.5;
	float side_heigh = scale * 0.4;




	//*******************************************************************//
	// On définit ensuite les points qui serviront à former les faces avant et arrière : 
	//*******************************************************************//

	commande.position.push_back({ depth / 2, 0,heigh / 2 });
	commande.uv.push_back({ 0,0 });
	commande.position.push_back({ -depth / 2, 0, heigh / 2 });
	commande.uv.push_back({ 0,0.5 });

	commande.position.push_back({ depth / 2, length / 2, side_heigh / 2 });
	commande.uv.push_back({ 1,0 });
	commande.position.push_back({ -depth / 2, length / 2, side_heigh / 2 });
	commande.uv.push_back({ 1,0.5 });

	commande.position.push_back({ depth / 2, length / 2, -side_heigh / 2 });
	commande.uv.push_back({ 1.5,0 });
	commande.position.push_back({ -depth / 2, length / 2, -side_heigh / 2 });
	commande.uv.push_back({ 1.5,0.5 });

	commande.position.push_back({ depth / 2, 0, -heigh / 2 });
	commande.uv.push_back({ 2.5,0 });
	commande.position.push_back({ -depth / 2, 0, -heigh / 2 });
	commande.uv.push_back({ 2.5,0.5 });

	commande.position.push_back({ depth / 2, -length / 2, -side_heigh / 2 });
	commande.uv.push_back({ 3,0 });
	commande.position.push_back({ -depth / 2, -length / 2, -side_heigh / 2 });
	commande.uv.push_back({ 3,0.5 });

	commande.position.push_back({ depth / 2, -length / 2, side_heigh / 2 });
	commande.uv.push_back({ 4,0 });
	commande.position.push_back({ -depth / 2, -length / 2, side_heigh / 2 });
	commande.uv.push_back({ 4,0.5 });




	//************************************************//
	//    d'abord on définit les point qui serviront pour les côtés, avec les coordonnées de texture qui conviennent //
	//************************************************//

	commande.position.push_back({ depth / 2, 0,heigh / 2 });
	commande.uv.push_back({0.6 , 0});
	commande.position.push_back({ -depth / 2, 0, heigh / 2 });
	commande.uv.push_back({ 0.6, 0 });

	commande.position.push_back({ depth / 2, length/2, side_heigh / 2 });
	commande.uv.push_back({ 0.5,1 });
	commande.position.push_back({ -depth / 2, length / 2, side_heigh / 2 });
	commande.uv.push_back({ 0.5,1 });

	commande.position.push_back({ depth / 2, length / 2, -side_heigh / 2 });
	commande.uv.push_back({ 0.1 , 1 });
	commande.position.push_back({ -depth / 2, length / 2, -side_heigh / 2 });
	commande.uv.push_back({ 0.1, 1 });

	commande.position.push_back({ depth / 2, 0, -heigh / 2 });
	commande.uv.push_back({ 0,0 });
	commande.position.push_back({ -depth / 2, 0, -heigh / 2 });
	commande.uv.push_back({ 0, 0 });

	commande.position.push_back({ depth / 2, -length / 2, -side_heigh / 2 });
	commande.uv.push_back({ 0.1, -1 });
	commande.position.push_back({ -depth / 2, -length / 2, -side_heigh / 2 });
	commande.uv.push_back({ 0.1, -1 });

	commande.position.push_back({ depth / 2, -length / 2, side_heigh / 2 });
	commande.uv.push_back({ 0.5 , -1 });
	commande.position.push_back({ -depth / 2, -length / 2, side_heigh / 2 });
	commande.uv.push_back({ 0.5 , -1 });

	//puis la connectivité des côtés :
	// cotés :
	commande.connectivity.push_back(uint3(10, 11, 8));
	commande.connectivity.push_back(uint3(8, 11, 9));

	commande.connectivity.push_back(uint3(3, 4, 2));
	commande.connectivity.push_back(uint3(4, 3, 5));

	// dessus :
	commande.connectivity.push_back(uint3(0, 1, 10));
	commande.connectivity.push_back(uint3(10, 1, 11));

	commande.connectivity.push_back(uint3(0, 1, 2));
	commande.connectivity.push_back(uint3(2, 1, 3));

	// dessous :
	commande.connectivity.push_back(uint3(6, 8, 9));
	commande.connectivity.push_back(uint3(6, 9, 7));

	commande.connectivity.push_back(uint3(6, 4, 5));
	commande.connectivity.push_back(uint3(5, 4, 7));






	
	// On définit ensuite la connectivité

	//face avant :
	commande.connectivity.push_back(uint3(22, 14, 12));
	commande.connectivity.push_back(uint3(20, 18, 16));
	commande.connectivity.push_back(uint3(22, 16, 14));
	commande.connectivity.push_back(uint3(20, 16, 22));


	//face arriere :
	commande.connectivity.push_back(uint3(23, 15, 13));
	commande.connectivity.push_back(uint3(21, 19, 17));
	commande.connectivity.push_back(uint3(23, 17, 15));
	commande.connectivity.push_back(uint3(21, 17, 23));


	



	commande.fill_empty_field();
	return commande;
}