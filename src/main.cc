#include "Puddi.h"
#include "LightSource.h"
#include "DrawableObject.h"
#include "Texture.h"
#include "Skybox.h"
#include "Font.h"
#include "Util.h"
#include "TerrainVertexMesh.h"
#include "HeightMapTerrain.h"
#include "Shadow.h"
#include "RenderGraph.h"
#include "Schematic.h"
#include "SourceCode.h"
#include "Token.h"
#include "Lexer.h"
#include "AST.h"
#include "SyntaxParser.h"
#include "TypeChecker.h"
#include "GrumpyConfig.h"
#include "TokenQueue.h"
#include "RTL.h"
#include "RTLCompiler.h"
#include "Camera.h"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <queue>

using namespace puddi;
using namespace grumpy;
using namespace std;

LightSource *lightSource;
Object *objectContainer;
DrawableObject *cube;
DrawableObject *rect;
DrawableObject *wall;

SourceCode *sourceCode;
Lexer *lexer;
TokenQueue *tokenQueue;
ASTNode *ast;
SyntaxParser *parser;
TypeChecker *typeChecker;
RTLProgram *rtlProgram;
RTLCompiler *rtlCompiler;

string sourceFile, tokenFile, astFile, rtlFile, rtlAstMapFile;

void reset();

