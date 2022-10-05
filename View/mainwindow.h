#ifndef SRC_VIEW_MAINWINDOW_H_
#define SRC_VIEW_MAINWINDOW_H_

#include <QCloseEvent>
#include <QColorDialog>
#include <QDial>
#include <QFileDialog>
#include <QIcon>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QScreen>
#include <QTimer>
#include <cmath>

#include "giflib/gifimage/qgifimage.h"
#include "gifworker.h"
#include "memento.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  // кнопки на главной панели инстрментов
  void on_actionOpen_File_triggered();
  void on_actionRemove_model_triggered();
  void on_actionRecord_Screencast_in_GIF_triggered();
  void on_actionMake_screenshot_triggered();
  void on_actionReset_model_triggered();
  void on_actionAdd_texture_triggered();

  // кнопки на панели инструментов модели
  void on_actionMove_model_triggered();
  void on_actionRotate_model_triggered();
  void on_actionExpand_model_triggered();
  void on_actionColors_triggered();
  void on_actionStyle_of_lines_and_vertices_triggered();
  void setIconRecord();
  void setIconPreviewRecord();

  // кнопки настройки цветов
  void on_backgroundColorButton_clicked();
  void on_surfaceColorButton_clicked();
  void on_vertexColorButton_clicked();
  void on_linesColorButton_clicked();

  /* К каждому слайдеру выведено по 2 функции - одна на изменение значения, а
   * вторая на факт нажатия. Это нужно, чтобы сообщение о том, какая у нас
   * сейчас величина на слайдере выводилось даже если мы ничего не изменили, а
   * просто ткнули на бегунок */
  // Бегунки перемещения
  void on_slider_x_move_valueChanged(int value);
  void on_slider_x_move_sliderPressed();
  void on_spinBox_x_move_valueChanged(int arg1);
  void on_slider_y_move_valueChanged(int value);
  void on_slider_y_move_sliderPressed();
  void on_spinBox_y_move_valueChanged(int arg1);
  void on_slider_z_move_valueChanged(int value);
  void on_slider_z_move_sliderPressed();
  void on_spinBox_z_move_valueChanged(int arg1);

  // Бегунки вращения
  void on_slider_x_rotation_valueChanged(int value);
  void on_slider_x_rotation_sliderPressed();
  void on_spinBox_x_rotation_valueChanged(int arg1);
  void on_slider_y_rotation_valueChanged(int value);
  void on_slider_y_rotation_sliderPressed();
  void on_spinBox_y_rotation_valueChanged(int arg1);
  void on_slider_z_rotation_valueChanged(int value);
  void on_slider_z_rotation_sliderPressed();
  void on_spinBox_z_rotation_valueChanged(int arg1);

  // Бегунок масштабирования
  void on_slider_scaling_valueChanged(int value);
  void on_slider_scaling_sliderPressed();
  void on_spinBox_scaling_valueChanged(int arg1);

  void on_actionProjection_type_triggered();  // кнопка типа проекции
  void on_actionLight_options_triggered();  // кнопка параметров света

  // Типы вершин
  void on_radio_form_none_clicked();
  void on_radio_form_square_clicked();
  void on_radio_form_circle_clicked();

  void on_actionDraw_model_triggered();  // кнопка отрисовки модели
  void on_actionRecord_Preview_triggered();  // кнопка записи превью
  void on_actionManual_triggered();  // кнопка информации

  void on_comboBox_drawingType_activated(int index);
  void on_slider_vertex_size_valueChanged(int value);
  void on_radio_line_dashed_clicked();
  void on_radio_line_solid_clicked();
  void on_slider_line_thickness_valueChanged(int value);

  void closeEvent(QCloseEvent *event);
  void on_circle_light_position_valueChanged(int value);
  void on_slider_light_position_valueChanged(int value);
  void on_actionLight_On_Off_triggered();
  void on_horizontalSlider_sliderMoved(int position);
  void on_slider_light_red_valueChanged(int value);
  void on_slider_light_green_valueChanged(int value);
  void on_slider_light_blue_valueChanged(int value);

  void grabAndAppendImageToGif();
  void on_radioSmooth_clicked();
  void on_radioFlat_clicked();

 private:
  enum ControlPages {
    MovePanel,
    RotatePanel,
    ScalingPanel,
    ColorPanel,
    LineStylePanel,
    LightPanel
  };  // страницы с элементами управления в QStackedWidget

  enum ProjectionType { ParallelProjection, CentralProjection };
  enum VertexForm { None = -1, Square, Circle };
  enum LineForm { SolidLine, DashedLine };

  void setNewSettings();
  void deleteMem();

  Ui::MainWindow *ui;
  QColorDialog *colors_window;  // диалоговое окно с палитрой для выбора цветов
  QString model_file;           // имя файла с моделью
  QString texture_file;         // имя файла с текстурой
  QString gif_file;
  QString screenshot_file;
  QString config_file;

  QLabel *message_label;  // сообщение в статус-баре
  QLabel *info_label;  // сообщения об изменении состояния элементов управления

  QColor background_color;
  QColor surface_color;
  QColor vertices_color;
  QColor lines_color;

  ProjectionType projection_type;
  bool light_on_off;
  double move_x_value;
  double move_y_value;
  double move_z_value;
  double rotate_x_value;
  double rotate_y_value;
  double rotate_z_value;
  double scale_model_value;
  double linesSize;
  int formModelDraw;
  double x_light;
  double y_light;
  bool m_light;
  float m_colorLightR;
  float m_colorLightG;
  float m_colorLightB;
  bool m_flatShading;
  s21::Controller &m_ctr = s21::Controller::Instance();
  // gif block
  GifWorker *gif_worker;
  bool preview_switched;  // флажок для записи превью
  unsigned int frames_in_gif;
  double angle;  // угол поворота модели при записи превью
  QList<QAction *> lst;  // список действий на панели
  void
  gifRecorder();  // передает в поток записи GIF нужные данные и запускает его
  // gif block

  VertexForm vertex_form;
  LineForm line_form;

  void change_panels(ControlPages n);  // выбор страницы в QStackedWidget в
                                       // зависимости от её номера
  void save_config();  // конфиг сохраняется но не читается пока что
  void read_config();
  Memento *mem;
};
}  // namespace s21
#endif  //  SRC_VIEW_MAINWINDOW_H_
