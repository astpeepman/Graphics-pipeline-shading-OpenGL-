// Lab2_OpenGLGraphics.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.


#include "Libs.h"


const GLuint WIDTH = 800, HEIGHT = 600;

//тональная компрессия
bool hdr = true;
bool hdrKeyPressed = false;
float exposure = 1.0f;
float gamma = 1.0f;

GLfloat rotSpeed=0.0f;
GLfloat translateSpeed = 0.0f;
GLfloat transPoint=0.0f;
Shader texShader;
Shader MainShader;

Cube testCube;
vector<Cube> cubes;

Camera camera(vec3(0.0f, 0.0f, 3.0f));

int sceneNum = 1;

GLfloat lastX = WIDTH/2, lastY = HEIGHT/2;//Начальное положение мыши
//углы поворота камеры
GLfloat cam_yaw = -90.0f;
GLfloat cam_pitch = 0.0f;
GLfloat fov = 45.0f;


GLfloat deltaTime = 0.0f;	// Время, прошедшее между последним и текущим кадром
GLfloat lastFrame = 0.0f;  	// Время вывода последнего кадра

vec3 ObjPosition;



GLfloat random(int a, int b) {

    int interval = b - a;
    GLfloat res = (rand() % (interval * 10 + 1) - interval * 5) / 10;
    return res;
}

void moveModel(Model& someModel, float sc);

string int_to_string(int n)
{
    ostringstream ss;
    ss << n;
    return ss.str();
}

bool negative=false;
//ИНИЦИАЛИЗАЦИЯ ОБЪЕКТОВ
        Shader lightingShad;
        Shader LampShad;
        CubeObject MainCube;
        glm::vec3 lightPos(0, 0, 0);
        Shader skyboxshader;
        Shader sun;
        SphereObject sun_Object;
        SphereObject Earth;
        SphereObject Moon;
        SphereObject Mars;
        SkyBox Skybox;
        float sc;
        InstanModel asteroids;
//до сюда
void InitAnimObjects() {
            lightingShad = Shader("AnimScene.vs", "AnimScene.fs");
            LampShad = Shader("lampShader.vs", "lampShader.fs");
            MainCube=CubeObject(vec3(0, 0, 0), lightingShad, Texture("Content\\textures\\container2.png"), Texture("Content\\textures\\container2_specular.png"));
            skyboxshader=Shader("Skybox.vs", "Skybox.fs");
            sun=Shader("sun.vs", "sun.fs");
            sun_Object= SphereObject(vec3(0.0f, 0.0f, 0.0f), 5.0f, sun, Texture("Content\\textures\\sun.jpg"), Texture("Content\\textures\\sun"), 100);
            Earth=SphereObject(vec3(0.0f, 0.0f, 0.0f), 1.0f, lightingShad, Texture("Content\\textures\\earth.jpg"), Texture("Content\\textures\\earth_spec.png"), 64);
            Moon=SphereObject(vec3(0, 0, 0), 0.3f, lightingShad, Texture("Content\\textures\\moon.jpg"), Texture("Content\\textures\\moon_spec.png"), 64);
            Mars=SphereObject(vec3(0, 0, 0), 0.8f, lightingShad, Texture("Content\\textures\\mars.jpg"), Texture("Content\\textures\\mars_spec.png"), 64);
            Skybox=SkyBox(skyboxshader);
            sc = 0;
            asteroids = InstanModel("Content\\Objects\\Rock\\rock.obj", vec3(0, 0, 0), Shader("Rocks_Instan.vs", "Rocks_Instan.fs"), 3000, 70, 15, 0.01f, 0.1f);

}
void ANIMSCENE(glm::mat4 projection, glm::mat4 view) {
    sc += 0.0005;//scale ИЗМЕНЯЕМ ЭТО ДЛЯ СКОРОСТИ АНИМАЦИИ

    //SUN
    sun.Use();
    sun_Object.setproj(projection);
    sun_Object.setview(view);
    sun_Object.setDiff("ourTexture");
    sun_Object.Rotate(-sc / 100, vec3(0, 1, 0));
    sun_Object.Draw();




    //PLANETS
    lightingShad.Use();
    /*MainCube.draw();*/

    //earth
    Earth.setLightPos(lightPos);
    Earth.setViewPos(camera.Position);
    Earth.setproj(projection);
    Earth.setview(view);
    Earth.SetDfSp();
    Earth.setLightParam(vec3(0.1f, 0.1f, 0.1f), vec3(1.0f, 0.8f, 0.8f), vec3(1.0f, 1.0f, 1.0f));
    Earth.setShinises(30.0f);

    Earth.Planet(vec3(sinf(sc) * 20, 0, cosf(sc) * 20), sc * 5000, vec3(0, -1, 0));
    Earth.Draw();


    //MOON
    Moon.Sputnik(vec3(sinf(sc * 100) * 2, 0, cosf(sc * 100) * 2), -sc * 5720, vec3(0, -1, 0), Earth.GetModel());
    Moon.Draw();

    //MARS
    Mars.setLightPos(lightPos);
    Mars.setViewPos(camera.Position);
    Mars.setproj(projection);
    Mars.setview(view);
    Mars.SetDfSp();
    Mars.setLightParam(vec3(0.1f, 0.1f, 0.1f), vec3(1.0f, 0.8f, 0.8f), vec3(1.0f, 1.0f, 1.0f));
    Mars.setShinises(30.0f);
    Mars.Planet(vec3(sinf(sc * 0.65) * 45, 0, cosf(sc * 0.66) * 45), sc * 3000, vec3(0, 1, 0));
    Mars.Draw();

    
    asteroids.Draw(view, projection);
    asteroids.move(sc);

    //SKYBOX DRAW
    Skybox.draw(view, projection, camera.GetViewMatrix());
}

