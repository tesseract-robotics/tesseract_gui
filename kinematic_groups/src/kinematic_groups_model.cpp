#include <tesseract_gui/kinematic_groups/kinematic_groups_model.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, ROBOT_ICON, (":/tesseract_gui/png/robotic-arm.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, TEXT_ICON, (":/tesseract_gui/png/text.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));

static const QString CHAIN_GROUPS_KEY = "Chain Groups";
static const QString JOINT_GROUPS_KEY = "Joint Groups";
static const QString LINK_GROUPS_KEY = "Link Groups";

namespace tesseract_gui
{
ChainGroupStandardItem::ChainGroupStandardItem(tesseract_srdf::ChainGroup group)
  : QStandardItem(*ROBOT_ICON(), "Chain Group")
  , group(std::move(group))
{
  ctor();
}

ChainGroupStandardItem::ChainGroupStandardItem(const QString &text, tesseract_srdf::ChainGroup group)
  : QStandardItem(*ROBOT_ICON(), text)
  , group(std::move(group))
{
  ctor();
}

ChainGroupStandardItem::ChainGroupStandardItem(const QIcon &icon, const QString &text, tesseract_srdf::ChainGroup group)
  : QStandardItem(icon, text)
  , group(std::move(group))
{
  ctor();
}

int ChainGroupStandardItem::type() const
{
  return static_cast<int>(StandardItemType::CHAIN_GROUP);
}

void ChainGroupStandardItem::ctor()
{
  {
    auto* name = new QStandardItem(*TEXT_ICON(), "base link");
    auto* value = new QStandardItem(QString::fromStdString(group.front().first));
    appendRow({name, value});
  }

  {
    auto* name = new QStandardItem(*TEXT_ICON(), "tip link");
    auto* value = new QStandardItem(QString::fromStdString(group.front().second));
    appendRow({name, value});
  }
}

JointGroupStandardItem::JointGroupStandardItem(tesseract_srdf::JointGroup group)
  : QStandardItem(*ROBOT_ICON(), "Joint Group")
  , group(std::move(group))
{
  ctor();
}

JointGroupStandardItem::JointGroupStandardItem(const QString &text, tesseract_srdf::JointGroup group)
  : QStandardItem(*ROBOT_ICON(), text)
  , group(std::move(group))
{
  ctor();
}

JointGroupStandardItem::JointGroupStandardItem(const QIcon &icon, const QString &text, tesseract_srdf::JointGroup group)
  : QStandardItem(icon, text)
  , group(std::move(group))
{
  ctor();
}

int JointGroupStandardItem::type() const
{
  return static_cast<int>(StandardItemType::JOINT_GROUP);
}

void JointGroupStandardItem::ctor()
{
  for (const auto& joint : group)
  {
    auto* name = new QStandardItem(*TEXT_ICON(), "joint");
    auto* value = new QStandardItem(QString::fromStdString(joint));
    appendRow({name, value});
  }
}

LinkGroupStandardItem::LinkGroupStandardItem(tesseract_srdf::LinkGroup group)
  : QStandardItem(*ROBOT_ICON(), "Joint Group")
  , group(std::move(group))
{
  ctor();
}

LinkGroupStandardItem::LinkGroupStandardItem(const QString &text, tesseract_srdf::LinkGroup group)
  : QStandardItem(*ROBOT_ICON(), text)
  , group(std::move(group))
{
  ctor();
}

LinkGroupStandardItem::LinkGroupStandardItem(const QIcon &icon, const QString &text, tesseract_srdf::LinkGroup group)
  : QStandardItem(icon, text)
  , group(std::move(group))
{
  ctor();
}

int LinkGroupStandardItem::type() const
{
  return static_cast<int>(StandardItemType::LINK_GROUP);
}

void LinkGroupStandardItem::ctor()
{
  for (const auto& link : group)
  {
    auto* name = new QStandardItem(*TEXT_ICON(), "link");
    auto* value = new QStandardItem(QString::fromStdString(link));
    appendRow({name, value});
  }
}

KinematicGroupsModel::KinematicGroupsModel(QObject *parent)
  : QStandardItemModel(parent)
{
  clear();
}
KinematicGroupsModel::KinematicGroupsModel(const KinematicGroupsModel &other)
: QStandardItemModel(other.d_ptr->parent)
{
  this->group_names_ = other.group_names_;
  this->chain_groups_ =  other.chain_groups_;
  this->joint_groups_ =  other.joint_groups_;
  this->link_groups_ =  other.link_groups_;
}
KinematicGroupsModel &KinematicGroupsModel::operator=(const KinematicGroupsModel &other)
{
  this->group_names_ = other.group_names_;
  this->chain_groups_ =  other.chain_groups_;
  this->joint_groups_ =  other.joint_groups_;
  this->link_groups_ =  other.link_groups_;
  return *this;
}

void KinematicGroupsModel::clear()
{
  QStandardItemModel::clear();
  setColumnCount(2);
  setHorizontalHeaderLabels({"Name", "Values"});

  appendRow(new QStandardItem(CHAIN_GROUPS_KEY));
  appendRow(new QStandardItem(JOINT_GROUPS_KEY));
  appendRow(new QStandardItem(LINK_GROUPS_KEY));

  Q_EMIT cleared();
}

void KinematicGroupsModel::addChainGroup(QString group_name, tesseract_srdf::ChainGroup group)
{
  if(group_names_.find(group_name.toStdString()) != group_names_.end())
    removeGroup(group_name);

  QStandardItem* item = findItems(CHAIN_GROUPS_KEY).at(0);
  item->appendRow(new ChainGroupStandardItem(group_name, group));

  chain_groups_[group_name.toStdString()] = group;
  group_names_.insert(group_name.toStdString());

  Q_EMIT groupAdded(group_name);
}

void KinematicGroupsModel::addJointGroup(QString group_name, tesseract_srdf::JointGroup group)
{
  if(group_names_.find(group_name.toStdString()) != group_names_.end())
    removeGroup(group_name);

  QStandardItem* item = findItems(JOINT_GROUPS_KEY).at(0);
  item->appendRow(new JointGroupStandardItem(group_name, group));

  joint_groups_[group_name.toStdString()] = group;
  group_names_.insert(group_name.toStdString());

  Q_EMIT groupAdded(group_name);
}

void KinematicGroupsModel::addLinkGroup(QString group_name, tesseract_srdf::LinkGroup group)
{
  if(group_names_.find(group_name.toStdString()) != group_names_.end())
    removeGroup(group_name);

  QStandardItem* item = findItems(LINK_GROUPS_KEY).at(0);
  item->appendRow(new LinkGroupStandardItem(group_name, group));

  link_groups_[group_name.toStdString()] = group;
  group_names_.insert(group_name.toStdString());

  Q_EMIT groupAdded(group_name);
}

void KinematicGroupsModel::removeGroup(QString group_name)
{
  group_names_.erase(group_name.toStdString());

  {
    QStandardItem* item = findItems(CHAIN_GROUPS_KEY).at(0);
    for (int i = item->rowCount(); i > 0; i--)
    {
      QStandardItem* child_item = item->child(i-1);
      if (child_item->text() == group_name)
        item->removeRow(i - 1);
    }

    auto it = chain_groups_.find(group_name.toStdString());
    if(it != chain_groups_.end())
      chain_groups_.erase(it);
  }

  {
    QStandardItem* item = findItems(JOINT_GROUPS_KEY).at(0);
    for (int i = item->rowCount(); i > 0; i--)
    {
      QStandardItem* child_item = item->child(i-1);
      if (child_item->text() == group_name)
        item->removeRow(i-1);
    }

    auto it = joint_groups_.find(group_name.toStdString());
    if(it != joint_groups_.end())
      joint_groups_.erase(it);
  }

  {
    QStandardItem* item = findItems(LINK_GROUPS_KEY).at(0);
    for (int i = item->rowCount(); i > 0; i--)
    {
      QStandardItem* child_item = item->child(i-1);
      if (child_item->text() == group_name)
        item->removeRow(i-1);
    }

    auto it = link_groups_.find(group_name.toStdString());
    if(it != link_groups_.end())
      link_groups_.erase(it);
  }

  Q_EMIT groupRemoved(group_name);
}

const tesseract_srdf::GroupNames& KinematicGroupsModel::getGroupNames() const
{
  return group_names_;
}

const tesseract_srdf::ChainGroups& KinematicGroupsModel::getChainGroups() const
{
  return chain_groups_;
}

const tesseract_srdf::JointGroups& KinematicGroupsModel::getJointGroups() const
{
  return joint_groups_;
}

const tesseract_srdf::LinkGroups& KinematicGroupsModel::getLinkGroups() const
{
  return link_groups_;
}

}
