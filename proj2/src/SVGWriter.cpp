#include "SVGWriter.h"
#include "svg.h"
#include <string>
#include <vector>
#include <iostream>

struct CSVGWriter::SImplementation {
    std::shared_ptr< CDataSink > DSink;
    svg_context_ptr DContext;

    static svg_return_t WriteFunction(svg_user_context_ptr user, const char *text) {
        SImplementation *Implementation = (SImplementation *)user;
        while(*text) {
        Implementation->DSink->Put(*text++);
        }
        return SVG_OK;
    }

    SImplementation(std::shared_ptr< CDataSink > sink, TSVGPixel width, TSVGPixel height) : DSink(sink) {
    DContext = svg_create(WriteFunction, nullptr, this, width, height);
    }

    ~SImplementation(){}

    std::string CreateStyleString(const TAttributes &style){
        return " ";
    }
        
    bool Circle(const SSVGPoint &center, TSVGReal radius, const TAttributes &style){
        svg_point_t Center{center.DX, center.DY};
        std::string Style = CreateStyleString(style);
        return svg_circle(DContext, &Center, radius, Style.c_str{}); 
        return false;
    }
    
    bool Rectangle(const SSVGPoint &topleft, const SSVGSize &size, const TAttributes &style){
        return false;
    }
    bool Line(const SSVGPoint &start, const SSVGPoint &end, const TAttributes &style){
        return false;
    }
    
    bool SimplePath(const std::vector<SSVGPoint> points, const TAttributes &style) {
        return false;
    }
    
    bool GroupBegin(const TAttributes &attrs) {
        return false;
    }

    bool GroupEnd() {
        return false;
    } 
};

CSVGWriter::CSVGWriter(std::shared_ptr< CDataSink > sink, TSVGPixel width, TSVGPixel height) {
    DImplementation = std::make_unique<SImplementation>();
}

CSVGWriter::~CSVGWriter() {

}

bool CSVGWriter::Circle(const SSVGPoint &center, TSVGReal radius, const TAttributes &style) {
    return DImplementation->Circle(center, radius, style);
}

bool CSVGWriter::Rectangle(const SSVGPoint &topleft, const SSVGSize &size, const TAttributes &style) {
     return DImplementation->Rectangle(topleft, size, style);
}

bool CSVGWriter::Line(const SSVGPoint &start, const SSVGPoint &end, const TAttributes &style) {
    return DImplementation->Line(start, end, style);
}

bool CSVGWriter::SimplePath(const std::vector<SSVGPoint> points, const TAttributes &style) {
    return DImplementation->SimplePath(points, style);

}

bool CSVGWriter::GroupBegin(const TAttributes &attrs) {
    return DImplementation->GroupBegin(attrs);
}

bool CSVGWriter::GroupEnd() {
    return DImplementation->GroupEnd();
}