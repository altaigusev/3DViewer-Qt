#ifndef SRC_VIEW_GIFWORKER_H_
#define SRC_VIEW_GIFWORKER_H_

#include <QDir>
#include <QThread>
#include <QTimer>

#include "giflib/gifimage/qgifimage.h"
#include "widget3d.h"

namespace s21 {
class GifWorker : public QThread {
  Q_OBJECT

 public:
  explicit GifWorker(QObject *parent = 0);
  ~GifWorker() {}

  void appendImage(QImage screenshot);
  void setSavePath(QString *filename);
  void setFrameCount(int frames);

 public slots:
  void gifSave();

 signals:
  void endOfRecord();
  void giveMeMoreScreenshots();

 private:
  int frame_count;
  Widget3d *gif_wd;
  QGifImage *gif_screencast;
  QString gif_filename;
  QImage img;

  // QThread interface
 protected:
  virtual void run();
};
}  // namespace s21
#endif  // SRC_VIEW_GIFWORKER_H_
