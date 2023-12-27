#include "controller.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

QTimer *screenshotTimer;

#include "myopenglwidget.h"
#include "ui_view.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  connect(this, SIGNAL(background_color_changed(QColor *)), ui->picture,
          SLOT(background_color_changed(QColor *)));
  connect(this, SIGNAL(edges_color_changed(QColor *)), ui->picture,
          SLOT(edges_color_changed(QColor *)));
  connect(this, SIGNAL(vertices_color_changed(QColor *)), ui->picture,
          SLOT(vertices_color_changed(QColor *)));
  connect(this, SIGNAL(vertices_width_changed(float *)), ui->picture,
          SLOT(vertices_width_changed(float *)));
  connect(this, SIGNAL(edges_width_changed(float *)), ui->picture,
          SLOT(edges_width_changed(float *)));
  connect(this, SIGNAL(edges_type_changed(edges_type_t *)), ui->picture,
          SLOT(edges_type_changed(edges_type_t *)));
  connect(this, SIGNAL(vertices_type_changed(vertex_type_t *)), ui->picture,
          SLOT(vertices_type_changed(vertex_type_t *)));
  connect(this, SIGNAL(projection_changed(projection_t *)), ui->picture,
          SLOT(projection_changed(projection_t *)));
  connect(this, SIGNAL(init_model(QString)), ui->picture,
          SLOT(init_model(QString)));
  connect(this, SIGNAL(scale_changed()), ui->picture, SLOT(redraw()));
  connect(this, SIGNAL(turn_changed()), ui->picture, SLOT(redraw()));
  connect(this, SIGNAL(move_changed()), ui->picture, SLOT(redraw()));
  connect(this, SIGNAL(camera_changed(int, char)), ui->picture, SLOT(camera_changed(int, char)));
  connect(this, SIGNAL(light_changed(int, char)), ui->picture, SLOT(light_changed(int, char)));
  connect(this, SIGNAL(clear_scene()), ui->picture, SLOT(clear_scene()));
  connect(this, SIGNAL(color_changed(int)), ui->picture, SLOT(color_changed(int)));

  connect(ui->picture, SIGNAL(model_information(FigureFacade &)), this,
          SLOT(model_information(FigureFacade &)));


  edges_width = 1.0;
  vertices_width = 1.0;
  vertices_type = NONE;
  edges_type = SOLID;

  QSettings settings("team", "3DViewer");
  ui->moveXSlider->setValue(settings.value("moveXSlider").toInt());
  ui->moveYSlider->setValue(settings.value("moveYSlider").toInt());
  ui->moveZSlider->setValue(settings.value("moveZSlider").toInt());
  ui->moveXBox->setValue(settings.value("moveXBox").toInt());
  ui->moveYBox->setValue(settings.value("moveYBox").toInt());
  ui->moveZBox->setValue(settings.value("moveZBox").toInt());

  ui->turnXBox->setValue(settings.value("turnXBox").toInt());
  ui->turnYBox->setValue(settings.value("turnYBox").toInt());
  ui->turnZBox->setValue(settings.value("turnZBox").toInt());
  ui->turnXSlider->setValue(settings.value("turnXSlider").toInt());
  ui->turnYSlider->setValue(settings.value("turnYSlider").toInt());
  ui->turnZSlider->setValue(settings.value("turnZSlider").toInt());

  ui->scaleBox->setValue(settings.value("scaleBox").toInt());
  ui->scaleSlider->setValue(settings.value("scaleSlider").toInt());

  ui->slider_edges_width->setValue(
      settings.value("slider_edges_width").toInt());
  ui->slider_vertices_width->setValue(
      settings.value("slider_vertices_width").toInt());

  ui->box_edges_type->setCurrentIndex(settings.value("box_edges_type").toInt());
  ui->box_vertices_type->setCurrentIndex(
      settings.value("box_vertices_type").toInt());


  scale = ui->scaleBox->value();
  rotate_x = ui->turnXBox->value();
  rotate_y = ui->turnYBox->value();
  rotate_z = ui->turnZBox->value();
  move_x = ui->moveXBox->value();
    move_y = ui->moveYBox->value();
    move_z = ui->moveZBox->value();
}

