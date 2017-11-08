//
//  OpenGLDrawLayer.cpp
//  testShader
//
//  Created by Mrlee on 2017/10/16.
//
//

#include "OpenGLDrawLayer.hpp"

OpenGLDrawLayer::OpenGLDrawLayer()
{
    
}

OpenGLDrawLayer::~OpenGLDrawLayer()
{
    
}

bool OpenGLDrawLayer::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    this->initOpenGL();
    return true;
}

void OpenGLDrawLayer::draw(cocos2d::Renderer* renderer,const cocos2d::Mat4&parentTransform,uint32_t parentFlags)
{
    Layer::draw(renderer, parentTransform, parentFlags);
    
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(OpenGLDrawLayer::onDraw, this, parentTransform, parentFlags);
    renderer->addCommand(&_customCommand);
}

//void OpenGLDrawLayer::onDraw(const Mat4 &transform, uint32_t flags)
//{
//    
//}

//------------------------ OpenGLDrawTriLayer ----------------------------//


OpenGLDrawTriLayer::OpenGLDrawTriLayer()
{
}

OpenGLDrawTriLayer::~OpenGLDrawTriLayer()
{
    
}


void OpenGLDrawTriLayer::initOpenGL()
{
    Size visibleSize = Director::getInstance()->getWinSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    /*
     在OpenGL中，GLSL的shader使用的流程与C语言相似，每个shader类似一个C模块，首先需要单独编译（compile），
     然后一组编译好的shader连接（link）成一个完整程序。
     */
    
    auto program = GLProgram::createWithFilenames("res/myVertextShader.vert", "res/myFragmentShader.frag");
    program->link();
    program->updateUniforms();
    this->setGLProgram(program);
    
    /*
     使用VBO和VAO的步骤都差不多，步骤如下：
     1 glGenXXX
     2 glBindXXX
     */
    // 创建和绑定vao
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    
    //创建和绑定vbo
    glGenBuffers(1, &_vertVBO);// 生成VBO
    glBindBuffer(GL_ARRAY_BUFFER, _vertVBO); // 关联到当前的VAO
    
    auto size = Director::getInstance()->getWinSize();
    float vertercies[] = {// 三角形顶点位置
        0, 0, // 第1个点坐标
        size.width, 0, // 第2个点坐标
        size.width / 2, size.height // 第3个点坐标
    };
    
    // 给VBO设置数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertercies), vertercies, GL_STREAM_DRAW);
    // 获得变量a_position在内存中的位置
    GLuint positionLocation = glGetAttribLocation(program->getProgram(), "a_position");
    glEnableVertexAttribArray(positionLocation);
    // 提交包含数据的数组指针
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    
    // 设置颜色
    float color[] = {// 三角形顶点颜色RGBA
        0, 1, 0, 1,
        1, 0, 0, 1,
        0, 0, 1, 1
    };
    glGenBuffers(1, &_colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
    
    // 获得变量a_color在内存中的位置
    GLuint colorLocation = glGetAttribLocation(program->getProgram(), "a_color");
    glEnableVertexAttribArray(colorLocation);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}


void OpenGLDrawTriLayer::onDraw(const Mat4 &transform, uint32_t flags)
{
    auto worldPos =  this->getParent()->convertToWorldSpace(this->getPosition());
    //绑定
    auto glProgramState = getGLProgramState();
//    glProgramState->setUniformMat4("u_mvpMatrix", transform);
    glProgramState->apply(transform);
    glProgramState->setUniformVec2("u_parentPos", worldPos);
    
//    auto glProgram = getGLProgram();
//    glProgram->use();
//    glProgram->setUniformsForBuiltins();
    
    /*
     VAO里的VBOs都设置好了以后，在绘制的地方只需要设置当前绑定的VAO是哪个，
     就能按照初始化的VAO来绘制，即调用glDrawArrays
     */
    
    // 设置当前绑定的VAO
    glBindVertexArray(_vao);
    
    m_index++;
    auto size = Director::getInstance()->getWinSize();
    float vertercies[] = {// 三角形顶点位置
        float(m_index), 0, // 第1个点坐标
        size.width, 0, // 第2个点坐标
        size.width / 2, size.height // 第3个点坐标
    };
    glBindBuffer(GL_ARRAY_BUFFER,_vertVBO);
    // 给VBO设置数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertercies), vertercies, GL_STREAM_DRAW);
    // 获得变量a_position在内存中的位置
    auto  program = this->getGLProgram();
    GLuint positionLocation = glGetAttribLocation(program->getProgram(), "a_position");
    glEnableVertexAttribArray(positionLocation);
    // 提交包含数据的数组指针
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    
    
    
    // 绘制三角形
    glDrawArrays(GL_TRIANGLES, 0, 3);
//    glDrawArrays(GL_LINE_LOOP, 0, 3);
    
    // 解绑当前VAO，但并不释放
    glBindVertexArray(0);
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 3);
    CHECK_GL_ERROR_DEBUG();
}

