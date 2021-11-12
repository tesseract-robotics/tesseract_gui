#ifndef TESSERACT_GUI_SCENE_GRAPH_MODEL_H
#define TESSERACT_GUI_SCENE_GRAPH_MODEL_H

#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_scene_graph/joint.h>
#include <QStandardItemModel>
#include <QMetaType>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

//namespace tesseract_gui
//{
//class SceneGraphModel : public QStandardItemModel
//{
//    Q_OBJECT
//public:
//  enum JointRoles {
//      NameRole = Qt::UserRole + 1,
//      MinRole = Qt::UserRole + 2,
//      MaxRole = Qt::UserRole + 3,
//      ValueRole = Qt::UserRole + 4
//  };

//  JointListModel(QObject *parent = nullptr);
//  JointListModel(const JointListModel &other);
//  JointListModel &operator=(const JointListModel &other);
//  ~JointListModel() override = default;

//  Q_INVOKABLE void add(tesseract_scene_graph::Joint::ConstPtr joint);
//  Q_INVOKABLE void clear();

//  QHash<int, QByteArray> roleNames() const override;

//Q_SIGNALS:

//  void jointValueChanged(QString name, double value);
//  void jointMinChanged(QString name, double value);
//  void jointMaxChanged(QString name, double value);

//private:
//  std::vector<tesseract_scene_graph::Joint::ConstPtr> joints_;

//  void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
//};
//}
//Q_DECLARE_METATYPE(tesseract_gui::JointListModel)


#endif // TESSERACT_GUI_SCENE_GRAPH_MODEL_H
