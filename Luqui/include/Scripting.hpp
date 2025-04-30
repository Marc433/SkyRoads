#ifndef __SCRIPTING__
#define __SCRIPTING__

#include "lua.hpp"
#include <string>
#include <filesystem>
#include <memory>

/**
 * @class LuaScript
 * @brief Wrapper class for Lua scripting functionality.
 *
 * The LuaScript class provides a high-level interface for loading and executing
 * Lua scripts. It manages a Lua state and provides methods for running Lua code
 * and handling errors.
 */
class LuaScript {
public:
  /**
   * @brief Constructor that initializes a Lua state and loads a script file.
   *
   * @param filepath Path to the Lua script file to load.
   */
  LuaScript(std::string filepath);

  /**
   * @brief Default destructor.
   */
  ~LuaScript() = default;

  /**
   * @brief Assignment operator.
   *
   * Creates a new Lua state and copies relevant data.
   *
   * @param other The LuaScript to copy from.
   * @return Reference to this LuaScript after assignment.
   */
  LuaScript& operator=(const LuaScript& other);

  /**
   * @brief Checks for Lua errors and throws exceptions if they occur.
   *
   * @param error The error code returned by a Lua operation.
   * @throws std::runtime_error If the error code indicates an error.
   */
  void check(int error);

  /**
   * @brief Gets the content of the loaded script.
   *
   * @return The script content as a string.
   */
  std::string getContent();

  /**
   * @brief Executes a Lua script string.
   *
   * Loads and runs the provided Lua code string in the current Lua state.
   *
   * @param str The Lua code to execute.
   * @throws std::runtime_error If there's an error loading or executing the script.
   */
  void run(const std::string& str);

private:
  /**
   * @brief Smart pointer to the Lua state with custom deleter.
   *
   * Uses unique_ptr with lua_close as the deleter function to ensure proper cleanup.
   */
  std::unique_ptr<lua_State, decltype(&lua_close)> state_;

  /**
   * @brief Raw pointer to the Lua state for convenience.
   *
   * Points to the same state as state_, but provides a more convenient access form.
   */
  lua_State* s_;

  /**
   * @brief The content of the loaded script file.
   */
  std::string content;

  /**
   * @brief Helper method to read the content of a file.
   *
   * @param filepath Path to the file to read.
   * @return The content of the file as a string.
   */
  std::string readFileContent(const std::filesystem::path& filepath);
};

#endif // !__SCRIPTING__