OpenGLDrawQuadLayer::OpenGLDrawQuadLayer()
{
}

OpenGLDrawQuadLayer::~OpenGLDrawQuadLayer()
{
    
}

void OpenGLDrawQuadLayer::initOpenGL()
{
    Size visibleSize = Director::getInstance()->getWinSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    /*
     在OpenGL中，GLSL的shader使用的流程与C语言相似，每个shader类似一个C模块，首先需要单独编译（compile），
     然后一组编译好的shader连接（link）成一个完整程序。
     */
    
    auto program = GLProgram::createWithFilenames("res/myVertextShader.vert", "res/myFragmentShader.frag");
    program->link();
    program->updateUniforms();
    this->setGLProgram(program);
    
    /*
     使用VBO和VAO的步骤都差不多，步骤如下：
     1 glGenXXX
     2 glBindXXX
     */
    // 创建和绑定vao
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    
    
    auto size = Director::getInstance()->getWinSize();
    
    typedef struct {
        float Position[2];
        float Color[4];
    } Vertex;
    
    Vertex data[] =
    {
        {{0,0},{0,1,0,1}},
        {{size.width * 0.5f, 0},{0,1,0,1}},
        { {0, size.height * 0.5f},{0,1,0,1}},
        {{size.width * 0.5f,size.height * 0.5f},{0,1,0,1}}
    };

    glGenBuffers(1,&_vertVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _vertVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(data),data,GL_STATIC_DRAW);
    
    GLuint positionLocation = glGetAttribLocation(program->getProgram(), "a_position");
    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(GLvoid*)offsetof(Vertex, Position));
    
    
    GLuint colorLocation = glGetAttribLocation(program->getProgram(), "a_color");
    glEnableVertexAttribArray(colorLocation);
    glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Color));
    
    
    GLuint indexVBO;
    GLubyte indices[] = { 0,1,2,  //第一个三角形索引
        2,3,1}; //第二个三角形索引
    
    glGenBuffers(1, &indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) , indices, GL_STATIC_DRAW);
    
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OpenGLDrawQuadLayer::onDraw(const Mat4 &transform, uint32_t flags)
{
    auto worldPos =  this->getParent()->convertToWorldSpace(this->getPosition());
    //绑定
    auto glProgramState = getGLProgramState();
    //    glProgramState->setUniformMat4("u_mvpMatrix", transform);
    glProgramState->apply(transform);
    glProgramState->setUniformVec2("u_parentPos", worldPos);
    
    //    auto glProgram = getGLProgram();
    //    glProgram->use();
    //    glProgram->setUniformsForBuiltins();
    
    /*
     VAO里的VBOs都设置好了以后，在绘制的地方只需要设置当前绑定的VAO是哪个，
     就能按照初始化的VAO来绘制，即调用glDrawArrays
     */
    
    // 设置当前绑定的VAO
    glBindVertexArray(_vao);
    
    glDrawElements(GL_TRIANGLES, 5, GL_UNSIGNED_BYTE,(GLvoid*)0);
    // 绘制三角形
 //   glDrawArrays(GL_TRIANGLES, 0, 6);
    
    // 解绑当前VAO，但并不释放
    glBindVertexArray(0);
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 6);
    CHECK_GL_ERROR_DEBUG();
}


