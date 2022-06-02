#include <Windows.h>
#include "GL/glew.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> 

#include "Offsets.h"
#include <MemMan.h>

#include "datatypes.h"
#include "helpers.h"
#include <iostream>
#include "config.h"


using namespace zombies;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}



int main(int, char**)
{
    

    //calling our memory
    MemMan memory;

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return -1;

    uintptr_t Process = memory.getProcess(L"plutonium-bootstrapper-win32.exe");

    if (Process == 0)
    {
        std::cout << "proc is invalid\n";
        return -1;
    }


    const char* glsl_version = "#version 130";

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    if (!monitor)
    {
        return 0;
    }

    int width = glfwGetVideoMode(monitor)->width;
    int height = glfwGetVideoMode(monitor)->height;

    glfwWindowHint(GLFW_FLOATING, true);
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_MAXIMIZED, true);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
   // glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, true);

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(width, height, "PERISH PROJECT", NULL, NULL);
    if (window == NULL)
        return 1;

    glfwSetWindowAttrib(window, GLFW_DECORATED, false);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync



    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to init openGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;



    ImGui::StyleColorsDark();


    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);


    bool menuShown = false;


    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //menu section

        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            menuShown = !menuShown;
            if (menuShown)
            {
                showMenu(window);
            }
            else
            {
                hideMenu(window);
            }
        }


        if (menuShown)
        {
            ImGui::Begin("perish project");
            ImGui::Checkbox("snaplines", &config::esp::snaplines);
            ImGui::Checkbox("box", &config::esp::boxesp);
            ImGui::Checkbox("free cam", &config::misc::noclip);
            ImGui::End();
        }

        viewMatrix Matrix = memory.readMem<viewMatrix>(aViewMatrix);

        if (config::misc::noclip)
        {
            memory.writeMem<int>(Offsets::NoClip, 1);
        }
        else
        {
            memory.writeMem<int>(Offsets::NoClip, 0);
        }


        memory.writeMem<float>(Offsets::oDistanceBetweeen, 1);

        if (config::esp::snaplines)
        {
            
            for (short int i = 1; i < 40; ++i)
            {
                uintptr_t Entity = memory.readMem<uintptr_t>(aEntityList + i * Offsets::oDistanceBetweeen);
                if (Entity == NULL) continue;

                //checking if dead
                int Health = memory.readMem<int>(Entity + Offsets::oHealth);
                if (Health <= 0) continue;


                int RenderFix = memory.readMem<int>(Entity + 0x4);
                if (RenderFix != 16) continue;

                //GetEntityLocation

                Vec3 Location = memory.readMem<Vec3>(Entity + Offsets::oLocation);

                //w2s Location

                Vec2 ScreenCoords;
                if (!WorldToScreen(Location, ScreenCoords, Matrix.Matrix)) continue;


                //drawToScreen
                glBegin(GL_LINES);
                glVertex2f(0.0f, -1.0f);
                glVertex2f(ScreenCoords.X, ScreenCoords.Y);
                glEnd();
            }
        }
        if (config::esp::boxesp)
        {
            //cba
        }



        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}