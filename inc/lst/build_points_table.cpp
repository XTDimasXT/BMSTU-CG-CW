void buildPointsTable() override 
{
    for (size_t i = 0; i < figure.edges_.array_edges_.size(); ++i) 
    {
        if (static_cast<size_t>
        (figure.edges_.array_edges_[i].first_point) <
        figure.points_.points_table_.size() &&
        static_cast<size_t>
        (figure.edges_.array_edges_[i].second_point) <
        figure.points_
        .points_table_[figure.edges_.array_edges_[i].first_point]
        .size()) 
        {
            figure.points_
            .points_table_[figure.edges_.array_edges_[i]
            .first_point]
            [figure.edges_.array_edges_[i].second_point] = 1;
            figure.points_
            .points_table_[figure.edges_.array_edges_[i]
            .first_point]
            [figure.edges_.array_edges_[i].second_point] = 1;
        } 
        else
        {
            return;
        }
    }
 }