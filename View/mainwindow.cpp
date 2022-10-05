#include "mainwindow.h"

#include "eye.h"
#include "simpleobject3d.h"
#include "ui_mainwindow.h"
#include "widget3d.h"
namespace s21 {
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  /* Gifs connect */
  gif_worker = new GifWorker();
  gif_worker->moveToThread(gif_worker);
  connect(gif_worker, SIGNAL(endOfRecord()), this, SLOT(setIconRecord()));
  connect(gif_worker, SIGNAL(endOfRecord()), this,
          SLOT(setIconPreviewRecord()));
  connect(gif_worker, SIGNAL(giveMeMoreScreenshots()), this,
          SLOT(grabAndAppendImageToGif()));
  preview_switched = false;

  lst = ui->mainToolBar->actions();
  frames_in_gif = 50;
  /* Gifs connect */

  QScreen *screen =
      QGuiApplication::primaryScreen();  // здесь и следующие 4 строки - задание
                                         // размера в пропорциях относительно
                                         // экрана устройства, на котором
                                         // открыта программа
  QRect screenGeometry = screen->geometry();
  int height = screenGeometry.height();
  int width = screenGeometry.width();
  resize(width * 0.8, height * 0.9);

  ui->controls->hide();  // панель инструментов по умолчанию скрыта
  colors_window = new QColorDialog();  // окно выбора цвета из палитры как
                                       // объект создается здесь

  message_label =
      new QLabel("No files opened");  // сообщение с именем открываемого файла
  info_label = new QLabel("");  // общие информационные собщения
  ui->statusbar->addWidget(message_label);  // добавляем сообщения в статус-бар
  ui->statusbar->addWidget(info_label);
  config_file = QCoreApplication::applicationDirPath() + "config.conf";
  read_config();

  // кнопки выбора цветов раскрашиваются по умолчанию
  QString qss = QString("background-color: %1;\ncolor:white;")
                    .arg(background_color.name());
  ui->backgroundColorButton->setStyleSheet(qss);
  qss = QString("background-color: %1;").arg(surface_color.name());
  ui->surfaceColorButton->setStyleSheet(qss);
  qss = QString("background-color: %1").arg(vertices_color.name());
  ui->vertexColorButton->setStyleSheet(qss);
  qss = QString("background-color: %1").arg(lines_color.name());
  ui->linesColorButton->setStyleSheet(qss);
  m_light = true;
  m_colorLightR = 1.0f;
  m_colorLightG = 1.0f;
  m_colorLightB = 1.0f;
  mem = nullptr;
}

MainWindow::~MainWindow() {
  if (message_label) delete message_label;
  if (colors_window) delete colors_window;
  if (info_label) delete info_label;
  if (gif_worker) delete gif_worker;
  delete ui;
  deleteMem();
}

void MainWindow::on_actionOpen_File_triggered() {
  model_file = QFileDialog::getOpenFileName(this, "Select a file to open...",
                                            QDir::homePath(), "*.obj");
  message_label->setText(model_file);
  deleteMem();
  if (ui->openGLWidget->addNewModel(model_file) == true) {
    QMessageBox msg;
    QIcon icn =
        QIcon(":/mainIcons/View/mainIcons/free-icon-cube-7355507-texture.png");
    msg.setIconPixmap(icn.pixmap(icn.actualSize(QSize(32, 32))));
    msg.setText("Press button to draw model");
    msg.exec();
    lst[8]->setIcon(
        QIcon(":/mainIcons/View/mainIcons/free-icon-cube-7355507-texture.png"));
  }
}

void MainWindow::on_actionDraw_model_triggered() {
  if (!model_file.isNull() && !model_file.isEmpty()) {
    ui->openGLWidget->LoadNewModel();
    setNewSettings();
    if (ui->openGLWidget->getm_Oobjects().size() > 0 &&
        ui->openGLWidget->getm_Oobjects().at(0) != nullptr) {
      mem = new Memento(
          background_color, surface_color, lines_color, vertices_color,
          vertex_form, ui->slider_vertex_size->value(), line_form,
          ui->slider_line_thickness->value() / 5, projection_type);
    }
    ui->openGLWidget->update();
  }
}