MainWindow::~MainWindow() {
  QSettings settings("team", "3DViewer");

  settings.setValue("moveXSlider", ui->moveXSlider->value());
   settings.setValue("moveYSlider", ui->moveYSlider->value());
   settings.setValue("moveZSlider", ui->moveYSlider->value());

   settings.setValue("moveXBox", ui->moveXBox->value());
   settings.setValue("moveYBox", ui->moveYBox->value());
   settings.setValue("moveZBox", ui->moveZBox->value());

  settings.setValue("turnXSlider", ui->turnXSlider->value());
  settings.setValue("turnYSlider", ui->turnYSlider->value());
  settings.setValue("turnZSlider", ui->turnYSlider->value());

  settings.setValue("turnXBox", ui->turnXBox->value());
  settings.setValue("turnYBox", ui->turnYBox->value());
  settings.setValue("turnZBox", ui->turnZBox->value());

  settings.setValue("scaleBox", ui->scaleBox->value());
  settings.setValue("scaleSlider", ui->scaleSlider->value());

  settings.setValue("slider_edges_width", ui->slider_edges_width->value());
  settings.setValue("slider_vertices_width",
                    ui->slider_vertices_width->value());

  settings.setValue("box_vertices_type", ui->box_vertices_type->currentIndex());
  settings.setValue("box_edges_type", ui->box_edges_type->currentIndex());

  delete ui;
}

void MainWindow::on_slider_vertices_width_valueChanged(int value) {
  vertices_width = value / 10;
  emit vertices_width_changed(&vertices_width);
}

void MainWindow::on_slider_edges_width_valueChanged(int value) {
  edges_width = value / 10;
  emit edges_width_changed(&edges_width);
}

void MainWindow::on_box_edges_type_currentIndexChanged(int index) {
  if (index == 0) {
    edges_type = SOLID;
  } else if (index == 1) {
    edges_type = DOTTED;
  }
  emit edges_type_changed(&edges_type);
}

void MainWindow::on_box_vertices_type_currentIndexChanged(int index) {
  if (index == 0) {
    vertices_type = NONE;
  } else if (index == 1) {
    vertices_type = SQUARE;
  } else if (index == 2) {
    vertices_type = CIRCLE;
  }
  emit vertices_type_changed(&vertices_type);
}

void MainWindow::on_actionLoad_file_triggered() {
  file_name = QFileDialog::getOpenFileName(this, "Select object file",
                                           QDir::homePath(), "*.obj");
  if (file_name.isEmpty()) {
    QMessageBox::critical(this, "cannot open file", "cannot open file");
  } else {
    emit init_model(file_name);
    to_default();
  }
}

void MainWindow::move_prev() {
  Move move_tmp;
  if (projection == PERSPECTIVE)
    move_tmp = {.dx = move_x * 10, .dy = move_y * 10, .dz = move_z};
  else
    move_tmp = {.dx = move_x, .dy = move_y, .dz = move_z};

  ui->picture->get_image().MoveFigure(move_tmp);
  emit move_changed();
}

void MainWindow::turn_prev() {
  Rotate rotate_tmp = {
      .angle_x = rotate_x, .angle_y = rotate_y, .angle_z = rotate_z};
  ui->picture->get_image().RotateFigure(rotate_tmp);
  emit turn_changed();
}

