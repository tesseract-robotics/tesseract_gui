#ifndef TESSERACT_GUI_KINEMATIC_GROUP_EDITOR_WIDGET_H
#define TESSERACT_GUI_KINEMATIC_GROUP_EDITOR_WIDGET_H


#include <tesseract_common/macros.h>
TESSERACT_COMMON_IGNORE_WARNINGS_PUSH
#ifndef Q_MOC_RUN
#include <tesseract_srdf/kinematics_information.h>
#include <memory>
#endif
TESSERACT_COMMON_IGNORE_WARNINGS_POP

#include <tesseract_gui/widgets/kinematic_groups/kinematic_groups_model.h>
#include <QWidget>
#include <QStandardItemModel>
#include <QStringListModel>

namespace Ui {
class KinematicGroupsEditorWidget;
}

namespace tesseract_gui
{
using ChainGroupValidator = std::function<bool(QString, QString)>;
using JointGroupValidator = std::function<bool(QStringList)>;
using LinkGroupValidator = std::function<bool(QStringList)>;

class KinematicGroupsEditorWidget : public QWidget
{
  Q_OBJECT

public:
  explicit KinematicGroupsEditorWidget(QStringList joint_names,
                                      QStringList link_names,
                                      ChainGroupValidator chain_group_validator = nullptr,
                                      JointGroupValidator joint_group_validator = nullptr,
                                      LinkGroupValidator link_group_validator = nullptr,
                                      QWidget *parent = nullptr);
  ~KinematicGroupsEditorWidget();

  void setModel(KinematicGroupsModel* model);

public Q_SLOTS:
  void onAddGroup();
  void onRemoveGroup();
  void onAddJoint();
  void onRemoveJoint();
  void onAddLink();
  void onRemoveLink();

private:
  std::unique_ptr<Ui::KinematicGroupsEditorWidget> ui_;
  QStringListModel link_names_model_;
  QStringListModel joint_names_model_;
  KinematicGroupsModel* group_model_;
  ChainGroupValidator chain_group_validator_;
  JointGroupValidator joint_group_validator_;
  LinkGroupValidator link_group_validator_;
};
}
#endif // TESSERACT_GUI_KINEMATIC_GROUP_EDITOR_WIDGET_H