//ИНИЦИАЛИЗАЦИЯ Статистических объектов
    Shader ConeShader;
    vector<Cone> Cones;
    vector<Cylinder> woods;
    LightCube lamp;
    int countOfPointLights = 10;
    vector<vec3>PointLightPositions;
    Shader PlaneShader;
    Quad Plane;
    vector<vec3>ConstLightPositions;
    GLfloat moveScale = 0;
//До сюда


void InitStaticObjects() {

    moveScale = 0.0f;
    ConeShader = Shader("StaticScene.vs", "StaticScene.fs");
    lamp = LightCube(vec3(0, 0, 0), LampShad);
    PlaneShader =Shader("Plane.vs", "Plane.fs");

    material PlaneMtl;
    PlaneMtl.diffuse = Texture("Content\\textures\\grass1.jpg");
    PlaneMtl.specular = Texture("Content\\textures\\grass1_spec.png");
    /*PlaneMtl.normal = Texture("Content\\textures\\grass1_NRM.png");
    PlaneMtl.shininess = 32.0f;*/

    Plane = Quad(vec3(0, 0, 0), 100, 100, PlaneMtl, PlaneShader);
    Plane.Rotate(-90, vec3(1, 0, 0));

    for (int i = 0; i < 50; i++) {
        GLfloat pos_x = random(-50, 50);
        GLfloat pos_z = random(-50, 50);
        Cones.push_back(Cone(vec3(pos_x, 1, pos_z), 3.0f, ConeShader, 6.0f, 50));
        woods.push_back(Cylinder(vec3(pos_x, 0, pos_z), 0.6f, ConeShader, 1.0f, 50));
    }

    for (int i = 0; i < countOfPointLights; i++) {
        PointLightPositions.push_back(vec3(random(-50, 50), 5, random(-50, 50)));
    }

    ConstLightPositions = PointLightPositions;

}
void STATICSCENE(glm::mat4 projection, glm::mat4 view) {
    moveScale += 0.005f;

    for (int i = 0; i < countOfPointLights; i++) {
        PointLightPositions[i].x = ConstLightPositions[i].x + sinf(moveScale) * i;
        PointLightPositions[i].z = ConstLightPositions[i].z + cosf(moveScale) * i;
    }
    
    ConeShader.Use();

    ConeShader.setVec3("viewPos", camera.Position);
    ConeShader.setVec3("material.diffuse", vec3(0.1f, 1.0f, 0.31f));
    ConeShader.setVec3("material.ambient", vec3(0.1f, 1.0f, 0.31f));
    ConeShader.setVec3("material.specular", vec3(0.5f, 0.5f, 0.5f));
    ConeShader.setFloat("material.shininess", 32.0f);

    // directional light
    ConeShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    ConeShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    ConeShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    ConeShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

    //pointLights
    for (int i = 0; i < countOfPointLights; i++) {
        string num = int_to_string(i);
        ConeShader.setVec3("pointLights[" + num + "].position", PointLightPositions[i]);
        ConeShader.setVec3("pointLights[" + num + "].ambient", 0.05f, 0.05f, 0.05f);
        ConeShader.setVec3("pointLights[" + num + "].diffuse", 0.8f, 0.8f, 0.8f);
        ConeShader.setVec3("pointLights[" + num + "].specular", 1.0f, 1.0f, 1.0f);
        ConeShader.setFloat("pointLights[" + num + "].constant", 1.0f);
        ConeShader.setFloat("pointLights[" + num + "].linear", 0.09);
        ConeShader.setFloat("pointLights[" + num + "].quadratic", 0.032);

    }


    // spotLight
    ConeShader.setVec3("spotLight.position", camera.Position);
    ConeShader.setVec3("spotLight.direction", camera.Front);
    ConeShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    ConeShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    ConeShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    ConeShader.setFloat("spotLight.constant", 1.0f);
    ConeShader.setFloat("spotLight.linear", 0.09);
    ConeShader.setFloat("spotLight.quadratic", 0.032);
    ConeShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    ConeShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));



    ConeShader.setMat4("projection", projection);
    ConeShader.setMat4("view", view);


    for (int i = 0; i < Cones.size(); i++) {
        Cones[i].draw();
    }

    ConeShader.setVec3("material.diffuse", vec3(0.59f, 0.29f, 0.31f));
    ConeShader.setVec3("material.ambient", vec3(0.59f, 0.29f, 0.31f));
    ConeShader.setVec3("material.specular", vec3(0.5f, 0.5f, 0.5f));
    ConeShader.setFloat("material.shininess", 32.0f);

    ConeShader.setMat4("projection", projection);
    ConeShader.setMat4("view", view);

    for (int i = 0; i < Cones.size(); i++) {
        woods[i].draw();
    }


    PlaneShader.Use();
    PlaneShader.setVec3("viewPos", camera.Position);

    // directional light
    PlaneShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    PlaneShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    PlaneShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    PlaneShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

    //pointLights
    for (int i = 0; i < countOfPointLights; i++) {
        string num = int_to_string(i);
        PlaneShader.setVec3("pointLights[" + num + "].position", PointLightPositions[i]);
        PlaneShader.setVec3("pointLights[" + num + "].ambient", 0.05f, 0.05f, 0.05f);
        PlaneShader.setVec3("pointLights[" + num + "].diffuse", 0.8f, 0.8f, 0.8f);
        PlaneShader.setVec3("pointLights[" + num + "].specular", 1.0f, 1.0f, 1.0f);
        PlaneShader.setFloat("pointLights[" + num + "].constant", 1.0f);
        PlaneShader.setFloat("pointLights[" + num + "].linear", 0.09);
        PlaneShader.setFloat("pointLights[" + num + "].quadratic", 0.032);

    }


    // spotLight
    PlaneShader.setVec3("spotLight.position", camera.Position);
    PlaneShader.setVec3("spotLight.direction", camera.Front);
    PlaneShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    PlaneShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    PlaneShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    PlaneShader.setFloat("spotLight.constant", 1.0f);
    PlaneShader.setFloat("spotLight.linear", 0.09);
    PlaneShader.setFloat("spotLight.quadratic", 0.032);
    PlaneShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    PlaneShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

    PlaneShader.setMat4("projection", projection);
    PlaneShader.setMat4("view", view);
    Plane.draw();


    LampShad.Use();
    LampShad.setMat4("projection", projection);
    LampShad.setMat4("view", view);
    for (int i = 0; i < countOfPointLights; i++) {
        lamp.UpdateModel();
        lamp.Trans(PointLightPositions[i]);
        lamp.ScaleModel(vec3(-0.5f, -0.5f, -0.5f));
        lamp.Draw();
    }
}