OpenGLDrawCubeLayer::OpenGLDrawCubeLayer()
{
}

OpenGLDrawCubeLayer::~OpenGLDrawCubeLayer()
{
    
}

void OpenGLDrawCubeLayer::initOpenGL()
{
    Size visibleSize = Director::getInstance()->getWinSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    /*
     在OpenGL中，GLSL的shader使用的流程与C语言相似，每个shader类似一个C模块，首先需要单独编译（compile），
     然后一组编译好的shader连接（link）成一个完整程序。
     */
    
    auto program = GLProgram::createWithFilenames("res/textureVertextShader.vert", "res/textureFragmentShader.frag");
    program->link();
    program->updateUniforms();
    this->setGLProgram(program);
    
    /*
     使用VBO和VAO的步骤都差不多，步骤如下：
     1 glGenXXX
     2 glBindXXX
     */
    // 创建和绑定vao
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    
    auto size = Director::getInstance()->getWinSize();
    typedef struct {
        float Position[3];
        float Color[4];
     float TexCoord[2];
    } Vertex;
    
    Vertex data[] =
    {
        // Front
        {{0.5, -0.5, 1}, {1, 0, 0, 1},{1,0}},
        {{0.5, 0.5, 1}, {0, 1, 0, 1},{1,1}},
        {{-0.5, 0.5, 1}, {0, 0, 1, 1},{0,1}},
        {{-0.5, -0.5, 1}, {0, 0, 0, 1},{0,0}},
        // Back
        {{0.5, 0.5, 0}, {1, 0, 0, 1},{1,1}},
        {{-0.5, -0.5, 0}, {0, 1, 0, 1},{0,0}},
        {{0.5, -0.5, 0}, {0, 0, 1, 1},{1,0}},
        {{-0.5, 0.5, 0}, {0, 0, 0, 1},{0,1}},
        // Left
        {{-0.5, -0.5, 1}, {1, 0, 0, 1},{0,1}},
        {{-0.5, 0.5, 1}, {0, 1, 0, 1},{1,1}},
        {{-0.5, 0.5, 0}, {0, 0, 1, 1},{1,0}},
        {{-0.5, -0.5, 0}, {0, 0, 0, 1},{0,0}},
        // Right
        {{0.5, -0.5, 0}, {1, 0, 0, 1},{0,0}},
        {{0.5, 0.5, 0}, {0, 1, 0, 1},{1,0}},
        {{0.5, 0.5, 1}, {0, 0, 1, 1},{1,1}},
        {{0.5, -0.5, 1}, {0, 0, 0, 1},{0,1}},
        // Top
        {{0.5, 0.5, 1}, {1, 0, 0, 1},{1,1}},
        {{0.5, 0.5, 0}, {0, 1, 0, 1},{1,0}},
        {{-0.5, 0.5, 0}, {0, 0, 1, 1},{0,0}},
        {{-0.5, 0.5, 1}, {0, 0, 0, 1},{0,1}},
        // Bottom
        {{0.5, -0.5, 0}, {1, 0, 0, 1},{1,0}},
        {{0.5, -0.5, 1}, {0, 1, 0, 1},{1,1}},
        {{-0.5, -0.5, 1}, {0, 0, 1, 1},{0,1}},
        {{-0.5, -0.5, 0}, {0, 0, 0, 1},{0,0}}
    };
    
    
    glGenBuffers(1,&_vertVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _vertVBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(data),data,GL_STATIC_DRAW);
    
    GLuint positionLocation = glGetAttribLocation(program->getProgram(), "a_position");
    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(GLvoid*)offsetof(Vertex, Position));
    
    
    GLuint colorLocation = glGetAttribLocation(program->getProgram(), "a_color");
    glEnableVertexAttribArray(colorLocation);
    glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Color));
    
    auto texCoord = glGetAttribLocation(program->getProgram(), "a_texCoord");
    glEnableVertexAttribArray(texCoord);
    glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex,TexCoord));
    
    GLuint indexVBO;
    GLubyte indices[] = { // Front
        0, 1, 2,
        2, 3, 0,
        // Back
        4, 5, 6,
        4, 5, 7,
        // Left
        8, 9, 10,
        10, 11, 8,
        // Right
        12, 13, 14,
        14, 15, 12,
        // Top
        16, 17, 18,
        18, 19, 16,
        // Bottom
        20, 21, 22,
        22, 23, 20
    }; //第二个三角形索引
    
    glGenBuffers(1, &indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) , indices, GL_STATIC_DRAW);
    
    
    _textureVBO = Director::getInstance()->getTextureCache()->addImage("res/HelloWorld.png")->getName();

    
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OpenGLDrawCubeLayer::onDraw(const Mat4 &transform, uint32_t flags)
{
    
    auto worldPos =  this->getParent()->convertToWorldSpace(this->getPosition());
    //绑定
    auto glProgramState = getGLProgramState();
    //    glProgramState->setUniformMat4("u_mvpMatrix", transform);
    glProgramState->apply(transform);
//    glProgramState->setUniformVec2("u_parentPos", worldPos);
    
    
    m_count += 0.01;
    // 设置当前绑定的VAO
    Mat4 mat_Scale;
    Mat4::createScale(Vec3(1.0f,1.0f,1.0f), &mat_Scale);
    
    Mat4 mat_Rotation;
    Mat4::createRotation(Vec3(0.0f,1.0f,1.0f), m_count, &mat_Rotation);
    
    Mat4 mat_Translation;
    Mat4::createTranslation(Vec3(0.0f,0.0f,-5.0f), &mat_Translation);
    
    Mat4 mat_Perspective;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    //    Mat4::createOrthographic(1, visibleSize.width/visibleSize.height, 0.1f, 10000.0f, &mat_Orthographic);
    Mat4::createPerspective(60, visibleSize.width/visibleSize.height, 0.1f, 100.0f, &mat_Perspective);
    
    Mat4 mat = mat_Perspective * mat_Translation * mat_Rotation * mat_Scale ;
    
    
    //    mat = mat_Orthographic * mat;
    
    //    glProgramState->setUniformMat4("u_transform", mat);
     GLuint transformLoc = glGetUniformLocation(getGLProgram()->getProgram(), "u_transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, mat.m);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     GL::bindTexture2D(_textureVBO);
    /*
     VAO里的VBOs都设置好了以后，在绘制的地方只需要设置当前绑定的VAO是哪个，
     就能按照初始化的VAO来绘制，即调用glDrawArrays
     */
    
    // 设置当前绑定的VAO
    glBindVertexArray(_vao);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glEnable(GL_DEPTH_BUFFER_BIT);
    
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE,(GLvoid*)0);
//    glDrawElements(GL_LINE_LOOP, 36, GL_UNSIGNED_BYTE,(GLvoid*)0);
    // 绘制三角形
    //   glDrawArrays(GL_TRIANGLES, 0, 6);
    
    // 解绑当前VAO，但并不释放
    glBindVertexArray(0);
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 6);
    CHECK_GL_ERROR_DEBUG();
}

