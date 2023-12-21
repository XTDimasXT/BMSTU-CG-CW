void FigureFacade::ReadFace(Face& face, const std::string& line) 
{
    std::istringstream iss(line);
    std::string token;
    int num_tokens = 0;
    
    while (iss >> token) 
    {
        if (num_tokens > 0) 
        {
            int point_index;
            std::istringstream(token) >> point_index;
            face.points.push_back(point_index);
        }
        num_tokens++;
    }
}
