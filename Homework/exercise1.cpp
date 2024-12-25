#include <RooGaussian.h>
#include <RooRealVar.h>
#include <RooPlot.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <RooDataSet.h>
#include <RooAddPdf.h>
using namespace RooFit;
using namespace std;

int main() {
    RooRealVar x("x", "x", -50, 50);
    RooRealVar mean("mean", "mean", 0);
    RooRealVar sigma1("sigma1", "sigma1", 1);
    RooRealVar sigma2("sigma2", "sigma2", 10);

    
    RooGaussian gauss1("gauss1", "gauss1", x, mean, sigma1);
    RooGaussian gauss2("gauss2", "gauss2", x, mean, sigma2);

    
    RooDataSet* d1 = gauss1.generate(x, 1000);
    RooDataSet* d2 = gauss2.generate(x, 1000);

    
    ofstream file1("gauss1.txt");
    if (file1.is_open()) {
        for (int i = 0; i < d1->numEntries(); i++) {
            const RooArgSet* row = d1->get(i);
            file1 << row->getRealValue("x") << endl;
        }
        file1.close();
    }

    ofstream file2("gauss2.txt");
    if (file2.is_open()) {
        for (int i = 0; i < d2->numEntries(); i++) {
            const RooArgSet* row = d2->get(i);
            file2 << row->getRealValue("x") << endl;
        }
        file2.close();
    }

    
    RooDataSet combinedData("combinedData", "Combined Data", x);
    combinedData.append(*d1);
    combinedData.append(*d2);

    
    RooRealVar frac("frac", "Fraction of Gaussian 1", 0.5, 0.0, 1.0);
    RooAddPdf compositeModel("compositeModel", "Composite Model", RooArgList(gauss1, gauss2), RooArgList(frac));

    
    compositeModel.fitTo(combinedData);

    
    RooPlot* frame1 = x.frame(Title("Gaussian 1"));
    d1->plotOn(frame1);
    gauss1.plotOn(frame1);

    RooPlot* frame2 = x.frame(Title("Gaussian 2"));
    d2->plotOn(frame2);
    gauss2.plotOn(frame2);

    
    RooPlot* frame3 = x.frame(Title("Composite Model"));
    combinedData.plotOn(frame3);
    compositeModel.plotOn(frame3);
    compositeModel.plotOn(frame3, Components(gauss1), LineColor(kRed));
    compositeModel.plotOn(frame3, Components(gauss2), LineColor(kBlue));

    
    TCanvas* c = new TCanvas("c", "Composite Model", 1200, 600);
    c->Divide(3);
    c->cd(1);
    frame1->Draw();
    c->cd(2);
    frame2->Draw();
    c->cd(3);
    frame3->Draw();

    c->SaveAs("compositeModel.png");

    return 0;
}