OpenGLDrawColorChangeLayer::OpenGLDrawColorChangeLayer()
{}
OpenGLDrawColorChangeLayer::~OpenGLDrawColorChangeLayer()
{}

void OpenGLDrawColorChangeLayer::initOpenGL()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto program = GLProgram::createWithFilenames("res/colorVertextShader.vert", "res/colorFragmentShader.frag");
    program->link();
    program->updateUniforms();
    this->setGLProgram(program);
    
    glGenVertexArrays(1,&_vao);
    glBindVertexArray(_vao);
    
    GLfloat vertex[] = {
        0.0f,0.0f,
        visibleSize.width * 0.5f,visibleSize.height,
        visibleSize.width,0.0f
    };
    
    glGenBuffers(1, &_vertVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _vertVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    GLint positionLocation = glGetAttribLocation(program->getProgram(), "a_position");
    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    
    // 设置颜色
    float color[] = {// 三角形顶点颜色RGBA
        0, 1, 0, 1,
        1, 0, 0, 1,
        0, 0, 1, 1
    };
    glGenBuffers(1, &_colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);

    // 获得变量a_color在内存中的位置
    GLuint colorLocation = glGetAttribLocation(program->getProgram(), "a_color");
    glEnableVertexAttribArray(colorLocation);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLDrawColorChangeLayer::onDraw(const Mat4 &transform,uint32_t flags)
{
    auto glprogrameState = this->getGLProgramState();
    glprogrameState->apply(transform);
  //  glprogrameState->setUniformVec4("u_color", Vec4(1.0f, 0.5f, 0.0f, 1.0f));
    m_timeIndex += 0.003;
    
    glprogrameState->setUniformFloat("u_time", m_timeIndex);
    
    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 6);
    CHECK_GL_ERROR_DEBUG();
}

