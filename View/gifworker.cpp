#include "gifworker.h"

namespace s21 {
GifWorker::GifWorker(QObject *parent)
    : frame_count(0),
      gif_wd(nullptr),
      gif_screencast(nullptr),
      gif_filename("") {}

void GifWorker::appendImage(QImage screenshot) { img = screenshot; }

void GifWorker::gifSave() {
  gif_screencast->save(gif_filename);
  emit endOfRecord();
}

void GifWorker::setSavePath(QString *filename) { gif_filename = *filename; }

void GifWorker::setFrameCount(int frames) { frame_count = frames; }

void GifWorker::run() {
  gif_screencast = new QGifImage();
  gif_screencast->setDefaultDelay(10);
  while (frame_count-- > 0) {
    img = img.scaled(640, 480, Qt::IgnoreAspectRatio);
    gif_screencast->addFrame(img);
    emit giveMeMoreScreenshots();
    msleep(100);
  }
  gifSave();
  emit endOfRecord();
  delete gif_screencast;
}
}  // namespace s21
