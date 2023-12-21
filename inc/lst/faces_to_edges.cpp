void FigureFacade::FaceToEdges(const Face& face, Edges& edges) 
{
    for (size_t i = 0; i < face.points.size(); ++i)
    {
        Edge edge;
        edge.first_point = face.points[i];
        edge.second_point = face.points[(i + 1) % face.points.size()];
        if (!ExistEdge(edge))
            edges.AppendEdge(edge);
    }
}