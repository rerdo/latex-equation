#ifndef ROOT_RENDERER_H
#define ROOT_RENDERER_H

#include <string>

class RootRenderer {
    public:
        RootRenderer();
        ~RootRenderer();

// render latex to a png file and return the path
        std::string renderLatex(const std::string& latex);

// render latex to an svg file with given filename
        std::string renderLatexSvg(const std::string& latex,
                                   const std::string& filename);
};

#endif // ROOT_RENDERER_H
