#include <UiUtils.h>
#include <FileUtils.h>
#include <FitUtils.h>

#include <CanvasHelper.h>

#include <TObjString.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TApplication.h>
#include <TPRegexp.h>

int plotEfficiency() {
  // Get list of files from the UI
  TList *files = UiUtils::getFileNamesList("Select ROOT files with PE output for different light guide length", kTRUE);

  // Prepare arrays for nPE(guideLength) plot and resolution(guideLength)
  std::vector<Double_t> guideLengths = { };
  std::vector<Double_t> nPes = { };
  std::vector<Double_t> nPeErrs = { };
  std::vector<Double_t> nPeRes = { };
  std::vector<Double_t> nPeResErrs = { };

  // Iterate through files and fit the PE output graphs
  for (TObject *o : *files) {
    // Get file name path
    TObjString *objString = (TObjString*) o;
    TString fileNamePath = objString->GetString();

    // Open ROOT file and get the tree
    TFile *file = FileUtils::openFile(fileNamePath.Data());
    TTree *tree = FileUtils::getTree(file, "lightguides");

    // Obtain the guide length (hack - parse filename)
    // Int_t gL = 0;
    // tree->SetBranchAddress("guideLength", &gL);
    // tree->GetEntry(1);
    // guideLengths.push_back(gL);
    TString fileName = gSystem->BaseName(fileNamePath);
    TObjArray *objArray = TPRegexp("(\\d+)-mm").MatchS(fileName);
    if (objArray->GetLast() < 1) {
      Error("plotEfficiency()", "Cannot parse file name");
      exit(1);
    }
    const TString str = ((TObjString*) objArray->At(1))->GetString();
    Int_t guideLength = str.Atoi();
    guideLengths.push_back(guideLength);

    // Prepare histogram with branch distribution
    TString histName = TString::Format("hist-%dmm", guideLength);
    TString histTitle = TString::Format("Light Guide %d mm", guideLength);
    // Int_t maxPE = FileUtils::getBranchMaximum(tree, "nPE")*0.3;
    TH1 *hist = new TH1I(histName.Data(), histTitle.Data(), 150, 0, 5000);
    hist->GetXaxis()->SetTitle("Number of Photo-Electrons per Event");
    hist->GetYaxis()->SetTitle("Counts");

    // Populate histogram (should pass "y:x >> histName")
    TString drawOption = TString::Format("nPE >> %s", histName.Data());
    tree->Draw(drawOption, "", "goff");

    // Create canvas before TH1::Fit(), otherwise default canvas is created
    TCanvas *canvas = new TCanvas(fileName.Data(), fileName.Data());
    canvas->SetWindowSize(500, 400);
    canvas->SetGrid();

    // Draw Histogram on the canvas
    hist->SetFillColor(EColor::kCyan);
    hist->Draw();

    // Fit histogram
    TF1 *fBall = FitUtils::getCrystalBallFunction(hist, kTRUE);
    fBall->SetLineColor(EColor::kPink+10);
    gStyle->SetOptFit(111);
    hist->Fit(fBall);
    hist->Fit(fBall, "M"); // Hack - second fit sometimes improves the results

    // Get fitting parameters
    Double_t m = hist->GetFunction("fBall")->GetParameter(2); // mean
    nPes.push_back(m);
    Double_t Dm = hist->GetFunction("fBall")->GetParError(2); // mean error
    Double_t s = hist->GetFunction("fBall")->GetParameter(3); // sigma
    nPeErrs.push_back(s);
    Double_t Ds = hist->GetFunction("fBall")->GetParError(3); // sigma error

    // Calculate resolution value and error
    Double_t r = s / m * 100;  // Resolution
    nPeRes.push_back(r);
    Double_t Dr = 100 * TMath::Sqrt(TMath::Power(1 / m * Ds, 2) + TMath::Power(s / m / m * Dm, 2)); // Indirect error
    nPeResErrs.push_back(Dr);

    // Beautify the canvas
    TPave *pave = CanvasHelper::getDefaultPaveStats(canvas);
    CanvasHelper::setPaveAlignment(pave, CanvasHelper::kPaveAlignRight | CanvasHelper::kPaveAlignTop);
    CanvasHelper::getInstance()->addCanvas(canvas);
  }

  // Prepare canvas for plotting joint light guide data
  TCanvas *canvas = new TCanvas();
  canvas->Divide(1, 2, 1E-5, 1E-5);

  // PLOT 1: nPE(lightGuideLength)
  canvas->cd(1)->SetGrid();

  TGraphErrors *gr1 = new TGraphErrors((int) guideLengths.size(), &guideLengths[0], &nPes[0], 0, &nPeErrs[0]); // convert vector to array

  // Sort points along X axis and draw the graph
  gr1->Sort(&TGraph::CompareX);

  // Set title and subtitle after drawing the TGraph because it clears the Canvas
  TString title1 = "Detected PE vs. Light Guide Length";
  title1 += "; Light Guide Length, mm";
  title1 += "; Number of PE";

  gr1->SetTitle(title1.Data());
  gr1->SetMarkerStyle(EMarkerStyle::kFullCircle);
  gr1->Draw("APL");

  // PLOT 2: opResolution(lightGuideLength)
  canvas->cd(2)->SetGrid();

  TGraphErrors *gr2 = new TGraphErrors((int) guideLengths.size(), &guideLengths[0], &nPeRes[0], 0, &nPeResErrs[0]); // convert vector to array

  // Sort points along X axis and draw the graph
  gr2->Sort(&TGraph::CompareX);

  // Set title and subtitle after drawing the TGraph because it clears the Canvas
  TString title2 = "Optical Resolution vs. Light Guide Length";
  title2 += "; Light Guide Length, mm";
  title2 += "; Optical Resolution, %";

  gr2->SetTitle(title2.Data());
  gr2->SetMarkerStyle(EMarkerStyle::kFullCircle);
  gr2->Draw("APL");

  // Beautify the canvas
  CanvasHelper::getInstance()->addCanvas(canvas);

  return 0;
}

#ifndef __CINT__

int main(int argc, char **argv) {
  // Instantiate TApplication
  TApplication *app = new TApplication("plotEfficiency", &argc, argv);

  // Launch the script
  int returnValue = 0 || plotEfficiency();

  // Enter the event loop
  app->Run();

  // Return success
  return returnValue;
}

#endif