void MainWindow::to_default()
{
    vertices_color = QColor::fromRgb(WHITE);
    edges_color = QColor::fromRgb(WHITE);
    background_color = QColor::fromRgb(BLACK);

    edges_type = SOLID;
    ui->box_edges_type->setCurrentIndex(0);
    vertices_type = NONE;
    ui->box_vertices_type->setCurrentIndex(0);
    projection = PARALLEL;

    file_name = "";

    vertices_width = 1.0;
    ui->slider_vertices_width->setValue(0);
    edges_width = 1.0;
    ui->slider_edges_width->setValue(0);

    scale = 1;
    ui->scaleBox->setValue(1.0);
    ui->scaleSlider->setValue(50);
    rotate_x = 0;
    ui->turnXBox->setValue(0);
    ui->turnXSlider->setValue(0);
    rotate_y = 0;
    ui->turnYBox->setValue(0);
    ui->turnYSlider->setValue(0);
    rotate_z = 0;
    ui->turnZBox->setValue(0);
    ui->turnZSlider->setValue(0);
    move_x = 0;
    ui->moveXBox->setValue(0);
    ui->moveXSlider->setValue(0);
    move_y = 0;
    ui->moveYBox->setValue(0);
    ui->moveYSlider->setValue(0);
    move_z = 0;
    ui->moveZBox->setValue(0);
    ui->moveZSlider->setValue(0);

    ui->MoveXCamera->setValue(50);
    emit camera_changed(0, 'x');
    ui->MoveYCamera->setValue(50);
    emit camera_changed(0, 'y');
    ui->horizontalSlider->setValue(50);
    ui->MoveZLight->setValue(50);
    emit light_changed(0, 'x');
    ui->MoveXLight->setValue(50);
    emit light_changed(0, 'y');
    ui->MoveYLight->setValue(50);
    emit light_changed(0, 'z');
}

void MainWindow::scale_prev() {
  Scale scale_tmp = {.kx = scale, .ky = scale, .kz = scale};
  ui->picture->get_image().ScaleFigure(scale_tmp);
  emit scale_changed();
}

void MainWindow::on_moveXSlider_sliderMoved(int position) {
  ui->moveXBox->setValue(position / 100.0);
  ui->picture->copy_image(ui->picture->get_figure(), ui->picture->get_image());
  Move move_tmp;
  if (projection == PERSPECTIVE)
    move_tmp = {.dx = double(ui->moveXBox->value()) * 10,
                .dy = move_y * 10,
                .dz = move_z};
  else
    move_tmp = {
        .dx = double(ui->moveXBox->value()), .dy = move_y, .dz = move_z};
  ui->picture->get_image().MoveFigure(move_tmp);
  move_x = double(ui->moveXBox->value());
  emit move_changed();

  turn_prev();
  scale_prev();
}

void MainWindow::on_moveYSlider_sliderMoved(int position) {
  ui->moveYBox->setValue(position / 100.0);
  ui->picture->copy_image(ui->picture->get_figure(), ui->picture->get_image());
  Move move_tmp;
  if (projection == PERSPECTIVE)
    move_tmp = {.dx = move_x * 10,
                .dy = double(ui->moveYBox->value()) * 10,
                .dz = move_z};
  else
    move_tmp = {
        .dx = move_x, .dy = double(ui->moveYBox->value()), .dz = move_z};
  ui->picture->get_image().MoveFigure(move_tmp);
  move_y = double(ui->moveYBox->value());
  emit move_changed();

  turn_prev();
  scale_prev();
}

void MainWindow::on_moveZSlider_sliderMoved(int position) {
  ui->moveZBox->setValue(position / 100.0);
  ui->picture->copy_image(ui->picture->get_figure(), ui->picture->get_image());
  Move move_tmp;
  if (projection == PERSPECTIVE)
    move_tmp = {.dx = move_x * 10,
                .dy = move_y * 10,
                .dz = double(ui->moveZBox->value())};
  else
    move_tmp = {
        .dx = move_x, .dy = move_y, .dz = double(ui->moveZBox->value())};
  ui->picture->get_image().MoveFigure(move_tmp);
  move_z = double(ui->moveZBox->value());
  emit move_changed();

  turn_prev();
  scale_prev();
}

void MainWindow::on_moveXBox_editingFinished() {
  ui->moveXSlider->setValue(ui->moveXBox->value() * 100);
  ui->picture->copy_image(ui->picture->get_figure(), ui->picture->get_image());
  Move move_tmp;
  if (projection == PERSPECTIVE)
    move_tmp = {.dx = double(ui->moveXBox->value()) * 10,
                .dy = move_y * 10,
                .dz = move_z};
  else
    move_tmp = {
        .dx = double(ui->moveXBox->value()), .dy = move_y, .dz = move_z};
  ui->picture->get_image().MoveFigure(move_tmp);
  move_x = double(ui->moveXBox->value());
  emit move_changed();

  turn_prev();
  scale_prev();
}

