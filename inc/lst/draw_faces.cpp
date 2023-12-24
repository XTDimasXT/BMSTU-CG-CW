void MyOpenGLWidget::draw_faces() 
{
    glColor3f(edges_color.redF(), edges_color.greenF(), edges_color.blueF());
    glLineWidth(edges_width);

    for (size_t i = 0; i < image.faces_.array_faces_.size(); ++i) 
    {
        const auto& current_face = image.faces_.array_faces_[i].points;
        size_t num_points = current_face.size();
        glBegin(num_points == 3 ? GL_TRIANGLES : GL_POLYGON);
        
        for (size_t j = 0; j < num_points; ++j) 
        {
            int vertex_index = current_face[j] - 1;
            glVertex3f(image.points_.
            array_points_[vertex_index].x_,
            image.points_.array_points_[vertex_index].y_,
            image.points_.array_points_[vertex_index].z_);
        }
        glEnd();
    }
}