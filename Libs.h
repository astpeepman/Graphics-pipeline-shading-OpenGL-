#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>


// GLM Mathematics
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <SOIL.h>

#include "Shader.h"
#include "3dMath.h"
#include "Triangle.h"
#include "Quad.h"
#include "Cube.h"


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


#define _USE_MATH_DEFINES 
#include <math.h>


#include <stdio.h>
#include <cstdlib>
#include <ctime> 

#include <vector>
#define ToRadian(x) ((x) * M_PI / 180.0f)
#define ToDegree(x) ((x) * 180.0f / M_PI)

#include "Camera.h"
#include "LightCube.h"
#include "CubeObject.h"
#include "SphereObject.h"
#include "SkyBox.h"
#include "Cone.h"
#include "Cylinder.h"
#include "Plane.h"
#include "Model.h"
#include "InstanModel.h"

using namespace std;