void MainWindow::on_moveYBox_editingFinished() {
  ui->moveYSlider->setValue(ui->moveYBox->value() * 100);
  ui->picture->copy_image(ui->picture->get_figure(), ui->picture->get_image());
  Move move_tmp;
  if (projection == PERSPECTIVE)
    move_tmp = {.dx = move_x * 10,
                .dy = double(ui->moveYBox->value()) * 10,
                .dz = move_z};
  else
    move_tmp = {
        .dx = move_x, .dy = double(ui->moveYBox->value()), .dz = move_z};

  ui->picture->get_image().MoveFigure(move_tmp);
  move_y = double(ui->moveYBox->value());
  emit move_changed();

  turn_prev();

  Scale scale_tmp = {.kx = scale, .ky = scale, .kz = scale};
  ui->picture->get_image().ScaleFigure(scale_tmp);
  emit scale_changed();
}

void MainWindow::on_moveZBox_editingFinished() {
  ui->moveZSlider->setValue(ui->moveZBox->value() * 100);
  ui->picture->copy_image(ui->picture->get_figure(), ui->picture->get_image());
  Move move_tmp;
  if (projection == PERSPECTIVE)
    move_tmp = {.dx = double(ui->moveXBox->value()) * 10,
                .dy = move_y * 10,
                .dz = move_z};
  else
    move_tmp = {
        .dx = double(ui->moveXBox->value()), .dy = move_y, .dz = move_z};
  ui->picture->get_image().MoveFigure(move_tmp);
  move_x = double(ui->moveXBox->value());
  emit move_changed();

  turn_prev();
  scale_prev();
}


void MainWindow::on_turnXSlider_sliderMoved(int position) {
  ui->turnXBox->setValue(position);
  ui->picture->copy_image(ui->picture->get_figure(), ui->picture->get_image());
  Rotate rotate_tmp = {.angle_x = double(ui->turnXBox->value()),
                       .angle_y = rotate_y,
                       .angle_z = rotate_z};
  ui->picture->get_image().RotateFigure(rotate_tmp);
  rotate_x = double(ui->turnXBox->value());
  emit turn_changed();

  move_prev();
  scale_prev();
}

void MainWindow::on_turnYSlider_sliderMoved(int position) {
  ui->turnYBox->setValue(position);
  ui->picture->copy_image(ui->picture->get_figure(), ui->picture->get_image());
  Rotate rotate_tmp = {.angle_x = rotate_x,
                       .angle_y = double(ui->turnYBox->value()),
                       .angle_z = rotate_z};
  ui->picture->get_image().RotateFigure(rotate_tmp);
  rotate_y = double(ui->turnYBox->value());
  emit turn_changed();

  move_prev();
  scale_prev();
}

void MainWindow::on_turnZSlider_sliderMoved(int position) {
  ui->turnZBox->setValue(position);
  ui->picture->copy_image(ui->picture->get_figure(), ui->picture->get_image());
  Rotate rotate_tmp = {.angle_x = rotate_x,
                       .angle_y = rotate_y,
                       .angle_z = double(ui->turnZBox->value())};
  ui->picture->get_image().RotateFigure(rotate_tmp);
  rotate_z = double(ui->turnZBox->value());
  emit turn_changed();

  move_prev();
  scale_prev();
}

void MainWindow::on_turnXBox_editingFinished() {
  ui->turnXSlider->setValue(ui->turnXBox->value());
  ui->picture->copy_image(ui->picture->get_figure(), ui->picture->get_image());
  Rotate rotate_tmp = {.angle_x = double(ui->turnXBox->value()),
                       .angle_y = rotate_y,
                       .angle_z = rotate_z};
  ui->picture->get_image().RotateFigure(rotate_tmp);
  rotate_x = double(ui->turnXBox->value());
  emit turn_changed();

  move_prev();
  scale_prev();
}