void MainWindow::setNewSettings() {
  QVector<SimpleObject3D *> vecObj = ui->openGLWidget->getm_Oobjects();
  ui->openGLWidget->setColorBack(
      QVector4D(background_color.redF(), background_color.greenF(),
                background_color.blueF(), background_color.alphaF()));

  for (auto obj : vecObj) {
    obj->setColorSurf(QVector4D(surface_color.redF(), surface_color.greenF(),
                                surface_color.blueF(), surface_color.alphaF()));
    obj->setColorLine(QVector4D(lines_color.redF(), lines_color.greenF(),
                                lines_color.blueF(), lines_color.alphaF()));

    obj->setColorDot(QVector4D(vertices_color.redF(), vertices_color.greenF(),
                               vertices_color.blueF(),
                               vertices_color.alphaF()));

    obj->pointsFoo(ui->slider_vertex_size->value(), vertices_color,
                   vertex_form);
    obj->lineWidth(ui->slider_line_thickness->value() / 5);
    obj->lineDash(line_form);
    ui->openGLWidget->setProjection(projection_type);
  }
}

void MainWindow::deleteMem() {
  if (mem != nullptr) {
    delete mem;
    mem = nullptr;
  }
}

void MainWindow::on_actionRemove_model_triggered() {
  message_label->setText("No files opened");
  ui->radio_form_none->setChecked(true);
  vertex_form = None;
  ui->slider_vertex_size->setValue(0);
  if (!model_file.isNull() && !model_file.isEmpty()) {
    ui->openGLWidget->deletModel();
    model_file.clear();
    deleteMem();
    lst[8]->setIcon(
        QIcon(":/mainIcons/View/mainIcons/free-icon-cube-7355507.png"));
  }
}

void MainWindow::setIconRecord() {
  lst[2]->setIcon(
      QIcon(":/mainIcons/View/mainIcons/premium-icon-record-4241010.png"));
}

void MainWindow::setIconPreviewRecord() {
  lst[3]->setIcon(QIcon(":/mainIcons/View/mainIcons/rotate-camera.png"));
}

void MainWindow::on_actionRecord_Screencast_in_GIF_triggered() {
  preview_switched = false;
  lst[2]->setIcon(QIcon(":/mainIcons/View/mainIcons/japanese-flag-9023.png"));
  gifRecorder();
}

void MainWindow::on_actionRecord_Preview_triggered() {
  preview_switched = true;
  ui->slider_y_rotation->setValue(0);
  angle = ui->slider_y_rotation->value();
  lst[3]->setIcon(QIcon(":/mainIcons/View/mainIcons/japanese-flag-9023.png"));
  gifRecorder();
}

void MainWindow::on_actionMake_screenshot_triggered() {
  QImage screenshot = ui->openGLWidget->grabFramebuffer();
  QFileDialog *filesavedialog =
      new QFileDialog(this, "Save file as...", QDir::homePath(), "*.bmp *.jpg");
  filesavedialog->show();
  QString savepath = filesavedialog->getSaveFileName();
  if (savepath.endsWith(".bmp")) {
    screenshot.save(savepath, "bmp");
    info_label->setText("New filе .bmp saved");
  } else if (savepath.endsWith(".jpg")) {
    screenshot.save(savepath, "jpg");
    info_label->setText("New filе .jpg saved");
  } else {
    QMessageBox msg;
    msg.setText("Invalid extension");
    msg.exec();
    info_label->setText("Screenshot file saved error");
  }
  filesavedialog->close();
}

