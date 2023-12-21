void MyOpenGLWidget::paintGL() 
{
    glClearColor(background_color.redF(), background_color.greenF(), background_color.blueF(), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (MORPH)
        init_camera(image);
    else
        init_camera(figure);

    if (OK) 
    { 
        if (MORPH)
        {
            draw_edges();
            draw_vertices();
        }
        else
        draw_faces();
    } 
    else 
    {
        figure_default();
        image_default();
        OK = true;
    }
}
