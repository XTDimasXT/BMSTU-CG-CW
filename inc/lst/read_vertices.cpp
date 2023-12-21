void FigureFacade::ReadVertex(Point& point, const std::string& line) 
{
    std::istringstream iss(line);
    std::string v;
    iss >> v >> point.x_ >> point.y_ >> point.z_;
}
