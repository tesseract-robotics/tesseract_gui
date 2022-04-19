#include <QApplication>
#include <QDebug>

#include "minimal_example.h"

int main(int argc, char** argv)
{
  QApplication app(argc, argv);

  IssueRenderWidget widget;
  widget.show();

  return app.exec();
}