void init(void)
{
    engine::AddRenderGraph();
    engine::AddRenderGraph();
    engine::AddRenderGraph();

    GLuint texture = Texture::LoadTexture("grumpycat", "textures/grumpycat1.png", "bumpmaps/grumpycat1_medium.png");
    //GLuint texture = Texture::LoadTexture("grumpycat", "textures/grumpycat1_d.png");
    //GLuint texture = Texture::LoadTexture("rough", "textures/rough4.jpg", "bumpmaps/rough4_bumpmap_fine.jpg");
    Texture::LoadBumpMap("rough1", "bumpmaps/rough1_bumpmap_fine.jpg");
    Texture::LoadBumpMap("rough2", "bumpmaps/rough2_bumpmap_fine.jpg");
    Texture::LoadBumpMap("rough3", "bumpmaps/rough3_bumpmap_fine.jpg");
    Texture::LoadBumpMap("rough5", "bumpmaps/rough5_bumpmap_fine.jpg");
    Texture::LoadBumpMap("rough4", "bumpmaps/rough4_bumpmap_fine.jpg");
    //GLuint bump = Texture::LoadBumpMap("rough", "bumpmaps/Shrek-and-Yoda_NRM.jpg");
    Texture::LoadCubeMap("skybox_2", "textures/skybox2.jpg");
    //GLuint texture = Texture::LoadTexture("shrek", "textures/Shrek-and-Yoda (copy).jpg");
    //GLuint texture = Texture::CreateTextureFromString("bakow", "bakow_string", "fonts/Bitstream/VeraMono-Bold.ttf", 24, vec3(1.0f, 0.0f, 0.0f));

    Schematic::LoadSchematic("models/rounded.obj", "pill");
    Schematic::LoadSchematic("models/cube rounded - 554 faces.obj", "rounded_cube");
    Schematic::LoadSchematic("models/grumpycat.obj", "grumpycat");
    Schematic::LoadSchematic("models/R2-D2.obj", "r2d2");

    engine::MainCamera->SetPosition(vec4(0.0f, -5.0f, 0.0f, 1.0f));
    engine::MainCamera->LookAt(vec4(0.0f, 0.0f, 0.0f, 1.0f));
    //engine::MainCamera->SetZoomOut(10.0f);

    lightSource = LightSource::ObtainLightSource();
    lightSource->ambient = vec4(1.0, 1.0, 1.0, 1.0);
    lightSource->diffuse = vec4(1.0, 1.0, 1.0, 1.0);
    lightSource->specular = vec4(1.0, 1.0, 1.0, 1.0);
    lightSource->position = vec4(-250.0f, -500.0f, 500.0f, 0.0f);
    lightSource->UpdateMatrix();
    LightSource::UpdateLightSourceMatricesInShaders();

    //wall = new Cube(engine::GetRootObject());
    //wall = new DrawableObject(engine::GetRootObject(), Schematic::GetSchematicByName("geodesic_weave"));
    //wall->RotateX(static_cast<float>(M_PI / 2.0f));
    //wall->SetScaleX(200.0f);
    //wall->SetScaleY(100.0f);
    //wall->SetScale(10.0f);
    //wall->SetPosition(vec4(0.0f, 10.f, 0.0f, 1.0f));
    //wall->SetMaterial(Material::Plastic(vec4(0.5f, 0.1f, 0.5f, 1.0f)));
    //wall->SetMaterial(Material::Vibrant(vec4(0.5f, 0.1f, 0.5f, 1.0f)));
    //wall->SetMaterial(Material::Medium(vec4(0.5f, 0.1f, 0.5f, 1.0f)));
    //wall->SetMaterial(Material::BlackRubber());
    //wall->SetBumpMap(bump);
    //wall->SetTexture(texture);

//    auto terrainMesh = new TerrainVertexMesh(HeightMapTerrain::CreateTerrainMeshFromFile("textures/ocaml_logo_2.png", 25.0f, 25.0f, 0.1f));
//    //auto terrainMesh = new TerrainVertexMesh(HeightMapTerrain::CreateTerrainMeshFromFile("textures/ou_logo_1.png", 25.0f, 25.0f, 0.1f));
//    Texture::LoadTexture("sand1", "textures/sand1.jpg", "bumpmaps/sand1_NRM.jpg");
//    Texture::LoadTexture("grass1", "textures/grass1.jpg", "bumpmaps/grass1_NRM.jpg");
//    Texture::LoadTexture("stone1", "textures/stone1.jpg", "bumpmaps/stone1_NRM.jpg");
//    Texture::LoadTexture("snow1", "textures/snow1.jpg", "bumpmaps/snow1_NRM.jpg");
//    terrainMesh->SetTexture1(Texture::GetTextureByName("sand1"));
//    terrainMesh->SetTexture2(Texture::GetTextureByName("grass1"));
//    terrainMesh->SetTexture3(Texture::GetTextureByName("stone1"));
//    terrainMesh->SetTexture4(Texture::GetTextureByName("snow1"));
//    terrainMesh->SetTextureHeights(vec4(0.0f, 0.3f, 0.5f, 0.9f));
//    //DrawableObject *terrainContainer = new DrawableObject(engine::GetRootObject(), terrainMesh);
//    //terrainContainer->Scale(0.01f);

//    auto terrain = new DrawableObject(engine::GetRootObject());
//    terrain->AddVertexMesh(terrainMesh);
//    terrain->SetScale(0.1f);
//    terrain->SetScaleX(0.25f);
//    terrain->SetScaleY(0.25f);
//    terrain->RotateZ(M_PI / 2.0f);
//    //terrain->Translate(vec4(-1000.0f, -1000.0f, -1000.0f, 0.0f));
//    //terrain->Translate(vec4(-100.0f, -100.0f, -100.0f, 0.0f));
//    //terrain->Translate(vec4(-20.0f, -125.0f, -125.0f, 0.0f));
//    terrain->Translate(vec4(0.0f, 150.0f, -125.0f, 0.0f));

//    for (int i = 1; i < 1; ++i)
//    {
//        for (int j = 0; j < 1; ++j)
//        {
//            auto terrain = new DrawableObject(engine::GetRootObject());
//            terrain->AddVertexMesh(new TerrainVertexMesh(*terrainMesh));
//            terrain->SetScale(1.0f);
//            //terrain->Translate(vec4(-1000.0f, -1000.0f, -1000.0f, 0.0f));
//            terrain->Translate(vec4(-100.0f, -100.0f, -100.0f, 0.0f));
//            terrain->Translate(vec4(-254.0f * i, -254.0f * j, 0.0f, 0.0f));
//            //terrain->Translate(vec4(-1024.0f * i, -1024.0f * j, 0.0f, 0.0f));
//        }
//    }
//
//    terrain = new DrawableObject(engine::GetRootObject());
//    terrain->AddVertexMesh(new TerrainVertexMesh(*terrainMesh));
//    terrain->SetScale(1.0f);
//    //terrain->Translate(vec4(-1000.0f, -1000.0f, -1000.0f, 0.0f));
//    terrain->Translate(vec4(-100.0f, -100.0f, -100.0f, 0.0f));
//    terrain->Translate(vec4(0.0f, -254.0f, 0.0f, 0.0f));
//
//    terrain = new DrawableObject(engine::GetRootObject());
//    terrain->AddVertexMesh(new TerrainVertexMesh(*terrainMesh));
//    terrain->SetScale(1.0f);
//    //terrain->Translate(vec4(-1000.0f, -1000.0f, -1000.0f, 0.0f));
//    terrain->Translate(vec4(-100.0f, -100.0f, -100.0f, 0.0f));
//    terrain->Translate(vec4(-254.0f, 0.0f, 0.0f, 0.0f));

    // CONTAINERS
    objectContainer = new Object(engine::GetRootObject());

    Skybox *skybox = new Skybox(engine::MainCamera);
    skybox->SetCubeMap(Texture::GetCubeMapByName("skybox_2"));
    skybox->SetEmissive(true);
    skybox->SetScale(engine::ViewDistance);
    skybox->DisableShadowCasting();

//    DrawableObject *cat = new DrawableObject(engine::GetRootObject(), Schematic::GetSchematicByName("grumpycat"));
//    cat->SetScale(0.5f);
//    cat->Translate(vec4(0.0f, 0.0f, 5.0f, 0.0f));

    // OBJECTS
    //rect = new Rectangle(objectContainer);
    //rect->SetTexture(texture);
    //rect->RotateX(M_PI / 2.0f);

    // MIDDLE CUBE
//    cube = new DrawableObject(objectContainer, Schematic::GetSchematicByName("cube"));
//    //cube->SetTexture(texture);
//    //cube->SetScale(100);
//    cube->Translate(vec4(-50.0f, 0.0f, 0.0f, 0.0f));
//    cube->Scale(0.01f);

    //DrawableObject *object = new DrawableObject(objectContainer, VertexMesh::GetVertexMeshPrototypeByName("a"));
    ////DrawableObject *object = new Rectangle(objectContainer);
    //object->SetTexture(Texture::GetTextureByName("myfont"));
    //object->SetBumpMapEnabled(false);
    //object->RotateX(M_PI / 2.0f);

    //int rows = 10;
    //int cols = 10;
    //for (int i = 0; i < rows; ++i)
    //{
        //for (int j = 0; j < cols; ++j)
        //{
            //DrawableObject *object = new DrawableObject(objectContainer, VertexMesh::GetVertexMeshPrototypeByName(std::string(1, i * cols + j + 32)));
            //object->SetTexture(Texture::GetTextureByName("myfont"));
            //object->RotateX(M_PI / 2.0f);
            //object->Translate(vec4(j, 0.0f, -i, 0.0f));
        //}
    //}

    reset();
}

