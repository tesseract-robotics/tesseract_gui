
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <QApplication>
#include <QDebug>
#include <sstream>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_gui/acm/allowed_collision_matrix_model.h>
#include <tesseract_gui/acm/allowed_collision_matrix_widget.h>

void onEntrySelected(const tesseract_common::AllowedCollisionEntries& selection)
{
  std::stringstream ss;
  ss << "Selected Rows:" << std::endl;
  for (const auto& s : selection)
    ss << "     (" << s.first.first << ", " << s.first.second << ", " << s.second << ")" << std::endl;

  qDebug() << QString::fromStdString(ss.str());
}

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    std::vector<std::string> links {"link_1", "link_2", "link_3", "link_4"};

    tesseract_gui::AllowedCollisionMatrixModel model;
    model.add("link_1", "link_2", "Adjacent");
    model.add("link_2", "link_3", "Adjacent");
    model.add("link_3", "link_4", "Adjacent");

    tesseract_gui::AllowedCollisionMatrixWidget widget;
    QObject::connect(&widget, &tesseract_gui::AllowedCollisionMatrixWidget::entrySelected, &onEntrySelected);

    widget.setModel(&model);
    widget.show();

    return app.exec();
}