//Объекты Тухватуллиной
    Shader FloorShader;
    Shader FloorShaderNonBump;
    Quad SceneFloor;
    Model bench;
    Model tumb;
    Model sofa1;
    Model flower;
    Model TV;
    Model table;
    Model dog;
    Model lampModel;
    Model Crunch;
    Model weight;
//До сюда
//void MyTrigger(Model m);
    
void InitTukhvatullinaSCENEObjects() {
    FloorShader=Shader("model2.vs", "model2.fs");
    FloorShaderNonBump = Shader("floor.vs", "floor.fs");

    material SceneFloorMtl;
    SceneFloorMtl.diffuse = Texture("Content\\textures\\floor5.jpg");
    SceneFloorMtl.specular = Texture("Content\\textures\\floor5_spec.png");
    SceneFloorMtl.normal = Texture("Content\\textures\\floor5_NRM.png");
    SceneFloorMtl.shininess = 40.0f;

    SceneFloor = Quad(vec3(0, 0, 0), 100, 100, SceneFloorMtl, FloorShader);
    SceneFloor.Rotate(-90, vec3(1, 0, 0));

    //Model some("", vec3(0, 0, 0), FloorShader);
    
    bench= Model("Content\\Objects\\Bench\\goya_bench3.obj", vec3(10.0f, 0, -23.0f), FloorShaderNonBump);
    tumb= Model("Content\\Objects\\tumb\\tumb.obj", vec3(7.8f, 0.0f, -10.4f), FloorShaderNonBump);
    sofa1= Model("Content\\Objects\\Sofa1\\Santorini2.obj", vec3(-9.0f, 0.0f, -20.0f), FloorShaderNonBump);
    flower= Model("Content\\Objects\\alocasia\\flower2.obj", vec3(-9.93f, 0, -4.248f), FloorShaderNonBump);
    TV= Model("Content\\Objects\\TV\\MyTV2.obj", vec3(0, 2.9f, -6.0f), FloorShader);
    table= Model("Content\\Objects\\Table\\Wood_Table.obj", vec3(-5.2f, 0, -20.0f), FloorShader);
    dog= Model("Content\\Objects\\Dog\\dog.obj", vec3(-12.9f, 0, -10.92f), FloorShaderNonBump);
    lampModel = Model("Content\\Objects\\lamp\\lamp2.obj", vec3(-27.2f, 0, -23.7f), FloorShader);
    Crunch= Model("Content\\Objects\\Crunch\\CrunchBox.obj", vec3(-4.4f, 3.6f, -19.2f), FloorShaderNonBump);
    weight=Model("Content\\Objects\\Weight\\weight.obj", vec3(-2.6f, 0, -27.9f), FloorShaderNonBump);

    TV.Rotate(-164.0f, vec3(0, 1, 0));

    bench.Scale(vec3(1.5f, 1.5f, 1.5f));
    bench.Rotate(67.0f, vec3(0, 1, 0));

    tumb.Scale(vec3(1.3f, 1.3f, 1.3f));
    tumb.Rotate(-170.0f, vec3(0, 1.0f, 0));
    //tumb.trans(vec3(10.0f, 0.0f, -5.0f));

    sofa1.Scale(vec3(0.0062f, 0.0062f, 0.0062f));
    sofa1.Rotate(-90.0f, vec3(1.0f, 0.0f, 0.0f));
    sofa1.Rotate(23.0f, vec3(0.0f, 0.0f, 1.0f));

    flower.Scale(vec3(0.006f, 0.006f, 0.006f));

    table.Scale(vec3(7.0f, 7.0f, 7.0f));
    table.Rotate(20, vec3(0, 1.0f, 0));

    dog.Rotate(-90, vec3(1.0f, 0, 0));
    dog.Rotate(-123.0f, vec3(0, 0, 1.0f));
    dog.Scale(vec3(0.1f, 0.1f, 0.1f));

    lampModel.Scale(vec3(0.25f, 0.25f, 0.25f));

    Crunch.Scale(vec3(1.2f));
    Crunch.Rotate(81, vec3(0, 1, 0));

    weight.Scale(vec3(0.1f));

}
void TukhvatullinaScene(glm::mat4 projection, glm::mat4 view) {
    FloorShader.Use();
    FloorShader.setVec3("viewPos", camera.Position);
    FloorShader.setFloat("material.shininess", 32.0f);

    // directional light
    FloorShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    FloorShader.setVec3("dirLight.ambient", 0.1f, 0.1f, 0.1f);
    FloorShader.setVec3("dirLight.diffuse", 0.6f, 0.6f, 0.6f);
    FloorShader.setVec3("dirLight.specular", 0.2f, 0.2f, 0.2f);

    //pointLights
    FloorShader.setVec3("pointLights.position", vec3(-27.2f, 9.0f, -23.7f));
    FloorShader.setVec3("pointLights.ambient", 0.1f, 0.1f, 0.1f);
    FloorShader.setVec3("pointLights.diffuse", 1.0f, 0.8157f, 0.1098f);
    FloorShader.setVec3("pointLights.specular", 0.8f, 0.8f, 0.8f);
    FloorShader.setFloat("pointLights.constant", 1.0f);
    FloorShader.setFloat("pointLights.linear", 0.009f);
    FloorShader.setFloat("pointLights.quadratic", 0.005f);

    FloorShader.setMat4("projection", projection);
    FloorShader.setMat4("view", view);

    SceneFloor.draw();
    
    TV.Draw();
    table.Draw();
    lampModel.Draw();


    //БЕЗ НОРМАЛЕЙ
    FloorShaderNonBump.Use();
    FloorShaderNonBump.setVec3("viewPos", camera.Position);
    FloorShaderNonBump.setFloat("material.shininess", 32.0f);

    // directional light
    FloorShaderNonBump.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    FloorShaderNonBump.setVec3("dirLight.ambient", 0.1f, 0.1f, 0.1f);
    FloorShaderNonBump.setVec3("dirLight.diffuse", 0.6f, 0.6f, 0.6f);
    FloorShaderNonBump.setVec3("dirLight.specular", 0.2f, 0.2f, 0.2f);

    //pointLights
    FloorShaderNonBump.setVec3("pointLights.position", vec3(-27.2f, 9.0f, -23.7f));
    FloorShaderNonBump.setVec3("pointLights.ambient", 0.1f, 0.1f, 0.1f);
    FloorShaderNonBump.setVec3("pointLights.diffuse", 1.0f, 0.8157f, 0.1098f);
    FloorShaderNonBump.setVec3("pointLights.specular", 0.8f, 0.8f, 0.8f);
    FloorShaderNonBump.setFloat("pointLights.constant", 1.0f);
    FloorShaderNonBump.setFloat("pointLights.linear", 0.009f);
    FloorShaderNonBump.setFloat("pointLights.quadratic", 0.005f);

    FloorShaderNonBump.setMat4("projection", projection);
    FloorShaderNonBump.setMat4("view", view);

    bench.Draw();
    tumb.Draw();
    sofa1.Draw();
    flower.Draw();
    dog.Draw();
    Crunch.Draw();
    weight.Draw();
    moveModel(flower, 5.0f);
}

