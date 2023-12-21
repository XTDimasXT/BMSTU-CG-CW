void MyOpenGLWidget::draw_edges() 
{
    if (edges_type == DOTTED) 
    {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1, 0x00FF);
    }

    glColor3f(edges_color.redF(), edges_color.greenF(), edges_color.blueF());
    glLineWidth(edges_width);
    glBegin(GL_LINES);

    for (size_t i = 0; i < image.edges_.array_edges_.size(); ++i) 
    {
        int v1 = image.edges_.array_edges_[i].first_point - 1;
        int v2 = image.edges_.array_edges_[i].second_point - 1;

        glVertex3f(image.points_.array_points_[v1].x_,
        image.points_.array_points_[v1].y_,
        image.points_.array_points_[v1].z_);

        glVertex3f(image.points_.array_points_[v2].x_,
        image.points_.array_points_[v2].y_,
        image.points_.array_points_[v2].z_);
    }
    glEnd();
    
    if (edges_type == DOTTED)
        glDisable(GL_LINE_STIPPLE);
}