OpenGLDrawTextureLayer::OpenGLDrawTextureLayer()
{
    
}

OpenGLDrawTextureLayer::~OpenGLDrawTextureLayer()
{
    
}

void OpenGLDrawTextureLayer::initOpenGL()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto program = GLProgram::createWithFilenames("res/textureVertextShader.vert", "res/textureFragmentShader.frag");
    program->updateUniforms();
    program->link();
    this->setGLProgram(program);
    
    glGenVertexArrays(1,&_vao);
    glBindVertexArray(_vao);
    
    
    typedef struct {
        float Position[2];
        float Color[4];
        float TexCoord[2];
    } Vertex;
    
//    Vertex data[] = {
//        // Positions // Colors // Texture Coords
//        {{visibleSize.width * 0.75f, visibleSize.height * 0.75f},{0.0f, 1.0f, 0.0f, 1.0f}
//            ,{1.0f, 1.0f}
//
//        }, // Top Right
//        {{visibleSize.width * 0.75f, visibleSize.height * 0.25f},{0.0f, 0.0f, 1.0f, 1.0f}
//            ,{1.0f, 0.0f}
//
//        },// Bottom Right
//        {{visibleSize.width * 0.25f, visibleSize.height * 0.25f},{0.0f, 0.0f, 0.0f, 1.0f}
//            ,{0.0f, 0.0f}
//
//        }, // Bottom Left
//        {{visibleSize.width * 0.25f, visibleSize.height * 0.75f},{0.0f, 1.0f, 1.0f, 1.0f}
//            ,{0.0f, 1.0f}
//
//        }// Top Left
//    };
    
    Vertex data[] = {
        // Positions // Colors // Texture Coords
        {{0.5, 0.5},{0.0f, 1.0f, 0.0f, 1.0f}
            ,{1.0f, 1.0f}
            
        }, // Top Right
        {{0.5, -0.5},{0.0f, 0.0f, 1.0f, 1.0f}
            ,{1.0f, 0.0f}
            
        },// Bottom Right
        {{-0.5, -0.5},{1.0f, 0.0f, 0.0f, 1.0f}
            ,{0.0f, 0.0f}
            
        }, // Bottom Left
        {{-0.5, 0.5},{1.0f, 1.0f, 1.0f, 1.0f}
            ,{0.0f, 1.0f}
            
        }// Top Left
    };

    
    glGenBuffers(1, &_vertVBO);
    glBindBuffer(GL_ARRAY_BUFFER, _vertVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    auto position = glGetAttribLocation(program->getProgram(), "a_position");
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex,Position));
    
