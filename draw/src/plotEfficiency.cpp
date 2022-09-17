#include "utilities.h"
#include <CanvasHelper.h>
#include <TCanvas.h>
#include <TApplication.h>

int plotEfficiency() {
  gStyle->SetOptFit(1);
  gStyle->SetOptStat(1);

  new TCanvas();
  TH1F *ha = new TH1F("ha", "Default ROOT Fit Statistics Box", 200, -5, 5);
  TF1 *f1 = new TF1("f_1", "[2]*TMath::Gaus(x,[0],[1])");
  f1->SetParameters(-1.061234, 1.235342, 1);
  ha->FillRandom("f_1", 100);
  ha->Fit("gaus");
  ha->Draw();

  TCanvas *c2 = new TCanvas();
  TH1F *hb = (TH1F*) ha->Clone("hb");
  hb->SetTitle("Statistics Box with Rounded Parameters");
  hb->Fit("gaus", "E");
  hb->Draw();

  TPave *pave = CanvasHelper::getDefaultPaveStats(c2);
  CanvasHelper::setPaveAlignment(pave, CanvasHelper::kPaveAlignRight | CanvasHelper::kPaveAlignTop);

  CanvasHelper::getInstance()->addCanvas(c2);
  return 0;
}

#ifndef __CINT__

int main(int argc, char **argv) {
  // Instantiate TApplication
  TApplication *app = new TApplication("plotEfficiency", &argc, argv);

  int returnValue = 0;
  returnValue = returnValue || plotEfficiency();

  // Enter the event loop
  app->Run();

  // Return success
  return returnValue;
}

#endif