void MainWindow::on_actionReset_model_triggered() {
  if (!model_file.isNull() && !model_file.isEmpty()) {
    if (mem != nullptr) {
      background_color = mem->BackgroundColor();
      surface_color = mem->SurfaceColor();
      lines_color = mem->LinessColor();
      vertices_color = mem->VerticesColor();
      vertex_form = (VertexForm)mem->VertexcesForm();
      ui->slider_vertex_size->setValue(mem->VertexcesSize());
      ui->slider_line_thickness->setValue(mem->LineThickness());
      line_form = (LineForm)mem->LineForm();
      projection_type = (ProjectionType)mem->ProjectionType();
      for (auto obj : ui->openGLWidget->getm_Oobjects()) {
        obj->changeMatrix();
      }
      setNewSettings();
      ui->openGLWidget->changeEye();
    }
    ui->openGLWidget->update();
  }
}

void MainWindow::on_actionMove_model_triggered() { change_panels(MovePanel); }

void MainWindow::on_actionRotate_model_triggered() {
  change_panels(RotatePanel);
}

void MainWindow::on_actionExpand_model_triggered() {
  change_panels(ScalingPanel);
}

void MainWindow::on_actionColors_triggered() { change_panels(ColorPanel); }

void MainWindow::on_actionStyle_of_lines_and_vertices_triggered() {
  change_panels(LineStylePanel);
}

void MainWindow::on_actionLight_options_triggered() {
  change_panels(LightPanel);
}

void MainWindow::on_backgroundColorButton_clicked() {
  colors_window->open();
  background_color = colors_window->getColor();
  if (background_color.isValid()) {
    ui->openGLWidget->setColorBack(
        QVector4D(background_color.redF(), background_color.greenF(),
                  background_color.blueF(), background_color.alphaF()));
    QString qss = QString("background-color: %1;").arg(background_color.name());
    if (background_color.black() > 127) qss += "color:white;";
    ui->backgroundColorButton->setStyleSheet(qss);
  }
  info_label->setText(
      "Background color R: " + QString::number(background_color.red()) +
      " G: " + QString::number(background_color.green()) +
      " B: " + QString::number(background_color.blue()) +
      " Alpha: " + QString::number(background_color.alpha()));
}

void MainWindow::on_surfaceColorButton_clicked() {
  colors_window->open();
  surface_color = colors_window->getColor();
  if (surface_color.isValid()) {
    for (auto obj : ui->openGLWidget->getm_Oobjects()) {
      obj->setColorSurf(QVector4D(surface_color.redF(), surface_color.greenF(),
                                  surface_color.blueF(),
                                  surface_color.alphaF()));
    }
    ui->openGLWidget->update();
    QString qss = QString("background-color: %1;\n").arg(surface_color.name());
    if (surface_color.black() > 127) qss += "color:white;";
    ui->surfaceColorButton->setStyleSheet(qss);
  }
  info_label->setText(
      "Surface color R: " + QString::number(surface_color.red()) +
      " G: " + QString::number(surface_color.green()) +
      " B: " + QString::number(surface_color.blue()) +
      " Alpha: " + QString::number(surface_color.alpha()));
}

void MainWindow::on_vertexColorButton_clicked() {
  colors_window->open();
  vertices_color = colors_window->getColor();
  if (vertices_color.isValid()) {
    for (auto obj : ui->openGLWidget->getm_Oobjects()) {
      obj->setColorDot(QVector4D(vertices_color.redF(), vertices_color.greenF(),
                                 vertices_color.blueF(),
                                 vertices_color.alphaF()));
    }
    ui->openGLWidget->update();

    QString qss = QString("background-color: %1;\n").arg(vertices_color.name());
    if (vertices_color.black() > 127) qss += "color:white;";
    ui->vertexColorButton->setStyleSheet(qss);
  }
  info_label->setText(
      "Vertices color R: " + QString::number(vertices_color.red()) +
      " G: " + QString::number(vertices_color.green()) +
      " B: " + QString::number(vertices_color.blue()) +
      " Alpha: " + QString::number(vertices_color.alpha()));
}