void reset()
{
    delete sourceCode;
    delete lexer;
    delete tokenQueue;
    delete ast;
    delete parser;
    delete typeChecker;
    delete rtlProgram;
    delete rtlCompiler;

    sourceCode = new SourceCode(engine::GetRootObject(), sourceFile, "myfont");
    sourceCode->Translate(vec4(0.0f, -30.0f, 0.0f, 0.0f));

    vector<LexToken> lTokens;
    ifstream infile(tokenFile);
    LexToken tok;
    int count = 0;
    while (infile >> tok.name >> tok.start >> tok.end)
    {
        tok.number = count++;
        tok.value = "";
        tok.end--;

        size_t lparen = tok.name.find('(');
        if (lparen != string::npos)
        {
        }

        lTokens.push_back(tok);

        //cout << tok.name << endl;
    }
    infile.close();

    tokenQueue = new TokenQueue(engine::GetRootObject());
    tokenQueue->SetPosition(sourceCode->GetPosition() + vec4(5.0f, 0.0f, 2.0f, 0.0f));

    lexer = new Lexer(engine::GetRootObject(), sourceCode, lTokens, tokenQueue, Schematic::GetSchematicByName("r2d2"));
    //lexer->BuildFromSchematic(Schematic::GetSchematicByName("grumpycat"));
    //lexer->SetScale(0.05f);
    //auto *mesh = new VertexMesh(VertexMesh::GetVertexMeshPrototypeByName("cube"));
    //auto *mesh = new VertexMesh(VertexMesh::GetVertexMeshPrototypeByName("sphere"));
    //lexer->AddVertexMesh(mesh);
    //lexer->SetTexture(Texture::GetTextureByName("grumpycat"));
    lexer->SetSkipVelocity(0.1f);
    lexer->SetReadVelocity(0.01f);
    //lexer->Translate(vec4(0.0f, -10.0f, 0.0f, 0.0f));
    lexer->RotateX(-M_PI / 2.0f);

    auto bytes = Util::ReadAllBytes(astFile);
    vector<char> sanitizedBytes;
    bool prevCharWasSpace = false;
    for (size_t i = 0; i < bytes.size(); ++i)
    {
        if (bytes[i] == ' ')
        {
            if (prevCharWasSpace)
                continue;
            prevCharWasSpace = true;
        }
        else
            prevCharWasSpace = false;

        sanitizedBytes.push_back(bytes[i]);

        //cout << bytes[i];;
    }

    queue<char> q;

    for (size_t i = 0; i < sanitizedBytes.size(); ++i)
        q.push(sanitizedBytes[i]);

    ast = new ASTNode(engine::GetRootObject(), nullptr, nullptr, q);
    //ast->Translate(vec4(ast->GetWidth() / 2.0f, 0.0f, 0.0f, 0.0f));
    ast->SetScaleX(0.5f);
    //ast->SetPosition(vec4(30.0f, 0.0f, 0.0f, 1.0f));
    //ast->SetAssignedLocation(vec4(30.0f, 0.0f, 0.0f, 1.0f));
    //ast->Hide();

    /*parser = new SyntaxParser(engine::GetRootObject(), ast);
    parser->AddVertexMesh(new VertexMesh(VertexMesh::GetVertexMeshPrototypeByName("cube")));*/
    parser = new SyntaxParser(engine::GetRootObject(), lexer, tokenQueue, ast, Schematic::GetSchematicByName("rounded_cube"));
    parser->SetTexture(Texture::GetTextureByName("grumpycat"));
//    parser->SetEmissive(true);
//    parser->SetEmissionColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
    parser->SetVelocity(0.025f);
    //parser->DisableRender();
    parser->SetPosition(ast->GetPosition() + vec4(0.0f, 0.0f, 5.0f, 1.0f));
    parser->SetHomePosition(parser->GetPosition());

    //for (auto it = lTokens.begin(); it != lTokens.end(); ++it)
    //    parser->AddToken(new Token(engine::GetRootObject(), *it));
//    for (int i = 0; i < lTokens.size() / 2; ++i)
//        parser->AddToken(new Token(engine::GetRootObject(), lTokens[i]));

    lexer->SetParser(parser);

    typeChecker = new TypeChecker(engine::GetRootObject(), ast, Schematic::GetSchematicByName("rounded_cube"));
    typeChecker->SetMaterial(Material::Medium(vec4(0.5f, 0.1f, 0.5f, 1.0f)));
    typeChecker->SetVelocity(0.025f);
    typeChecker->SetPosition(ast->GetPosition() + vec4(0.0f, 0.0f, 7.0f, 1.0f));
    typeChecker->SetHomePosition(typeChecker->GetPosition());
    typeChecker->SetBumpMap(Texture::GetBumpMapByName("rough1"));

    parser->SetTypeChecker(typeChecker);

    rtlProgram = new RTLProgram(engine::GetRootObject(), "myfont", rtlFile);
    rtlProgram->Translate(vec4(0.0f, 25.0f, 0.0f, 0.0f));

    rtlCompiler = new RTLCompiler(engine::GetRootObject(), ast, rtlProgram, 0.01f, rtlAstMapFile, Schematic::GetSchematicByName("rounded_cube"));
    rtlCompiler->SetMaterial(Material::Medium(vec4(0.3f, 0.6f, 0.25f, 1.0f)));
    rtlCompiler->SetPosition(ast->GetPosition() + vec4(0.0f, 0.0f, 9.0f, 1.0f));
    rtlCompiler->SetHomePosition(rtlCompiler->GetPosition());

    typeChecker->SetRTLCompiler(rtlCompiler);
}

