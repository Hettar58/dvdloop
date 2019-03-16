#include <iostream>
using namespace std;
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include "Texture.h"
GLuint loadTexture(const char * texture_file_path) {
	glEnable(GL_TEXTURE_2D);
	int width, height, nrChannels;
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//paramètres de remplissage de la texture. (GL_REPEAT = la texture se répète tant que le polygone n'est pas rempli
	//s et t sont les équivalents de X et Y pour une texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//paramètres de filtrage de texture.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	unsigned char * data = stbi_load(texture_file_path, &width, &height, &nrChannels, STBI_rgb_alpha);

	if (data) {
		//si on peut lire les données de la texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Loaded texture " << texture_file_path << std::endl;
	}
	else {
		std::cout << "Erreur, impossible de charger la texture " << "texture_file_path" << std::endl;
	}

	stbi_image_free(data);
	

	return texture;
}