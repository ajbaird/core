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

#pragma once
#include <biogears/cdm/enums/SEPatientActionsEnums.h>
#include <biogears/cdm/patient/actions/SEPatientAction.h>
#include <biogears/schema/cdm/PatientActions.hxx>

namespace biogears {
namespace io {
  class PatientActions;
}
class BIOGEARS_API SESpinalCordInjury : public SEPatientAction {
  friend io::PatientActions;

public:
  SESpinalCordInjury();
  virtual ~SESpinalCordInjury() override;

  static constexpr const char* TypeTag() { return "SESpinalCordInjury"; };
  const char* classname() const override { return TypeTag(); }

  virtual void Clear() override; // clear memory

  virtual bool IsValid() const override;
  virtual bool IsActive() const override;

  virtual bool Load(const CDM::SpinalCordInjuryData& in, std::default_random_engine* rd = nullptr);
  virtual CDM::SpinalCordInjuryData* Unload() const override;

protected:
  virtual void Unload(CDM::SpinalCordInjuryData& data) const;

public:
  virtual SESCILocationType GetLocation() const;
  virtual bool HasLocation() const;
  virtual void SetLocation(SESCILocationType t);
  virtual void InvalidateLocation();

  virtual void ToString(std::ostream& str) const override;

  bool operator==(const SESpinalCordInjury& rhs) const;
  bool operator!=(const SESpinalCordInjury& rhs) const;

protected:
  SESCILocationType m_Location;
};
}
