#ifndef COMMON_H
#define COMMON_H

#define _USE_MATH_DEFINES
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/GLU.h>
#include <glm.hpp>
#include <gtx/transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <IL/il.h>

#define MIPMODE_NONE 2
#define MIPMODE_NEAREST 3
#define MIPMODE_LINEAR 1

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::sin;
using std::cos;
using std::vector;
using std::map;
using std::rand;

#define offset(i) ((char *)NULL +(i))

#endif

