#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "RooPlot.h"
#include "TTree.h"
#include "TH1D.h"
#include "TRandom.h"
#include<fstream>
using namespace RooFit;
using namespace std;

TH1D * makeTH1(){
    TH1D *hist = new TH1D("hist", "hist", 100, 0, 100);
    TRandom *rand = new TRandom(0);

    for(int i=0 ; i<100; i++){
        hist -> Fill(rand->Gaus(50, 20));
    }

    return hist;
}

TTree * makeTTree(){
    TTree *tree = new TTree();

    double x,y;

    tree->Branch("x", &x, "x/D");
    tree->Branch("y", &y, "y/D");

    TRandom *rand = new TRandom(0);

    for(int i=0; i<1000; i++){
        x = rand->Gaus(0,3);
        y = rand -> Uniform(-15,15);
        tree->Fill();
    }
    return tree;
}

void hist(){
    TH1D *hh = makeTH1();

    RooRealVar x("x", "x", -10, 10);
    RooDataHist dh("dh", "dh", x, Import(*hh));

    RooPlot *frame = x.frame(Title("imported TH1 with poisson error bars"));
    dh.plotOn(frame);

    RooRealVar mean("mean", "mean", 0, -10, 10);
    RooRealVar sigma("sigma", "sigma", 3, .1, 10);
    RooGaussian gauss("gauss", "gauss", x, mean, sigma);

    gauss.fitTo(dh, PrintLevel(-1)); // suppresses the detailed output
    gauss.plotOn(frame); 

    TCanvas *c = new TCanvas();
    
    RooPlot *frame2 = x.frame(Title("imported TH1 with internal error"));
    dh.plotOn(frame2, DataError(RooAbsData::SumW2));
    gauss.plotOn(frame2);

    TTree * tree = makeTTree();

    RooRealVar y("y","y",-10, 10);

    RooDataSet ds("ds", "ds", RooArgSet(x, y), Import(*tree));

    ofstream outstream("data.txt");
    ds.write(outstream);
    outstream.close();

    RooDataSet *dataset = RooDataSet::read("data.txt", RooArgList(x, y), "D");

    cout<<"original data line 20"<<endl;
    ds.get(20)->Print("V");

    cout<<"imported data"<<endl;
    dataset->get(20)->Print("V");

    RooPlot *frame3 = y.frame(Title("Unbinned data shown in default frame binning"));
    ds.plotOn(frame3); // Default binning (100 bins).

    RooPlot *frame4 = y.frame(Title("Unbinned data shown with custom binning"));
    ds.plotOn(frame4, Binning(20)); // Custom binning (20 bins).

    RooPlot *frame5 = y.frame(Title("Unbinned data read back from ASCII file"));
    ds.plotOn(frame5, Binning(20));
    dataset->plotOn(frame5, Binning(20), MarkerColor(kRed), MarkerStyle(5)); // Overlay imported data.

    c->Divide(3, 2); // Divide the canvas into 3x2 sections.

    c->cd(1); frame->Draw();     // Imported TH1 with Poisson errors.
    c->cd(2); frame2->Draw();    // Imported TH1 with internal errors.
    c->cd(4); frame3->Draw();    // Default binning.
    c->cd(5); frame4->Draw();    // Custom binning.
    c->cd(6); frame5->Draw(); 

    c-> SaveAs("hist.png");
    }
