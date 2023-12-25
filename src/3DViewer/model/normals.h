#ifndef NORMALS_H
#define NORMALS_H

#include "action_data.h"
#include "factoryMethod.h"
#include "point.h"

struct Normal {
    double x_;
    double y_;
    double z_;
    Normal() = default;
    ~Normal() = default;
    Normal(double x, double y, double z) : x_(x), y_(y), z_(z) {} ;
};

class Normals : public Product {
 public:
  Normals() = default;
  ~Normals() = default;

  void AppendNormal(Normal normal);

  std::vector<Normal> array_normals_;
};

class NormalsCreator : public Creator {
 public:
  virtual Product* Create() const override { return (Product*)(new Normals); }
};

#endif  // NORMALS_H