void MainWindow::on_linesColorButton_clicked() {
  colors_window->open();
  lines_color = colors_window->getColor();
  if (lines_color.isValid()) {
    for (auto obj : ui->openGLWidget->getm_Oobjects()) {
      obj->setColorLine(QVector4D(lines_color.redF(), lines_color.greenF(),
                                  lines_color.blueF(), lines_color.alphaF()));
    }
    ui->openGLWidget->update();

    QString qss = QString("background-color: %1;\n").arg(lines_color.name());
    if (lines_color.black() > 127) qss += "color:white;";
    ui->linesColorButton->setStyleSheet(qss);
  }
  info_label->setText("Lines color R: " + QString::number(lines_color.red()) +
                      " G: " + QString::number(lines_color.green()) +
                      " B: " + QString::number(lines_color.blue()) +
                      " Alpha: " + QString::number(lines_color.alpha()));
}

void MainWindow::change_panels(ControlPages n) {
  if (ui->controls->currentIndex() != n) {
    ui->controls->setCurrentIndex(n);
    ui->controls->show();
  } else {
    if (ui->controls->isHidden()) {
      ui->controls->show();
    } else {
      ui->controls->hide();
    }
  }
}

void MainWindow::save_config() {
    QFile config(config_file);
      if (config.open(QIODevice::ReadWrite)) {
        QTextStream stream(&config);
        stream << "background_color:" << background_color.name() << "\n";
        stream << "surface_color:" << surface_color.name() << "\n";
        stream << "lines_color:" << lines_color.name() << "\n";
        stream << "vertices_color:" << vertices_color.name() << "\n";
        stream << "vertex_form:" << vertex_form << "\n";
        stream << "line_form:" << line_form << "\n";
        stream << "vertex_size:" << ui->slider_vertex_size->value() << "\n";
        stream << "line_thickness:" << ui->slider_line_thickness->value() << "\n";
        stream << "projection_type:" << projection_type << "\n";
        stream << "light:" << m_light << "/n";
        stream << "color_blue:" << m_colorLightB << "\n";
        stream << "color_green:" << m_colorLightG << "\n";
        stream << "color_red:" << m_colorLightR << "\n";
        stream << "slider_ligth_x:" << x_light << "\n";
        stream << "slider_ligth_y:" << y_light << "\n";
        stream << "circle_ligth_x:" << x_light << "\n";
        stream << "circle_ligth_y:" << x_light << "\n";
      }
      config.close();
}

void MainWindow::read_config() {
  QFile config(config_file);
  if (config.open(QIODevice::ReadOnly)) {
    QTextStream stream(&config);
    while (!stream.atEnd()) {
      QString line = stream.readLine();
      if (line.startsWith("background_color:"))
        background_color.setNamedColor(line.split(':')[1]);
      if (line.startsWith("surface_color:"))
        surface_color.setNamedColor(line.split(':')[1]);
      if (line.startsWith("lines_color:"))
        lines_color.setNamedColor(line.split(':')[1]);
      if (line.startsWith("vertices_color:"))
        vertices_color.setNamedColor(line.split(':')[1]);
      if (line.startsWith("vertex_form:"))
        vertex_form = VertexForm(line.split(':')[1].toInt());
      if (line.startsWith("line_form:"))
        line_form = LineForm(line.split(':')[1].toInt());
      if (line.startsWith("vertex_size:"))
        ui->slider_vertex_size->setValue(line.split(':')[1].toInt());
      if (line.startsWith("line_thickness:"))
        ui->slider_line_thickness->setValue(line.split(':')[1].toInt());
      if (line.startsWith("projection_type:"))
        projection_type = ProjectionType(line.split(':')[1].toInt());
      if (line.startsWith("light:")) {
          if (line.split(':')[1] == "true") {
              m_light = true;
          } else {
              m_light = false;
          }
      }
      if (line.startsWith("color_blue:"))
          m_colorLightB = ProjectionType(line.split(':')[1].toFloat());
      if (line.startsWith("color_green:"))
          m_colorLightG = ProjectionType(line.split(':')[1].toFloat());
      if (line.startsWith("color_red:"))
          m_colorLightR = ProjectionType(line.split(':')[1].toFloat());
      if (line.startsWith("slider_ligth_x:"))
          x_light = ProjectionType(line.split(':')[1].toFloat());
      if (line.startsWith("slider_ligth_y:"))
          y_light = ProjectionType(line.split(':')[1].toFloat());
      if (line.startsWith("circle_ligth_x:"))
          x_light = ProjectionType(line.split(':')[1].toFloat());
      if (line.startsWith("circle_ligth_y:"))
          y_light = ProjectionType(line.split(':')[1].toFloat());
    }
  } else {
    background_color = "black";
    surface_color = "red";
    vertices_color = "yellow";
    lines_color = "green";
    ui->slider_vertex_size->setValue(0.0);
    linesSize = 2.0;
    projection_type = CentralProjection;
  }
  config.close();
}

