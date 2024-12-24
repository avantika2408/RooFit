#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
using namespace RooFit;

void basics(){
    RooRealVar x("x", "x", -10, 10);
    RooRealVar mean("mean", "mean of gaussian", 1, -10, 10);
    RooRealVar sigma("sigma","width of gaussian", 1, .1, 10 );

    RooGaussian gauss("gauss", "gaussian PDF", x, mean, sigma);

    RooPlot *xframe = x.frame(Title("gaussian pdf"));

    TCanvas *c1 = new TCanvas();

    gauss.plotOn(xframe);

    xframe -> Draw();

    sigma.setVal(3);
    gauss.plotOn(xframe, LineColor(kRed));
    xframe->Draw();
    c1->SaveAs("plotting.png");
}
