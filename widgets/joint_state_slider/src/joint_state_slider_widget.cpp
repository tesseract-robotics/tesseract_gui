#include <tesseract_gui/widgets/joint_state_slider/joint_state_slider_widget.h>
#include "ui_joint_state_slider_widget.h"

#include <QLabel>
#include <QSlider>
#include <QGridLayout>
#include <QFrame>
#include <QDoubleSpinBox>

const double SLIDER_RESOLUTION = 0.001;

namespace tesseract_gui
{
JointStateSliderWidget::JointStateSliderWidget(QWidget *parent)
  : QWidget(parent)
  , ui_(std::make_unique<Ui::JointStateSliderWidget>())
{
  ui_->setupUi(this);

  layout_ = new QGridLayout();
  setLayout(layout_);
}

JointStateSliderWidget::~JointStateSliderWidget() = default;

void JointStateSliderWidget::setJoints(const std::vector<tesseract_scene_graph::Joint::ConstPtr>& joints)
{
  state_.clear();
  while(layout_->count() > 0)
  {
    QLayoutItem *item = layout_->takeAt(0);
    delete item->widget();
    delete item;
  }

  int row {0};
  for (const auto& joint : joints)
  {
    std::string name = joint->getName();
    double range = (joint->limits->upper - joint->limits->lower);
    double value = joint->limits->lower + (range / 2.0);
    state_[name] = value;

    auto* name_lable = new QLabel();
    name_lable->setText(QString("%1:").arg(QString::fromStdString(joint->getName())));
    layout_->addWidget(name_lable, row, 0);

    auto* min_lable = new QLabel();
    min_lable->setFrameShape(QFrame::StyledPanel);
    min_lable->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
    min_lable->setText(QString("").setNum(joint->limits->lower, 'g', 3));
    min_lable->setAlignment(Qt::AlignmentFlag::AlignRight);
    layout_->addWidget(min_lable, row, 1);

    auto* slider = new QSlider();
    slider->setOrientation(Qt::Orientation::Horizontal);
    slider->setMinimum(joint->limits->lower / SLIDER_RESOLUTION);
    slider->setMaximum(joint->limits->upper / SLIDER_RESOLUTION);
    slider->setValue(value / SLIDER_RESOLUTION);
    layout_->addWidget(slider, row, 2);

    auto* max_lable = new QLabel();
    max_lable->setFrameShape(QFrame::StyledPanel);
    max_lable->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
    max_lable->setText(QString("").setNum(joint->limits->upper, 'g', 3));
    max_lable->setAlignment(Qt::AlignmentFlag::AlignRight);
    layout_->addWidget(max_lable, row, 3);

    auto* spin = new QDoubleSpinBox();
    spin->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));
    spin->setMinimum(joint->limits->lower);
    spin->setMaximum(joint->limits->upper);
    spin->setValue(value);
    spin->setDecimals(3);
    spin->setSingleStep(SLIDER_RESOLUTION);
    layout_->addWidget(spin, row, 4);

    connect(slider, &QSlider::sliderMoved, spin, [this, name, spin](int value){
      double v = value * SLIDER_RESOLUTION;
      spin->blockSignals(true);
      spin->setValue(v);
      spin->blockSignals(false);
      state_[name] = v;
      Q_EMIT jointValueChanged(QString::fromStdString(name), v);
      Q_EMIT jointStateChanged(state_);
    });

    connect(spin, qOverload<double>(&QDoubleSpinBox::valueChanged), slider, [this, name, slider](double value){
      slider->blockSignals(true);
      slider->setValue(value / SLIDER_RESOLUTION);
      slider->blockSignals(false);
      state_[name] = value;
      Q_EMIT jointValueChanged(QString::fromStdString(name), value);
      Q_EMIT jointStateChanged(state_);
    });

    ++row;
  }
  layout_->setColumnStretch(2, 1);
  layout_->setColumnStretch(0, 0);
}

std::unordered_map<std::string, double> JointStateSliderWidget::getJointState() const
{
  return state_;
}

}