void MainWindow::on_actionAdd_texture_triggered() {
  auto lst = ui->mainToolBar->actions();
  if (texture_file.isEmpty()) {
    texture_file = QFileDialog::getOpenFileName(
        this, "Select a file to open...", QDir::homePath(), "*.bmp");
    if (!texture_file.isEmpty()) {
      lst[7]->setIcon(QIcon(":/mainIcons/View/mainIcons/wall.png"));
      lst[7]->setToolTip("Remove texture");

      for (auto obj : ui->openGLWidget->getm_Oobjects()) {
        obj->setForm(0x0004);
        obj->setColorLine(QVector4D(0.0, 0.0, 0.0, 0.0));
        obj->initTexture(texture_file);
        obj->setColorSurf(QVector4D(0.0, 0.0, 0.0, 0.0));
      }
      ui->openGLWidget->update();
    }

  } else {
    texture_file.clear();
    lst[7]->setIcon(QIcon(":/mainIcons/View/mainIcons/wall-2.png"));
    lst[7]->setToolTip("Add texture");

    for (auto obj : ui->openGLWidget->getm_Oobjects()) {
      obj->setForm(formModelDraw);
      obj->setColorLine(QVector4D(lines_color.redF(), lines_color.greenF(),
                                  lines_color.blueF(), lines_color.alphaF()));
      obj->delTexture();
      obj->setColorSurf(QVector4D(surface_color.redF(), surface_color.greenF(),
                                  surface_color.blueF(),
                                  surface_color.alphaF()));
      obj->setForm(GL_LINES);
    }
    ui->comboBox_drawingType->setCurrentIndex(1);
    ui->openGLWidget->update();
  }
}

void MainWindow::on_actionProjection_type_triggered() {
  auto lst = ui->mainToolBar->actions();
  if (projection_type == ParallelProjection) {
    lst[9]->setIcon(
        QIcon(":/mainIcons/View/mainIcons/rays-of-light-pngrepo-com.png"));
    lst[9]->setToolTip("Central projection");
    projection_type = CentralProjection;
  } else if (projection_type == CentralProjection) {
    lst[9]->setIcon(
        QIcon(":/mainIcons/View/mainIcons/icons8-menu-rounded-96.png"));
    lst[9]->setToolTip("Parallel projection");
    projection_type = ParallelProjection;
  }
  ui->openGLWidget->setProjection(projection_type);
  ui->openGLWidget->update();
}

void MainWindow::on_slider_x_move_valueChanged(int value) {
  ui->spinBox_x_move->setValue(value);
  info_label->setText("X: " + QString::number(value));
}

