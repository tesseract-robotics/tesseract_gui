#include <tesseract_gui/widgets/kinematic_groups/opw_params_standard_item.h>
#include <tesseract_gui/common/standard_item_type.h>

Q_GLOBAL_STATIC_WITH_ARGS(QIcon, CUBE_ICON, (":/tesseract_gui/png/cube.png"));
Q_GLOBAL_STATIC_WITH_ARGS(QIcon, NUMERIC_ICON, (":/tesseract_gui/png/numeric.png"));

namespace tesseract_gui
{
OPWParamsStandardItem::OPWParamsStandardItem(opw_kinematics::Parameters<double> params)
  : QStandardItem(*CUBE_ICON(), "OPW")
  , params(std::move(params))
{
  ctor();
}

OPWParamsStandardItem::OPWParamsStandardItem(const QString &text, opw_kinematics::Parameters<double> params)
  : QStandardItem(*CUBE_ICON(), text)
  , params(std::move(params))
{
  ctor();
}

OPWParamsStandardItem::OPWParamsStandardItem(const QIcon &icon, const QString &text, opw_kinematics::Parameters<double> params)
  : QStandardItem(icon, text)
  , params(std::move(params))
{
  ctor();
}

int OPWParamsStandardItem::type() const
{
  return static_cast<int>(StandardItemType::OPW_PARAMS);
}

void OPWParamsStandardItem::ctor()
{
  auto* params_item = new QStandardItem("params");
  {
    auto* name = new QStandardItem(*NUMERIC_ICON(), "a1");
    auto* value = new QStandardItem(QString("%1").arg(params.a1));
    params_item->appendRow({name, value});
  }
  {
    auto* name = new QStandardItem(*NUMERIC_ICON(), "a2");
    auto* value = new QStandardItem(QString("%1").arg(params.a2));
    params_item->appendRow({name, value});
  }
  {
    auto* name = new QStandardItem(*NUMERIC_ICON(), "b");
    auto* value = new QStandardItem(QString("%1").arg(params.b));
    params_item->appendRow({name, value});
  }
  {
    auto* name = new QStandardItem(*NUMERIC_ICON(), "c1");
    auto* value = new QStandardItem(QString("%1").arg(params.c1));
    params_item->appendRow({name, value});
  }
  {
    auto* name = new QStandardItem(*NUMERIC_ICON(), "c2");
    auto* value = new QStandardItem(QString("%1").arg(params.c2));
    params_item->appendRow({name, value});
  }
  {
    auto* name = new QStandardItem(*NUMERIC_ICON(), "c3");
    auto* value = new QStandardItem(QString("%1").arg(params.c3));
    params_item->appendRow({name, value});
  }
  {
    auto* name = new QStandardItem(*NUMERIC_ICON(), "c4");
    auto* value = new QStandardItem(QString("%1").arg(params.c4));
    params_item->appendRow({name, value});
  }
  appendRow(params_item);

  auto* offsets_item = new QStandardItem("offsets");
  for (std::size_t i = 0; i < 6; ++i)
  {
    auto* name = new QStandardItem(*NUMERIC_ICON(), QString("[%1]").arg(i));
    auto* value = new QStandardItem(QString("%1").arg(params.offsets[0]));
    offsets_item->appendRow({name, value});
  }
  appendRow(offsets_item);

  auto* correction_item = new QStandardItem("sign correction");
  for (std::size_t i = 0; i < 6; ++i)
  {
    auto* name = new QStandardItem(*NUMERIC_ICON(), QString("[%1]").arg(i));
    auto* value = new QStandardItem(QString("%1").arg(params.sign_corrections[0]));
    correction_item->appendRow({name, value});
  }
  appendRow(correction_item);
}
}

