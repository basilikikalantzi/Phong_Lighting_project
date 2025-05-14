# Simulating Phong Lighting with Interactive Camera Around a 3D Face

**Course:** Computer Graphics I (2024–2025)  
**Assignment:** Moving Phong Lighting  

*The application has been implemented and tested successfully in Visual Studio.*

---
## Getting Started

If you want to see a simulation of the project, a demo video is available in the `simulation_output-video` folder.

To build and run the application:

1. **Clone the repository:**
   ```bash
   git clone https://github.com/username/Phong_Lighting_project.git

2. **Enter the project directory and create an out-of-source build folder**

3. **Open CMake GUI:**
  - **Where is the source code:** Browse to the root of the cloned repository.  
  - **Where to build the binaries:** Browse to the newly created `build` folder.

4. **Configure & Generate:**
  - Click **Configure**, select the Visual Studio generator (e.g. “Visual Studio 17 2022”), choose the target platform (e.g. x64), then click **Finish**.  
  - Once configuration completes, click **Generate** to create the solution and project files in `build`.

5. **Open the solution from the build folder:**
  - In File Explorer, navigate to the `project1/build` folder and double-click on `LearnOpenGL.sln`.

6. **Set startup project:**
  - In Solution Explorer, right-click **3.model_loading__1.model_loading** and choose **Set as Startup Project**.

7. **Run the application:**
  - Press **F5** or click **Start** to build and launch the program.


---

## Assignment Description

The application must load a 3D face model and simulate Phong lighting using a point light represented by a rotating sphere. Specifically, the requirements are:

- **Model Loading:**
  - Load `woman1.obj` using the `Model` class from LearnOpenGL.
- **Camera Controls:**
  - Horizontal translation with `A`/`D` keys (X axis).
  - Vertical translation with `W`/`S` keys (Y axis).
  - Rotation with mouse movement.
- **Exit:** Close the application with the `Esc` key.
- **Sphere (Light Source):**
  - Implemented via the `Sphere` class:
    - Setup of VAO, VBO, and EBO for vertices (position, normals, UVs).
    - Wireframe rendering and rotation in the X–Z plane around the face.
- **Shaders:**
  - Sphere vertex/fragment shaders for transforms and Phong lighting calculations.
  - Model vertex/fragment shaders for Phong illumination, with an option to use either solid color or texture.
- **Rotation Speed Control:**
  - Increase speed by 0.05 with `H`.
  - Decrease speed by 0.05 with `J` (clamped at zero).
  - Debounce mechanism to allow only one speed change per frame.

---

## Implementation

**Initial Build**

The project is based on the LearnOpenGL repository, limited to the `model_loading` example. A `build` folder was created and populated via CMake. In Visual Studio, the startup project was set to `3.model_loading__1.model_loading`.

**Implementation Steps**

1. **Model Loading & Camera:**
   - The `Model` class successfully loaded `woman1.obj` including normals and texture coordinates.
   - The `Camera` class was extended to handle WASD input for translation and mouse movement for rotation, with `Esc` closing the window.

2. **Sphere as Point Light:**
   - The `Sphere` class was implemented with adjustable division parameters and manages GPU buffer creation.
   - The sphere is rendered in wireframe mode and rotates around the face with a consistent angular speed.

3. **Shaders:**
   - Separate shaders were developed for the sphere and the model. Each vertex shader handles transformations, and the fragment shaders compute ambient, diffuse, and specular components.

4. **Speed Control & Debounce:**
   - The angular speed variable is adjusted via the `H`/`J` keys in increments of 0.05, clamped at zero, with a debounce to limit to one change per frame.

**Dependencies**

All required libraries (GLFW, GLAD, GLM, Assimp) are included within the `3.model_loading__1.model_loading` folder.
