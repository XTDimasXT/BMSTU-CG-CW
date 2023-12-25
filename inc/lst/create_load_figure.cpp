void FigureFacade::DownloadFigure(const std::string& filename, ConcreteFigureBuilder& builder) 
{
    std::ifstream fin(filename);
    if (fin.is_open()) 
    {
        ReadFigure(fin, builder);
        fin.close();
    }
}

void FigureFacade::ReadFigure(std::ifstream& fin, FigureBuilder& builder) {
    std::string line;

    EdgesCreator edgesCreator;
    Edges* edges = (Edges*)edgesCreator.Create();

    PointsCreator pointsCretor;
    Points* points = (Points*)pointsCretor.Create();

    FacesCreator facesCreator;
    Faces* faces = (Faces*)facesCreator.Create();

    NormalsCreator normalsCreator;
    Normals* normals = (Normals*)normalsCreator.Create();

    while (std::getline(fin, line)) {
        if (line.substr(0, 2) == "v ") {
        Point vertex;
        ReadVertex(vertex, line);
        points->AppendPoint(vertex);
        } else if (line.substr(0, 2) == "f ") {
        Face face;
        ReadFace(face, line);
        faces->AppendFace(face);
        } else if (line.substr(0,3) == "vn ") {
            Normal normal;
            ReadNormal(normal, line);
            normals->AppendNormal(normal);
        }
    }
    builder.buildFaces(*faces);
    builder.buildPoints(*points);
    builder.buildNormals(*normals);
    builder.buildPointsTable();

    for (size_t i = 0; i < faces->array_faces_.size(); ++i) {
        FaceToEdges(faces->array_faces_[i], *edges);
    }
    builder.buildEdges(*edges);
}