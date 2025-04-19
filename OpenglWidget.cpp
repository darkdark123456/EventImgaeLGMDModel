#include "OpenglWidget.h"
#include <QOpenGLTexture>

OpenGLWidget::OpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent), texture(nullptr) {
    setWindowFlags(Qt::Window);
    resize(640, 480);
}

OpenGLWidget::~OpenGLWidget() {
    makeCurrent();
    delete texture;
    doneCurrent();
}

void OpenGLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_TEXTURE_2D);
}

void OpenGLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void OpenGLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glClear(GL_COLOR_BUFFER_BIT);
    std::lock_guard<std::mutex> lock(frameMutex);
    if (currentFrame.isNull()) return;

    if (!texture) {
        texture = new QOpenGLTexture(currentFrame);
    } else {
        texture->destroy();
        texture->create();
        texture->setData(currentFrame);
    }

    texture->bind();

    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex2f(-1, -1);
    glTexCoord2f(1, 1); glVertex2f(1, -1);
    glTexCoord2f(1, 0); glVertex2f(1, 1);
    glTexCoord2f(0, 0); glVertex2f(-1, 1);
    glEnd();

    texture->release();

// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    std::lock_guard<std::mutex> lock(frameMutex);
//    if (currentFrame.isNull()) return;

//    if (!texture) {
//        texture = new QOpenGLTexture(currentFrame);
//    } else {
//        texture->setData(currentFrame);
//    }

//    texture->bind();

//    glBegin(GL_QUADS);
//    glTexCoord2f(0, 1); glVertex2f(-1, -1);
//    glTexCoord2f(1, 1); glVertex2f(1, -1);
//    glTexCoord2f(1, 0); glVertex2f(1, 1);
//    glTexCoord2f(0, 0); glVertex2f(-1, 1);
//    glEnd();

//    texture->release();
}

void OpenGLWidget::updateFrame(const QImage &image) {
    std::lock_guard<std::mutex> lock(frameMutex);
    currentFrame = image;
    update();
}
