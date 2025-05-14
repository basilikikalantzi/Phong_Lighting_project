#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>
#include <Sphere.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.5f, 15.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(false);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);    // build and compile shaders
    // -------------------------
    Shader ourShader("1.model_loading.vs", "1.model_loading.fs");

    // load models
    // -----------
  
    Model ourModel(FileSystem::getPath("resources/woman1.obj"));

    // Create a sphere object for lighting representation
    Sphere sphere(25, 25);
    float orbitRadius = 10.0f;  // Distance of the light source from the center
    float orbitSpeed = 1.5f;   // Rotation speed of the light source

    // Debounce timing for key presses 
    float lastKeyPressTime = 0.0f; 
    float debounceTime = 0.15f;    

    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);  // White light
    glm::vec3 lightPos(0.0f, 0.0f, 0.0f);  // Updated every frame

    // Create a shader program for the Phong lighting model
    Shader phongShader("lightning_sphere.vs", "lightning_sphere.fs");
   
   

      // render loop
      // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Calculate the new light position (orbiting effect)
        float time = static_cast<float>(glfwGetTime());
        float x = orbitRadius * cos(time * orbitSpeed);
        float z = orbitRadius * sin(time * orbitSpeed);
        lightPos = glm::vec3(x, 0.0f, z);

        // Use the model's shader and set uniform values
        ourShader.use();
        ourShader.setVec3("lightPos", lightPos);   // Pass updated light position
        ourShader.setVec3("lightColor", lightColor);
        ourShader.setVec3("viewPos", camera.Position);

        
        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);

  
        phongShader.use();

        // Set up lighting parameters
        phongShader.setVec3("lightPos", glm::vec3(x, 0.0f, z)); // Sphere's position is the light source
        phongShader.setVec3("viewPos", camera.Position); // Camera position
        phongShader.setVec3("lightColor", glm::vec3(7.0f, 7.0f, 7.0f)); // White light
        phongShader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f)); // Sphere color 

        // Reuse the projection and view matrices
        phongShader.setMat4("projection", projection);
        phongShader.setMat4("view", view);

        // Sphere transformation matrix
        glm::mat4 sphereModel = glm::mat4(1.0f);
        sphereModel = glm::translate(sphereModel, glm::vec3(x, 0.0f, z));
        phongShader.setMat4("model", sphereModel);

        // Enable wireframe mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // Draw the sphere
        sphere.Draw();
        
        // Reset to normal rendering if needed
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        float currentTime = glfwGetTime();  // Get the current time
        // Increase orbit speed when 'H' is pressed
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS && currentTime - lastKeyPressTime >= debounceTime) {
            orbitSpeed += 0.05f;
            lastKeyPressTime = currentTime;
        }

        // Decrease orbit speed when 'J' is pressed
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && currentTime - lastKeyPressTime >= debounceTime) {
            orbitSpeed = std::max(0.0f, orbitSpeed - 0.05f);  // Ensure orbitSpeed doesn't go negative
            lastKeyPressTime = currentTime;
        }


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
