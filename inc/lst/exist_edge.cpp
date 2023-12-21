bool FigureFacade::ExistEdge(const Edge& edge) 
{
    if (static_cast<size_t>(edge.first_point) < points_.points_table_.size() &&
    static_cast<size_t>(edge.second_point) <
    points_.points_table_[edge.first_point].size()) 
    {
        return points_.points_table_[edge.first_point][edge.second_point] == 1;
    } else 
    {
        return false;
    }
}