//----------------------------------------------------------------------------

bool holdingMouseClick = false;
int update()
{
    SDL_Event ev;
    while (SDL_PollEvent(&ev))
    {
        // send event to camera
        engine::MainCamera->InputEvent(ev);

        // quit
        if (ev.type == SDL_QUIT)
        {
            return 1;
        }
        // key press
        else if (ev.type == SDL_KEYDOWN)
        {
            switch (ev.key.keysym.sym)
            {
            // EXIT PROGRAM
            case SDLK_ESCAPE:
                return 1;
                break;
            // ENABLE FULLSCREEN
            case SDLK_f:
                engine::ToggleFullScreen();
                break;
            /*case SDLK_1:
                parser->AddTokenToQueue(new Token(engine::GetRootObject(), LexToken()));
                break;*/
            case SDLK_r:
                reset();
                break;
            case SDLK_MINUS:
            case SDLK_UNDERSCORE:
                GrumpyConfig::SetGameSpeed(std::max(0.0f, GrumpyConfig::GetGameSpeed() - 0.1f));
                break;
            case SDLK_PLUS:
            case SDLK_EQUALS:
                GrumpyConfig::SetGameSpeed(GrumpyConfig::GetGameSpeed() + 0.1f);
                break;
            }
        }
        // mouse click
        else if (ev.type == SDL_MOUSEBUTTONDOWN)
        {
            holdingMouseClick = true;
        }
        // mouse release
        else if (ev.type == SDL_MOUSEBUTTONUP)
        {
            holdingMouseClick = false;
        }
        // mouse motion
        else if (ev.type == SDL_MOUSEMOTION)
        {
            //
        }
        // window event
        else if (ev.type == SDL_WINDOWEVENT)
        {
            engine::UpdateProjectionMatrixAndViewport();
        }
    }
    if (cube != nullptr)
        cube->RotateZ(1.0f / 2000.0f * GrumpyConfig::GetGameSpeedFactor());

    //rect->RotateZ(1.0f / 2000.0f * GrumpyConfig::GetGameSpeedFactor());

    return 0;
}

