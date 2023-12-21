void MyOpenGLWidget::init_camera(FigureFacade &fig) 
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    double min_x = -1.0;
    double max_x = 1.0;
    double min_y = -1.0;
    double max_y = 1.0;
    double min_z = 1.0;
    double max_z = 10.0;

    if (!fig.points_.array_points_.empty())
        fig.points_.FindMinMaxPoints(min_x, max_x, min_y, max_y, min_z, max_z);
    
    double padding = 2.0;

    double width = max_x - min_x + 2 * padding;
    double height = max_y - min_y + 2 * padding;
    double depth = max_z - min_z + 2 * padding;

    double centerX = (max_x + min_x) / 2;
    double centerY = (max_y + min_y) / 2;
    double centerZ = (max_z + min_z) / 2;

    double halfWidth = width / 2;
    double halfHeight = height / 2;
    double halfDepth = depth / 2;

    double diagonal = sqrt(halfWidth * halfWidth + halfHeight * halfHeight +
    halfDepth * halfDepth);

    min_x = centerX - diagonal;
    max_x = centerX + diagonal;
    min_y = centerY - diagonal;
    max_y = centerY + diagonal;
    min_z = centerZ - diagonal;
    max_z = centerZ + diagonal;

    GLfloat lightPos[] = {static_cast<GLfloat>(max_x * 1.5) + lightD[0], 
    static_cast<GLfloat>(max_y * 1.5) + lightD[1], static_cast<GLfloat>(max_z + 2.0f) + 
    lightD[2], 1.0f};
    GLfloat lightAmbient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);

    glOrtho(min_x, max_x, min_y, max_y, 0, std::numeric_limits<float>::max());
    glTranslatef(0.0f + cameraD[0], 0.0f + cameraD[1], -fabs(min_z) + cameraD[2]);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}