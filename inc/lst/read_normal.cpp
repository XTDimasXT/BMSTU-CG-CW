void FigureFacade::ReadNormal(Normal& normal, const std::string& line)
{
    std::istringstream iss(line);
    std::string v;
    iss >> v >> normal.x_ >> normal.y_ >> normal.z_;
}