//Объекты Хароисова
    Shader ModelShader;
    Quad ScenePlane;
    Model Build;
    Model Build2;
    Model Build3;
    Model Build4;
    Model Build5;
    //Model tree;
    Model Alucy;
    //Model Dodge;
    //Model Helicopter;
    Model streetLamp;
    Model some;
    Model StopSign;
    Model StreetBench;
//До сюда
    
void initKharisovObjects() {
    ModelShader=Shader("model2.vs", "model2.fs");
    Shader ScenePlaneShader = ModelShader;
    ObjPosition = vec3(0.0f);

    material ScenePlaneMaterial;
    ScenePlaneMaterial.diffuse = Texture("Content\\textures\\brick2.jpg");
    ScenePlaneMaterial.specular = Texture("Content\\textures\\brick2_spec.png");
    ScenePlaneMaterial.normal = Texture("Content\\textures\\brick2_NRM.png");
    ScenePlaneMaterial.shininess = 32.0f;


    ScenePlane = Quad(vec3(0, 0, 0), 100, 100, ScenePlaneMaterial, ModelShader);
    ScenePlane.Rotate(-90, vec3(1, 0, 0));

    Build=Model("Content\\Objects\\Buildings\\Buildings\\Build1_3_2.obj", vec3(0), ModelShader);

    Build2 = Model("Content\\Objects\\Buildings\\Buildings\\Build2.obj", vec3(17.9f, 0, -2.1f), ModelShader);
    
    Build3 = Model("Content\\Objects\\Buildings\\Buildings\\Build3.obj", vec3(-18.3f, 0, -0.6f), ModelShader);

    Build4 = Model("Content\\Objects\\Buildings\\Buildings\\Build4.obj", vec3(-6.8f, 0, -36.2f), ModelShader);
    Build4.Rotate(179, vec3(0, 1, 0));

    Build5 = Model("Content\\Objects\\Buildings\\Buildings\\Build5.obj", vec3(18.5f, 0, -37.5f), ModelShader);
    Build5.Rotate(181, vec3(0, 1, 0));

    //tree = Model("Content\\Objects\\tree\\Tree.obj", vec3(4.2f, 0, -15.2f), ModelShader);

    Alucy = Model("Content\\Objects\\Alucy\\Alucy2.obj", vec3(32.7f, 0, -26.7), ModelShader);
    Alucy.Scale(vec3(0.2f));
    Alucy.Rotate(67, vec3(0, 1, 0));

    //Dodge = Model("Content\\Objects\\Cars\\Dodge\\Dodge Challenger SRT8.obj", vec3(4.6f, 0.0f, -19.19f), ModelShader);
    //Dodge.Scale(vec3(0.001f));
    //Dodge.Rotate(-90.0f, vec3(1.0f, 0.0f, 0.0f));
    //Dodge.Rotate(23.0f, vec3(0.0f, 0.0f, 1.0f));

    streetLamp = Model("Content\\Objects\\streetLight\\streetlamp.obj", vec3(4.8f, 0, -25.8f), ModelShader);
    streetLamp.Rotate(352, vec3(0, 1, 0));
    streetLamp.Scale(vec3(0.5f));

    StopSign = Model("Content\\Objects\\StopSign\\stopsign1.obj", vec3(8.0f,0.0f,-23.3f), ModelShader);
    StopSign.Rotate(267, vec3(0, 1, 0));
    StopSign.Scale(vec3(0.75));

    StreetBench = Model("Content\\Objects\\StreetBench\\streetBench.obj", vec3(8.784f,0.0f,-12.624f), ModelShader);
    StreetBench.Rotate(166, vec3(0, 1, 0));
    StreetBench.Scale(vec3(0.08));

}
void KharisovScene(mat4 projection, mat4 view) {
    ModelShader.Use();
    ModelShader.setVec3("viewPos", camera.Position);

    // directional light
    ModelShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    ModelShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
    ModelShader.setVec3("dirLight.diffuse", 0.7f, 0.7f, 0.7f);
    ModelShader.setVec3("dirLight.specular", 0.2f, 0.2f, 0.2f);

    //pointLights
    ModelShader.setVec3("pointLights.position", vec3(5.3f, 4.8f, -27.1f));
    ModelShader.setVec3("pointLights.ambient", 0.1f, 0.09f, 0.03f);
    ModelShader.setVec3("pointLights.diffuse", 1.0f, 0.898f, 0.3608f);
    ModelShader.setVec3("pointLights.specular", 0.8f, 0.8f, 0.8f);
    ModelShader.setFloat("pointLights.constant", 1.0f);
    ModelShader.setFloat("pointLights.linear", 0.009f);
    ModelShader.setFloat("pointLights.quadratic", 0.005f);

    ModelShader.setMat4("projection", projection);
    ModelShader.setMat4("view", view);

    ScenePlane.draw();

    Build.Draw();
    Build2.Draw();
    Build3.Draw();
    Build4.Draw();
    Build5.Draw();
    //tree.Draw();
    //Dodge.Draw();
    streetLamp.Draw();

    Alucy.Draw();
    StopSign.Draw();
    StreetBench.Draw();

    moveModel(StopSign, 0.1f);
}



