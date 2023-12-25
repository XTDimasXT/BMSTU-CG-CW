void FigureFacade::ReadFace(Face& face, const std::string& line) {
  std::istringstream iss(line);
  std::string token;
  int num_tokens = 0;
  while (iss >> token) {
    if (num_tokens > 0) {
      int point_index;
      char tmp;
      int tmpI;
      int normal_index;

      if (token.find("//") != std::string::npos) {
        std::istringstream(token) >> point_index >> tmp >> tmp >> normal_index;
      } else {
        std::istringstream(token) >> point_index >> tmp >> tmpI >> tmp >> normal_index;
      }

      face.points.push_back(point_index);
      face.normal = normal_index;
    }
    num_tokens++;
  }
}