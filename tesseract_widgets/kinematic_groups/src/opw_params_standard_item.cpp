#include <tesseract_widgets/kinematic_groups/opw_params_standard_item.h>
#include <tesseract_widgets/common/standard_item_utils.h>
#include <tesseract_widgets/common/standard_item_type.h>

namespace tesseract_gui
{
OPWParamsStandardItem::OPWParamsStandardItem(opw_kinematics::Parameters<double> params)
  : QStandardItem(QIcon(":/tesseract_widgets/png/cube.png"), "OPW")
  , params(params)
{
  ctor();
}

OPWParamsStandardItem::OPWParamsStandardItem(const QString &text, opw_kinematics::Parameters<double> params)
  : QStandardItem(QIcon(":/tesseract_widgets/png/cube.png"), text)
  , params(params)
{
  ctor();
}

OPWParamsStandardItem::OPWParamsStandardItem(const QIcon &icon, const QString &text, opw_kinematics::Parameters<double> params)
  : QStandardItem(icon, text)
  , params(params)
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
  params_item->appendRow(createStandardItemFloat("a1", params.a1));
  params_item->appendRow(createStandardItemFloat("a2", params.a2));
  params_item->appendRow(createStandardItemFloat("b", params.b));
  params_item->appendRow(createStandardItemFloat("c1", params.c1));
  params_item->appendRow(createStandardItemFloat("c2", params.c2));
  params_item->appendRow(createStandardItemFloat("c3", params.c3));
  params_item->appendRow(createStandardItemFloat("c4", params.c4));
  appendRow(params_item);

  auto* offsets_item = new QStandardItem("offsets");
  for (std::size_t i = 0; i < 6; ++i)
    offsets_item->appendRow(createStandardItemFloat(QString("[%1]").arg(i).toStdString(),params.offsets[0]));

  appendRow(offsets_item);

  auto* correction_item = new QStandardItem("sign correction");
  for (std::size_t i = 0; i < 6; ++i)
    correction_item->appendRow(createStandardItemFloat(QString("[%1]").arg(i).toStdString(), params.sign_corrections[0]));

  appendRow(correction_item);
}
}
