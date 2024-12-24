
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include "RooGenericPdf.h"
#include "RooConstVar.h"
#include "RooFormulaVar.h"
 
using namespace RooFit;

void funcs(){
    RooRealVar x("x", "x", -20, 20);

    RooRealVar alpha("alpha", "alpha", 5, .1, 10);

    RooGenericPdf genpdf("genpdf", "genpdf", "(1+0.1*abs(x)+sin(sqrt(abs(x*alpha+0.1))))", RooArgSet(x, alpha));

    RooDataSet *data = genpdf.generate(x,1000);
    genpdf.fitTo(*data, PrintLevel(-1));

    RooPlot *xframe = x.frame(Title("interpreted expression pdf"));
    data->plotOn(xframe);
    genpdf.plotOn(xframe);

    TCanvas *c1 = new TCanvas();
    xframe->Draw();
    c1->SaveAs("func1.png");

    // RooRealVar mean2("mean", "mean2", 10, 0, 200);
    // RooRealVar sigma("sigma", "sigma", 3, .1, 100);
    // RooFormulaVar mean("mean", "mean", "sqrt(mean2)", mean2);
    // RooGaussian g2("g2", "g2", x, mean, sigma);
    
    // RooGaussian g1("g1", "g1",x,  RooConst(10), RooConst(3));
    // RooDataSet *data2 = g1.generate(x, 1000);
    
    // // ??
    // std::unique_ptr<RooFitResult> fitResult{g2.fitTo(*data2, Save(), PrintLevel(-1))};
    // fitResult->Print(); 

    // RooPlot *xframe2 = x.frame(Title("Tailored Gaussian pdf"));
    // data2->plotOn(xframe2);
    // g2.plotOn(xframe2);
    
    // TCanvas *c = new TCanvas("rf103_interprfuncs", "rf103_interprfuncs", 800, 400);
    // c->Divide(2);
    // c->cd(1);
    // gPad->SetLeftMargin(0.15);
    // xframe->GetYaxis()->SetTitleOffset(1.4);
    // xframe->Draw();
    // c->cd(2);
    // gPad->SetLeftMargin(0.15);
    // xframe2->GetYaxis()->SetTitleOffset(1.4);
    // xframe2->Draw();

    // c->SaveAs("func2.png");


   // Construct parameter mean2 and sigma
   RooRealVar mean2("mean2", "mean^2", 10, 0, 200);
   RooRealVar sigma("sigma", "sigma", 3, 0.1, 10);
 
   // Construct interpreted function mean = sqrt(mean^2)
   RooFormulaVar mean("mean", "mean", "sqrt(mean2)", mean2);
 
   // Construct a gaussian g2(x,sqrt(mean2),sigma) ;
   RooGaussian g2("g2", "h2", x, mean, sigma);
 
   RooGaussian g1("g1", "g1", x, RooConst(10), RooConst(3));
   RooDataSet *data2 = g1.generate(x, 1000);
 
  
   std::unique_ptr<RooFitResult> fitResult{g2.fitTo(*data2, Save(), PrintLevel(-1))};
   fitResult->Print();
 
  
   RooPlot *xframe2 = x.frame(Title("Tailored Gaussian pdf"));
   data2->plotOn(xframe2);
   g2.plotOn(xframe2);
 
   TCanvas *c = new TCanvas("rf103_interprfuncs", "rf103_interprfuncs", 800, 400);
   c->Divide(2);
   c->cd(1);
   gPad->SetLeftMargin(0.15);
   xframe->GetYaxis()->SetTitleOffset(1.4);
   xframe->Draw();
   c->cd(2);
   gPad->SetLeftMargin(0.15);
   xframe2->GetYaxis()->SetTitleOffset(1.4);
   xframe2->Draw();
    c->SaveAs("func2.png");

}