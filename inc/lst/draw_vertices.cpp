void MyOpenGLWidget::draw_vertices() 
{
    if (vertex_type != NONE) 
    {
        if (vertex_type == CIRCLE) 
            glEnable(GL_POINT_SMOOTH);
            
        glColor3f(vertices_color.redF(), vertices_color.greenF(),
        vertices_color.blueF());
        glPointSize(vertices_width);
        glBegin(GL_POINTS);

        for (size_t i = 0; i < image.points_.array_points_.size(); i++) 
        {
            glVertex3f(image.points_.array_points_[i].x_,
            image.points_.array_points_[i].y_,
            image.points_.array_points_[i].z_);
        }
        glEnd();

        if (vertex_type == CIRCLE) glDisable(GL_POINT_SMOOTH);
    }
}