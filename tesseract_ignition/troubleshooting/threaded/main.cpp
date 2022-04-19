#include <QApplication>
#include <QDebug>

#include "renderer.h"

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  GLWidget widget;
  widget.show();

  return app.exec();
}