bool keys[1024];//отслеживание нажатия кнопок
void do_movement()
{
    
    // Camera controls
    if (keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (keys[GLFW_KEY_LEFT_SHIFT])
        camera.MovementSpeed = 10.0f;
    else
        camera.MovementSpeed = 3.0f;
    if (keys[GLFW_KEY_1]) {
        cout << "Scene 1" << endl;
        sceneNum = 1;
        camera.SetCamPosition(vec3(6.0f, 3.0f, 7.0f));
    }
    if (keys[GLFW_KEY_2]) {
        cout << "Scene 2" << endl;
        sceneNum = 2;
        camera.SetCamPosition(vec3(-20.0f, 7.0f, -14.0f));
    }
    if (keys[GLFW_KEY_3]) {
        cout << "Scene 3" << endl;
        sceneNum = 3;
        camera.SetCamPosition(vec3(5.9f, 5.8f, -27.0f));
    }

    if (keys[GLFW_KEY_Q]) {
        if (exposure > 0.0f) {
            exposure -= 0.001f;
            cout << "hdr: " << (hdr ? "on" : "off") << "| exposure: " << exposure << "| gamma: " << gamma << endl;
        }
        else
            exposure = 0.0f;
    }

    if (keys[GLFW_KEY_E]) {
        exposure += 0.001f;
        cout << "hdr: " << (hdr ? "on" : "off") << "| exposure: " << exposure << "| gamma: " << gamma << endl;
    }

    if (keys[GLFW_KEY_Z]) {
        if (gamma > 0.0f) {
            gamma -= 0.001f;
            cout << "hdr: " << (hdr ? "on" : "off") << "| exposure: " << exposure << "| gamma: "<<gamma<< endl;
        }
        else
            gamma = 0.0f;
    }
    if (keys[GLFW_KEY_X]) {
        gamma += 0.001f;
        cout << "hdr: " << (hdr ? "on" : "off") << "| exposure: " << exposure << "| gamma: " << gamma << endl;
    }

   
}

int ModelDir = 0;
void moveModel(Model& someModel, float sc) {
    if (keys[GLFW_KEY_KP_8]) {
        ObjPosition.x += 1.0f * sc;
        someModel.trans(vec3(1.0f * sc, 0, 0));
        cout << "Key 8 PRESSED" << endl;
    }
    if (keys[GLFW_KEY_KP_2]) {
        ObjPosition.x -= 1.0f * sc;
        someModel.trans(vec3(-1.0f * sc, 0, 0));
        cout << "Key 2 PRESSED" << endl;
    }
    if (keys[GLFW_KEY_KP_6]) {
        ObjPosition.z += 1.0f * sc;
        someModel.trans(vec3(0, 0, 1.0f * sc));
        cout << "Key 6 PRESSED" << endl;
    }
    if (keys[GLFW_KEY_KP_4]) {
        ObjPosition.z -= 1.0f * sc;
        someModel.trans(vec3(0, 0, -1.0f * sc));
        cout << "Key 4 PRESSED" << endl;
    }
    if (keys[GLFW_KEY_ENTER]) {
        cout << ObjPosition.x << "  " << ObjPosition.y << "  " << ObjPosition.z << endl;
        cout << ModelDir << endl;
    }
    if (keys[GLFW_KEY_KP_ADD]) {
        someModel.Rotate(1, vec3(0, 1.0f * sc, 0));
        ModelDir += 1;
        cout << "Key + PRESSED" << endl;
    }
    if (keys[GLFW_KEY_KP_SUBTRACT]) {
        someModel.Rotate(-1, vec3(0, 1.0f * sc, 0));
        ModelDir -= 1;
        cout << "Key - PRESSED" << endl;
    }
    if (keys[GLFW_KEY_UP]) {
        ObjPosition.y += 1.0f * sc;
        someModel.trans(vec3(0, 1.0f * sc, 0));
        cout << "Key UP PRESSED" << endl;
    }
    if (keys[GLFW_KEY_DOWN]) {
        ObjPosition.y -= 1.0f * sc;
        someModel.trans(vec3(0, -1.0f * sc, 0));
        cout << "Key DOWN PRESSED" << endl;
    }
}

bool mouseset = false;
void Keyboards(GLFWwindow* window, int key, int scancode, int action, int mode) {
    // Когда пользователь нажимает ESC, мы устанавливаем свойство WindowShouldClose в true, 
    // и приложение после этого закроется
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        if (!mouseset) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//Захват мыши
            mouseset = true;
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);//Захват мыши
            mouseset = false;
        }
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS && !hdrKeyPressed) {
        hdr = !hdr;
        hdrKeyPressed = true;
        cout << "hdr: " << (hdr ? "on" : "off") << "| exposure: " << exposure << "| gamma: " << gamma << endl;
    }

    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
        hdrKeyPressed = false;
    }




    if (action == GLFW_PRESS)
        keys[key] = true;
    else if (action == GLFW_RELEASE)
        keys[key] = false;
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}