void MainWindow::on_slider_x_move_sliderPressed() {
  int value = ui->slider_x_move->value();
  info_label->setText("X: " + QString::number(value));
}

void MainWindow::on_slider_y_move_valueChanged(int value) {
  ui->spinBox_y_move->setValue(value);
  info_label->setText("Y: " + QString::number(value));
}

void MainWindow::on_slider_y_move_sliderPressed() {
  int value = ui->slider_y_move->value();
  info_label->setText("Y: " + QString::number(value));
}

void MainWindow::on_slider_z_move_valueChanged(int value) {
  ui->spinBox_z_move->setValue(value);
  info_label->setText("Z: " + QString::number(value));
}

void MainWindow::on_slider_z_move_sliderPressed() {
  int value = ui->slider_z_move->value();
  info_label->setText("Z: " + QString::number(value));
}

void MainWindow::on_slider_x_rotation_valueChanged(int value) {
  ui->spinBox_x_rotation->setValue(value);
  info_label->setText("X axis rotation: " + QString::number(value) +
                      " degrees");
}

void MainWindow::on_slider_x_rotation_sliderPressed() {
  int value = ui->slider_x_rotation->value();
  info_label->setText("X axis rotation: " + QString::number(value) +
                      " degrees");
}

void MainWindow::on_slider_y_rotation_valueChanged(int value) {
  ui->spinBox_y_rotation->setValue(value);
  info_label->setText("Y axis rotation: " + QString::number(value) +
                      " degrees");
}

void MainWindow::on_slider_y_rotation_sliderPressed() {
  int value = ui->slider_y_rotation->value();
  info_label->setText("Y axis rotation: " + QString::number(value) +
                      " degrees");
}

void MainWindow::on_slider_z_rotation_valueChanged(int value) {
  ui->spinBox_z_rotation->setValue(value);
  info_label->setText("Z axis rotation: " + QString::number(value) +
                      " degrees");
}

void MainWindow::on_slider_z_rotation_sliderPressed() {
  int value = ui->slider_z_rotation->value();
  info_label->setText("Z axis rotation: " + QString::number(value) +
                      " degrees");
}

void MainWindow::on_slider_scaling_valueChanged(int value) {
  ui->spinBox_scaling->setValue(value);
  info_label->setText("Scale factor: " + QString::number(value));
}

void MainWindow::on_slider_scaling_sliderPressed() {
  int value = ui->slider_scaling->value();
  info_label->setText("Scale factor: " + QString::number(value));
}

void MainWindow::on_radio_form_square_clicked() {
  vertex_form = Square;
  for (auto obj : ui->openGLWidget->getm_Oobjects()) {
    obj->pointsFoo(ui->slider_vertex_size->value(), vertices_color,
                   vertex_form);
  }
  ui->openGLWidget->update();
}

void MainWindow::on_radio_form_none_clicked() {
  vertex_form = None;
  for (auto obj : ui->openGLWidget->getm_Oobjects()) {
    obj->pointsFoo(0, vertices_color, vertex_form);
  }

  ui->openGLWidget->update();
}

void MainWindow::on_radio_form_circle_clicked() {
  vertex_form = Circle;
  for (auto obj : ui->openGLWidget->getm_Oobjects()) {
    obj->pointsFoo(ui->slider_vertex_size->value(), vertices_color,
                   vertex_form);
  }

  ui->openGLWidget->update();
}

void MainWindow::on_comboBox_drawingType_activated(int index) {
  if (index == 0) {
    formModelDraw = -1;
    for (auto obj : ui->openGLWidget->getm_Oobjects()) {
      obj->setPointSize(1);
    }
  } else if (index == 1) {
    formModelDraw = GL_LINES;
  } else if (index == 2) {
    formModelDraw = GL_LINE_LOOP;
  } else if (index == 3) {
    formModelDraw = GL_TRIANGLES;
  } else if (index == 4) {
    formModelDraw = GL_QUADS;
  } else if (index == 5) {
    formModelDraw = GL_POLYGON;
  }
  for (auto obj : ui->openGLWidget->getm_Oobjects()) {
    obj->setForm(formModelDraw);
  }
  ui->openGLWidget->update();
}

