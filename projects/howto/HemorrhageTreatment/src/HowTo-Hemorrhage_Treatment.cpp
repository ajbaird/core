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



// Include the various types you will be using in your code
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/actions/SEHemorrhage.h>
#include <biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h>
#include <biogears/cdm/properties/SEScalarTypes.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SEEnergySystem.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>
#include <biogears/cdm/patient/actions/SESubstanceBolus.h>


using namespace biogears;
//--------------------------------------------------------------------------------------------------
/// \brief
/// Usage for applying a Hemorrhage insult from the command line and ouputing patient vitals
///
/// \details
/// Refer to the SEHemorrhage class
/// Refer to the SESubstanceManager class
/// Refer to the SESubstanceIVFluids class for applying an IV to the patient
//--------------------------------------------------------------------------------------------------

void HowToHemorrhageTreatment()
{
  // Create the engine and load the patient
  std::unique_ptr<PhysiologyEngine> bg = CreateBioGearsEngine("HowToHemorrhageTreatment.log");
  bg->GetLogger()->Info("HowToHemorrhageTreatment");
    
// Load patient
  if (!bg->LoadState("./states/StandardMale@0s.xml")) {
    bg->GetLogger()->Error("Could not load state, check the error");
    return;
  }

  //set the data file
  // Create data requests for each value that should be written to the output log as the engine is executing
  // Physiology System Names are defined on the System Objects
  // defined in the Physiology.xsd file
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("HeartRate", FrequencyUnit::Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("BloodVolume", VolumeUnit::mL);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("CardiacOutput", VolumePerTimeUnit::mL_Per_min);
  bg->GetEngineTrack()->GetDataRequestManager().CreatePhysiologyDataRequest().Set("MeanArterialPressure", PressureUnit::mmHg);

  bg->GetEngineTrack()->GetDataRequestManager().SetResultsFilename("HowToHemorrhageTreatment.csv");

  
  std::string hemorrageLocation;
  int hemorrageRate;

  std::cout << "Please type the location of the hemorrhage as: leftleg, spleen, or aorta. Followed by the rate of the bleed (assuming units of mL/min), ex. leftleg 150 " << std::endl;
  std::cin >> hemorrageLocation >> hemorrageRate;

  //Create variables for scenario
  SEHemorrhage hem; //hemorrhage object
  hem.GetInitialRate().SetValue(hemorrageRate, VolumePerTimeUnit::mL_Per_min);
  //Process update to hemorrhage action
  hem.SetCompartment(hemorrageLocation);
  bg->ProcessAction(hem);
  
  int action;
  bool simulation = true;
  double rate;
  double concentration;
  SESubstance* vas = bg->GetSubstanceManager().GetSubstance("Vasopressin");
  SESubstanceBolus bolus(*vas);
  SESubstanceCompoundInfusion* saline = nullptr;

  
  do {
    bg->GetLogger()->Info("Enter Integer for Action to Perform : \n\t[1] Status \n\t[2] IVFluids \n\t[3] tourniquet \n\t[4] Vasopressin Admin \n\t[5] Quit \n");
    std::cin >> action;
    switch (action) {
    case 1:
      bg->GetLogger()->Info("");
      bg->GetLogger()->Info(std::stringstream() << "Simulation Time  : " << bg->GetSimulationTime(TimeUnit::min) << "min");
      bg->GetLogger()->Info(std::stringstream() << "Blood Volume : " << bg->GetCardiovascularSystem()->GetBloodVolume(VolumeUnit::mL) << VolumeUnit::mL);
      bg->GetLogger()->Info(std::stringstream() << "Mean Arterial Pressure : " << bg->GetCardiovascularSystem()->GetMeanArterialPressure(PressureUnit::mmHg) << PressureUnit::mmHg);
      bg->GetLogger()->Info(std::stringstream() << "Heart Rate : " << bg->GetCardiovascularSystem()->GetHeartRate(FrequencyUnit::Per_min) << "bpm");
      break;
    case 2:
      bg->GetLogger()->Info("Enter IV Fluids Rate in mL/min (bag volume is 500 mL), followed by ENTER : ");
      std::cin >> rate;
      saline->GetRate().SetValue(rate, VolumePerTimeUnit::mL_Per_min);
      bg->ProcessAction(*saline);
      break;
    case 3:
      bg->GetLogger()->Info("Administering tourniquet");
      hem.GetInitialRate().SetValue(0, VolumePerTimeUnit::mL_Per_min);
      break;
    case 4:
      bg->GetLogger()->Info("Enter a concentration in ug/mL (will be delivered by a 10 mL push intravesnously), followed by ENTER : ");
      std::cin >> concentration;
      bolus.GetConcentration().SetValue(concentration, MassPerVolumeUnit::ug_Per_mL);
      bolus.GetDose().SetValue(10, VolumeUnit::mL);
      bolus.SetAdminRoute(CDM::enumBolusAdministration::Intravenous);
      break;
    case 5:
      simulation = false;
      break;
      }
    bg->AdvanceModelTime(1.0, TimeUnit::s);
    bg->GetEngineTrack()->TrackData(bg->GetSimulationTime(TimeUnit::s));
    }  while(simulation);
  
  bg->GetLogger()->Info("Finished");

}
    



