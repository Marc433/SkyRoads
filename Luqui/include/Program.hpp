#ifndef __PROGRAM__
#define __PROGRAM__

#include <GL/glew.h>
#include "Shader.hpp"
#include <string>
#include "unordered_map"
#include "glm/glm.hpp"
#include "glm/mat4x4.hpp"

/**
 * @class Program
 * @brief Represents an OpenGL shader program.
 *
 * The Program class encapsulates an OpenGL shader program, which is a collection
 * of compiled shaders linked together to create a complete rendering pipeline.
 * It provides methods for attaching shaders, linking the program, and setting
 * uniform variables.
 */
class Program {
public:
	/**
	 * @brief Default constructor.
	 *
	 * Creates a new OpenGL program object.
	 */
	Program();

	/**
	 * @brief Destructor.
	 *
	 * Deletes the OpenGL program object.
	 */
	~Program();

	/**
	 * @brief Attaches a shader to the program.
	 *
	 * @param shader Pointer to the Shader object to attach.
	 */
	void attach(Shader* shader);

	/**
	 * @brief Links the attached shaders into a complete program.
	 *
	 * @return true if linking succeeded, false otherwise.
	 */
	bool link();

	/**
	 * @brief Gets the OpenGL program ID.
	 *
	 * @return The OpenGL program ID.
	 */
	GLuint get_id();

	/**
	 * @brief Sets an integer uniform variable in the shader program.
	 *
	 * @param name The name of the uniform variable.
	 * @param value The integer value to set.
	 */
	void setUniform(const std::string& name, int value);

	/**
	 * @brief Activates the shader program for rendering.
	 */
	void use() const;

	/**
	 * @brief Deactivates the shader program.
	 */
	void unuse();

	/**
	 * @brief Static helper method to create and initialize a Program with vertex and fragment shaders.
	 *
	 * @param program Reference to the Program object to initialize.
	 * @param vertex_shader Pointer to the vertex shader to use.
	 * @param fragment_shader Pointer to the fragment shader to use.
	 * @return true if the program was created and linked successfully, false otherwise.
	 */
	static bool createProgram(Program& program, Shader* vertex_shader, Shader* fragment_shader);

	/**
	 * @brief Sets a vec3 uniform variable in the shader program.
	 *
	 * @param name The name of the uniform variable.
	 * @param value The vec3 value to set.
	 */
	void setVec3(const std::string& name, const glm::vec3& value);

	/**
	 * @brief Sets an integer uniform variable in the shader program.
	 *
	 * @param name The name of the uniform variable.
	 * @param value The integer value to set.
	 */
	void setInt(const std::string& name, const int value);

	/**
	 * @brief Sets a float uniform variable in the shader program.
	 *
	 * @param name The name of the uniform variable.
	 * @param value The float value to set.
	 */
	void setFloat(const std::string& name, const float value);

	/**
	 * @brief Sets a mat4 uniform variable in the shader program.
	 *
	 * @param name The name of the uniform variable.
	 * @param value The mat4 value to set.
	 */
	void setmat4(const std::string& name, glm::mat4& value);

private:
	/**
	 * @brief Maps uniform names to their locations.
	 *
	 * Caches uniform locations to avoid repeated OpenGL calls.
	 */
	std::unordered_map<std::string, int> uniform_map;

	/**
	 * @brief Gets or caches a uniform location.
	 *
	 * @param name The name of the uniform variable.
	 * @return The location of the uniform variable.
	 */
	int MapUniformLocation(const std::string& name);

	/**
	 * @brief Gets a uniform location from OpenGL.
	 *
	 * @param location The name of the uniform variable.
	 * @return The location of the uniform variable.
	 */
	int GetUniformLocation(const std::string& location);

	/**
	 * @brief The OpenGL program ID.
	 */
	GLuint id;
};
#endif // !__PROGRAM__