void MyTrigger(Model m) {
    if (camera.Position.x <= m.position.x + 5 && camera.Position.x >= m.position.x - 5 &&
        camera.Position.z < m.position.z + 5 && camera.Position.z >= m.position.z - 5 &&
        camera.Position.y < m.position.y + 10 && camera.Position.y>0) {
        m.Rotate(0.1, vec3(0, 1, 0));
    }
}




int main()
{

    ObjPosition = vec3(0, 0, 0);
    srand(static_cast<unsigned int>(time(0)));
    
    //Тут GLFW
    glfwInit();//инициализация glfw

    //Задается минимальная требуемая версия OpenGL (минор и мажор)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //Установка профайла для которого создается контекст
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);//Возможность изменение окна (TRUE/FALSE)

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "lab2", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, Keyboards);//нажатия кнопок
    glfwSetCursorPosCallback(window, mouse_callback);//Движение мыши
    glfwSetScrollCallback(window, scroll_callback);//Колесико

    //ТУТ GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    //даем понять glew размеры окна glfw
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);



    //инициализация объектов
    InitAnimObjects();
    InitStaticObjects();

    //ПЕРЕКЛЮЧИТЬ ДЛЯ ВЫБОРА СЦЕНЫ
    initKharisovObjects();
    //InitTukhvatullinaSCENEObjects();

    //ВЫВОД СЦЕНЫ В ТЕКСТУРУ
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    // screen quad VAO
    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


    Shader screenShader("framebuffers_screen.vs", "framebuffers_screen.fs");
    screenShader.Use();
    screenShader.setInt("screenTexture", 0);


    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    // создание текстурного объекта
    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
    // присоедиение текстуры к объекту текущего кадрового буфера
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WIDTH, HEIGHT);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        do_movement();
        
        //прикрепляем фреймбуффер
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glEnable(GL_DEPTH_TEST); // тест глубины

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Параметры камеры
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        
        switch (sceneNum) {
        case 1: {
            ANIMSCENE(projection, view);
            break;
        }
        case 2: {
            STATICSCENE(projection, view);
            break;
        }
        case 3: {
            //ПЕРЕКЛЮЧИТЬ ДЛЯ ВЫБОРА СЦЕНЫ

            KharisovScene(projection, view);
            //TukhvatullinaScene(projection, view);
            break;
        }
        }
        

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST); 
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT);

        screenShader.Use();
        glBindVertexArray(quadVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane

        screenShader.setInt("hdr", hdr);
        screenShader.setFloat("exposure", exposure);
        screenShader.setFloat("gamma", gamma);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
    
}
