/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "LStepExpressFake.h"

LStepExpressFake::LStepExpressFake( const ioport_t ioPort )
  :VLStepExpress(ioPort),
   autoStatus_(1)
{
  axisStatus_ = std::vector<int>{ VLStepExpress::AXISSTANDSANDREADY,
                                  VLStepExpress::AXISSTANDSANDREADY,
                                  VLStepExpress::AXISSTANDSANDREADY,
                                  VLStepExpress::AXISSTANDSANDREADY };
  axis_ = std::vector<int>{ 1, 1, 1, 1 };
  axisDirection_ = std::vector<int>{ 0, 0, 0, 0 };
  dim_ = std::vector<int>{ 2, 2, 2, 3 };
  pa_ = std::vector<int>{ 0, 0, 0, 0 };
  position_ = std::vector<double>{ 0.0, 0.0, 0.0, 0.0 };
  moverel_ = std::vector<double>{ 0.0, 0.0, 0.0, 0.0 };

  joystickEnabled_ = false;
  joystickAxisEnabled_ = std::vector<int>{ 1, 1, 1, 1 };
}

LStepExpressFake::~LStepExpressFake()
{

}

void LStepExpressFake::GetAutoStatus(int & value)
{
  value = autoStatus_;
}

void LStepExpressFake::SetAutoStatus(int value)
{
  autoStatus_ = value;
}

void LStepExpressFake::GetAxisStatus(std::vector<int> & values)
{
  values = axisStatus_;
}

void LStepExpressFake::GetAxisEnabled(std::vector<int> & values)
{
  values = axis_;
}

void LStepExpressFake::GetAxisEnabled(VLStepExpress::Axis axis, int & value)
{
  value = axis_[axis];
}

void LStepExpressFake::SetAxisEnabled(std::vector<int> & values)
{
  axis_ = values;

  std::vector<int>::iterator itaxis = axis_.begin();
  for (std::vector<int>::iterator it = axisStatus_.begin();
      it!=axisStatus_.end();
      ++it, ++itaxis) {
    if ((*itaxis)==0) {
      *it = VLStepExpress::AXISDISABLED;
    } else {
      *it = VLStepExpress::AXISSTANDSANDREADY;
    }
  }
}

void LStepExpressFake::SetAxisEnabled(VLStepExpress::Axis axis, int value)
{
  axis_[axis] = value;

  if (axis_[axis]==0) {
    axisStatus_[axis] = VLStepExpress::AXISDISABLED;
  } else {
    axisStatus_[axis] = VLStepExpress::AXISSTANDSANDREADY;
  }
}

void LStepExpressFake::GetAxisDirection(std::vector<int> & values)
{
  values = axisDirection_;
}

void LStepExpressFake::GetAxisDirection(VLStepExpress::Axis axis, int & value)
{
  value = axisDirection_[axis];
}

void LStepExpressFake::SetAxisDirection(std::vector<int> & values)
{
  axisDirection_ = values;
}

void LStepExpressFake::SetAxisDirection(VLStepExpress::Axis axis, int value)
{
  axisDirection_[axis] = value;
}

void LStepExpressFake::GetDimension(std::vector<int> & values)
{
  values = dim_;
}

void LStepExpressFake::GetDimension(VLStepExpress::Axis axis, int & value)
{
  value = dim_[axis];
}

void LStepExpressFake::SetDimension(std::vector<int> & values)
{
  dim_ = values;
}

void LStepExpressFake::SetDimension(VLStepExpress::Axis axis, int value)
{
  dim_[axis] = value;
}

void LStepExpressFake::GetPowerAmplifierStatus(std::vector<int> & values)
{
  values = pa_;
}

void LStepExpressFake::GetPowerAmplifierStatus(VLStepExpress::Axis axis, int & value)
{
  value = pa_[axis];
}

void LStepExpressFake::SetPowerAmplifierStatus(std::vector<int> & values)
{
  pa_ = values;
}

void LStepExpressFake::SetPowerAmplifierStatus(VLStepExpress::Axis axis, int value)
{
  pa_[axis] = value;
}