void MainWindow::on_turnYBox_editingFinished() {
  ui->turnYSlider->setValue(ui->turnYBox->value());
  ui->picture->copy_image(ui->picture->get_figure(), ui->picture->get_image());
  Rotate rotate_tmp = {.angle_x = rotate_x,
                       .angle_y = double(ui->turnYBox->value()),
                       .angle_z = rotate_z};
  ui->picture->get_image().RotateFigure(rotate_tmp);
  rotate_y = double(ui->turnYBox->value());
  emit turn_changed();

  move_prev();
  scale_prev();
}

void MainWindow::on_turnZBox_editingFinished() {
  ui->turnZSlider->setValue(ui->turnZBox->value());
  ui->picture->copy_image(ui->picture->get_figure(), ui->picture->get_image());
  Rotate rotate_tmp = {.angle_x = rotate_x,
                       .angle_y = rotate_y,
                       .angle_z = double(ui->turnZBox->value())};
  ui->picture->get_image().RotateFigure(rotate_tmp);
  rotate_z = double(ui->turnZBox->value());
  emit turn_changed();

  move_prev();
  scale_prev();
}

void MainWindow::on_scaleSlider_sliderMoved(int position) {
  if (position > 50) {
    ui->scaleBox->setValue((position - 46) * 0.2);
  } else if (position == 50) {
    ui->scaleBox->setValue(1);
  } else {
    ui->scaleBox->setValue(position / 50.0);
  }
  ui->picture->copy_image(ui->picture->get_figure(), ui->picture->get_image());
  Scale scale_tmp = {.kx = ui->scaleBox->value(),
                     .ky = ui->scaleBox->value(),
                     .kz = ui->scaleBox->value()};
  ui->picture->get_image().ScaleFigure(scale_tmp);
  scale = ui->scaleBox->value();
  emit scale_changed();

  turn_prev();
  move_prev();
}

void MainWindow::on_scaleBox_editingFinished() {
  double value = ui->scaleBox->value();
  if (value > 1) {
    ui->scaleSlider->setValue(value * 5 + 46);
  } else if (value == 1) {
    ui->scaleSlider->setValue(50);
  } else {
    ui->scaleSlider->setValue(value * 50);
  }

  ui->picture->copy_image(ui->picture->get_figure(), ui->picture->get_image());
  Scale scale_tmp = {.kx = ui->scaleBox->value(),
                     .ky = ui->scaleBox->value(),
                     .kz = ui->scaleBox->value()};
  ui->picture->get_image().ScaleFigure(scale_tmp);
  scale = ui->scaleBox->value();
  emit scale_changed();

  turn_prev();
  move_prev();
}

void MainWindow::model_information(FigureFacade &figure) {
  QString info = "File: " + file_name.last(file_name.length() -
                                           file_name.lastIndexOf('/') - 1);
  info += "; Vertices: " + QString::number(figure.points_.array_points_.size());
  info +=
      "; Edges: " + QString::number(figure.edges_.array_edges_.size()) + ".";
  ui->statusbar->showMessage(info);
}

void MainWindow::on_MoveXCamera_sliderMoved(int position)
{
    emit camera_changed(position - 50, 'x');
}


void MainWindow::on_MoveYCamera_sliderMoved(int position)
{
    emit camera_changed(position - 50, 'y');
}


void MainWindow::on_MoveXLight_sliderMoved(int position)
{
    emit light_changed(position - 50, 'x');
}


void MainWindow::on_MoveYLight_sliderMoved(int position)
{
    emit light_changed(position - 50, 'y');
}


void MainWindow::on_MoveZLight_sliderMoved(int position)
{
    emit light_changed(position - 50, 'z');
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    double sc = scale;
    if (value > 50) {
     sc += (value - 46) * 0.2;
    } else if (value == 50) {
      sc += 1;
    } else {
      sc += value / 50.0;
    }
    ui->picture->copy_image(ui->picture->get_figure(), ui->picture->get_image());
    Scale scale_tmp = {.kx = sc,
                       .ky = sc,
                       .kz = sc};
    ui->picture->get_image().ScaleFigure(scale_tmp);

    emit scale_changed();

    turn_prev();
    move_prev();
}


void MainWindow::on_pushButton_clicked()
{
    to_default();
    emit clear_scene();
}



void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    color = index;
    emit color_changed(color);
}

