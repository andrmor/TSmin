#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "SteppingAction.hh"

#include <sstream>

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "QGSP_BIC_HP.hh"
#include "G4StepLimiterPhysics.hh"

#include "G4HadronElasticProcess.hh"
#include "G4NeutronHPElasticData.hh"
#include "G4NeutronHPThermalScatteringData.hh"
#include "G4NeutronHPElastic.hh"
#include "G4NeutronHPThermalScattering.hh"
#include "G4NeutronInelasticProcess.hh"
#include "G4ParticleHPInelasticData.hh"
#include "G4ParticleHPInelastic.hh"
#include "G4HadronCaptureProcess.hh"
#include "G4ParticleHPCaptureData.hh"
#include "G4ParticleHPCapture.hh"
#include "G4HadronFissionProcess.hh"
#include "G4ParticleHPFissionData.hh"
#include "G4ParticleHPFission.hh"
#include "G4SystemOfUnits.hh"

int main(int, char**)
{
    int  numNeutrons = 100;
    double energy = 0.025*eV;
    long Seed = 1000000;

    G4RunManager* runManager = new G4RunManager;

    CLHEP::RanecuEngine* randGen = new CLHEP::RanecuEngine();
    randGen->setSeed(Seed);
    G4Random::setTheEngine(randGen);

    G4VModularPhysicsList* physicsList = new QGSP_BIC_HP;
    runManager->SetUserInitialization(physicsList);

    DetectorConstruction * theDetector = new DetectorConstruction();
    runManager->SetUserInitialization(theDetector);

    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/run/initialize"); // needed here otherwise G4ProcessManager for neutron does not exist

    //setting up neutron TS physics following Hadr04 example
        G4ParticleDefinition* neutron = G4Neutron::Neutron();
        G4ProcessManager* pManager = neutron->GetProcessManager();
        if (!pManager)
        {
            std::cout << "Process manager for neutron not found. Make sure /run/initialize was already performed!";
            exit(-1);
        }

        // delete all neutron processes if already registered
        G4VProcess* process = nullptr;
        process = pManager->GetProcess("hadElastic");       if (process) pManager->RemoveProcess(process);
        process = pManager->GetProcess("neutronInelastic"); if (process) pManager->RemoveProcess(process);
        process = pManager->GetProcess("nCapture");         if (process) pManager->RemoveProcess(process);
        process = pManager->GetProcess("nFission");         if (process) pManager->RemoveProcess(process);

        // (re) create process: elastic
        G4HadronElasticProcess* process1 = new G4HadronElasticProcess();
        pManager->AddDiscreteProcess(process1);
        G4ParticleHPElastic*  model1a = new G4ParticleHPElastic();
        process1->RegisterMe(model1a);
        process1->AddDataSet(new G4ParticleHPElasticData());
        model1a->SetMinEnergy(4*eV);
        G4ParticleHPThermalScattering* model1b = new G4ParticleHPThermalScattering();
        process1->RegisterMe(model1b);
        process1->AddDataSet(new G4ParticleHPThermalScatteringData());

        // (re) create process: inelastic
        G4NeutronInelasticProcess* process2 = new G4NeutronInelasticProcess();
        pManager->AddDiscreteProcess(process2);
        G4ParticleHPInelasticData* dataSet2 = new G4ParticleHPInelasticData();
        process2->AddDataSet(dataSet2);
        G4ParticleHPInelastic* model2 = new G4ParticleHPInelastic();
        process2->RegisterMe(model2);

        // (re) create process: nCapture
        G4HadronCaptureProcess* process3 = new G4HadronCaptureProcess();
        pManager->AddDiscreteProcess(process3);
        G4ParticleHPCaptureData* dataSet3 = new G4ParticleHPCaptureData();
        process3->AddDataSet(dataSet3);
        G4ParticleHPCapture* model3 = new G4ParticleHPCapture();
        process3->RegisterMe(model3);

        // (re) create process: nFission
        G4HadronFissionProcess* process4 = new G4HadronFissionProcess();
        pManager->AddDiscreteProcess(process4);
        G4ParticleHPFissionData* dataSet4 = new G4ParticleHPFissionData();
        process4->AddDataSet(dataSet4);
        G4ParticleHPFission* model4 = new G4ParticleHPFission();
        process4->RegisterMe(model4);
    runManager->PhysicsHasBeenModified();


    runManager->SetUserAction(new PrimaryGeneratorAction(energy, numNeutrons));
    double SumPath = 0;
    int    SumNum  = 0;
    runManager->SetUserAction(new SteppingAction(SumPath, SumNum));

   // G4UImanager* UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/run/verbose 0");
    UImanager->ApplyCommand("/control/verbose 0");
    UImanager->ApplyCommand("/hits/verbose 0");
    UImanager->ApplyCommand("/tracking/verbose 0"); //set to 2 to debug
    UImanager->ApplyCommand("/run/initialize");

    UImanager->ApplyCommand("/run/beamOn");

    G4cout.flush();
    if (SumNum != 0)
    {
        std::cout << "\n>>>>";
        std::cout << "\nMean free path: " << SumPath/SumNum << " mm\n<<<<\n\n";
    }

    delete runManager;
}
