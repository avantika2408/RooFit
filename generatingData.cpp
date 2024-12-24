#include<RooRealVar.h>
#include<RooDataSet.h>
#include<TCanvas.h>
#include<RooPlot.h>
#include<RooGaussian.h>
#include<TAxis.h>
using namespace RooFit;

void data(){
    RooRealVar x("x", "x", -10, 10);
    RooRealVar mean("mean", "mean of gaussian", 1, -10, 10 );
    RooRealVar sigma("sigma", "sigma of gaussian", 1, .1, 10);

    RooGaussian gauss("guass", "gaussian", x, mean, sigma);

    RooPlot  *xframe =  x.frame(Title("gauss"));

    gauss.plotOn(xframe);

    RooDataSet *data = gauss.generate(x, 10000);

    RooPlot *xframe2 = x.frame(Title("gaussian with data"));
    data->plotOn(xframe2);
    

    TCanvas *c1 = new TCanvas();

    gauss.fitTo(*data, PrintLevel(-1));

    mean.Print();
    sigma.Print();

    c1->Divide(2);

    c1->cd(1);
    gPad->SetLeftMargin(0.15);
    xframe->GetYaxis()->SetTitleOffset(1.6);
    xframe->Draw();

    c1->cd(2);
    gPad->SetLeftMargin(0.15);
    xframe2->GetYaxis()->SetTitleOffset(1.6);
    xframe2->Draw();

    c1->SaveAs("generatingData.png");

}