void MainWindow::on_slider_vertex_size_valueChanged(int value) {
  for (auto obj : ui->openGLWidget->getm_Oobjects()) {
    obj->pointsFoo(value, vertices_color, vertex_form);
  }
  ui->openGLWidget->update();
}

void MainWindow::on_actionManual_triggered() {
  QMessageBox msg;
  QPair<long, long> rez = m_ctr.getInfo();
  msg.setText("Filename: " + model_file +
              "\nNumber of vertices: " + QString::number(rez.first) +
              "\nNumber of surfaces: " + QString::number(rez.second));
  msg.exec();
}

void MainWindow::on_slider_line_thickness_valueChanged(int value) {
  for (auto obj : ui->openGLWidget->getm_Oobjects()) {
    obj->lineWidth(value / 5);
  }
  ui->openGLWidget->update();
}

void MainWindow::on_radio_line_solid_clicked() {
  line_form = SolidLine;
  for (auto obj : ui->openGLWidget->getm_Oobjects()) {
    obj->lineDash(line_form);
  }
  ui->openGLWidget->update();
}

void MainWindow::on_radio_line_dashed_clicked() {
  line_form = DashedLine;
  for (auto obj : ui->openGLWidget->getm_Oobjects()) {
    obj->lineDash(line_form);
  }
  ui->openGLWidget->update();
}

void MainWindow::on_spinBox_x_move_valueChanged(int arg1) {
  for (auto obj : ui->openGLWidget->getm_Oobjects()) {
    obj->translateModelX(-(float)arg1 / 5);
  }
  ui->openGLWidget->update();
  ui->slider_x_move->setValue(arg1);
}

void MainWindow::on_spinBox_y_move_valueChanged(int arg1) {
  for (auto obj : ui->openGLWidget->getm_Oobjects()) {
    obj->translateModelY(-(float)arg1 / 5);
  }
  ui->openGLWidget->update();
  ui->slider_y_move->setValue(arg1);
}

void MainWindow::on_spinBox_z_move_valueChanged(int arg1) {
  for (auto obj : ui->openGLWidget->getm_Oobjects()) {
    obj->translateModelZ(-(float)arg1 / 5);
  }
  ui->openGLWidget->update();
  ui->slider_z_move->setValue(arg1);
}

void MainWindow::on_spinBox_x_rotation_valueChanged(int arg1) {
  for (auto obj : ui->openGLWidget->getm_Oobjects()) {
    obj->rotateModelX((float)arg1);
  }
  ui->openGLWidget->update();
  ui->slider_x_rotation->setValue(arg1);
}

void MainWindow::on_spinBox_y_rotation_valueChanged(int arg1) {
  for (auto obj : ui->openGLWidget->getm_Oobjects()) {
    obj->rotateModelY((float)arg1);
  }
  ui->openGLWidget->update();
  ui->slider_y_rotation->setValue(arg1);
}

void MainWindow::on_spinBox_z_rotation_valueChanged(int arg1) {
  for (auto obj : ui->openGLWidget->getm_Oobjects()) {
    obj->rotateModelZ((float)arg1);
  }
  ui->openGLWidget->update();
  ui->slider_z_rotation->setValue(arg1);
}

void MainWindow::on_spinBox_scaling_valueChanged(int arg1) {
  for (auto obj : ui->openGLWidget->getm_Oobjects()) {
    obj->scaleModel(-(float)arg1 / 50);
  }
  ui->openGLWidget->update();

  ui->slider_scaling->setValue(arg1);
}

void MainWindow::closeEvent(QCloseEvent *event) {
  if (event) {
    save_config();
    exit(0);
  }
}

