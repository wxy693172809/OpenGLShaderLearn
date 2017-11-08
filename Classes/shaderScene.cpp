//
//  shaderScene.cpp
//  testShader
//
//  Created by Mrlee on 2017/10/13.
//
//

#include "shaderScene.hpp"
#include "HelloWorldScene.h"
#include "OpenGLDrawLayer.hpp"

shaderScene::shaderScene()
{
    
}

shaderScene::~shaderScene()
{
    
}


Scene* shaderScene::createScene()
{
    auto scene = Scene::create();
    auto layer = shaderScene::create();
    
    
//    auto moveMove1 = MoveBy::create(1, Vec2(200,200));
//    auto moveMove2 = moveMove1->reverse();
//    layer->runAction(RepeatForever::create(Sequence::create(moveMove1,moveMove2, NULL)));
    scene->addChild(layer);
    
    return scene;
}

 bool shaderScene::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getWinSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto layer = OpenGLDrawCubeLayer::create();
 //   layer->setPosition(Vec2(-100.0f,-100.0f));
    this->addChild(layer);

//    auto layer2 = OpenGLDrawQuadLayer::create();
//    this->addChild(layer2);
    
//    auto layer2 = OpenGLDrawCubeLayer::create();
//    this->addChild(layer2);
    
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
    
    // 创建和绑定vbo
    glGenBuffers(1, &_vertVBO);// 生成VBO
    glBindBuffer(GL_ARRAY_BUFFER, _vertVBO); // 关联到当前的VAO
    
    auto size = Director::getInstance()->getWinSize();
    float vertercies[] = {// 三角形顶点位置
        0, 0, // 第1个点坐标
        size.width, 0, // 第2个点坐标
        size.width / 2, size.height // 第3个点坐标
    };
    
    // 给VBO设置数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertercies), vertercies, GL_STATIC_DRAW);
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
    return true;
}


void shaderScene::draw(cocos2d::Renderer* renderer,const cocos2d::Mat4&parentTransform,uint32_t parentFlags)
{
    Layer::draw(renderer, parentTransform, parentFlags);
    
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(shaderScene::onDraw, this, parentTransform, parentFlags);
    renderer->addCommand(&_customCommand);
}

void shaderScene::onDraw(const Mat4 &transform, uint32_t flags)
{
    //绑定
    auto glProgramState = getGLProgramState();
    glProgramState->apply(transform);
    glProgramState->setUniformVec2("u_parentPos", Vec2(100.0f,100.0f));
    
 //   auto glProgram = getGLProgram();
 //   glProgram->use();
  //  glProgram->setUniformsForBuiltins();
    
        /*
     VAO里的VBOs都设置好了以后，在绘制的地方只需要设置当前绑定的VAO是哪个，
     就能按照初始化的VAO来绘制，即调用glDrawArrays
     */
    
    // 设置当前绑定的VAO
    glBindVertexArray(_vao);
    
    // 绘制三角形
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    // 解绑当前VAO，但并不释放
    glBindVertexArray(0);
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 3);
    CHECK_GL_ERROR_DEBUG();
}
