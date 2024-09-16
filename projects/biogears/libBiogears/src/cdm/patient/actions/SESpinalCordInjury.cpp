/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#include <biogears/cdm/patient/actions/SESpinalCordInjury.h>

#include "io/cdm/PatientActions.h"

namespace biogears {
SESpinalCordInjury::SESpinalCordInjury()
  : SEPatientAction()
{
  m_Location = SESCILocationType::Invalid;
}
//-----------------------------------------------------------------------------
SESpinalCordInjury::~SESpinalCordInjury()
{
  Clear();
}
//-----------------------------------------------------------------------------
void SESpinalCordInjury::Clear()
{
  SEPatientAction::Clear();
  m_Location = SESCILocationType::Invalid;
}
//-----------------------------------------------------------------------------
bool SESpinalCordInjury::IsValid() const
{
  return SEPatientAction::IsValid() && HasLocation();
}
//-----------------------------------------------------------------------------
bool SESpinalCordInjury::IsActive() const
{
  return IsValid() ? !(m_Location == (SESCILocationType::Invalid)) : false;
} //-----------------------------------------------------------------------------
bool SESpinalCordInjury::Load(const CDM::SpinalCordInjuryData& in, std::default_random_engine* rd)
{
  io::PatientActions::UnMarshall(in, *this, rd);
  return true;
}
//-----------------------------------------------------------------------------
CDM::SpinalCordInjuryData* SESpinalCordInjury::Unload() const
{
  CDM::SpinalCordInjuryData* data(new CDM::SpinalCordInjuryData());
  Unload(*data);
  return data;
}
//-----------------------------------------------------------------------------
void SESpinalCordInjury::Unload(CDM::SpinalCordInjuryData& data) const
{
  io::PatientActions::Marshall(*this, data);
}
//-----------------------------------------------------------------------------
SESCILocationType SESpinalCordInjury::GetLocation() const
{
  return SESCILocationType();
}
//-----------------------------------------------------------------------------
bool SESpinalCordInjury::HasLocation() const
{
  return m_Location == SESCILocationType::Invalid ? false : true;
}
//-----------------------------------------------------------------------------
void SESpinalCordInjury::SetLocation(SESCILocationType location)
{
  m_Location = location;
}
//-----------------------------------------------------------------------------
void SESpinalCordInjury::InvalidateLocation()
{
  m_Location = SESCILocationType::Invalid;
}
//-----------------------------------------------------------------------------
void SESpinalCordInjury::ToString(std::ostream& str) const
{
  if (m_Location == SESCILocationType::Invalid) {
    str << "Patient Action : Remove spinal cord injury";
    if (HasComment())
      str << "\n\tComment: ";
    str << m_Comment;
    str << "\n\tCompartment: ";
    HasLocation() ? str << GetLocation() : str << "No location Set";
  } else {
    str << "Patient Action : Spinal Cord Injury";
    if (HasComment())
      str << "\n\tComment: " << m_Comment;
    str << "\n\tLocation:  ";
    str << m_Location;
    str << "\n\tCompartment: ";
    HasLocation() ? str << GetLocation() : str << "No location Set";
  }
  str << std::flush;
}
//-----------------------------------------------------------------------------
bool SESpinalCordInjury::operator==(const SESpinalCordInjury& rhs) const
{
  return m_Comment == rhs.m_Comment
    && m_Location == rhs.m_Location;
}
//-----------------------------------------------------------------------------
bool SESpinalCordInjury::operator!=(const SESpinalCordInjury& rhs) const
{
  return !(*this == rhs);
}

}

//-----------------------------------------------------------------------------