void MainWindow::on_circle_light_position_valueChanged(int value) {
  x_light =
      ui->slider_light_position->value() * cos(float(-value) * M_PI / 180.0f);
  y_light =
      ui->slider_light_position->value() * sin(float(-value) * M_PI / 180.0f);
  ui->openGLWidget->getEye()->setLightPosition(x_light, y_light);
  ui->openGLWidget->update();
}

void MainWindow::on_slider_light_position_valueChanged(int value) {
  x_light = value * cos(ui->circle_light_position->value());
  y_light = value * sin(ui->circle_light_position->value());
  ui->openGLWidget->getEye()->setLightPosition(x_light, y_light);
  ui->openGLWidget->update();
}

void MainWindow::on_actionLight_On_Off_triggered() {
  if (m_light) {
    m_light = false;
    ui->openGLWidget->getEye()->setColorLightRed(1.0f);
    ui->openGLWidget->getEye()->setColorLightGreen(1.0f);
    ui->openGLWidget->getEye()->setColorLightBlue(1.0f);
  } else {
    m_light = true;
    ui->openGLWidget->getEye()->setColorLightRed(m_colorLightR);
    ui->openGLWidget->getEye()->setColorLightGreen(m_colorLightG);
    ui->openGLWidget->getEye()->setColorLightBlue(m_colorLightB);
  }
  ui->openGLWidget->getEye()->lightOnOff(m_light);
  ui->openGLWidget->update();
}

void MainWindow::on_horizontalSlider_sliderMoved(int position) {
  ui->openGLWidget->getEye()->setLightPower(float(position) / 10);
  ui->openGLWidget->update();
}

void MainWindow::on_slider_light_red_valueChanged(int value) {
  m_colorLightR = float(value) / 100;
  ui->openGLWidget->getEye()->setColorLightRed(m_colorLightR);
  ui->openGLWidget->update();
}

void MainWindow::on_slider_light_green_valueChanged(int value) {
  m_colorLightG = float(value) / 100;
  ui->openGLWidget->getEye()->setColorLightGreen(m_colorLightG);
  ui->openGLWidget->update();
}

void MainWindow::on_slider_light_blue_valueChanged(int value) {
  m_colorLightB = float(value) / 100;
  ui->openGLWidget->getEye()->setColorLightBlue(m_colorLightB);
  ui->openGLWidget->update();
}

void MainWindow::gifRecorder() {
  QFileDialog filesavedialog(this, "Save file as...", QDir::homePath(),
                             "*.gif");
  QString savepath = filesavedialog.getSaveFileName();
  if (savepath.isEmpty()) {
    setIconRecord();
    setIconPreviewRecord();
  }
  QImage screenshot = ui->openGLWidget->grabFramebuffer();
  gif_worker->appendImage(screenshot);
  if (savepath.endsWith(".gif")) {
    message_label->setText("gif record in " + savepath);
    gif_worker->setSavePath(&savepath);
    gif_worker->setFrameCount(frames_in_gif);
    gif_worker->moveToThread(gif_worker);
    gif_worker->start();
  } else {
    message_label->setText("gif extension needed");
  }
}

void MainWindow::grabAndAppendImageToGif() {
  if (preview_switched) {
    angle += 360.0 / (frames_in_gif);
    angle = fmod(angle, 360.0);
    ui->slider_y_rotation->setValue(angle);
  }
  QImage screenshot = ui->openGLWidget->grabFramebuffer();
  gif_worker->appendImage(screenshot);
}

void MainWindow::on_radioSmooth_clicked() {
    ui->openGLWidget->getEye()->flatShadingOn(false);
    ui->openGLWidget->update();
}


void MainWindow::on_radioFlat_clicked() {
    ui->openGLWidget->getEye()->flatShadingOn(true);
    ui->openGLWidget->update();
}
}  // namespace s21