//    glGenBuffers(1,&_colorVBO);
//    glBindBuffer(GL_ARRAY_BUFFER,_colorVBO);
//    glBufferData(GL_ARRAY_BUFFER,sizeof(data),data,GL_STATIC_DRAW);
    auto color = glGetAttribLocation(program->getProgram(),"a_color");
    glEnableVertexAttribArray(color);
    glVertexAttribPointer(color,4,GL_FLOAT,GL_FALSE,sizeof(Vertex),(GLvoid*)offsetof(Vertex, Color));
    
//    glGenBuffers(1,&_texCoordVBO);
//    glBindBuffer(GL_ARRAY_BUFFER,_texCoordVBO);
//    glBufferData(GL_ARRAY_BUFFER,sizeof(data),data,GL_STATIC_DRAW);
    auto texCoord = glGetAttribLocation(program->getProgram(), "a_texCoord");
    glEnableVertexAttribArray(texCoord);
    glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex,TexCoord));
    
    GLubyte vertexIndex[] = {
        0,1,3,
        1,2,3
    };
    glGenBuffers(1, &_indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertexIndex), vertexIndex, GL_STATIC_DRAW);
    
    _textureVBO = Director::getInstance()->getTextureCache()->addImage("res/HelloWorld.png")->getName();
    

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_TEXTURE_2D, 0);
}

void OpenGLDrawTextureLayer::onDraw(const Mat4 &transform,uint32_t flags)
{
    auto glProgramState = getGLProgramState();
    glProgramState->apply(transform);
//
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glEnable(GL_DEPTH_BUFFER_BIT);
    m_count += 0.1;
    // 设置当前绑定的VAO
    Mat4 mat_Scale;
    Mat4::createScale(Vec3(1.0f,1.0f,1.0f), &mat_Scale);
    
    Mat4 mat_Rotation;
    Mat4::createRotation(Vec3(0.0f,1.0f,0.0f), m_count, &mat_Rotation);
    
    Mat4 mat_Translation;
    Mat4::createTranslation(Vec3(0.0f,0.0f,-1.0f), &mat_Translation);
    
    Mat4 mat_Perspective;
    Size visibleSize = Director::getInstance()->getVisibleSize();
//    Mat4::createOrthographic(1, visibleSize.width/visibleSize.height, 0.1f, 10000.0f, &mat_Orthographic);
    Mat4::createPerspective(45, visibleSize.width/visibleSize.height, 0.1f, 100.0f, &mat_Perspective);
    
//    Mat4::createLookAt(<#const cocos2d::Vec3 &eyePosition#>, <#const cocos2d::Vec3 &targetPosition#>, <#const cocos2d::Vec3 &up#>, <#cocos2d::Mat4 *dst#>)
    
    Mat4 mat = mat_Perspective * mat_Translation * mat_Rotation * mat_Scale ;
    
   
//    mat = mat_Orthographic * mat;
    
//    glProgramState->setUniformMat4("u_transform", mat);
    GLuint transformLoc = glGetUniformLocation(getGLProgram()->getProgram(), "u_transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, mat.m);
    
    GL::bindTexture2D(_textureVBO);
    Director::getInstance()->setDepthTest(true);
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LEQUAL);
//    glClearDepthf(1.0f);
 //   glActiveTexture(GL_TEXTURE0);
 //   glBindBuffer(GL_TEXTURE_2D, _textureVBO);
  //  glUniform1i(glGetUniformLocation(getGLProgram()->getProgram(),"ourTexture1"), 0);
    
    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE,(GLvoid*)0);
    // 绘制三角形
    //   glDrawArrays(GL_TRIANGLES, 0, 6);
    
    // 解绑当前VAO，但并不释放
    glBindVertexArray(0);
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 6);
    CHECK_GL_ERROR_DEBUG();
}