void LStepExpressFake::GetPosition(std::vector<double> & values)
{
  values = position_;
}

void LStepExpressFake::GetPosition(VLStepExpress::Axis axis, double & value)
{
  value = position_[axis];
}

void LStepExpressFake::SetPosition(std::vector<double> & values)
{
  position_ = values;
}

void LStepExpressFake::SetPosition(VLStepExpress::Axis axis, double value)
{
  position_[axis] = value;
}

void LStepExpressFake::MoveAbsolute(std::vector<double> & values)
{
  position_ = values;
}

void LStepExpressFake::MoveAbsolute(double x, double y, double z, double a)
{
  position_[VLStepExpress::X] = x;
  position_[VLStepExpress::Y] = y;
  position_[VLStepExpress::Z] = z;
  position_[VLStepExpress::A] = a;
}

void LStepExpressFake::MoveAbsolute(VLStepExpress::Axis axis, double value)
{
  position_[axis] = value;
}

void LStepExpressFake::MoveRelative(std::vector<double> & values)
{
  moverel_ = values;
  std::vector<double>::iterator itpos = position_.begin();
  for (std::vector<double>::iterator it = moverel_.begin();
      it!=moverel_.end();
      ++it, ++itpos) {
      *itpos += *it;
  }
}

void LStepExpressFake::MoveRelative(double x, double y, double z, double a)
{
  moverel_[VLStepExpress::X] = x;
  moverel_[VLStepExpress::Y] = y;
  moverel_[VLStepExpress::Z] = z;
  moverel_[VLStepExpress::A] = a;
  std::vector<double>::iterator itpos = position_.begin();
  for (std::vector<double>::iterator it = moverel_.begin();
      it!=moverel_.end();
      ++it, ++itpos) {
      *itpos += *it;
  }
}

void LStepExpressFake::MoveRelative(VLStepExpress::Axis axis, double value)
{
  moverel_[VLStepExpress::X] = 0.0;
  moverel_[VLStepExpress::Y] = 0.0;
  moverel_[VLStepExpress::Z] = 0.0;
  moverel_[VLStepExpress::A] = 0.0;
  moverel_[axis] = value;
  std::vector<double>::iterator itpos = position_.begin();
  for (std::vector<double>::iterator it = moverel_.begin();
       it!=moverel_.end();
       ++it, ++itpos) {
    *itpos += *it;
  }
}

void LStepExpressFake::MoveRelative()
{
  std::vector<double>::iterator itpos = position_.begin();
  for (std::vector<double>::iterator it = moverel_.begin();
      it!=moverel_.end();
      ++it, ++itpos) {
      *itpos += *it;
  }
}

void LStepExpressFake::GetSystemStatus(std::vector<int> & values)
{
  values.resize(4, 5);
}

void LStepExpressFake::GetSystemStatusText(std::string& value)
{
  value = "?sysstatus";
}

void LStepExpressFake::GetSystemStatus(VLStepExpress::Axis axis, int & value)
{
  value = 5;
}

bool LStepExpressFake::GetJoystickEnabled()
{
  return joystickEnabled_;
}

void LStepExpressFake::SetJoystickEnabled(bool enabled)
{
  joystickEnabled_ = enabled;
}

void LStepExpressFake::GetJoystickAxisEnabled(std::vector<int> & values)
{
  values = joystickAxisEnabled_;
}

void LStepExpressFake::GetJoystickAxisEnabled(VLStepExpress::Axis axis, int & value)
{
  value = joystickAxisEnabled_[axis];
}

void LStepExpressFake::SetJoystickAxisEnabled(std::vector<int> & values)
{
  joystickAxisEnabled_ = values;
}

void LStepExpressFake::SetJoystickAxisEnabled(VLStepExpress::Axis axis, int value)
{
  joystickAxisEnabled_[axis] = value;
}

void LStepExpressFake::SendCommand(const std::string & command)
{
  std::cout << "SendCommand: " << command << std::endl;
}