void enableShadows()
{
    engine::EnableShadows(SHADOW_MODE_UNI, SHADOW_RESOLUTION_EXTRA_HIGH);
    // overwrite z range to give better shadow precision
    Shadow::SetZRange(100.0f, engine::ViewDistance);

    engine::SetShadowLightPosition(vec3(lightSource->position));
}

int main(int argc, char **argv)
{
    if (int initStatus = engine::Init(1000.0f) != 0)
        return initStatus;

    if (argc >= 4)
    {
        sourceFile = argv[1];
        tokenFile = argv[2];
        astFile = argv[3];
        rtlFile = argv[4];
        rtlAstMapFile = argv[5];
    }
    else
    {
        sourceFile = "program.gpy";
        tokenFile = "tokens.in";
        astFile = "ast.in";
        rtlFile = "program.rtl";
        rtlAstMapFile = "rtlmap.in";
        cout << "using default input file names.\n";
    }

    cout << "source file: " << sourceFile
        << "\ntoken file: " << tokenFile
        << "\nAST file: " << astFile
        << "\nRTL file: " << rtlFile
        << "\nRTL->AST map file: " << rtlAstMapFile
        << endl;

    init();

    engine::RegisterPostInitFunction(enableShadows);

    engine::RegisterUpdateFunction(update);

    return engine::Run();
}
