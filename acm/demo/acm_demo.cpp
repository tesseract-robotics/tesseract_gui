
#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#include <QtGui/QtGui>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickItem>
#include <QtQuick/QQuickView>
#include <QDebug>
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_gui/acm/allowed_collision_matrix_model.h>

void onEntrySelected(QString link1_name, QString link2_name, QString reason)
{
  qDebug() << "Row Selected: (" << link1_name << ", " << link2_name << ", " << reason << ")";
}

int main(int argc, char ** argv)
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    std::vector<std::string> links {"link_1", "link_2", "link_3", "link_4"};

    tesseract_gui::AllowedCollisionMatrixModel model;
    model.add("link_1", "link_2", "Adjacent");
    model.add("link_2", "link_3", "Adjacent");
    model.add("link_3", "link_4", "Adjacent");

    QObject::connect(&model, &tesseract_gui::AllowedCollisionMatrixModel::entrySelected, &onEntrySelected);

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.rootContext()->setContextProperty("acmModel", &model);
    view.setSource(QUrl("qrc:/AllowedCollisionMatrix/AllowedCollisionMatrix.qml"));
    view.show();

    return app.exec();
}
