#include "root-renderer.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TCanvas.h"
#include "TImage.h"
#include <stdexcept>


RootRenderer::RootRenderer(){} // constructor

RootRenderer::~RootRenderer() = default; // destructor

std::string RootRenderer::renderLatex(const std::string& latex){
    // create a canvas
    static TCanvas* canvas = nullptr;
    if (!canvas) {
        canvas = new TCanvas("c_rootlatex", "ROOT Latex", 1200, 300);
    }

    canvas->Clear();
    canvas->cd();

    // draw Latex
    TLatex tex;
    tex.SetTextSize(.2);
    tex.SetTextAlign(22); // center alignment
    tex.DrawLatexNDC(0.5, 0.5, latex.c_str());

    canvas->Update();
    //canvas->SaveAs("equation.svg");

    // save as PNG and return path
    std::string filename = "equation.png";

    TImage* img = TImage::Create();
    img->FromPad(canvas);
    img->SetImageQuality(TImage::kImgBest);
    img->WriteImage(filename.c_str());
    delete img;

    return filename;
};

std::string RootRenderer::renderLatexSvg(const std::string& latex,
                                         const std::string& filename) {
    // create a canvas
    static TCanvas* canvas = nullptr;
    int W = 2400;
    int H = 800;

    if (!canvas)
        canvas = new TCanvas("c_rootlatex_svg", "ROOT Latex SVG", W, H);
    else
        canvas->SetCanvasSize(W, H);

    canvas->Clear();
    canvas->cd();

    TLatex tex;
    tex.SetTextSize(.06);
    tex.SetTextAlign(22); // center alignment
    tex.DrawLatexNDC(0.5, 0.5, latex.c_str());
    canvas->Update();

    canvas->SaveAs(filename.c_str());

    return filename;
};
